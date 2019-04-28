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
    CML::coupledFvPatchField

Description
    CML::coupledFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef coupledFvPatchField_H
#define coupledFvPatchField_H

#include "BlockLduInterfaceField.hpp"
#include "CoeffField.hpp"
#include "fvPatchField.hpp"
#include "coupledFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class coupledFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class coupledFvPatchField
:
    public BlockLduInterfaceField<Type>,
    public fvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName(coupledFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        coupledFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch and internal field and patch field
        coupledFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const Field<Type>&
        );

        //- Construct from patch, internal field and dictionary
        coupledFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping the given coupledFvPatchField onto a new patch
        coupledFvPatchField
        (
            const coupledFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        coupledFvPatchField
        (
            const coupledFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const = 0;

        //- Construct as copy setting internal field reference
        coupledFvPatchField
        (
            const coupledFvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone
        (
            const DimensionedField<Type, volMesh>&
        ) const = 0;


    // Member functions

        // Access

            //- Return true if this patch field is derived from
            //  coupledFvPatchField<Type>.
            virtual bool coupled() const
            {
                return true;
            }

            //- Return neighbour field of internal field
            virtual tmp<Field<Type> > patchNeighbourField() const = 0;


        // Evaluation functions

            //- Return patch-normal gradient
            virtual tmp<Field<Type> > snGrad
            (
                const scalarField& deltaCoeffs
            ) const;

            //- Return patch-normal gradient
            virtual tmp<Field<Type> > snGrad() const
            {
                NotImplemented;
                return *this;
            }

            //- Initialise the evaluation of the patch field
            virtual void initEvaluate
            (
                const Pstream::commsTypes commsType
            );

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType
            );

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
            virtual tmp<Field<Type> > gradientInternalCoeffs
            (
                const scalarField& deltaCoeffs
            ) const;

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type> > gradientInternalCoeffs() const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type> > gradientBoundaryCoeffs
            (
                const scalarField& deltaCoeffs
            ) const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            virtual tmp<Field<Type> > gradientBoundaryCoeffs() const;


        // Coupled interface functionality

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                const scalarField& psiInternal,
                scalarField& result,
                const lduMatrix&,
                const scalarField& coeffs,
                const direction,
                const Pstream::commsTypes commsType
            ) const = 0;

        // Block coupled interface functionality

            //- Initialise neighbour matrix update
            virtual void initInterfaceMatrixUpdate
            (
                Field<Type>&,
                const Field<Type>&,
                const BlockLduMatrix<Type>&,
                const CoeffField<Type>&,
                const Pstream::commsTypes commsType
            ) const
            {}

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                Field<Type>&,
                const Field<Type>&,
                const BlockLduMatrix<Type>&,
                const CoeffField<Type>&,
                const Pstream::commsTypes commsType
            ) const
            {
                NotImplemented
            }

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::coupledFvPatchField<Type>::coupledFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    BlockLduInterfaceField<Type>(refCast<const lduInterface>(p)),
    fvPatchField<Type>(p, iF)
{}


template<class Type>
CML::coupledFvPatchField<Type>::coupledFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const Field<Type>& f
)
:
    BlockLduInterfaceField<Type>(refCast<const lduInterface>(p)),
    fvPatchField<Type>(p, iF, f)
{}


template<class Type>
CML::coupledFvPatchField<Type>::coupledFvPatchField
(
    const coupledFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    BlockLduInterfaceField<Type>(refCast<const lduInterface>(p)),
    fvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
CML::coupledFvPatchField<Type>::coupledFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    BlockLduInterfaceField<Type>(refCast<const lduInterface>(p)),
    fvPatchField<Type>(p, iF, dict)
{}


template<class Type>
CML::coupledFvPatchField<Type>::coupledFvPatchField
(
    const coupledFvPatchField<Type>& ptf
)
:
    BlockLduInterfaceField<Type>(refCast<const lduInterface>(ptf.patch())),
    fvPatchField<Type>(ptf)
{}


template<class Type>
CML::coupledFvPatchField<Type>::coupledFvPatchField
(
    const coupledFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    BlockLduInterfaceField<Type>(refCast<const lduInterface>(ptf.patch())),
    fvPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::Field<Type> > CML::coupledFvPatchField<Type>::snGrad
(
    const scalarField& deltaCoeffs
) const
{
    return
        deltaCoeffs
       *(this->patchNeighbourField() - this->patchInternalField());
}


template<class Type>
void CML::coupledFvPatchField<Type>::initEvaluate(const Pstream::commsTypes)
{
    if (!this->updated())
    {
        this->updateCoeffs();
    }
}


template<class Type>
void CML::coupledFvPatchField<Type>::evaluate(const Pstream::commsTypes)
{
    if (!this->updated())
    {
        this->updateCoeffs();
    }

    Field<Type>::operator=
    (
        this->patch().weights()*this->patchInternalField()
      + (1.0 - this->patch().weights())*this->patchNeighbourField()
    );

    fvPatchField<Type>::evaluate();
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::coupledFvPatchField<Type>::valueInternalCoeffs
(
    const tmp<scalarField>& w
) const
{
    return Type(pTraits<Type>::one)*w;
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::coupledFvPatchField<Type>::valueBoundaryCoeffs
(
    const tmp<scalarField>& w
) const
{
    return Type(pTraits<Type>::one)*(1.0 - w);
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::coupledFvPatchField<Type>::gradientInternalCoeffs
(
    const scalarField& deltaCoeffs
) const
{
    return -Type(pTraits<Type>::one)*deltaCoeffs;
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::coupledFvPatchField<Type>::gradientInternalCoeffs() const
{
    NotImplemented;
    return -Type(pTraits<Type>::one)*this->patch().deltaCoeffs();
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::coupledFvPatchField<Type>::gradientBoundaryCoeffs
(
    const scalarField& deltaCoeffs
) const
{
    return -this->gradientInternalCoeffs(deltaCoeffs);
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::coupledFvPatchField<Type>::gradientBoundaryCoeffs() const
{
    NotImplemented;
    return -this->gradientInternalCoeffs();
}


template<class Type>
void CML::coupledFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
