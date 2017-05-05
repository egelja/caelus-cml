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
    CML::jumpCyclicFvPatchField

Description
    CML::jumpCyclicFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef jumpCyclicFvPatchField_H
#define jumpCyclicFvPatchField_H

#include "cyclicFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class cyclicFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class jumpCyclicFvPatchField
:
    public cyclicFvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("jumpCyclic");


    // Constructors

        //- Construct from patch and internal field
        jumpCyclicFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        jumpCyclicFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given jumpCyclicFvPatchField onto a new patch
        jumpCyclicFvPatchField
        (
            const jumpCyclicFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        jumpCyclicFvPatchField
        (
            const jumpCyclicFvPatchField<Type>&
        );

        //- Construct as copy setting internal field reference
        jumpCyclicFvPatchField
        (
            const jumpCyclicFvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );


    // Member functions

        // Access

            //- Return the interface type
            virtual const word& interfaceFieldType() const
            {
                return cyclicFvPatchField<Type>::type();
            }

            //- Return the "jump" across the patch as a "half" field
            virtual tmp<Field<scalar> > jump() const = 0;


        // Evaluation functions

            //- Return neighbour coupled given internal cell data
            tmp<Field<Type> > patchNeighbourField() const;

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                const scalarField& psiInternal,
                scalarField& result,
                const lduMatrix&,
                const scalarField& coeffs,
                const direction cmpt,
                const Pstream::commsTypes commsType
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
jumpCyclicFvPatchField<Type>::jumpCyclicFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    cyclicFvPatchField<Type>(p, iF)
{}


template<class Type>
jumpCyclicFvPatchField<Type>::jumpCyclicFvPatchField
(
    const jumpCyclicFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    cyclicFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
jumpCyclicFvPatchField<Type>::jumpCyclicFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    cyclicFvPatchField<Type>(p, iF, dict)
{
    // Call this evaluation in derived classes
    //this->evaluate(Pstream::blocking);
}


template<class Type>
jumpCyclicFvPatchField<Type>::jumpCyclicFvPatchField
(
    const jumpCyclicFvPatchField<Type>& ptf
)
:
    cyclicLduInterfaceField(),
    cyclicFvPatchField<Type>(ptf)
{}


template<class Type>
jumpCyclicFvPatchField<Type>::jumpCyclicFvPatchField
(
    const jumpCyclicFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    cyclicFvPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<Field<Type> > jumpCyclicFvPatchField<Type>::patchNeighbourField() const
{
    const Field<Type>& iField = this->internalField();
    const labelUList& nbrFaceCells =
        this->cyclicPatch().neighbFvPatch().faceCells();

    tmp<Field<Type> > tpnf(new Field<Type>(this->size()));
    Field<Type>& pnf = tpnf();

    tmp<Field<scalar> > tjf = jump();
    if (!this->cyclicPatch().owner())
    {
        tjf = -tjf;
    }
    const Field<scalar>& jf = tjf();

    if (this->doTransform())
    {
        forAll(*this, facei)
        {
            pnf[facei] = transform
            (
                this->forwardT()[0], iField[nbrFaceCells[facei]]
	     ) - pTraits<Type>::one*jf[facei];
        }
    }
    else
    {
        forAll(*this, facei)
        {
            pnf[facei] = 
                iField[nbrFaceCells[facei]] - pTraits<Type>::one*jf[facei];
        }
    }

    return tpnf;
}


template<class Type>
void jumpCyclicFvPatchField<Type>::updateInterfaceMatrix
(
    const scalarField& psiInternal,
    scalarField& result,
    const lduMatrix&,
    const scalarField& coeffs,
    const direction cmpt,
    const Pstream::commsTypes
) const
{
    scalarField pnf(this->size());

    const labelUList& nbrFaceCells =
        this->cyclicPatch().neighbFvPatch().faceCells();

    // CAELUSFIX: use of reinterpret_cast should be safe
    // Compiler is tryin to instantiate all templates and we need 
    // only scalarField here
    if (&psiInternal 
        == 
        reinterpret_cast<const scalarField*>(&this->internalField())
       )
    {
        tmp<Field<scalar> > tjf = jump();
        if (!this->cyclicPatch().owner())
        {
            tjf = -tjf;
        }
        const Field<scalar>& jf = tjf();

        forAll(*this, facei)
        {
            pnf[facei] = psiInternal[nbrFaceCells[facei]] - jf[facei];
        }
    }
    else
    {
        forAll(*this, facei)
        {
            pnf[facei] = psiInternal[nbrFaceCells[facei]];
        }
    }

    // Transform according to the transformation tensors
    this->transformCoupleField(pnf, cmpt);

    // Multiply the field by coefficients and add into the result
    const labelUList& faceCells = this->cyclicPatch().faceCells();
    forAll(faceCells, elemI)
    {
        result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
