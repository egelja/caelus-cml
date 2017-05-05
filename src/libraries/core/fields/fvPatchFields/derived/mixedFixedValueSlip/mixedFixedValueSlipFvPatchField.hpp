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
    CML::mixedFixedValueSlipFvPatchField

Description
    A mixed boundary type that blends between fixedValue and slip, as opposed
    to the standard mixed condition that blends between fixedValue and
    fixedGradient; required to implement maxwellSlipU condition.

SourceFiles
    mixedFixedValueSlipFvPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef mixedFixedValueSlipFvPatchField_H
#define mixedFixedValueSlipFvPatchField_H

#include "transformFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class mixedFixedValueSlipFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class mixedFixedValueSlipFvPatchField
:
    public transformFvPatchField<Type>
{
    // Private data

        //- Value field used for boundary condition
        Field<Type> refValue_;

        //- Fraction (0-1) of value used for boundary condition
        scalarField valueFraction_;

public:

    //- Runtime type information
    TypeName("mixedFixedValueSlip");


    // Constructors

        //- Construct from patch and internal field
        mixedFixedValueSlipFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mixedFixedValueSlipFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given mixedFixedValueSlipFvPatchField
        //- onto a new patch
        mixedFixedValueSlipFvPatchField
        (
            const mixedFixedValueSlipFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        mixedFixedValueSlipFvPatchField
        (
            const mixedFixedValueSlipFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new mixedFixedValueSlipFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        mixedFixedValueSlipFvPatchField
        (
            const mixedFixedValueSlipFvPatchField<Type>&,
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
                new mixedFixedValueSlipFvPatchField<Type>(*this, iF)
            );
        }

    // Member functions

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

        // Return defining fields

            virtual Field<Type>& refValue()
            {
                return refValue_;
            }

            virtual const Field<Type>& refValue() const
            {
                return refValue_;
            }

            virtual scalarField& valueFraction()
            {
                return valueFraction_;
            }

            virtual const scalarField& valueFraction() const
            {
                return valueFraction_;
            }

        // Evaluation functions

            //- Return gradient at boundary
            virtual tmp<Field<Type> > snGrad() const;

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            );

            //- Return face-gradient transform diagonal
            virtual tmp<Field<Type> > snGradTransformDiag() const;


        //- Write
        virtual void write(Ostream&) const;


    // Member operators

        virtual void operator=(const UList<Type>&) {}

        virtual void operator=(const fvPatchField<Type>&) {}
        virtual void operator+=(const fvPatchField<Type>&) {}
        virtual void operator-=(const fvPatchField<Type>&) {}
        virtual void operator*=(const fvPatchField<scalar>&) {}
        virtual void operator/=(const fvPatchField<scalar>&) {}

        virtual void operator+=(const Field<Type>&) {}
        virtual void operator-=(const Field<Type>&) {}

        virtual void operator*=(const Field<scalar>&) {}
        virtual void operator/=(const Field<scalar>&) {}

        virtual void operator=(const Type&) {}
        virtual void operator+=(const Type&) {}
        virtual void operator-=(const Type&) {}
        virtual void operator*=(const scalar) {}
        virtual void operator/=(const scalar) {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "symmTransformField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
mixedFixedValueSlipFvPatchField<Type>::mixedFixedValueSlipFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    transformFvPatchField<Type>(p, iF),
    refValue_(p.size()),
    valueFraction_(p.size(), 1.0)
{}


template<class Type>
mixedFixedValueSlipFvPatchField<Type>::mixedFixedValueSlipFvPatchField
(
    const mixedFixedValueSlipFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    transformFvPatchField<Type>(ptf, p, iF, mapper),
    refValue_(ptf.refValue_, mapper),
    valueFraction_(ptf.valueFraction_, mapper)
{}


template<class Type>
mixedFixedValueSlipFvPatchField<Type>::mixedFixedValueSlipFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    transformFvPatchField<Type>(p, iF),
    refValue_("refValue", dict, p.size()),
    valueFraction_("valueFraction", dict, p.size())
{}


template<class Type>
mixedFixedValueSlipFvPatchField<Type>::mixedFixedValueSlipFvPatchField
(
    const mixedFixedValueSlipFvPatchField<Type>& ptf
)
:
    transformFvPatchField<Type>(ptf),
    refValue_(ptf.refValue_),
    valueFraction_(ptf.valueFraction_)
{}

template<class Type>
mixedFixedValueSlipFvPatchField<Type>::mixedFixedValueSlipFvPatchField
(
    const mixedFixedValueSlipFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    transformFvPatchField<Type>(ptf, iF),
    refValue_(ptf.refValue_),
    valueFraction_(ptf.valueFraction_)
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Map from self
template<class Type>
void mixedFixedValueSlipFvPatchField<Type>::autoMap
(
    const fvPatchFieldMapper& m
)
{
    Field<Type>::autoMap(m);
    refValue_.autoMap(m);
    valueFraction_.autoMap(m);
}


// Reverse-map the given fvPatchField onto this fvPatchField
template<class Type>
void mixedFixedValueSlipFvPatchField<Type>::rmap
(
    const fvPatchField<Type>& ptf,
    const labelList& addr
)
{
    transformFvPatchField<Type>::rmap(ptf, addr);

    const mixedFixedValueSlipFvPatchField<Type>& dmptf =
        refCast<const mixedFixedValueSlipFvPatchField<Type> >(ptf);

    refValue_.rmap(dmptf.refValue_, addr);
    valueFraction_.rmap(dmptf.valueFraction_, addr);
}


// Return gradient at boundary
template<class Type>
tmp<Field<Type> > mixedFixedValueSlipFvPatchField<Type>::snGrad() const
{
    tmp<vectorField> nHat = this->patch().nf();
    Field<Type> pif(this->patchInternalField());

    return
    (
        valueFraction_*refValue_
      + (1.0 - valueFraction_)*transform(I - sqr(nHat), pif) - pif
    )*this->patch().deltaCoeffs();
}


// Evaluate the field on the patch
template<class Type>
void mixedFixedValueSlipFvPatchField<Type>::evaluate(const Pstream::commsTypes)
{
    if (!this->updated())
    {
        this->updateCoeffs();
    }

    vectorField nHat(this->patch().nf());

    Field<Type>::operator=
    (
        valueFraction_*refValue_
      +
        (1.0 - valueFraction_)
       *transform(I - nHat*nHat, this->patchInternalField())
    );

    transformFvPatchField<Type>::evaluate();
}


// Return defining fields
template<class Type>
tmp<Field<Type> >
mixedFixedValueSlipFvPatchField<Type>::snGradTransformDiag() const
{
    vectorField nHat(this->patch().nf());
    vectorField diag(nHat.size());

    diag.replace(vector::X, mag(nHat.component(vector::X)));
    diag.replace(vector::Y, mag(nHat.component(vector::Y)));
    diag.replace(vector::Z, mag(nHat.component(vector::Z)));

	// CAELUSFIX
#   ifdef _WIN32
    typedef typename powProduct<vector, pTraits<Type>::rank>::type prodType;

     tmp<Field<prodType> > a =
          pow<vector, pTraits<Type>::rank>(diag, pTraits<prodType>::zero);

    return
        valueFraction_*Type(pTraits<Type>::one)
      + (1.0 - valueFraction_)*transformFieldMask<Type>(a);
#   else
    return
        valueFraction_*Type(pTraits<Type>::one)
      + (1.0 - valueFraction_)
       *transformFieldMask<Type>(pow<vector, pTraits<Type>::rank>(diag));
#   endif
}


// Write
template<class Type>
void mixedFixedValueSlipFvPatchField<Type>::write(Ostream& os) const
{
    transformFvPatchField<Type>::write(os);
    refValue_.writeEntry("refValue", os);
    valueFraction_.writeEntry("valueFraction", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
