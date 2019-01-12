/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2015 OpenFOAM Foundation
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
    CML::cyclicACMIFvPatchField

Description
    This boundary condition enforces a cyclic condition between a pair of
    boundaries, whereby communication between the patches is performed using
    an arbitrarily coupled mesh interface (ACMI) interpolation.

    \heading Patch usage

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            cyclicACMI;
    }
    \endverbatim

SeeAlso
    CML::AMIInterpolation


\*---------------------------------------------------------------------------*/

#ifndef cyclicACMIFvPatchField_H
#define cyclicACMIFvPatchField_H

#include "coupledFvPatchField.hpp"
#include "cyclicACMILduInterfaceField.hpp"
#include "cyclicACMIFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class cyclicACMIFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicACMIFvPatchField
:
    virtual public cyclicACMILduInterfaceField,
    public coupledFvPatchField<Type>
{
    // Private data

        //- Local reference cast into the cyclic patch
        const cyclicACMIFvPatch& cyclicACMIPatch_;


    // Private Member Functions

        //- Return neighbour side field given internal fields
        template<class Type2>
        tmp<Field<Type2> > neighbourSideField
        (
            const Field<Type2>&
        ) const;


public:

    //- Runtime type information
    TypeName(cyclicACMIFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicACMIFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        cyclicACMIFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given cyclicACMIFvPatchField onto a new patch
        cyclicACMIFvPatchField
        (
            const cyclicACMIFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        cyclicACMIFvPatchField(const cyclicACMIFvPatchField<Type>&);

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new cyclicACMIFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        cyclicACMIFvPatchField
        (
            const cyclicACMIFvPatchField<Type>&,
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
                new cyclicACMIFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return local reference cast into the cyclic AMI patch
            const cyclicACMIFvPatch& cyclicACMIPatch() const
            {
                return cyclicACMIPatch_;
            }


        // Evaluation functions

            //- Return true if coupled. Note that the underlying patch
            //  is not coupled() - the points don't align
            virtual bool coupled() const;

            //- Return true if this patch field fixes a value
            //  Needed to check if a level has to be specified while solving
            //  Poissons equations
            virtual bool fixesValue() const
            {
                const scalarField& mask =
                    cyclicACMIPatch_.cyclicACMIPatch().mask();

                if (gMax(mask) > 1e-5)
                {
                    // regions connected
                    return false;
                }
                else
                {
                    // fully separated
                    return nonOverlapPatchField().fixesValue();
                }
            }


            //- Return neighbour coupled internal cell data
            virtual tmp<Field<Type> > patchNeighbourField() const;

            //- Return reference to neighbour patchField
            const cyclicACMIFvPatchField<Type>& neighbourPatchField() const;

            //- Return reference to non-overlapping patchField
            const fvPatchField<Type>& nonOverlapPatchField() const;

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

            //- Return patch-normal gradient
            virtual tmp<Field<Type> > snGrad
            (
                const scalarField& deltaCoeffs
            ) const;

            //- Update the coefficients associated with the patch field
            void updateCoeffs();

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

            //- Manipulate matrix
            virtual void manipulateMatrix(fvMatrix<Type>& matrix);


        // Cyclic AMI coupled interface functions

            //- Does the patch field perform the transformation
            virtual bool doTransform() const
            {
                return
                    !(cyclicACMIPatch_.parallel() || pTraits<Type>::rank == 0);
            }

            //- Return face transformation tensor
            virtual const tensorField& forwardT() const
            {
                return cyclicACMIPatch_.forwardT();
            }

            //- Return neighbour-cell transformation tensor
            virtual const tensorField& reverseT() const
            {
                return cyclicACMIPatch_.reverseT();
            }

            //- Return rank of component for transform
            virtual int rank() const
            {
                return pTraits<Type>::rank;
            }


        // I-O

            //- Write
            virtual void write(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "transformField.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::cyclicACMIFvPatchField<Type>::cyclicACMIFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    cyclicACMILduInterfaceField(),
    coupledFvPatchField<Type>(p, iF),
    cyclicACMIPatch_(refCast<const cyclicACMIFvPatch>(p))
{}


template<class Type>
CML::cyclicACMIFvPatchField<Type>::cyclicACMIFvPatchField
(
    const cyclicACMIFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    cyclicACMILduInterfaceField(),
    coupledFvPatchField<Type>(ptf, p, iF, mapper),
    cyclicACMIPatch_(refCast<const cyclicACMIFvPatch>(p))
{
    if (!isA<cyclicACMIFvPatch>(this->patch()))
    {
        FatalErrorInFunction
            << "    patch type '" << p.type()
            << "' not constraint type '" << typeName << "'"
            << "\n    for patch " << p.name()
            << " of field " << this->dimensionedInternalField().name()
            << " in file " << this->dimensionedInternalField().objectPath()
            << exit(FatalIOError);
    }
}


template<class Type>
CML::cyclicACMIFvPatchField<Type>::cyclicACMIFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    cyclicACMILduInterfaceField(),
    coupledFvPatchField<Type>(p, iF, dict),
    cyclicACMIPatch_(refCast<const cyclicACMIFvPatch>(p))
{
    if (!isA<cyclicACMIFvPatch>(p))
    {
        FatalIOErrorInFunction(dict)
            << "    patch type '" << p.type()
            << "' not constraint type '" << typeName << "'"
            << "\n    for patch " << p.name()
            << " of field " << this->dimensionedInternalField().name()
            << " in file " << this->dimensionedInternalField().objectPath()
            << exit(FatalIOError);
    }

    if (!dict.found("value") && this->coupled())
    {
        this->evaluate(Pstream::blocking);
    }
}


template<class Type>
CML::cyclicACMIFvPatchField<Type>::cyclicACMIFvPatchField
(
    const cyclicACMIFvPatchField<Type>& ptf
)
:
    cyclicACMILduInterfaceField(),
    coupledFvPatchField<Type>(ptf),
    cyclicACMIPatch_(ptf.cyclicACMIPatch_)
{}


template<class Type>
CML::cyclicACMIFvPatchField<Type>::cyclicACMIFvPatchField
(
    const cyclicACMIFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    cyclicACMILduInterfaceField(),
    coupledFvPatchField<Type>(ptf, iF),
    cyclicACMIPatch_(ptf.cyclicACMIPatch_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
bool CML::cyclicACMIFvPatchField<Type>::coupled() const
{
    return cyclicACMIPatch_.coupled();
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::cyclicACMIFvPatchField<Type>::patchNeighbourField() const
{
    const Field<Type>& iField = this->internalField();
    const labelUList& nbrFaceCellsCoupled =
        cyclicACMIPatch_.cyclicACMIPatch().neighbPatch().faceCells();
    const labelUList& faceCellsNonOverlap =
        cyclicACMIPatch_.cyclicACMIPatch().nonOverlapPatch().faceCells();

    Field<Type> pnfCoupled(iField, nbrFaceCellsCoupled);
    Field<Type> pfNonOverlap(iField, faceCellsNonOverlap);

    tmp<Field<Type> > tpnf
    (
        new Field<Type>
        (
            cyclicACMIPatch_.interpolate
            (
                pnfCoupled,
                pfNonOverlap
            )
        )
    );

    if (doTransform())
    {
        tpnf() = transform(forwardT(), tpnf());
    }

    return tpnf;
}


template<class Type>
const CML::cyclicACMIFvPatchField<Type>&
CML::cyclicACMIFvPatchField<Type>::neighbourPatchField() const
{
    const GeometricField<Type, fvPatchField, volMesh>& fld =
        static_cast<const GeometricField<Type, fvPatchField, volMesh>&>
        (
            this->internalField()
        );

    return refCast<const cyclicACMIFvPatchField<Type> >
    (
        fld.boundaryField()[cyclicACMIPatch_.neighbPatchID()]
    );
}


template<class Type>
const CML::fvPatchField<Type>&
CML::cyclicACMIFvPatchField<Type>::nonOverlapPatchField() const
{
    const GeometricField<Type, fvPatchField, volMesh>& fld =
        static_cast<const GeometricField<Type, fvPatchField, volMesh>&>
        (
            this->internalField()
        );

    return fld.boundaryField()[cyclicACMIPatch_.nonOverlapPatchID()];
}


template<class Type>
void CML::cyclicACMIFvPatchField<Type>::updateInterfaceMatrix
(
    const scalarField& psiInternal,
    scalarField& result,
    const lduMatrix&,
    const scalarField& coeffs,
    const direction cmpt,
    const Pstream::commsTypes
) const
{
    // note: only applying coupled contribution

    const labelUList& nbrFaceCellsCoupled =
        cyclicACMIPatch_.cyclicACMIPatch().neighbPatch().faceCells();

    scalarField pnf(psiInternal, nbrFaceCellsCoupled);

    // Transform according to the transformation tensors
    transformCoupleField(pnf, cmpt);

    const labelUList& faceCells = cyclicACMIPatch_.faceCells();

    pnf = cyclicACMIPatch_.interpolate(pnf);

    forAll(faceCells, elemI)
    {
        result[faceCells[elemI]] -= coeffs[elemI]*pnf[elemI];
    }
}


template<class Type>
CML::tmp<CML::Field<Type> > CML::cyclicACMIFvPatchField<Type>::snGrad
(
    const scalarField& deltaCoeffs
) const
{
    // note: only applying coupled contribution
    return coupledFvPatchField<Type>::snGrad(deltaCoeffs);
}


template<class Type>
void CML::cyclicACMIFvPatchField<Type>::updateCoeffs()
{
    // update non-overlap patch - some will implement updateCoeffs, and
    // others will implement evaluate

    // scale neighbour field by (1 - mask)

    const scalarField& mask = cyclicACMIPatch_.cyclicACMIPatch().mask();
    const fvPatchField<Type>& npf = nonOverlapPatchField();
    const_cast<fvPatchField<Type>&>(npf).updateCoeffs(1.0 - mask);
}


template<class Type>
void CML::cyclicACMIFvPatchField<Type>::initEvaluate
(
    const Pstream::commsTypes comms
)
{
    // update non-overlap patch (if not already updated by updateCoeffs)

    // scale neighbour field by (1 - mask)

    fvPatchField<Type>& npf =
        const_cast<fvPatchField<Type>&>(nonOverlapPatchField());

    if (!npf.updated())
    {
        const scalarField& mask = cyclicACMIPatch_.cyclicACMIPatch().mask();

        npf.evaluate(comms);

        npf *= 1.0 - mask;
    }
}


template<class Type>
void CML::cyclicACMIFvPatchField<Type>::evaluate
(
    const Pstream::commsTypes comms
)
{
    // blend contributions from the coupled and non-overlap patches

    // neighbour patch field is updated via updateCoeffs or initEvaluate
    // and is already scaled by (1 - mask)
    const fvPatchField<Type>& npf = nonOverlapPatchField();

    coupledFvPatchField<Type>::evaluate(comms);
    const Field<Type>& cpf = *this;

    const scalarField& mask = cyclicACMIPatch_.cyclicACMIPatch().mask();
    Field<Type>::operator=(mask*cpf + npf);

    fvPatchField<Type>::evaluate();
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::cyclicACMIFvPatchField<Type>::valueInternalCoeffs
(
    const tmp<scalarField>& w
) const
{
    // note: do not blend based on mask field
    // - when applied this is scaled by the areas which are already scaled
    return coupledFvPatchField<Type>::valueInternalCoeffs(w);
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::cyclicACMIFvPatchField<Type>::valueBoundaryCoeffs
(
    const tmp<scalarField>& w
) const
{
    // note: do not blend based on mask field
    // - when applied this is scaled by the areas which are already scaled
    return coupledFvPatchField<Type>::valueBoundaryCoeffs(w);
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::cyclicACMIFvPatchField<Type>::gradientInternalCoeffs
(
    const scalarField& deltaCoeffs
) const
{
    // note: do not blend based on mask field
    // - when applied this is scaled by the areas which are already scaled
    return coupledFvPatchField<Type>::gradientInternalCoeffs(deltaCoeffs);
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::cyclicACMIFvPatchField<Type>::gradientInternalCoeffs() const
{
    // note: do not blend based on mask field
    // - when applied this is scaled by the areas which are already scaled
    return coupledFvPatchField<Type>::gradientInternalCoeffs();
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::cyclicACMIFvPatchField<Type>::gradientBoundaryCoeffs
(
    const scalarField& deltaCoeffs
) const
{
    // note: do not blend based on mask field
    // - when applied this is scaled by the areas which are already scaled
    return coupledFvPatchField<Type>::gradientBoundaryCoeffs(deltaCoeffs);
}


template<class Type>
CML::tmp<CML::Field<Type> >
CML::cyclicACMIFvPatchField<Type>::gradientBoundaryCoeffs() const
{
    // note: do not blend based on mask field
    // - when applied this is scaled by the areas which are already scaled
    return coupledFvPatchField<Type>::gradientBoundaryCoeffs();
}


template<class Type>
void CML::cyclicACMIFvPatchField<Type>::manipulateMatrix
(
    fvMatrix<Type>& matrix
)
{
    const scalarField& mask = cyclicACMIPatch_.cyclicACMIPatch().mask();

    // nothing to be done by the AMI, but re-direct to non-overlap patch
    // with non-overlap patch weights
    const fvPatchField<Type>& npf = nonOverlapPatchField();

    const_cast<fvPatchField<Type>&>(npf).manipulateMatrix(matrix, 1.0 - mask);
}


template<class Type>
void CML::cyclicACMIFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
