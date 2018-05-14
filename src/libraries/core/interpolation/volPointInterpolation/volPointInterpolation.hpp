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

Class
    CML::volPointInterpolation

Description
    Interpolate from cell centres to points (vertices) using inverse distance
    weighting

SourceFiles
    volPointInterpolation.cpp
    volPointInterpolate.cpp

\*---------------------------------------------------------------------------*/

#ifndef volPointInterpolation_H
#define volPointInterpolation_H

#include "MeshObject.hpp"
#include "scalarList.hpp"
#include "volFields.hpp"
#include "pointFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;
class pointMesh;

/*---------------------------------------------------------------------------*\
                       Class volPointInterpolation Declaration
\*---------------------------------------------------------------------------*/

class volPointInterpolation
:
    public MeshObject<fvMesh, volPointInterpolation>
{
    // Private data

        //- Interpolation scheme weighting factor array.
        scalarListList pointWeights_;


        // Boundary handling

            //- Boundary addressing
            autoPtr<primitivePatch> boundaryPtr_;

            //- Per boundary face whether is on non-coupled patch
            boolList boundaryIsPatchFace_;

            //- Per mesh(!) point whether is on non-coupled patch (on any
            //  processor)
            boolList isPatchPoint_;

            //- Per boundary point the weights per pointFaces.
            scalarListList boundaryPointWeights_;


    // Private Member Functions

        //- Construct addressing over all boundary faces
        void calcBoundaryAddressing();

        //- Make weights for internal and coupled-only boundarypoints
        void makeInternalWeights(scalarField& sumWeights);

        //- Make weights for points on uncoupled patches
        void makeBoundaryWeights(scalarField& sumWeights);

        //- Construct all point weighting factors
        void makeWeights();

        //- Helper: push master point data to collocated points
        template<class Type>
        void pushUntransformedData(List<Type>&) const;

        //- Get boundary field in same order as boundary faces. Field is
        //  zero on all coupled and empty patches
        template<class Type>
        tmp<Field<Type> > flatBoundaryField
        (
            const GeometricField<Type, fvPatchField, volMesh>& vf
        ) const;

        //- Add separated contributions
        template<class Type>
        void addSeparated
        (
            GeometricField<Type, pointPatchField, pointMesh>&
        ) const;

        //- Disallow default bitwise copy construct
        volPointInterpolation(const volPointInterpolation&);

        //- Disallow default bitwise assignment
        void operator=(const volPointInterpolation&);


public:

    // Declare name of the class and its debug switch
    ClassName("volPointInterpolation");


    // Constructors

        //- Constructor given fvMesh and pointMesh.
        explicit volPointInterpolation(const fvMesh&);


    //- Destructor
    ~volPointInterpolation();


    // Member functions

        // Edit

            //- Update mesh topology using the morph engine
            void updateMesh();

            //- Correct weighting factors for moving mesh.
            bool movePoints();


    // Interpolation functions

        //- Interpolate volField using inverse distance weighting
        //  returning pointField
        template<class Type>
        tmp<GeometricField<Type, pointPatchField, pointMesh> > interpolate
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;

        //- Interpolate tmp<volField> using inverse distance weighting
        //  returning pointField
        template<class Type>
        tmp<GeometricField<Type, pointPatchField, pointMesh> > interpolate
        (
            const tmp<GeometricField<Type, fvPatchField, volMesh> >&
        ) const;

        //- Interpolate volField using inverse distance weighting
        //  returning pointField with the same patchField types. Assigns
        //  to any fixedValue boundary conditions to make them consistent
        //  with internal field
        template<class Type>
        tmp<GeometricField<Type, pointPatchField, pointMesh> > interpolate
        (
            const GeometricField<Type, fvPatchField, volMesh>&,
            const wordList& patchFieldTypes
        ) const;

        //- Interpolate tmp<volField> using inverse distance weighting
        //  returning pointField with the same patchField types. Assigns
        //  to any fixedValue boundary conditions to make them consistent
        //  with internal field
        template<class Type>
        tmp<GeometricField<Type, pointPatchField, pointMesh> > interpolate
        (
            const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
            const wordList& patchFieldTypes
        ) const;


        // Low level

            //- Interpolate internal field from volField to pointField
            //  using inverse distance weighting
            template<class Type>
            void interpolateInternalField
            (
                const GeometricField<Type, fvPatchField, volMesh>&,
                GeometricField<Type, pointPatchField, pointMesh>&
            ) const;

            //- Interpolate boundary field without applying constraints/boundary
            //  conditions
            template<class Type>
            void interpolateBoundaryField
            (
                const GeometricField<Type, fvPatchField, volMesh>& vf,
                GeometricField<Type, pointPatchField, pointMesh>& pf
            ) const;

            //- Interpolate boundary with constraints/boundary conditions
            template<class Type>
            void interpolateBoundaryField
            (
                const GeometricField<Type, fvPatchField, volMesh>& vf,
                GeometricField<Type, pointPatchField, pointMesh>& pf,
                const bool overrideFixedValue
            ) const;

            //- Interpolate from volField to pointField
            //  using inverse distance weighting
            template<class Type>
            void interpolate
            (
                const GeometricField<Type, fvPatchField, volMesh>&,
                GeometricField<Type, pointPatchField, pointMesh>&
            ) const;

            //- Interpolate volField using inverse distance weighting
            //  returning pointField with name. Optionally caches
            template<class Type>
            tmp<GeometricField<Type, pointPatchField, pointMesh> > interpolate
            (
                const GeometricField<Type, fvPatchField, volMesh>&,
                const word& name,
                const bool cache
            ) const;


        // Interpolation for displacement (applies 2D correction)

            //- Interpolate from volField to pointField
            //  using inverse distance weighting
            void interpolateDisplacement
            (
                const volVectorField&,
                pointVectorField&
            ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "pointFields.hpp"
#include "emptyFvPatch.hpp"
#include "coupledPointPatchField.hpp"
#include "pointConstraints.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void CML::volPointInterpolation::pushUntransformedData
(
    List<Type>& pointData
) const
{
    // Transfer onto coupled patch
    const globalMeshData& gmd = mesh().globalData();
    const indirectPrimitivePatch& cpp = gmd.coupledPatch();
    const labelList& meshPoints = cpp.meshPoints();

    const mapDistribute& slavesMap = gmd.globalCoPointSlavesMap();
    const labelListList& slaves = gmd.globalCoPointSlaves();

    List<Type> elems(slavesMap.constructSize());
    forAll(meshPoints, i)
    {
        elems[i] = pointData[meshPoints[i]];
    }

    // Combine master data with slave data
    forAll(slaves, i)
    {
        const labelList& slavePoints = slaves[i];

        // Copy master data to slave slots
        forAll(slavePoints, j)
        {
            elems[slavePoints[j]] = elems[i];
        }
    }

    // Push slave-slot data back to slaves
    slavesMap.reverseDistribute(elems.size(), elems, false);

    // Extract back onto mesh
    forAll(meshPoints, i)
    {
        pointData[meshPoints[i]] = elems[i];
    }
}


template<class Type>
void CML::volPointInterpolation::addSeparated
(
    GeometricField<Type, pointPatchField, pointMesh>& pf
) const
{
    if (debug)
    {
        Pout<< "volPointInterpolation::addSeparated" << endl;
    }

    typename GeometricField<Type, pointPatchField, pointMesh>::
        GeometricBoundaryField& pfbf = pf.boundaryField();

    forAll(pfbf, patchI)
    {
        if (pfbf[patchI].coupled())
        {
            refCast<coupledPointPatchField<Type> >
                (pfbf[patchI]).initSwapAddSeparated
                (
                    Pstream::nonBlocking,
                    pf.internalField()
                );
        }
    }

    // Block for any outstanding requests
    Pstream::waitRequests();

    forAll(pfbf, patchI)
    {
        if (pfbf[patchI].coupled())
        {
            refCast<coupledPointPatchField<Type> >
                (pfbf[patchI]).swapAddSeparated
                (
                    Pstream::nonBlocking,
                    pf.internalField()
                );
        }
    }
}


template<class Type>
void CML::volPointInterpolation::interpolateInternalField
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    GeometricField<Type, pointPatchField, pointMesh>& pf
) const
{
    if (debug)
    {
        Pout<< "volPointInterpolation::interpolateInternalField("
            << "const GeometricField<Type, fvPatchField, volMesh>&, "
            << "GeometricField<Type, pointPatchField, pointMesh>&) : "
            << "interpolating field from cells to points"
            << endl;
    }

    const labelListList& pointCells = vf.mesh().pointCells();

    // Multiply volField by weighting factor matrix to create pointField
    forAll(pointCells, pointi)
    {
        if (!isPatchPoint_[pointi])
        {
            const scalarList& pw = pointWeights_[pointi];
            const labelList& ppc = pointCells[pointi];

            pf[pointi] = pTraits<Type>::zero;

            forAll(ppc, pointCelli)
            {
                pf[pointi] += pw[pointCelli]*vf[ppc[pointCelli]];
            }
        }
    }
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::volPointInterpolation::flatBoundaryField
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    const fvMesh& mesh = vf.mesh();
    const fvBoundaryMesh& bm = mesh.boundary();

    tmp<Field<Type> > tboundaryVals
    (
        new Field<Type>(mesh.nFaces()-mesh.nInternalFaces())
    );
    Field<Type>& boundaryVals = tboundaryVals();

    forAll(vf.boundaryField(), patchI)
    {
        label bFaceI = bm[patchI].patch().start() - mesh.nInternalFaces();

        if
        (
           !isA<emptyFvPatch>(bm[patchI])
        && !vf.boundaryField()[patchI].coupled()
        )
        {
            SubList<Type>
            (
                boundaryVals,
                vf.boundaryField()[patchI].size(),
                bFaceI
            ).assign(vf.boundaryField()[patchI]);
        }
        else
        {
            const polyPatch& pp = bm[patchI].patch();

            forAll(pp, i)
            {
                boundaryVals[bFaceI++] = pTraits<Type>::zero;
            }
        }
    }

    return tboundaryVals;
}


template<class Type>
void CML::volPointInterpolation::interpolateBoundaryField
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    GeometricField<Type, pointPatchField, pointMesh>& pf
) const
{
    const primitivePatch& boundary = boundaryPtr_();

    Field<Type>& pfi = pf.internalField();

    // Get face data in flat list
    tmp<Field<Type> > tboundaryVals(flatBoundaryField(vf));
    const Field<Type>& boundaryVals = tboundaryVals();


    // Do points on 'normal' patches from the surrounding patch faces
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    forAll(boundary.meshPoints(), i)
    {
        label pointI = boundary.meshPoints()[i];

        if (isPatchPoint_[pointI])
        {
            const labelList& pFaces = boundary.pointFaces()[i];
            const scalarList& pWeights = boundaryPointWeights_[i];

            Type& val = pfi[pointI];

            val = pTraits<Type>::zero;
            forAll(pFaces, j)
            {
                if (boundaryIsPatchFace_[pFaces[j]])
                {
                    val += pWeights[j]*boundaryVals[pFaces[j]];
                }
            }
        }
    }

    // Sum collocated contributions
    pointConstraints::syncUntransformedData(mesh(), pfi, plusEqOp<Type>());

    // And add separated contributions
    addSeparated(pf);

    // Push master data to slaves. It is possible (not sure how often) for
    // a coupled point to have its master on a different patch so
    // to make sure just push master data to slaves.
    pushUntransformedData(pfi);
}


template<class Type>
void CML::volPointInterpolation::interpolateBoundaryField
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    GeometricField<Type, pointPatchField, pointMesh>& pf,
    const bool overrideFixedValue
) const
{
    interpolateBoundaryField(vf, pf);

    // Apply constraints
    const pointConstraints& pcs = pointConstraints::New(pf.mesh());

    pcs.constrain(pf, overrideFixedValue);
}


template<class Type>
void CML::volPointInterpolation::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    GeometricField<Type, pointPatchField, pointMesh>& pf
) const
{
    if (debug)
    {
        Pout<< "volPointInterpolation::interpolate("
            << "const GeometricField<Type, fvPatchField, volMesh>&, "
            << "GeometricField<Type, pointPatchField, pointMesh>&) : "
            << "interpolating field from cells to points"
            << endl;
    }

    interpolateInternalField(vf, pf);

    // Interpolate to the patches preserving fixed value BCs
    interpolateBoundaryField(vf, pf, false);
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::pointPatchField, CML::pointMesh> >
CML::volPointInterpolation::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const wordList& patchFieldTypes
) const
{
    const pointMesh& pm = pointMesh::New(vf.mesh());

    // Construct tmp<pointField>
    tmp<GeometricField<Type, pointPatchField, pointMesh> > tpf
    (
        new GeometricField<Type, pointPatchField, pointMesh>
        (
            IOobject
            (
                "volPointInterpolate(" + vf.name() + ')',
                vf.instance(),
                pm.thisDb()
            ),
            pm,
            vf.dimensions(),
            patchFieldTypes
        )
    );

    interpolateInternalField(vf, tpf());

    // Interpolate to the patches overriding fixed value BCs
    interpolateBoundaryField(vf, tpf(), true);

    return tpf;
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::pointPatchField, CML::pointMesh> >
CML::volPointInterpolation::interpolate
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const wordList& patchFieldTypes
) const
{
    // Construct tmp<pointField>
    tmp<GeometricField<Type, pointPatchField, pointMesh> > tpf =
        interpolate(tvf(), patchFieldTypes);
    tvf.clear();
    return tpf;
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::pointPatchField, CML::pointMesh> >
CML::volPointInterpolation::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name,
    const bool cache
) const
{
    typedef GeometricField<Type, pointPatchField, pointMesh> PointFieldType;

    const pointMesh& pm = pointMesh::New(vf.mesh());
    const objectRegistry& db = pm.thisDb();

    if (!cache || vf.mesh().changing())
    {
        // Delete any old occurrences to avoid double registration
        if (db.objectRegistry::template foundObject<PointFieldType>(name))
        {
            PointFieldType& pf = const_cast<PointFieldType&>
            (
                db.objectRegistry::template lookupObject<PointFieldType>(name)
            );

            if (pf.ownedByRegistry())
            {
                solution::cachePrintMessage("Deleting", name, vf);
                pf.release();
                delete &pf;
            }
        }


        tmp<GeometricField<Type, pointPatchField, pointMesh> > tpf
        (
            new GeometricField<Type, pointPatchField, pointMesh>
            (
                IOobject
                (
                    name,
                    vf.instance(),
                    pm.thisDb()
                ),
                pm,
                vf.dimensions()
            )
        );

        interpolate(vf, tpf());

        return tpf;
    }
    else
    {
        if (!db.objectRegistry::template foundObject<PointFieldType>(name))
        {
            solution::cachePrintMessage("Calculating and caching", name, vf);
            tmp<PointFieldType> tpf = interpolate(vf, name, false);
            PointFieldType* pfPtr = tpf.ptr();
            regIOobject::store(pfPtr);
            return *pfPtr;
        }
        else
        {
            PointFieldType& pf = const_cast<PointFieldType&>
            (
                db.objectRegistry::template lookupObject<PointFieldType>(name)
            );

            if (pf.upToDate(vf))    //TBD: , vf.mesh().points()))
            {
                solution::cachePrintMessage("Reusing", name, vf);
                return pf;
            }
            else
            {
                solution::cachePrintMessage("Deleting", name, vf);
                pf.release();
                delete &pf;

                solution::cachePrintMessage("Recalculating", name, vf);
                tmp<PointFieldType> tpf = interpolate(vf, name, false);

                solution::cachePrintMessage("Storing", name, vf);
                PointFieldType* pfPtr = tpf.ptr();
                regIOobject::store(pfPtr);

                // Note: return reference, not pointer
                return *pfPtr;
            }
        }
    }
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::pointPatchField, CML::pointMesh> >
CML::volPointInterpolation::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return interpolate(vf, "volPointInterpolate(" + vf.name() + ')', false);
}


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::pointPatchField, CML::pointMesh> >
CML::volPointInterpolation::interpolate
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
) const
{
    // Construct tmp<pointField>
    tmp<GeometricField<Type, pointPatchField, pointMesh> > tpf =
        interpolate(tvf());
    tvf.clear();
    return tpf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
