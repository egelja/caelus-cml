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
    CML::freestreamFvPatchField

Description
    This boundary condition provides a free-stream condition.  It is a 'mixed'
    condition derived from the \c inletOutlet condition, whereby the mode of
    operation switches between fixed (free stream) value and zero gradient
    based on the sign of the flux.

    \heading Patch usage

    \table
        Property     | Description             | Required    | Default value
        freestreamValue   | freestream velocity          | yes         |
        phi          | flux field name         | no          | phi
    \endtable

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            freestream;
        phi             phi;
    }
    \endverbatim

SeeAlso
    CML::mixedFvPatchField
    CML::inletOutletFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef freestreamFvPatchField_H
#define freestreamFvPatchField_H

#include "inletOutletFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class freestreamFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class freestreamFvPatchField
:
    public inletOutletFvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("freestream");


    // Constructors

        //- Construct from patch and internal field
        freestreamFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        freestreamFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given freestreamFvPatchField onto a new patch
        freestreamFvPatchField
        (
            const freestreamFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        freestreamFvPatchField
        (
            const freestreamFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new freestreamFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        freestreamFvPatchField
        (
            const freestreamFvPatchField<Type>&,
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
                new freestreamFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Return defining fields

            const Field<Type>& freestreamValue() const
            {
                return this->refValue();
            }

            Field<Type>& freestreamValue()
            {
                return this->refValue();
            }


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
freestreamFvPatchField<Type>::freestreamFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    inletOutletFvPatchField<Type>(p, iF)
{}


template<class Type>
freestreamFvPatchField<Type>::freestreamFvPatchField
(
    const freestreamFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    inletOutletFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
freestreamFvPatchField<Type>::freestreamFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    inletOutletFvPatchField<Type>(p, iF)
{
    this->phiName_ = dict.lookupOrDefault<word>("phi","phi");

    freestreamValue() = Field<Type>("freestreamValue", dict, p.size());

    if (dict.found("value"))
    {
        fvPatchField<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else
    {
        fvPatchField<Type>::operator=(freestreamValue());
    }
}


template<class Type>
freestreamFvPatchField<Type>::freestreamFvPatchField
(
    const freestreamFvPatchField<Type>& ptf
)
:
    inletOutletFvPatchField<Type>(ptf)
{}


template<class Type>
freestreamFvPatchField<Type>::freestreamFvPatchField
(
    const freestreamFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    inletOutletFvPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void freestreamFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    if (this->phiName_ != "phi")
    {
        os.writeKeyword("phi")
            << this->phiName_ << token::END_STATEMENT << nl;
    }
    freestreamValue().writeEntry("freestreamValue", os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
