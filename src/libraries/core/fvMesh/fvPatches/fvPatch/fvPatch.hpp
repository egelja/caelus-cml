/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::fvPatch

Description
    A finiteVolume patch using a polyPatch and a fvBoundaryMesh

SourceFiles
    fvPatch.cpp
    fvPatchNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef fvPatch_H
#define fvPatch_H

#include "polyPatch.hpp"
#include "labelList.hpp"
#include "SubList.hpp"
#include "typeInfo.hpp"
#include "tmp.hpp"
#include "primitiveFields.hpp"
#include "SubField.hpp"
#include "fvPatchFieldsFwd.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvBoundaryMesh;
class surfaceInterpolation;

/*---------------------------------------------------------------------------*\
                           Class fvPatch Declaration
\*---------------------------------------------------------------------------*/

class fvPatch
{
    // Private data

        //- Reference to the underlying polyPatch
        const polyPatch& polyPatch_;

        //- Reference to boundary mesh
        const fvBoundaryMesh& boundaryMesh_;


    // Private Member Functions

        //- Disallow construct as copy
        fvPatch(const fvPatch&);

        //- Disallow assignment
        void operator=(const fvPatch&);


protected:

    // Protected Member Functions

        //- Make patch weighting factors
        virtual void makeWeights(scalarField&) const;

        //- Initialise the patches for moving points
        virtual void initMovePoints();

        //- Correct patches after moving points
        virtual void movePoints();


public:

    typedef fvBoundaryMesh BoundaryMesh;

    friend class fvBoundaryMesh;
    friend class surfaceInterpolation;

    //- Runtime type information
    TypeName(polyPatch::typeName_());


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            autoPtr,
            fvPatch,
            polyPatch,
            (const polyPatch& patch, const fvBoundaryMesh& bm),
            (patch, bm)
        );


    // Constructors

        //- Construct from polyPatch and fvBoundaryMesh
        fvPatch(const polyPatch&, const fvBoundaryMesh&);


    // Selectors

        //- Return a pointer to a new patch created on freestore from polyPatch
        static autoPtr<fvPatch> New
        (
            const polyPatch&,
            const fvBoundaryMesh&
        );


    //- Destructor
    virtual ~fvPatch();


    // Member Functions

    // Access

            //- Return the polyPatch
            const polyPatch& patch() const
            {
                return polyPatch_;
            }

            //- Return name
            const word& name() const
            {
                return polyPatch_.name();
            }

            //- Return start label of this patch in the polyMesh face list
            label start() const
            {
                return polyPatch_.start();
            }

            //- Return size
            virtual label size() const
            {
                return polyPatch_.size();
            }

            //- Return true if this patch is coupled
            virtual bool coupled() const
            {
                return polyPatch_.coupled();
            }

            //- Return true if the given type is a constraint type
            static bool constraintType(const word& pt);

            //- Return a list of all the constraint patch types
            static wordList constraintTypes();

            //- Return the index of this patch in the fvBoundaryMesh
            label index() const
            {
                return polyPatch_.index();
            }

            //- Return boundaryMesh reference
            const fvBoundaryMesh& boundaryMesh() const
            {
                return boundaryMesh_;
            }

            //- Slice list to patch
            template<class T>
            const typename List<T>::subList patchSlice(const List<T>& l) const
            {
                return typename List<T>::subList(l, size(), start());
            }

            //- Return faceCells
            virtual const labelUList& faceCells() const;


        // Access functions for geometrical data

            //- Return face centres
            const vectorField& Cf() const;

            //- Return neighbour cell centres
            tmp<vectorField> Cn() const;

            //- Return face area vectors
            const vectorField& Sf() const;

            //- Return face area magnitudes
            const scalarField& magSf() const;

            //- Return face normals
            tmp<vectorField> nf() const;

            //- Return cell-centre to face-centre vector
            //  except for coupled patches for which the cell-centre
            //  to coupled-cell-centre vector is returned
            virtual tmp<vectorField> delta() const;

            //- Return cell-centre to face-centre vector
            virtual tmp<vectorField> deltaFull() const;



        // Access functions for demand driven data

            //- Return patch weighting factors
            const scalarField& weights() const;

            //- Return the face - cell distance coeffient
            //  except for coupled patches for which the cell-centre
            //  to coupled-cell-centre distance coeffient is returned
            const scalarField& deltaCoeffs() const;


        // Evaluation functions

            //- Return given internal field next to patch as patch field
            template<class Type>
            tmp<Field<Type> > patchInternalField(const UList<Type>&) const;

            //- Return given internal field next to patch as patch field
            template<class Type>
            void patchInternalField(const UList<Type>&, Field<Type>&) const;

            //- Return the corresponding patchField of the named field
            template<class GeometricField, class Type>
            const typename GeometricField::PatchFieldType& patchField
            (
                const GeometricField&
            ) const;

            //- Lookup and return the patchField of the named field from the
            //  local objectRegistry.
            //  N.B.  The dummy pointer arguments are used if this function is
            //  instantiated within a templated function to avoid a bug in gcc.
            //  See inletOutletFvPatchField.C and outletInletFvPatchField.C
            template<class GeometricField, class Type>
            const typename GeometricField::PatchFieldType& lookupPatchField
            (
                const word& name,
                const GeometricField* = nullptr,
                const Type* = nullptr
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::Field<Type> > CML::fvPatch::patchInternalField
(
    const UList<Type>& f
) const
{
    tmp<Field<Type> > tpif(new Field<Type>(size()));
    Field<Type>& pif = tpif();

    const labelUList& faceCells = this->faceCells();

    forAll(pif, facei)
    {
        pif[facei] = f[faceCells[facei]];
    }

    return tpif;
}


template<class Type>
void CML::fvPatch::patchInternalField
(
    const UList<Type>& f,
    Field<Type>& pif
) const
{
    pif.setSize(size());

    const labelUList& faceCells = this->faceCells();

    forAll(pif, facei)
    {
        pif[facei] = f[faceCells[facei]];
    }
}


template<class GeometricField, class Type>
const typename GeometricField::PatchFieldType& CML::fvPatch::patchField
(
    const GeometricField& gf
) const
{
    return gf.boundaryField()[index()];
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
