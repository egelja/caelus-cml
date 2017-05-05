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
    CML::basicSymmetryFvPatchField

Description
    A symmetry patch


\*---------------------------------------------------------------------------*/

#ifndef basicSymmetryFvPatchField_H
#define basicSymmetryFvPatchField_H

#include "transformFvPatchField.hpp"
#include "symmetryFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class basicSymmetryFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class basicSymmetryFvPatchField
:
    public transformFvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName(symmetryFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        basicSymmetryFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        basicSymmetryFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given basicSymmetryFvPatchField onto a new
        //  patch
        basicSymmetryFvPatchField
        (
            const basicSymmetryFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        basicSymmetryFvPatchField
        (
            const basicSymmetryFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new basicSymmetryFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        basicSymmetryFvPatchField
        (
            const basicSymmetryFvPatchField<Type>&,
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
                new basicSymmetryFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

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
};


// * * * * * * * * * * * Template Specialisations  * * * * * * * * * * * * * //

template<>
tmp<scalarField > basicSymmetryFvPatchField<scalar>::snGrad() const;

template<>
void basicSymmetryFvPatchField<scalar>::evaluate
(
    const Pstream::commsTypes commsType
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "symmTransformField.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::basicSymmetryFvPatchField<Type>::basicSymmetryFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    transformFvPatchField<Type>(p, iF)
{}


template<class Type>
CML::basicSymmetryFvPatchField<Type>::basicSymmetryFvPatchField
(
    const basicSymmetryFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    transformFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
CML::basicSymmetryFvPatchField<Type>::basicSymmetryFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    transformFvPatchField<Type>(p, iF, dict)
{
    this->evaluate();
}


template<class Type>
CML::basicSymmetryFvPatchField<Type>::basicSymmetryFvPatchField
(
    const basicSymmetryFvPatchField<Type>& ptf
)
:
    transformFvPatchField<Type>(ptf)
{}


template<class Type>
CML::basicSymmetryFvPatchField<Type>::basicSymmetryFvPatchField
(
    const basicSymmetryFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    transformFvPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::Field<Type> >
CML::basicSymmetryFvPatchField<Type>::snGrad() const
{
    tmp<vectorField> nHat = this->patch().nf();

    return
    (
        transform(I - 2.0*sqr(nHat), this->patchInternalField())
      - this->patchInternalField()
    )*(this->patch().deltaCoeffs()/2.0);
}


template<class Type>
void CML::basicSymmetryFvPatchField<Type>::evaluate(const Pstream::commsTypes)
{
    if (!this->updated())
    {
        this->updateCoeffs();
    }

    tmp<vectorField> nHat = this->patch().nf();

    Field<Type>::operator=
    (
        (
            this->patchInternalField()
          + transform(I - 2.0*sqr(nHat), this->patchInternalField())
        )/2.0
    );

    transformFvPatchField<Type>::evaluate();
}

template<class Type>
CML::tmp<CML::Field<Type> >
CML::basicSymmetryFvPatchField<Type>::snGradTransformDiag() const
{
    const vectorField nHat(this->patch().nf());

    vectorField diag(nHat.size());

    diag.replace(vector::X, mag(nHat.component(vector::X)));
    diag.replace(vector::Y, mag(nHat.component(vector::Y)));
    diag.replace(vector::Z, mag(nHat.component(vector::Z)));

    return transformFieldMask<Type>(pow<vector, pTraits<Type>::rank>(diag));
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
