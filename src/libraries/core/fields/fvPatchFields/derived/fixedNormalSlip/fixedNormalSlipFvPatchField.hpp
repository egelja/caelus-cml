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
    CML::fixedNormalSlipFvPatchField

Description
    CML::fixedNormalSlipFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef fixedNormalSlipFvPatchField_H
#define fixedNormalSlipFvPatchField_H

#include "transformFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class fixedNormalSlipFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fixedNormalSlipFvPatchField
:
    public transformFvPatchField<Type>
{
    // Private data

        //- Value the normal component of which the boundary is set to
        Field<Type> fixedValue_;


public:

    //- Runtime type information
    TypeName("fixedNormalSlip");


    // Constructors

        //- Construct from patch and internal field
        fixedNormalSlipFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixedNormalSlipFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given fixedNormalSlipFvPatchField
        //  onto a new patch
        fixedNormalSlipFvPatchField
        (
            const fixedNormalSlipFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        fixedNormalSlipFvPatchField
        (
            const fixedNormalSlipFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new fixedNormalSlipFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        fixedNormalSlipFvPatchField
        (
            const fixedNormalSlipFvPatchField<Type>&,
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
                new fixedNormalSlipFvPatchField<Type>(*this, iF)
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

            virtual Field<Type>& fixedValue()
            {
                return fixedValue_;
            }

            virtual const Field<Type>& fixedValue() const
            {
                return fixedValue_;
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


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::fixedNormalSlipFvPatchField<Type>::fixedNormalSlipFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    transformFvPatchField<Type>(p, iF),
    fixedValue_(p.size(), pTraits<Type>::zero)
{}


template<class Type>
CML::fixedNormalSlipFvPatchField<Type>::fixedNormalSlipFvPatchField
(
    const fixedNormalSlipFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    transformFvPatchField<Type>(ptf, p, iF, mapper),
    fixedValue_(ptf.fixedValue_, mapper)
{}


template<class Type>
CML::fixedNormalSlipFvPatchField<Type>::fixedNormalSlipFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    transformFvPatchField<Type>(p, iF),
    fixedValue_("fixedValue", dict, p.size())
{
    evaluate();
}


template<class Type>
CML::fixedNormalSlipFvPatchField<Type>::fixedNormalSlipFvPatchField
(
    const fixedNormalSlipFvPatchField<Type>& ptf
)
:
    transformFvPatchField<Type>(ptf),
    fixedValue_(ptf.fixedValue_)
{}


template<class Type>
CML::fixedNormalSlipFvPatchField<Type>::fixedNormalSlipFvPatchField
(
    const fixedNormalSlipFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    transformFvPatchField<Type>(ptf, iF),
    fixedValue_(ptf.fixedValue_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::fixedNormalSlipFvPatchField<Type>::autoMap
(
    const fvPatchFieldMapper& m
)
{
    transformFvPatchField<Type>::autoMap(m);
    fixedValue_.autoMap(m);
}


template<class Type>
void CML::fixedNormalSlipFvPatchField<Type>::rmap
(
    const fvPatchField<Type>& ptf,
    const labelList& addr
)
{
    transformFvPatchField<Type>::rmap(ptf, addr);

    const fixedNormalSlipFvPatchField<Type>& dmptf =
        refCast<const fixedNormalSlipFvPatchField<Type> >(ptf);

    fixedValue_.rmap(dmptf.fixedValue_, addr);
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::fixedNormalSlipFvPatchField<Type>::snGrad() const
{
    const vectorField nHat(this->patch().nf());
    const Field<Type> pif(this->patchInternalField());

    return
    (
        (nHat*(nHat & fixedValue_) + transform(I - sqr(nHat), pif)) - pif
    )*this->patch().deltaCoeffs();
}


template<class Type>
void CML::fixedNormalSlipFvPatchField<Type>::evaluate
(
    const Pstream::commsTypes
)
{
    if (!this->updated())
    {
        this->updateCoeffs();
    }

    const vectorField nHat(this->patch().nf());

    Field<Type>::operator=
    (
        nHat*(nHat & fixedValue_)
      + transform(I - sqr(nHat), this->patchInternalField())
    );

    transformFvPatchField<Type>::evaluate();
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::fixedNormalSlipFvPatchField<Type>::snGradTransformDiag() const
{
    const vectorField nHat(this->patch().nf());
    vectorField diag(nHat.size());

    diag.replace(vector::X, mag(nHat.component(vector::X)));
    diag.replace(vector::Y, mag(nHat.component(vector::Y)));
    diag.replace(vector::Z, mag(nHat.component(vector::Z)));

    return transformFieldMask<Type>(pow<vector, pTraits<Type>::rank>(diag));
}


template<class Type>
void CML::fixedNormalSlipFvPatchField<Type>::write(Ostream& os) const
{
    transformFvPatchField<Type>::write(os);
    fixedValue_.writeEntry("fixedValue", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
