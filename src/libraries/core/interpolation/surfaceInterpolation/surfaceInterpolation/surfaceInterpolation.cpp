/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Description
    Cell to face interpolation scheme. Included in fvMesh.

\*---------------------------------------------------------------------------*/

#include "fvMesh.hpp"
#include "surfaceFieldsFwd.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "demandDrivenData.hpp"
#include "coupledFvPatch.hpp"


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(surfaceInterpolation, 0);
}


// * * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * //

void CML::surfaceInterpolation::clearOut()
{
    deleteDemandDrivenData(weights_);
    deleteDemandDrivenData(deltaCoeffs_);
    deleteDemandDrivenData(nonOrthDeltaCoeffs_);
    deleteDemandDrivenData(nonOrthCorrectionVectors_);
	deleteDemandDrivenData(fvNonOrthCorrectionVectors_);
}


// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * //

CML::surfaceInterpolation::surfaceInterpolation(const fvMesh& fvm)
:
    mesh_(fvm),
    weights_(nullptr),
    deltaCoeffs_(nullptr),
    nonOrthDeltaCoeffs_(nullptr),
    nonOrthCorrectionVectors_(nullptr),
	fvNonOrthCorrectionVectors_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor * * * * * * * * * * * * * * * //

CML::surfaceInterpolation::~surfaceInterpolation()
{
    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::surfaceScalarField&
CML::surfaceInterpolation::weights() const
{
    if (!weights_)
    {
        makeWeights();
    }

    return (*weights_);
}


const CML::surfaceScalarField&
CML::surfaceInterpolation::deltaCoeffs() const
{
    if (!deltaCoeffs_)
    {
        makeDeltaCoeffs();
    }

    return (*deltaCoeffs_);
}


const CML::surfaceScalarField&
CML::surfaceInterpolation::nonOrthDeltaCoeffs() const
{
    if (!nonOrthDeltaCoeffs_)
    {
        makeNonOrthDeltaCoeffs();
    }

    return (*nonOrthDeltaCoeffs_);
}


const CML::surfaceVectorField&
CML::surfaceInterpolation::nonOrthCorrectionVectors() const
{
    if (!nonOrthCorrectionVectors_)
    {
        makeNonOrthCorrectionVectors();
    }

    return (*nonOrthCorrectionVectors_);
}


const CML::FieldField<CML::fvsPatchField, CML::vector>&
CML::surfaceInterpolation::fvNonOrthCorrectionVectors() const
{
    // These get made at the same time as the nonOrthCorrection
    // vectors, so make them if those don't exist
    if (!nonOrthCorrectionVectors_)
    {
        makeNonOrthCorrectionVectors();
    }

    return (*fvNonOrthCorrectionVectors_);
}

// Do what is necessary if the mesh has moved
bool CML::surfaceInterpolation::movePoints()
{
    deleteDemandDrivenData(weights_);
    deleteDemandDrivenData(deltaCoeffs_);
    deleteDemandDrivenData(nonOrthDeltaCoeffs_);
    deleteDemandDrivenData(nonOrthCorrectionVectors_);
	deleteDemandDrivenData(fvNonOrthCorrectionVectors_);

    return true;
}


void CML::surfaceInterpolation::makeWeights() const
{
    if (debug)
    {
        Pout<< "surfaceInterpolation::makeWeights() : "
            << "Constructing weighting factors for face interpolation"
            << endl;
    }

    weights_ = new surfaceScalarField
    (
        IOobject
        (
            "weights",
            mesh_.pointsInstance(),
            mesh_
        ),
        mesh_,
        dimless
    );
    surfaceScalarField& weights = *weights_;

    // Set local references to mesh data
    // (note that we should not use fvMesh sliced fields at this point yet
    //  since this causes a loop when generating weighting factors in
    //  coupledFvPatchField evaluation phase)
    const labelUList& owner = mesh_.owner();
    const labelUList& neighbour = mesh_.neighbour();

    const vectorField& Cf = mesh_.faceCentres();
    const vectorField& C = mesh_.cellCentres();
    const vectorField& Sf = mesh_.faceAreas();

    // ... and reference to the internal field of the weighting factors
    scalarField& w = weights.internalField();

    forAll(owner, facei)
    {
        // Note: mag in the dot-product.
        // For all valid meshes, the non-orthogonality will be less that
        // 90 deg and the dot-product will be positive.  For invalid
        // meshes (d & s <= 0), this will stabilise the calculation
        // but the result will be poor.
        scalar SfdOwn = mag(Sf[facei] & (Cf[facei] - C[owner[facei]]));
        scalar SfdNei = mag(Sf[facei] & (C[neighbour[facei]] - Cf[facei]));
        w[facei] = SfdNei/(SfdOwn + SfdNei);
    }

    forAll(mesh_.boundary(), patchi)
    {
        mesh_.boundary()[patchi].makeWeights
        (
            weights.boundaryField()[patchi]
        );
    }

    if (debug)
    {
        Pout<< "surfaceInterpolation::makeWeights() : "
            << "Finished constructing weighting factors for face interpolation"
            << endl;
    }
}


void CML::surfaceInterpolation::makeDeltaCoeffs() const
{
    if (debug)
    {
        Pout<< "surfaceInterpolation::makeDeltaCoeffs() : "
            << "Constructing differencing factors array for face gradient"
            << endl;
    }

    // Force the construction of the weighting factors
    // needed to make sure deltaCoeffs are calculated for parallel runs.
    weights();

    deltaCoeffs_ = new surfaceScalarField
    (
        IOobject
        (
            "deltaCoeffs",
            mesh_.pointsInstance(),
            mesh_
        ),
        mesh_,
        dimless/dimLength
    );
    surfaceScalarField& DeltaCoeffs = *deltaCoeffs_;


    // Set local references to mesh data
    const volVectorField& C = mesh_.C();
    const labelUList& owner = mesh_.owner();
    const labelUList& neighbour = mesh_.neighbour();

    forAll(owner, facei)
    {
        DeltaCoeffs[facei] = 1.0/mag(C[neighbour[facei]] - C[owner[facei]]);
    }

    forAll(DeltaCoeffs.boundaryField(), patchi)
    {
        DeltaCoeffs.boundaryField()[patchi] =
            1.0/mag(mesh_.boundary()[patchi].delta());
    }
}


void CML::surfaceInterpolation::makeNonOrthDeltaCoeffs() const
{
    if (debug)
    {
        Pout<< "surfaceInterpolation::makeNonOrthDeltaCoeffs() : "
            << "Constructing differencing factors array for face gradient"
            << endl;
    }

    // Force the construction of the weighting factors
    // needed to make sure deltaCoeffs are calculated for parallel runs.
    weights();

    nonOrthDeltaCoeffs_ = new surfaceScalarField
    (
        IOobject
        (
            "nonOrthDeltaCoeffs",
            mesh_.pointsInstance(),
            mesh_
        ),
        mesh_,
        dimless/dimLength
    );
    surfaceScalarField& nonOrthDeltaCoeffs = *nonOrthDeltaCoeffs_;


    // Set local references to mesh data
    const volVectorField& C = mesh_.C();
    const labelUList& owner = mesh_.owner();
    const labelUList& neighbour = mesh_.neighbour();
    const surfaceVectorField& Sf = mesh_.Sf();
    const surfaceScalarField& magSf = mesh_.magSf();

    forAll(owner, facei)
    {
        vector delta = C[neighbour[facei]] - C[owner[facei]];
        vector unitArea = Sf[facei]/magSf[facei];

        // Standard cell-centre distance form
        //NonOrthDeltaCoeffs[facei] = (unitArea & delta)/magSqr(delta);

        // Slightly under-relaxed form
        //NonOrthDeltaCoeffs[facei] = 1.0/mag(delta);

        // More under-relaxed form
        //NonOrthDeltaCoeffs[facei] = 1.0/(mag(unitArea & delta) + VSMALL);

        // Stabilised form for bad meshes
        nonOrthDeltaCoeffs[facei] = 1.0/max(unitArea & delta, 0.05*mag(delta));
    }

    forAll(nonOrthDeltaCoeffs.boundaryField(), patchi)
    {
        vectorField delta(mesh_.boundary()[patchi].delta());

        nonOrthDeltaCoeffs.boundaryField()[patchi] =
            1.0/max(mesh_.boundary()[patchi].nf() & delta, 0.05*mag(delta));
    }
}


void CML::surfaceInterpolation::makeNonOrthCorrectionVectors() const
{
    if (debug)
    {
        Pout<< "surfaceInterpolation::makeNonOrthCorrectionVectors() : "
            << "Constructing non-orthogonal correction vectors"
            << endl;
    }

    nonOrthCorrectionVectors_ = new surfaceVectorField
    (
        IOobject
        (
            "nonOrthCorrectionVectors",
            mesh_.pointsInstance(),
            mesh_
        ),
        mesh_,
        dimless
    );
    surfaceVectorField& corrVecs = *nonOrthCorrectionVectors_;

	// Create FieldField with size of nonOrthCorrectionVectors boundaryField
	fvNonOrthCorrectionVectors_ = new FieldField<fvsPatchField, vector>(corrVecs.boundaryField().size());
	
	// Values for fixedValue boundary conditions
	FieldField<fvsPatchField, vector>& fvCorrVecs = *fvNonOrthCorrectionVectors_; 

    // Set local references to mesh data
    const volVectorField& C = mesh_.C();
    const labelUList& owner = mesh_.owner();
    const labelUList& neighbour = mesh_.neighbour();
    const surfaceVectorField& Sf = mesh_.Sf();
    const surfaceScalarField& magSf = mesh_.magSf();
    const surfaceScalarField& NonOrthDeltaCoeffs = nonOrthDeltaCoeffs();

    forAll(owner, facei)
    {
        vector unitArea = Sf[facei]/magSf[facei];
        vector delta = C[neighbour[facei]] - C[owner[facei]];

        corrVecs[facei] = unitArea - delta*NonOrthDeltaCoeffs[facei];
    }

    // Boundary correction vectors set to zero for fixedGradient boundary patches
    // and calculated consistently with internal corrections for
    // coupled patches
	
    forAll(corrVecs.boundaryField(), patchi)
    {
	    // Clone the nonOrthoCorrectionVectors boundaryField values
	    fvCorrVecs.set(patchi, corrVecs.boundaryField()[patchi].clone(corrVecs.dimensionedInternalField()));
		
        fvsPatchVectorField& patchCorrVecs = corrVecs.boundaryField()[patchi];

        if (!patchCorrVecs.coupled())
        {
		    // Set the correction to zero. This is okay for fixedGradient conditions
			patchCorrVecs = vector::zero;
			
			// Initialise correction to zero
			fvCorrVecs[patchi] = vector::zero;
			
		    const fvsPatchScalarField& patchNonOrthDeltaCoeffs
                = NonOrthDeltaCoeffs.boundaryField()[patchi];

            const fvPatch& p = patchCorrVecs.patch();
			
		    // For non-coupled boundaries the patch delta is already corrected for
 			// non-orthogonality. We therefore need to calculate the uncorrected delta.
			const vectorField Cf(mesh_.boundary()[patchi].Cf());
			const vectorField Cn(mesh_.boundary()[patchi].Cn());

            forAll(p, patchFacei)
            {
			    vector delta = Cf[patchFacei] - Cn[patchFacei];
				
                vector unitArea =
                    Sf.boundaryField()[patchi][patchFacei]
                   /magSf.boundaryField()[patchi][patchFacei];
 
                fvCorrVecs[patchi][patchFacei] =
                    unitArea - delta*patchNonOrthDeltaCoeffs[patchFacei];
            }
        }
        else
        {
            const fvsPatchScalarField& patchNonOrthDeltaCoeffs
                = NonOrthDeltaCoeffs.boundaryField()[patchi];

            const fvPatch& p = patchCorrVecs.patch();
			
            const vectorField patchDeltas(mesh_.boundary()[patchi].delta());

            forAll(p, patchFacei)
            {
                vector unitArea =
                    Sf.boundaryField()[patchi][patchFacei]
                   /magSf.boundaryField()[patchi][patchFacei];

                const vector& delta = patchDeltas[patchFacei];
 
                patchCorrVecs[patchFacei] =
                    unitArea - delta*patchNonOrthDeltaCoeffs[patchFacei];
				
            }
			// Set the boundary values for coupled patches for safety
			fvCorrVecs[patchi] =  patchCorrVecs;
        }
    }
	
    if (debug)
    {
        Pout<< "surfaceInterpolation::makeNonOrthCorrectionVectors() : "
            << "Finished constructing non-orthogonal correction vectors"
            << endl;
    }
}


// ************************************************************************* //
