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
    CML::transformFvPatchField

Description
    CML::transformFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef transformFvPatchField_H
#define transformFvPatchField_H

#include "fvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class transformFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class transformFvPatchField
:
    public fvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("transform");


    // Constructors

        //- Construct from patch and internal field
        transformFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        transformFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping the given transformFvPatchField<Type>
        //  onto a new patch
        transformFvPatchField
        (
            const transformFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        transformFvPatchField
        (
            const transformFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const = 0;

        //- Construct as copy setting internal field reference
        transformFvPatchField
        (
            const transformFvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<Type> > clone
        (
            const DimensionedField<Type, volMesh>&
        ) const = 0;


    // Member functions

        // Attributes

            //- Return true if the value of the patch field
            //  is altered by assignment
            virtual bool assignable() const
            {
                return true;
            }

        // Evaluation functions

            //- Return gradient at boundary
            virtual tmp<Field<Type> > snGrad() const = 0;

            //- Return face-gradient transform diagonal
            virtual tmp<Field<Type> > snGradTransformDiag() const = 0;

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type> > valueInternalCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type> > valueBoundaryCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type> > gradientInternalCoeffs() const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type> > gradientBoundaryCoeffs() const;


    // Member operators

        virtual void operator=(const fvPatchField<Type>&);
};


// * * * * * * * * * * * Template Specialisations  * * * * * * * * * * * * * //

template<>
tmp<scalarField > transformFvPatchField<scalar>::gradientInternalCoeffs() const;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IOstreams.hpp"
#include "transformField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
transformFvPatchField<Type>::transformFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
   fvPatchField<Type>(p, iF)
{}


template<class Type>
transformFvPatchField<Type>::transformFvPatchField
(
    const transformFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
transformFvPatchField<Type>::transformFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fvPatchField<Type>(p, iF, dict)
{}


template<class Type>
transformFvPatchField<Type>::transformFvPatchField
(
    const transformFvPatchField<Type>& ptf
)
:
    fvPatchField<Type>(ptf)
{}


template<class Type>
transformFvPatchField<Type>::transformFvPatchField
(
    const transformFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fvPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<Field<Type> > transformFvPatchField<Type>::valueInternalCoeffs
(
    const tmp<scalarField>&
) const
{
    return pTraits<Type>::one - snGradTransformDiag();
}


template<class Type>
tmp<Field<Type> > transformFvPatchField<Type>::valueBoundaryCoeffs
(
    const tmp<scalarField>&
) const
{
    return
        *this
      - cmptMultiply
        (
            valueInternalCoeffs(this->patch().weights()),
            this->patchInternalField()
        );
}


template<class Type>
tmp<Field<Type> > transformFvPatchField<Type>::gradientInternalCoeffs() const
{
    return -this->patch().deltaCoeffs()*snGradTransformDiag();
}


template<class Type>
tmp<Field<Type> > transformFvPatchField<Type>::gradientBoundaryCoeffs() const
{
    return
        snGrad()
      - cmptMultiply(gradientInternalCoeffs(), this->patchInternalField());
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void transformFvPatchField<Type>::operator=
(
    const fvPatchField<Type>& ptf
)
{
    this->evaluate();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
