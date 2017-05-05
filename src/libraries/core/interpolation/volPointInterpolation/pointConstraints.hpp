/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2016 OpenFOAM Foundation
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

Class
    CML::pointConstraints

Description
    Application of (multi-)patch point contraints.

    Note: includes all points which are on the boundary of a patch
          with a constraint. It includes them (even though the constraint
          will already be implemented through the patch evaluation)
          since these points might be
          coupled to points which are not on any constraint patch and we
          don't want to get inconsistency between the two points.

SourceFiles
    pointConstraints.cpp

\*---------------------------------------------------------------------------*/

#ifndef pointConstraints_HPP
#define pointConstraints_HPP

#include "MeshObject.hpp"
#include "tensorField.hpp"
#include "pointFieldsFwd.hpp"
#include "pointConstraint.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class pointMesh;
class polyMesh;

/*---------------------------------------------------------------------------*\
                       Class pointConstraints Declaration
\*---------------------------------------------------------------------------*/

class pointConstraints
:
    public MeshObject<pointMesh, pointConstraints>
{
    // Private data

        // Patch-patch constraints

            //- Mesh points on which to apply special constraints
            labelList patchPatchPointConstraintPoints_;
            //- Special constraints (as tensors)
            tensorField patchPatchPointConstraintTensors_;
            //- Special constraints (raw)
            List<pointConstraint> patchPatchPointConstraints_;


    // Private Member Functions

        //- Make patch-patch constraints
        void makePatchPatchAddressing();

        //- Disallow default bitwise copy construct
        pointConstraints(const pointConstraints&);

        //- Disallow default bitwise assignment
        void operator=(const pointConstraints&);


public:

    // Declare name of the class and its debug switch
    ClassName("pointConstraints");


    // Constructors

        //- Constructor from pointMesh.
        explicit pointConstraints(const pointMesh&);


    //- Destructor
    ~pointConstraints();


    // Member functions

        // Access

            //- Mesh points on which to apply special constraints
            const labelList& patchPatchPointConstraintPoints() const
            {
                return patchPatchPointConstraintPoints_;
            }

            //- Special constraints
            const tensorField& patchPatchPointConstraintTensors() const
            {
                return patchPatchPointConstraintTensors_;
            }

            //- Actual constraints
            const List<pointConstraint>& patchPatchPointConstraints() const
            {
                return patchPatchPointConstraints_;
            }


        // Edit

            //- Update mesh topology using the morph engine
            void updateMesh(const mapPolyMesh&);

            //- Correct weighting factors for moving mesh.
            bool movePoints();


        // Interpolation functions

            //- Helper: sync data on collocated points only
            template<class Type, class CombineOp>
            static void syncUntransformedData
            (
                const polyMesh& mesh,
                List<Type>& pointData,
                const CombineOp& cop
            );

            //- Helper: set patchField values from internal values (on
            //  valuePointPatchFields). Opposite of
            //  pointPatchField::setInInternalField
            template<class Type>
            static void setPatchFields
            (
                GeometricField<Type, pointPatchField, pointMesh>&
            );

            //- Apply patch-patch constraints only
            template<class Type>
            void constrainCorners
            (
                GeometricField<Type, pointPatchField, pointMesh>& pf
            ) const;

            //- Apply boundary conditions (single-patch constraints) and
            //  patch-patch constraints on generic pointField.
            //  Optionally override valuePointPatchFields with constrained
            //  values (see setPatchFields above)
            template<class Type>
            void constrain
            (
                GeometricField<Type, pointPatchField, pointMesh>& pf,
                const bool overrideValue = false
            ) const;

            //- Apply boundary conditions (single-patch constraints),
            //  patch-patch constraints and
            //  two-D constraints on displacement field
            void constrainDisplacement
            (
                pointVectorField& displacement,
                const bool overrideValue = false
            ) const;
};


template<>
void pointConstraints::constrainCorners<scalar>
(
    GeometricField<scalar, pointPatchField, pointMesh>& pf
) const;
template<>
void pointConstraints::constrainCorners<label>
(
    GeometricField<label, pointPatchField, pointMesh>& pf
) const;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "pointFields.hpp"
#include "valuePointPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, class CombineOp>
void CML::pointConstraints::syncUntransformedData
(
    const polyMesh& mesh,
    List<Type>& pointData,
    const CombineOp& cop
)
{
    // Transfer onto coupled patch
    const globalMeshData& gmd = mesh.globalData();
    const indirectPrimitivePatch& cpp = gmd.coupledPatch();
    const labelList& meshPoints = cpp.meshPoints();

    const mapDistribute& slavesMap = gmd.globalCoPointSlavesMap();
    const labelListList& slaves = gmd.globalCoPointSlaves();

    List<Type> elems(slavesMap.constructSize());
    forAll(meshPoints, i)
    {
        elems[i] = pointData[meshPoints[i]];
    }

    // Pull slave data onto master. No need to update transformed slots.
    slavesMap.distribute(elems, false);

    // Combine master data with slave data
    forAll(slaves, i)
    {
        Type& elem = elems[i];

        const labelList& slavePoints = slaves[i];

        // Combine master with untransformed slave data
        forAll(slavePoints, j)
        {
            cop(elem, elems[slavePoints[j]]);
        }

        // Copy result back to slave slots
        forAll(slavePoints, j)
        {
            elems[slavePoints[j]] = elem;
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
void CML::pointConstraints::setPatchFields
(
    GeometricField<Type, pointPatchField, pointMesh>& pf
)
{
    typename GeometricField<Type, pointPatchField, pointMesh>::
        GeometricBoundaryField& pfbf = pf.boundaryField();

    forAll(pfbf, patchi)
    {
        pointPatchField<Type>& ppf = pfbf[patchi];

        if (isA<valuePointPatchField<Type> >(ppf))
        {
            refCast<valuePointPatchField<Type> >(ppf) =
                ppf.patchInternalField();
        }
    }
}


template<class Type>
void CML::pointConstraints::constrainCorners
(
    GeometricField<Type, pointPatchField, pointMesh>& pf
) const
{
    forAll(patchPatchPointConstraintPoints_, pointi)
    {
        pf[patchPatchPointConstraintPoints_[pointi]] = transform
        (
            patchPatchPointConstraintTensors_[pointi],
            pf[patchPatchPointConstraintPoints_[pointi]]
        );
    }
}


template<class Type>
void CML::pointConstraints::constrain
(
    GeometricField<Type, pointPatchField, pointMesh>& pf,
    const bool overrideFixedValue
) const
{
    // Override constrained pointPatchField types with the constraint value.
    // This relies on only constrained pointPatchField implementing the evaluate
    // function
    pf.correctBoundaryConditions();

    // Sync any dangling points
    syncUntransformedData
    (
        mesh()(),
        pf.internalField(),
        maxMagSqrEqOp<Type>()
    );

    // Apply multiple constraints on edge/corner points
    constrainCorners(pf);

    if (overrideFixedValue)
    {
        setPatchFields(pf);
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
