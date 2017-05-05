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
    CML::calculatedFvsPatchField

Description
    CML::calculatedFvsPatchField


\*---------------------------------------------------------------------------*/

#ifndef calculatedFvsPatchField_H
#define calculatedFvsPatchField_H

#include "fvsPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class calculatedFvsPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class calculatedFvsPatchField
:
    public fvsPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("calculated");


    // Constructors

        //- Construct from patch and internal field
        calculatedFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct from patch, internal field and dictionary
        calculatedFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        calculatedFvsPatchField
        (
            const calculatedFvsPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        calculatedFvsPatchField
        (
            const calculatedFvsPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvsPatchField<Type> > clone() const
        {
            return tmp<fvsPatchField<Type> >
            (
                new calculatedFvsPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        calculatedFvsPatchField
        (
            const calculatedFvsPatchField<Type>&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvsPatchField<Type> > clone
        (
            const DimensionedField<Type, surfaceMesh>& iF
        ) const
        {
            return tmp<fvsPatchField<Type> >
            (
                new calculatedFvsPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return true if this patch field fixes a value.
            //  Needed to check if a level has to be specified while solving
            //  Poissons equations.
            virtual bool fixesValue() const
            {
                return true;
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvPatchFieldMapper.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
const word& fvsPatchField<Type>::calculatedType()
{
    return calculatedFvsPatchField<Type>::typeName;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
calculatedFvsPatchField<Type>::calculatedFvsPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF
)
:
    fvsPatchField<Type>(p, iF)
{}


template<class Type>
calculatedFvsPatchField<Type>::calculatedFvsPatchField
(
    const calculatedFvsPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fvsPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
calculatedFvsPatchField<Type>::calculatedFvsPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF,
    const dictionary& dict
)
:
    fvsPatchField<Type>(p, iF, Field<Type>("value", dict, p.size()))
{}


template<class Type>
calculatedFvsPatchField<Type>::calculatedFvsPatchField
(
    const calculatedFvsPatchField<Type>& ptf
)
:
    fvsPatchField<Type>(ptf)
{}


template<class Type>
calculatedFvsPatchField<Type>::calculatedFvsPatchField
(
    const calculatedFvsPatchField<Type>& ptf,
    const DimensionedField<Type, surfaceMesh>& iF
)
:
    fvsPatchField<Type>(ptf, iF)
{}


template<class Type>
tmp<fvsPatchField<Type> > fvsPatchField<Type>::NewCalculatedType
(
    const fvPatch& p
)
{
    typename patchConstructorTable::iterator patchTypeCstrIter =
        patchConstructorTablePtr_->find(p.type());

    if (patchTypeCstrIter != patchConstructorTablePtr_->end())
    {
        return patchTypeCstrIter()
        (
            p,
            DimensionedField<Type, surfaceMesh>::null()
        );
    }
    else
    {
        return tmp<fvsPatchField<Type> >
        (
            new calculatedFvsPatchField<Type>
            (
                p,
                DimensionedField<Type, surfaceMesh>::null()
            )
        );
    }
}


template<class Type>
template<class Type2>
tmp<fvsPatchField<Type> > fvsPatchField<Type>::NewCalculatedType
(
    const fvsPatchField<Type2>& pf
)
{
    return NewCalculatedType(pf.patch());
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
