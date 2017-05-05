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
    CML::calculatedPointPatchField

Description
    A calculated boundary condition for pointField


\*---------------------------------------------------------------------------*/

#ifndef calculatedPointPatchField_H
#define calculatedPointPatchField_H

#include "pointPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class calculatedPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class calculatedPointPatchField
:
    public pointPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("calculated");


    // Constructors

        //- Construct from patch and internal field
        calculatedPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        calculatedPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        calculatedPointPatchField
        (
            const calculatedPointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new calculatedPointPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        calculatedPointPatchField
        (
            const calculatedPointPatchField<Type>&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual autoPtr<pointPatchField<Type> >
        clone(const DimensionedField<Type, pointMesh>& iF) const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new calculatedPointPatchField<Type>
                (
                    *this,
                    iF
                )
            );
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
const word& pointPatchField<Type>::calculatedType()
{
    return calculatedPointPatchField<Type>::typeName;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
calculatedPointPatchField<Type>::calculatedPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    pointPatchField<Type>(p, iF)
{}


template<class Type>
calculatedPointPatchField<Type>::calculatedPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    pointPatchField<Type>(p, iF, dict)
{}


template<class Type>
calculatedPointPatchField<Type>::calculatedPointPatchField
(
    const calculatedPointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper&
)
:
    pointPatchField<Type>(p, iF)
{}


template<class Type>
calculatedPointPatchField<Type>::calculatedPointPatchField
(
    const calculatedPointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    pointPatchField<Type>(ptf, iF)
{}


template<class Type>
template<class Type2>
autoPtr<pointPatchField<Type> >
pointPatchField<Type>::NewCalculatedType
(
    const pointPatchField<Type2>& pf
)
{
    typename pointPatchConstructorTable::iterator patchTypeCstrIter =
        pointPatchConstructorTablePtr_->find(pf.patch().type());

    if (patchTypeCstrIter != pointPatchConstructorTablePtr_->end())
    {
        return autoPtr<pointPatchField<Type> >
        (
            patchTypeCstrIter()
            (
                pf.patch(),
                Field<Type>::null()
            )
        );
    }
    else
    {
        return autoPtr<pointPatchField<Type> >
        (
            new calculatedPointPatchField<Type>
            (
                pf.patch(),
                Field<Type>::null()
            )
        );
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
