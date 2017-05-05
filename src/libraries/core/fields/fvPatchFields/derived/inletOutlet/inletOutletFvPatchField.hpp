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
    CML::inletOutletFvPatchField

Description
    CML::inletOutletFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef inletOutletFvPatchField_H
#define inletOutletFvPatchField_H

#include "mixedFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class inletOutletFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class inletOutletFvPatchField
:
    public mixedFvPatchField<Type>
{

protected:

    // Protected data

        //- Name of flux field
        word phiName_;


public:

    //- Runtime type information
    TypeName("inletOutlet");


    // Constructors

        //- Construct from patch and internal field
        inletOutletFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        inletOutletFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given inletOutletFvPatchField onto a new patch
        inletOutletFvPatchField
        (
            const inletOutletFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        inletOutletFvPatchField
        (
            const inletOutletFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new inletOutletFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        inletOutletFvPatchField
        (
            const inletOutletFvPatchField<Type>&,
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
                new inletOutletFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;


    // Member operators

        virtual void operator=(const fvPatchField<Type>& pvf);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::inletOutletFvPatchField<Type>::inletOutletFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    mixedFvPatchField<Type>(p, iF),
    phiName_("phi")
{
    this->refValue() = pTraits<Type>::zero;
    this->refGrad() = pTraits<Type>::zero;
    this->valueFraction() = 0.0;
}


template<class Type>
CML::inletOutletFvPatchField<Type>::inletOutletFvPatchField
(
    const inletOutletFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchField<Type>(ptf, p, iF, mapper),
    phiName_(ptf.phiName_)
{}


template<class Type>
CML::inletOutletFvPatchField<Type>::inletOutletFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchField<Type>(p, iF),
    phiName_(dict.lookupOrDefault<word>("phi", "phi"))
{
    this->refValue() = Field<Type>("inletValue", dict, p.size());

    if (dict.found("value"))
    {
        fvPatchField<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else
    {
        fvPatchField<Type>::operator=(this->refValue());
    }

    this->refGrad() = pTraits<Type>::zero;
    this->valueFraction() = 0.0;
}


template<class Type>
CML::inletOutletFvPatchField<Type>::inletOutletFvPatchField
(
    const inletOutletFvPatchField<Type>& ptf
)
:
    mixedFvPatchField<Type>(ptf),
    phiName_(ptf.phiName_)
{}


template<class Type>
CML::inletOutletFvPatchField<Type>::inletOutletFvPatchField
(
    const inletOutletFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    mixedFvPatchField<Type>(ptf, iF),
    phiName_(ptf.phiName_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::inletOutletFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    const Field<scalar>& phip =
        this->patch().template lookupPatchField<surfaceScalarField, scalar>
        (
            phiName_
        );

    this->valueFraction() = 1.0 - pos(phip);

    mixedFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void CML::inletOutletFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    if (phiName_ != "phi")
    {
        os.writeKeyword("phi") << phiName_ << token::END_STATEMENT << nl;
    }
    this->refValue().writeEntry("inletValue", os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void CML::inletOutletFvPatchField<Type>::operator=
(
    const fvPatchField<Type>& ptf
)
{
    fvPatchField<Type>::operator=
    (
        this->valueFraction()*this->refValue()
        + (1 - this->valueFraction())*ptf
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
