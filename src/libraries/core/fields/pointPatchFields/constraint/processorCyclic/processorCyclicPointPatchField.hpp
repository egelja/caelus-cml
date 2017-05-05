/*---------------------------------------------------------------------------*\
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
    CML::processorCyclicPointPatchField

Description
    CML::processorCyclicPointPatchField


\*---------------------------------------------------------------------------*/

#ifndef processorCyclicPointPatchField_H
#define processorCyclicPointPatchField_H

#include "coupledPointPatchField.hpp"
#include "processorCyclicPointPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class processorCyclicPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class processorCyclicPointPatchField
:
    public coupledPointPatchField<Type>
{
    // Private data

        //- Local reference to processor patch
        const processorCyclicPointPatch& procPatch_;

        //- Receive buffer for non-blocking communication
        mutable Field<Type> receiveBuf_;
 

public:

    //- Runtime type information
    TypeName(processorCyclicPointPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        processorCyclicPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        processorCyclicPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        processorCyclicPointPatchField
        (
            const processorCyclicPointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new processorCyclicPointPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        processorCyclicPointPatchField
        (
            const processorCyclicPointPatchField<Type>&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual autoPtr<pointPatchField<Type> > clone
        (
            const DimensionedField<Type, pointMesh>& iF
        ) const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new processorCyclicPointPatchField<Type>
                (
                    *this,
                    iF
                )
            );
        }


    // Destructor

        ~processorCyclicPointPatchField();


    // Member functions

        // Access

            //- Return true if running parallel
            virtual bool coupled() const
            {
                if (Pstream::parRun())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            //- Does the patch field perform the transfromation
            virtual bool doTransform() const
            {
                return
                   !(
                        pTraits<Type>::rank == 0
                     || procPatch_.procPolyPatch().parallel()
                    );
            }


        // Evaluation functions

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            )
            {}

            //- Initialise swap of non-collocated patch point values
            virtual void initSwapAddSeparated
            (
                const Pstream::commsTypes commsType,
                Field<Type>&
            ) const;

            //- Complete swap of patch point values and add to local values
            virtual void swapAddSeparated
            (
                const Pstream::commsTypes commsType,
                Field<Type>&
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "transformField.hpp"
#include "processorPolyPatch.hpp"


// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * //

template<class Type>
CML::processorCyclicPointPatchField<Type>::processorCyclicPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    coupledPointPatchField<Type>(p, iF),
    procPatch_(refCast<const processorCyclicPointPatch>(p)),
    receiveBuf_(0)
{}


template<class Type>
CML::processorCyclicPointPatchField<Type>::processorCyclicPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    coupledPointPatchField<Type>(p, iF, dict),
    procPatch_(refCast<const processorCyclicPointPatch>(p)),
    receiveBuf_(0)
{}


template<class Type>
CML::processorCyclicPointPatchField<Type>::processorCyclicPointPatchField
(
    const processorCyclicPointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    coupledPointPatchField<Type>(ptf, p, iF, mapper),
    procPatch_(refCast<const processorCyclicPointPatch>(ptf.patch())),
    receiveBuf_(0)
{}


template<class Type>
CML::processorCyclicPointPatchField<Type>::processorCyclicPointPatchField
(
    const processorCyclicPointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    coupledPointPatchField<Type>(ptf, iF),
    procPatch_(refCast<const processorCyclicPointPatch>(ptf.patch())),
    receiveBuf_(0)
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class Type>
CML::processorCyclicPointPatchField<Type>::~processorCyclicPointPatchField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::processorCyclicPointPatchField<Type>::initSwapAddSeparated
(
    const Pstream::commsTypes commsType,
    Field<Type>& pField
) const
{
    if (Pstream::parRun())
    {
        // Get internal field into correct order for opposite side
        Field<Type> pf
        (
            this->patchInternalField
            (
                pField,
                procPatch_.reverseMeshPoints()
            )
        );

        if (commsType == Pstream::nonBlocking)
        {
            receiveBuf_.setSize(pf.size());
            IPstream::read
            (
                commsType,
                procPatch_.neighbProcNo(),
                reinterpret_cast<char*>(receiveBuf_.begin()),
                receiveBuf_.byteSize(),
                procPatch_.tag()
            );
        }
        
        OPstream::write
        (
            commsType,
            procPatch_.neighbProcNo(),
            reinterpret_cast<const char*>(pf.begin()),
            pf.byteSize(),
            procPatch_.tag()
        );
    }
}


template<class Type>
void CML::processorCyclicPointPatchField<Type>::swapAddSeparated
(
    const Pstream::commsTypes commsType,
    Field<Type>& pField
) const
{
    if (Pstream::parRun())
    {
        // If nonblocking data has already been received into receiveBuf_
        if (commsType != Pstream::nonBlocking)
        {
            receiveBuf_.setSize(this->size());
            IPstream::read
            (
                commsType,
                procPatch_.neighbProcNo(),
                reinterpret_cast<char*>(receiveBuf_.begin()),
                receiveBuf_.byteSize(),
                procPatch_.tag()
            );
        }

        if (doTransform())
        {
            const processorCyclicPolyPatch& ppp =
                procPatch_.procCyclicPolyPatch();
            const tensor& forwardT = ppp.forwardT()[0];

            transform(receiveBuf_, forwardT, receiveBuf_);
        }

        // All points are separated
        this->addToInternalField(pField, receiveBuf_);
    }
}


// ************************************************************************* //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
