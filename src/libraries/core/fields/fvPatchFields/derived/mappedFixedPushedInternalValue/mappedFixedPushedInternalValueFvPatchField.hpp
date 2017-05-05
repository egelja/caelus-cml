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
    CML::mappedFixedPushedInternalValueFvPatchField

Description
    This boundary condition maps the boundary values of a neighbour patch
    field to the boundary and internal cell values of *this.

    \heading Patch usage

    \table
        Property     | Description             | Required    | Default value
        fieldName    | name of field to be mapped | no       | this field name
        setAverage   | flag to activate setting of average value | yes |
        average      | average value to apply if \c setAverage = yes | yes |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            mappedFixedPushedInternalValue;
        fieldName       T;
        setAverage      no;
        average         0;
        value           uniform 0;
    }
    \endverbatim

Note
    This boundary condition can only be applied to patches that are of
    the \c mappedPolyPatch type.

SeeAlso
    CML::mappedPatchBase
    CML::mappedPolyPatch
    CML::mappedFvPatch
    CML::mappedFixedValueFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef mappedFixedPushedInternalValueFvPatchField_H
#define mappedFixedPushedInternalValueFvPatchField_H

#include "mappedFixedValueFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
        Class mappedFixedPushedInternalValueFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class mappedFixedPushedInternalValueFvPatchField
:
    public mappedFixedValueFvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("mappedFixedPushedInternalValue");


    // Constructors

        //- Construct from patch and internal field
        mappedFixedPushedInternalValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mappedFixedPushedInternalValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given a
        //  mappedFixedPushedInternalValueFvPatchField  onto a new patch
        mappedFixedPushedInternalValueFvPatchField
        (
            const mappedFixedPushedInternalValueFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        mappedFixedPushedInternalValueFvPatchField
        (
            const mappedFixedPushedInternalValueFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new mappedFixedPushedInternalValueFvPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        mappedFixedPushedInternalValueFvPatchField
        (
            const mappedFixedPushedInternalValueFvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<Type> > clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<Type> >
            (
                new mappedFixedPushedInternalValueFvPatchField<Type>
                (
                    *this,
                    iF
                )
            );
        }


    // Member functions

        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "UIndirectList.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::mappedFixedPushedInternalValueFvPatchField<Type>::
mappedFixedPushedInternalValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    mappedFixedValueFvPatchField<Type>(p, iF)
{}


template<class Type>
CML::mappedFixedPushedInternalValueFvPatchField<Type>::
mappedFixedPushedInternalValueFvPatchField
(
    const mappedFixedPushedInternalValueFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mappedFixedValueFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
CML::mappedFixedPushedInternalValueFvPatchField<Type>::
mappedFixedPushedInternalValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    mappedFixedValueFvPatchField<Type>(p, iF, dict)
{}


template<class Type>
CML::mappedFixedPushedInternalValueFvPatchField<Type>::
mappedFixedPushedInternalValueFvPatchField
(
    const mappedFixedPushedInternalValueFvPatchField<Type>& ptf
)
:
    mappedFixedValueFvPatchField<Type>(ptf)
{}


template<class Type>
CML::mappedFixedPushedInternalValueFvPatchField<Type>::
mappedFixedPushedInternalValueFvPatchField
(
    const mappedFixedPushedInternalValueFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    mappedFixedValueFvPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::mappedFixedPushedInternalValueFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    // Retrieve the neighbour values and assign to this patch boundary field
    mappedFixedValueFvPatchField<Type>::updateCoeffs();

    // Assign the patch internal field to its boundary value
    Field<Type>& intFld = const_cast<Field<Type>&>(this->internalField());
    UIndirectList<Type>(intFld, this->patch().faceCells()) = *this;
}


template<class Type>
void CML::mappedFixedPushedInternalValueFvPatchField<Type>::write
(
    Ostream& os
) const
{
    mappedFixedValueFvPatchField<Type>::write(os);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
