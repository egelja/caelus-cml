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
    CML::turbulentInletFvPatchField

Description
    This boundary condition generates a fluctuating inlet condition by adding
    a random component to a reference (mean) field.

    \f[
        x_p = (1 - \alpha) x_p^{n-1} + \alpha (x_{ref} + s C_{RMS} x_{ref})
    \f]

    where

    \vartable
        x_p     | patch values
        x_{ref} | reference patch values
        n       | time level
        \alpha  | fraction of new random component added to previous time value
        C_{RMS} | RMS coefficient
        s       | fluctuation scale
    \endvartable

    \heading Patch usage

    \table
        Property     | Description             | Required    | Default value
        fluctuationScale | RMS fluctuation scale (fraction of mean) | yes |
        referenceField | reference (mean) field | yes        |
        alpha | fraction of new random component added to previous| no| 0.1
    \endtable

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            turbulentInlet;
        fluctuationScale 0.1;
        referenceField  uniform 10;
        alpha           0.1;
    }
    \endverbatim

SeeAlso
    CML::fixedValueFvPatchField

SourceFiles
    turbulentInletFvPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef turbulentInletFvPatchField_H
#define turbulentInletFvPatchField_H

#include "Random.hpp"
#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class turbulentInletFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class turbulentInletFvPatchField
:
    public fixedValueFvPatchField<Type>
{
    // Private data

        //- Random number generator
        Random ranGen_;

        //- Fluctuation scale
        Type fluctuationScale_;

        //- Reference field
        Field<Type> referenceField_;

        //- Fraction of RMS component to apply to last time step values
        scalar alpha_;

        //- Current time index (used for updating)
        label curTimeIndex_;


public:

    //- Runtime type information
    TypeName("turbulentInlet");


    // Constructors

        //- Construct from patch and internal field
        turbulentInletFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        turbulentInletFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given turbulentInletFvPatchField
        //  onto a new patch
        turbulentInletFvPatchField
        (
            const turbulentInletFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        turbulentInletFvPatchField
        (
            const turbulentInletFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new turbulentInletFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        turbulentInletFvPatchField
        (
            const turbulentInletFvPatchField<Type>&,
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
                new turbulentInletFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return the fluctuation scale
            const Type& fluctuationScale() const
            {
                return fluctuationScale_;
            }

            //- Return reference to the fluctuation scale to allow adjustment
            Type& fluctuationScale()
            {
                return fluctuationScale_;
            }

            //- Return the reference field
            const Field<Type>& referenceField() const
            {
                return referenceField_;
            }

            //- Return reference to the reference field to allow adjustment
            Field<Type>& referenceField()
            {
                return referenceField_;
            }


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchField<Type>&,
                const labelList&
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
turbulentInletFvPatchField<Type>::turbulentInletFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(p, iF),
    ranGen_(label(0)),
    fluctuationScale_(pTraits<Type>::zero),
    referenceField_(p.size()),
    alpha_(0.1),
    curTimeIndex_(-1)
{}


template<class Type>
turbulentInletFvPatchField<Type>::turbulentInletFvPatchField
(
    const turbulentInletFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<Type>(ptf, p, iF, mapper),
    ranGen_(label(0)),
    fluctuationScale_(ptf.fluctuationScale_),
    referenceField_(ptf.referenceField_, mapper),
    alpha_(ptf.alpha_),
    curTimeIndex_(-1)
{}


template<class Type>
turbulentInletFvPatchField<Type>::turbulentInletFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<Type>(p, iF),
    ranGen_(label(0)),
    fluctuationScale_(pTraits<Type>(dict.lookup("fluctuationScale"))),
    referenceField_("referenceField", dict, p.size()),
    alpha_(dict.lookupOrDefault<scalar>("alpha", 0.1)),
    curTimeIndex_(-1)
{
    if (dict.found("value"))
    {
        fixedValueFvPatchField<Type>::operator==
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else
    {
        fixedValueFvPatchField<Type>::operator==(referenceField_);
    }
}


template<class Type>
turbulentInletFvPatchField<Type>::turbulentInletFvPatchField
(
    const turbulentInletFvPatchField<Type>& ptf
)
:
    fixedValueFvPatchField<Type>(ptf),
    ranGen_(ptf.ranGen_),
    fluctuationScale_(ptf.fluctuationScale_),
    referenceField_(ptf.referenceField_),
    alpha_(ptf.alpha_),
    curTimeIndex_(-1)
{}


template<class Type>
turbulentInletFvPatchField<Type>::turbulentInletFvPatchField
(
    const turbulentInletFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(ptf, iF),
    ranGen_(ptf.ranGen_),
    fluctuationScale_(ptf.fluctuationScale_),
    referenceField_(ptf.referenceField_),
    alpha_(ptf.alpha_),
    curTimeIndex_(-1)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void turbulentInletFvPatchField<Type>::autoMap
(
    const fvPatchFieldMapper& m
)
{
    fixedValueFvPatchField<Type>::autoMap(m);
    referenceField_.autoMap(m);
}


template<class Type>
void turbulentInletFvPatchField<Type>::rmap
(
    const fvPatchField<Type>& ptf,
    const labelList& addr
)
{
    fixedValueFvPatchField<Type>::rmap(ptf, addr);

    const turbulentInletFvPatchField<Type>& tiptf =
        refCast<const turbulentInletFvPatchField<Type> >(ptf);

    referenceField_.rmap(tiptf.referenceField_, addr);
}


template<class Type>
void turbulentInletFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    if (curTimeIndex_ != this->db().time().timeIndex())
    {
        Field<Type>& patchField = *this;

        Field<Type> randomField(this->size());

        forAll(patchField, facei)
        {
            randomField[facei] = ranGen_.sample01<Type>();
        }

        // Correction-factor to compensate for the loss of RMS fluctuation
        // due to the temporal correlation introduced by the alpha parameter.
        scalar rmsCorr = sqrt(12*(2*alpha_ - sqr(alpha_)))/alpha_;

        patchField =
            (1 - alpha_)*patchField
          + alpha_*
            (
                referenceField_
              + rmsCorr*cmptMultiply
                (
                    randomField - 0.5*pTraits<Type>::one,
                    fluctuationScale_
                )*mag(referenceField_)
            );

        curTimeIndex_ = this->db().time().timeIndex();
    }

    fixedValueFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void turbulentInletFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    os.writeKeyword("fluctuationScale")
        << fluctuationScale_ << token::END_STATEMENT << nl;
    referenceField_.writeEntry("referenceField", os);
    os.writeKeyword("alpha") << alpha_ << token::END_STATEMENT << nl;
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
