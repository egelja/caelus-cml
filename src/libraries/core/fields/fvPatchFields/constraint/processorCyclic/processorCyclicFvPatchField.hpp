/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::processorCyclicFvPatchField

Description
    CML::processorCyclicFvPatchField

SourceFiles
    processorCyclicFvPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef processorCyclicFvPatchField_H
#define processorCyclicFvPatchField_H

#include "processorCyclicFvPatch.hpp"
#include "processorFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class processorCyclicFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class processorCyclicFvPatchField
:
    public processorFvPatchField<Type>
{
    // Private data

        //- Local reference cast into the processor patch
        const processorCyclicFvPatch& procPatch_;

public:

    //- Runtime type information
    TypeName(processorCyclicFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        processorCyclicFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch and internal field and patch field
        processorCyclicFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const Field<Type>&
        );

        //- Construct from patch, internal field and dictionary
        processorCyclicFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given processorCyclicFvPatchField onto a
        //  new patch
        processorCyclicFvPatchField
        (
            const processorCyclicFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        processorCyclicFvPatchField(const processorCyclicFvPatchField<Type>&);

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new processorCyclicFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        processorCyclicFvPatchField
        (
            const processorCyclicFvPatchField<Type>&,
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
                new processorCyclicFvPatchField<Type>(*this, iF)
            );
        }


    // Destructor

        ~processorCyclicFvPatchField();


    // Member functions

        // Access

            //- Does the patch field perform the transfromation
            virtual bool doTransform() const
            {
                return !(procPatch_.parallel() || pTraits<Type>::rank == 0);
            }

            //- Return face transformation tensor
            virtual const tensorField& forwardT() const
            {
                return procPatch_.forwardT();
            }


};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "processorCyclicFvPatch.hpp"
#include "demandDrivenData.hpp"
#include "transformField.hpp"

// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * //

template<class Type>
CML::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    processorFvPatchField<Type>(p, iF),
    procPatch_(refCast<const processorCyclicFvPatch>(p))
{}


template<class Type>
CML::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const Field<Type>& f
)
:
    //coupledFvPatchField<Type>(p, iF, f),
    processorFvPatchField<Type>(p, iF, f),
    procPatch_(refCast<const processorCyclicFvPatch>(p))
{}


// Construct by mapping given processorCyclicFvPatchField<Type>
template<class Type>
CML::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
    const processorCyclicFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    //coupledFvPatchField<Type>(ptf, p, iF, mapper),
    processorFvPatchField<Type>(ptf, p, iF, mapper),
    procPatch_(refCast<const processorCyclicFvPatch>(p))
{
    if (!isType<processorCyclicFvPatch>(this->patch()))
    {
        FatalErrorInFunction
            << "\n    patch type '" << p.type()
            << "' not constraint type '" << typeName << "'"
            << "\n    for patch " << p.name()
            << " of field " << this->dimensionedInternalField().name()
            << " in file " << this->dimensionedInternalField().objectPath()
            << exit(FatalIOError);
    }
}


template<class Type>
CML::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    //coupledFvPatchField<Type>(p, iF, dict),
    processorFvPatchField<Type>(p, iF, dict),
    procPatch_(refCast<const processorCyclicFvPatch>(p))
{
    if (!isType<processorCyclicFvPatch>(p))
    {
        FatalIOErrorInFunction(dict)
            << "\n    patch type '" << p.type()
            << "' not constraint type '" << typeName << "'"
            << "\n    for patch " << p.name()
            << " of field " << this->dimensionedInternalField().name()
            << " in file " << this->dimensionedInternalField().objectPath()
            << exit(FatalIOError);
    }

    if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        WarningInFunction
            << "Scheduled communication with split cyclics not supported."
            << endl;
    }
}


template<class Type>
CML::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
    const processorCyclicFvPatchField<Type>& ptf
)
:
    //processorLduInterfaceField(),
    //coupledFvPatchField<Type>(ptf),
    processorFvPatchField<Type>(ptf),
    procPatch_(refCast<const processorCyclicFvPatch>(ptf.patch()))
{}


template<class Type>
CML::processorCyclicFvPatchField<Type>::processorCyclicFvPatchField
(
    const processorCyclicFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    //coupledFvPatchField<Type>(ptf, iF),
    processorFvPatchField<Type>(ptf, iF),
    procPatch_(refCast<const processorCyclicFvPatch>(ptf.patch()))
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class Type>
CML::processorCyclicFvPatchField<Type>::~processorCyclicFvPatchField()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
