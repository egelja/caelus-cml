/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    CML::uniformFixedValuePointPatchField

Description
    Enables the specification of a uniform fixed value boundary condition.

    Example of the boundary condition specification:
    \verbatim
    inlet
    {
        type            uniformFixedValue;
        uniformValue    constant 0.2;
    }
    \endverbatim

    The uniformValue entry is a DataEntry type, able to describe time
    varying functions.  The example above gives the usage for supplying a
    constant value.

SourceFiles
    uniformFixedValuePointPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef uniformFixedValuePointPatchField_H
#define uniformFixedValuePointPatchField_H

#include "fixedValuePointPatchField.hpp"
#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
          Class uniformFixedValuePointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class uniformFixedValuePointPatchField
:
    public fixedValuePointPatchField<Type>
{
    // Private data

        autoPtr<DataEntry<Type> > uniformValue_;


public:

    //- Runtime type information
    TypeName("uniformFixedValue");


    // Constructors

        //- Construct from patch and internal field
        uniformFixedValuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        uniformFixedValuePointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        uniformFixedValuePointPatchField
        (
            const uniformFixedValuePointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy
        uniformFixedValuePointPatchField
        (
            const uniformFixedValuePointPatchField<Type>&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new uniformFixedValuePointPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        uniformFixedValuePointPatchField
        (
            const uniformFixedValuePointPatchField<Type>&,
            const DimensionedField<Type, pointMesh>&
        );


        //- Construct and return a clone setting internal field reference
        virtual autoPtr<pointPatchField<Type> > clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new uniformFixedValuePointPatchField<Type>
                (
                    *this,
                    iF
                )
            );
        }


    // Member functions

        // Access

            //- Return the fluctuation scale
            const DataEntry<Type>& uniformValue() const
            {
                return uniformValue_;
            }

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

// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * * //

template<class Type>
uniformFixedValuePointPatchField<Type>::
uniformFixedValuePointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    fixedValuePointPatchField<Type>(p, iF),
    uniformValue_()
{}


template<class Type>
uniformFixedValuePointPatchField<Type>::
uniformFixedValuePointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    fixedValuePointPatchField<Type>(p, iF, dict, false),
    uniformValue_(DataEntry<Type>::New("uniformValue", dict))
{
    if (dict.found("value"))
    {
        fixedValuePointPatchField<Type>::operator==
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else
    {
        const scalar t = this->db().time().timeOutputValue();
        fixedValuePointPatchField<Type>::operator=(uniformValue_->value(t));
    }
}


template<class Type>
uniformFixedValuePointPatchField<Type>::
uniformFixedValuePointPatchField
(
    const uniformFixedValuePointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    fixedValuePointPatchField<Type>(ptf, p, iF, mapper),
    uniformValue_(ptf.uniformValue_().clone().ptr())
{
    // For safety re-evaluate
    const scalar t = this->db().time().timeOutputValue();
    fixedValuePointPatchField<Type>::operator=(uniformValue_->value(t));
}


template<class Type>
uniformFixedValuePointPatchField<Type>::
uniformFixedValuePointPatchField
(
    const uniformFixedValuePointPatchField<Type>& ptf
)
:
    fixedValuePointPatchField<Type>(ptf),
    uniformValue_(ptf.uniformValue_().clone().ptr())
{}


template<class Type>
uniformFixedValuePointPatchField<Type>::
uniformFixedValuePointPatchField
(
    const uniformFixedValuePointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    fixedValuePointPatchField<Type>(ptf, iF),
    uniformValue_(ptf.uniformValue_().clone().ptr())
{
    // For safety re-evaluate
    const scalar t = this->db().time().timeOutputValue();
    fixedValuePointPatchField<Type>::operator==(uniformValue_->value(t));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void uniformFixedValuePointPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    const scalar t = this->db().time().timeOutputValue();
    fixedValuePointPatchField<Type>::operator==(uniformValue_->value(t));

    fixedValuePointPatchField<Type>::updateCoeffs();
}


template<class Type>
void uniformFixedValuePointPatchField<Type>::
write(Ostream& os) const
{
    // Note: write value
    fixedValuePointPatchField<Type>::write(os);
    uniformValue_->writeData(os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ******************************

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
