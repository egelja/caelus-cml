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
    CML::directionMixedFvPatchField

Description
    CML::directionMixedFvPatchField

SourceFiles
    directionMixedFvPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef directionMixedFvPatchField_H
#define directionMixedFvPatchField_H

#include "transformFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class directionMixedFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class directionMixedFvPatchField
:
    public transformFvPatchField<Type>
{
    // Private data

        //- Value field
        Field<Type> refValue_;

        //- Normal gradient field
        Field<Type> refGrad_;

        //- Fraction (0-1) of value used for boundary condition
        symmTensorField valueFraction_;


public:

    //- Runtime type information
    TypeName("directionMixed");


    // Constructors

        //- Construct from patch and internal field
        directionMixedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        directionMixedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given directionMixedFvPatchField onto
        //  a new patch
        directionMixedFvPatchField
        (
            const directionMixedFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new directionMixedFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        directionMixedFvPatchField
        (
            const directionMixedFvPatchField<Type>&,
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
                new directionMixedFvPatchField<Type>(*this, iF)
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

            //- Return false: this patch field is not altered by assignment
            virtual bool assignable() const
            {
                return false;
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


        // Return defining fields

            virtual Field<Type>& refValue()
            {
                return refValue_;
            }

            virtual const Field<Type>& refValue() const
            {
                return refValue_;
            }

            virtual Field<Type>& refGrad()
            {
                return refGrad_;
            }

            virtual const Field<Type>& refGrad() const
            {
                return refGrad_;
            }

            virtual symmTensorField& valueFraction()
            {
                return valueFraction_;
            }

            virtual const symmTensorField& valueFraction() const
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

        virtual void operator=(const fvPatchField<Type>&) {}
        virtual void operator+=(const fvPatchField<Type>&) {}
        virtual void operator-=(const fvPatchField<Type>&) {}
        virtual void operator*=(const fvPatchField<Type>&) {}
        virtual void operator/=(const fvPatchField<Type>&) {}

        virtual void operator=(const Field<Type>&) {}
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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::directionMixedFvPatchField<Type>::directionMixedFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    transformFvPatchField<Type>(p, iF),
    refValue_(p.size()),
    refGrad_(p.size()),
    valueFraction_(p.size())
{}


template<class Type>
CML::directionMixedFvPatchField<Type>::directionMixedFvPatchField
(
    const directionMixedFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    transformFvPatchField<Type>(ptf, p, iF, mapper),
    refValue_(ptf.refValue_, mapper),
    refGrad_(ptf.refGrad_, mapper),
    valueFraction_(ptf.valueFraction_, mapper)
{}


template<class Type>
CML::directionMixedFvPatchField<Type>::directionMixedFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    transformFvPatchField<Type>(p, iF, dict),
    refValue_("refValue", dict, p.size()),
    refGrad_("refGradient", dict, p.size()),
    valueFraction_("valueFraction", dict, p.size())
{
    evaluate();
}


template<class Type>
CML::directionMixedFvPatchField<Type>::directionMixedFvPatchField
(
    const directionMixedFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    transformFvPatchField<Type>(ptf, iF),
    refValue_(ptf.refValue_),
    refGrad_(ptf.refGrad_),
    valueFraction_(ptf.valueFraction_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::directionMixedFvPatchField<Type>::autoMap
(
    const fvPatchFieldMapper& m
)
{
    transformFvPatchField<Type>::autoMap(m);
    refValue_.autoMap(m);
    refGrad_.autoMap(m);
    valueFraction_.autoMap(m);
}


template<class Type>
void CML::directionMixedFvPatchField<Type>::rmap
(
    const fvPatchField<Type>& ptf,
    const labelList& addr
)
{
    transformFvPatchField<Type>::rmap(ptf, addr);

    const directionMixedFvPatchField<Type>& dmptf =
        refCast<const directionMixedFvPatchField<Type> >(ptf);

    refValue_.rmap(dmptf.refValue_, addr);
    refGrad_.rmap(dmptf.refGrad_, addr);
    valueFraction_.rmap(dmptf.valueFraction_, addr);
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::directionMixedFvPatchField<Type>::snGrad() const
{
    const Field<Type> pif(this->patchInternalField());

    tmp<Field<Type> > normalValue = transform(valueFraction_, refValue_);

    tmp<Field<Type> > gradValue = pif + refGrad_/this->patch().deltaCoeffs();

    tmp<Field<Type> > transformGradValue =
        transform(I - valueFraction_, gradValue);

    return
        (normalValue + transformGradValue - pif)*
        this->patch().deltaCoeffs();
}


template<class Type>
void CML::directionMixedFvPatchField<Type>::evaluate(const Pstream::commsTypes)
{
    if (!this->updated())
    {
        this->updateCoeffs();
    }

    tmp<Field<Type> > normalValue = transform(valueFraction_, refValue_);

    tmp<Field<Type> > gradValue =
        this->patchInternalField() + refGrad_/this->patch().deltaCoeffs();

    tmp<Field<Type> > transformGradValue =
        transform(I - valueFraction_, gradValue);

    Field<Type>::operator=(normalValue + transformGradValue);

    transformFvPatchField<Type>::evaluate();
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::directionMixedFvPatchField<Type>::snGradTransformDiag() const
{
    vectorField diag(valueFraction_.size());

    diag.replace
    (
        vector::X,
        sqrt(mag(valueFraction_.component(symmTensor::XX)))
    );
    diag.replace
    (
        vector::Y,
        sqrt(mag(valueFraction_.component(symmTensor::YY)))
    );
    diag.replace
    (
        vector::Z,
        sqrt(mag(valueFraction_.component(symmTensor::ZZ)))
    );

    return transformFieldMask<Type>(pow<vector, pTraits<Type>::rank>(diag));
}


template<class Type>
void CML::directionMixedFvPatchField<Type>::write(Ostream& os) const
{
    transformFvPatchField<Type>::write(os);
    refValue_.writeEntry("refValue", os);
    refGrad_.writeEntry("refGradient", os);
    valueFraction_.writeEntry("valueFraction", os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
