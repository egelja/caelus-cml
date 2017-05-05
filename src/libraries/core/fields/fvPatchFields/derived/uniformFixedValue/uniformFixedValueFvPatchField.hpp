/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::uniformFixedValueFvPatchField

Description
    CML::uniformFixedValueFvPatchField

SourceFiles
    uniformFixedValueFvPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef uniformFixedValueFvPatchField_H
#define uniformFixedValueFvPatchField_H

#include "Random.hpp"
#include "fixedValueFvPatchFields.hpp"
#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class uniformFixedValueFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class uniformFixedValueFvPatchField
:
    public fixedValueFvPatchField<Type>
{
    // Private data

        autoPtr<DataEntry<Type> > uniformValue_;


public:

    //- Runtime type information
    TypeName("uniformFixedValue");


    // Constructors

        //- Construct from patch and internal field
        uniformFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        uniformFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given uniformFixedValueFvPatchField
        //  onto a new patch
        uniformFixedValueFvPatchField
        (
            const uniformFixedValueFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        uniformFixedValueFvPatchField
        (
            const uniformFixedValueFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new uniformFixedValueFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        uniformFixedValueFvPatchField
        (
            const uniformFixedValueFvPatchField<Type>&,
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
                new uniformFixedValueFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );


        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
uniformFixedValueFvPatchField<Type>::uniformFixedValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(p, iF),
    uniformValue_()
{}


template<class Type>
uniformFixedValueFvPatchField<Type>::uniformFixedValueFvPatchField
(
    const uniformFixedValueFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper&
)
:
    fixedValueFvPatchField<Type>(p, iF),
    uniformValue_(ptf.uniformValue_().clone().ptr())
{
    const scalar t = this->db().time().timeOutputValue();
    fvPatchField<Type>::operator==(uniformValue_->value(t));
}


template<class Type>
uniformFixedValueFvPatchField<Type>::uniformFixedValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<Type>(p, iF),
    uniformValue_(DataEntry<Type>::New("uniformValue", dict))
{
    const scalar t = this->db().time().timeOutputValue();
    fvPatchField<Type>::operator==(uniformValue_->value(t));
}


template<class Type>
uniformFixedValueFvPatchField<Type>::uniformFixedValueFvPatchField
(
    const uniformFixedValueFvPatchField<Type>& ptf
)
:
    fixedValueFvPatchField<Type>(ptf),
    uniformValue_
    (
        ptf.uniformValue_.valid()
      ? ptf.uniformValue_().clone().ptr()
      : NULL
    )
{
    const scalar t = this->db().time().timeOutputValue();

    if (ptf.uniformValue_.valid())
    {
        fvPatchField<Type>::operator==(uniformValue_->value(t));
    }
}


template<class Type>
uniformFixedValueFvPatchField<Type>::uniformFixedValueFvPatchField
(
    const uniformFixedValueFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(ptf, iF),
    uniformValue_
    (
        ptf.uniformValue_.valid()
      ? ptf.uniformValue_().clone().ptr()
      : NULL
    )
{
    const scalar t = this->db().time().timeOutputValue();

    if (ptf.uniformValue_.valid())
    {
        fvPatchField<Type>::operator==(uniformValue_->value(t));
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void uniformFixedValueFvPatchField<Type>::autoMap
(
    const fvPatchFieldMapper& m
)
{
    this->setSize(m.size());
    const scalar t = this->db().time().timeOutputValue();
    fvPatchField<Type>::operator==(uniformValue_->value(t));
}


template<class Type>
void uniformFixedValueFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    const scalar t = this->db().time().timeOutputValue();
    fvPatchField<Type>::operator==(uniformValue_->value(t));

    fixedValueFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void uniformFixedValueFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    uniformValue_->writeData(os);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
