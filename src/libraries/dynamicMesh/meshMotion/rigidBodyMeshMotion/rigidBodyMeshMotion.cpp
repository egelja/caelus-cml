/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
Copyright (C) 2017 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "rigidBodyMeshMotion.hpp"
#include "rigidBodyRestraint.hpp"
#include "rigidBodyModel.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "polyMesh.hpp"
#include "pointPatchDist.hpp"
#include "pointConstraints.hpp"
#include "uniformDimensionedFields.hpp"
#include "forces.hpp"
#include "mathematicalConstants.hpp"
#include "OFstream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(rigidBodyMeshMotion, 0);

    addToRunTimeSelectionTable
    (
        motionSolver,
        rigidBodyMeshMotion,
        dictionary
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //
void CML::rigidBodyMeshMotion::initialiseStateFile()
{
    if (Pstream::master())
    {
        // For individual bodies
        bodyStateFilePtrs_.setSize(model_.nBodies());
		forAll(bodyMeshes_,bi)
		{
			const label bodyID = bodyMeshes_[bi].bodyID_;
			if (bodyStateFilePtrs_[bi].empty())
			{
				if (Pstream::parRun())
				{
                    mkDir(db().time().path()/".."/"States"/db().time().timeName());
                    bodyStateFilePtrs_[bi].reset(new OFstream(db().time().path()/".."/"States"/db().time().timeName()/model_.name(bodyID) +".dat"));
				}
                else
                {
				    mkDir(db().time().path()/"States"/db().time().timeName());
                    bodyStateFilePtrs_[bi].reset(new OFstream(db().time().path()/"States"/db().time().timeName()/model_.name(bodyID) +".dat"));
				}
	
				bodyStateFilePtrs_[bi]()  << "#"
				    << token::SPACE << "Time"
					<< token::SPACE << "(Fx  Fy  Fz)"
					<< token::SPACE << "(Mx  My  Mz)"
					<< token::SPACE << "(CoGx  CoGy  CoGz)"
					<< token::SPACE << "(Ux   Uy   Uz)"
					<< token::SPACE << "(Wx   Wy   Wz)"
                    << token::SPACE << "(Rotation Matrix)"
					<< endl;
			} 
		}
			
        // For individual restraints
    	restraintStateFilePtrs_.setSize(model_.nRests());
		forAll(model_.restraints(),ri)
		{
			Info << "Restraint " << model_.restraints()[ri].name();
			if (restraintStateFilePtrs_[ri].empty())
			{
				if (Pstream::parRun())
				{
                    restraintStateFilePtrs_[ri].reset(new OFstream(db().time().path()/".."/"States"/db().time().timeName()/model_.restraints()[ri].name() +".dat"));
                }
                else
                {
                    restraintStateFilePtrs_[ri].reset(new OFstream(db().time().path()/"States"/db().time().timeName()/model_.restraints()[ri].name() +".dat"));
			    }
	
				restraintStateFilePtrs_[ri]()  << "#"
					<< token::SPACE << "Time"
					<< token::SPACE << "displacement"
					<< token::SPACE << "velocity"
					<< token::SPACE << "Force"
					<< endl;
			} 
    	}    
    }
}


CML::rigidBodyMeshMotion::bodyMesh::bodyMesh
(
    const polyMesh& mesh,
    const word& name,
    const label bodyID,
    const dictionary& dict
)
:
    name_(name),
    bodyID_(bodyID),
    patches_(wordReList(dict.lookup("patches"))),
    patchSet_(mesh.boundaryMesh().patchSet(patches_)),
    di_(readScalar(dict.lookup("innerDistance"))),
    do_(readScalar(dict.lookup("outerDistance"))),
    weight_
    (
        IOobject
        (
            name_ + ".motionScale",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        ),
        pointMesh::New(mesh),
        dimensionedScalar("zero", dimless, 0.0)
    )
{}


CML::rigidBodyMeshMotion::rigidBodyMeshMotion
(
    const polyMesh& mesh,
    const IOdictionary& dict
)
:
    displacementMotionSolver(mesh, dict, typeName),
    model_
    (
        mesh.time(),
        coeffDict(),
        IOobject
        (
            "rigidBodyMotionState",
            mesh.time().timeName(),
            "uniform",
            mesh
        ).headerOk()
      ? IOdictionary
        (
            IOobject
            (
                "rigidBodyMotionState",
                mesh.time().timeName(),
                "uniform",
                mesh,
                IOobject::READ_IF_PRESENT,
                IOobject::NO_WRITE,
                false
            )
        )
      : coeffDict()
    ),
    rhoInf_(1.0),
    rhoName_(coeffDict().lookupOrDefault<word>("rho", "rho")),
    curTimeIndex_(-1),
    motionOutputControl_(mesh.time(), coeffDict().subDict("outputControl"))
{
    if (rhoName_ == "rhoInf")
    {
        rhoInf_ = readScalar(coeffDict().lookup("rhoInf"));
    }

    const dictionary& bodiesDict = coeffDict().subDict("bodies");

    forAllConstIter(IDLList<entry>, bodiesDict, iter)
    {
        const dictionary& bodyDict = iter().dict();

        if (bodyDict.found("patches"))
        {
            const label bodyID = model_.bodyID(iter().keyword());

            if (bodyID == -1)
            {
                FatalErrorInFunction
                    << "Body " << iter().keyword()
                    << " has been merged with another body"
                       " and cannot be assigned a set of patches"
                    << exit(FatalError);
            }

            bodyMeshes_.append
            (
                new bodyMesh
                (
                    mesh,
                    iter().keyword(),
                    bodyID,
                    bodyDict
                )
            );
        }
    }

    // Calculate scaling factor everywhere for each meshed body
    forAll(bodyMeshes_, bi)
    {
        const pointMesh& pMesh = pointMesh::New(mesh);

        pointPatchDist pDist(pMesh, bodyMeshes_[bi].patchSet_, points0());

        pointScalarField& scale = bodyMeshes_[bi].weight_;

        // Scaling: 1 up to di then linear down to 0 at do away from patches
        scale.internalField() =
            min
            (
                max
                (
                    (bodyMeshes_[bi].do_ - pDist.internalField())
                   /(bodyMeshes_[bi].do_ - bodyMeshes_[bi].di_),
                    scalar(0)
                ),
                scalar(1)
            );

        // Convert the scale function to a cosine
        scale.internalField() =
            min
            (
                max
                (
                    0.5
                  - 0.5
                   *cos(scale.internalField()
                   *CML::constant::mathematical::pi),
                    scalar(0)
                ),
                scalar(1)
            );

        pointConstraints::New(pMesh).constrain(scale);
        //scale.write();
    }
    initialiseStateFile();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::rigidBodyMeshMotion::~rigidBodyMeshMotion()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::pointField>
CML::rigidBodyMeshMotion::curPoints() const
{
    return points0() + pointDisplacement_.internalField();
}


void CML::rigidBodyMeshMotion::solve()
{
    const Time& t = mesh().time();

    if (mesh().nPoints() != points0().size())
    {
        FatalErrorInFunction
            << "The number of points in the mesh seems to have changed." << endl
            << "In constant/polyMesh there are " << points0().size()
            << " points; in the current mesh there are " << mesh().nPoints()
            << " points." << exit(FatalError);
    }

    // Store the motion state at the beginning of the time-step
    if (curTimeIndex_ != this->db().time().timeIndex())
    {
        model_.newTime();
        curTimeIndex_ = this->db().time().timeIndex();
    }

    if (db().foundObject<uniformDimensionedVectorField>("g"))
    {
        model_.g() =
            db().lookupObject<uniformDimensionedVectorField>("g").value();
    }

	Field<spatialVector> fx(model_.nBodies(), Zero);

    forAll(bodyMeshes_, bi)
    {
        const label bodyID = bodyMeshes_[bi].bodyID_;

        dictionary forcesDict;
        forcesDict.add("type", forces::typeName);
        forcesDict.add("patches", bodyMeshes_[bi].patches_);
        forcesDict.add("rhoInf", rhoInf_);
        forcesDict.add("rho", rhoName_);
        forcesDict.add("CofR", vector::zero);

        forces f("forces", db(), forcesDict);
        f.calcForcesMoment();

        fx[bodyID] = spatialVector(f.momentEff(), f.forceEff());
    }

    model_.solve
    (
        t.deltaTValue(),
        scalarField(model_.nDoF(), Zero),
        fx
    );

    if (Pstream::master() && motionOutputControl_.output() && model_.report())
    {
        forAll(bodyMeshes_, bi)
        {
		   
		   const label bodyID = bodyMeshes_[bi].bodyID_;
		   
           bodyStateFilePtrs_[bi]() 
				<< db().time().timeOutputValue()
				<< token::SPACE << fx[bodyID].l()
                << token::SPACE << fx[bodyID].w();
            
            model_.status(bodyMeshes_[bi].bodyID_);
            model_.writeState(bodyMeshes_[bi].bodyID_,bodyStateFilePtrs_[bi]);
        }
        
        forAll(model_.restraints(),ri)
        {
           restraintStateFilePtrs_[ri]() << db().time().timeOutputValue() << token::SPACE;
        }
        model_.writeRestState(restraintStateFilePtrs_);
    }

    // Update the displacements
    if (bodyMeshes_.size() == 1)
    {
        pointDisplacement_.internalField() = model_.transformPoints
        (
            bodyMeshes_[0].bodyID_,
            bodyMeshes_[0].weight_,
            points0()
        ) - points0();
    }
    else
    {
        labelList bodyIDs(bodyMeshes_.size());
        List<const scalarField*> weights(bodyMeshes_.size());
        forAll(bodyIDs, bi)
        {
            bodyIDs[bi] = bodyMeshes_[bi].bodyID_;
            weights[bi] = &bodyMeshes_[bi].weight_;
        }

        pointDisplacement_.internalField() =
            model_.transformPoints(bodyIDs, weights, points0()) - points0();
    }

    // Displacement has changed. Update boundary conditions
    pointConstraints::New
    (
        pointDisplacement_.mesh()
    ).constrainDisplacement(pointDisplacement_);
}


bool CML::rigidBodyMeshMotion::writeObject
(
    IOstream::streamFormat fmt,
    IOstream::versionNumber ver,
    IOstream::compressionType cmp
) const
{
   // Write state
   IOdictionary dict
    (
        IOobject
        (
            "rigidBodyMotionState",
            mesh().time().timeName(),
            "uniform",
            mesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        )
    );

    model_.state().write(dict);
    return dict.regIOobject::write();
}


bool CML::rigidBodyMeshMotion::read()
{
    if (displacementMotionSolver::read())
    {
        model_.read(coeffDict());

        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
