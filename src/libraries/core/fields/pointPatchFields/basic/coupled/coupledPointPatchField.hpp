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
    CML::coupledPointPatchField

Description
    A Coupled boundary condition for pointField


\*---------------------------------------------------------------------------*/

#ifndef coupledPointPatchField_H
#define coupledPointPatchField_H

#include "pointPatchField.hpp"
#include "coupledPointPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class coupledPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class coupledPointPatchField
:
    public pointPatchField<Type>
{

public:

    //- Runtime type information
    TypeName(coupledPointPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        coupledPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        coupledPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary& dict
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        coupledPointPatchField
        (
            const coupledPointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const = 0;

        //- Construct as copy setting internal field reference
        coupledPointPatchField
        (
            const coupledPointPatchField<Type>&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual autoPtr<pointPatchField<Type> > clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const = 0;


    // Member functions

        // Access

            //- Return true if this patch field is derived from
            //  coupledFvPatchField<Type>.
            virtual bool coupled() const
            {
                return true;
            }

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            ) = 0;

            //- Initialise swap of patch point values
            virtual void initSwapAddSeparated
            (
                const Pstream::commsTypes,
                Field<Type>&
            ) const
            {}

            //- Complete swap of patch point values and add to local values
            virtual void swapAddSeparated
            (
                const Pstream::commsTypes,
                Field<Type>&
            ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * * //

template<class Type>
coupledPointPatchField<Type>::coupledPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    pointPatchField<Type>(p, iF)
{}


template<class Type>
coupledPointPatchField<Type>::coupledPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    pointPatchField<Type>(p, iF, dict)
{}


template<class Type>
coupledPointPatchField<Type>::coupledPointPatchField
(
    const coupledPointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper&
)
:
    pointPatchField<Type>(p, iF)
{}


template<class Type>
coupledPointPatchField<Type>::coupledPointPatchField
(
    const coupledPointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    pointPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
