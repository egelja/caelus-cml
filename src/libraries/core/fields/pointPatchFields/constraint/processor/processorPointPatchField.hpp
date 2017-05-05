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
    CML::processorPointPatchField

Description
    CML::processorPointPatchField


\*---------------------------------------------------------------------------*/

#ifndef processorPointPatchField_H
#define processorPointPatchField_H

#include "coupledPointPatchField.hpp"
#include "processorPointPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class processorPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class processorPointPatchField
:
    public coupledPointPatchField<Type>
{
    // Private data

        //- Local reference to processor patch
        const processorPointPatch& procPatch_;


public:

    //- Runtime type information
    TypeName(processorPointPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        processorPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        processorPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        processorPointPatchField
        (
            const processorPointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new processorPointPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        processorPointPatchField
        (
            const processorPointPatchField<Type>&,
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
                new processorPointPatchField<Type>
                (
                    *this,
                    iF
                )
            );
        }


    //- Destructor
    virtual ~processorPointPatchField();


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

            //- Does the patch field perform the transformation
            virtual bool doTransform() const
            {
                return
                   !(
                        procPatch_.procPolyPatch().parallel()
                     || pTraits<Type>::rank == 0
                    );
            }


        //- Constraint handling

            //- Return the constraint type this pointPatchField implements
            virtual const word& constraintType() const
            {
                return type();
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

#include "processorPolyPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * //

template<class Type>
processorPointPatchField<Type>::processorPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    coupledPointPatchField<Type>(p, iF),
    procPatch_(refCast<const processorPointPatch>(p))
{}


template<class Type>
processorPointPatchField<Type>::processorPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    coupledPointPatchField<Type>(p, iF, dict),
    procPatch_(refCast<const processorPointPatch>(p))
{}


template<class Type>
processorPointPatchField<Type>::processorPointPatchField
(
    const processorPointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    coupledPointPatchField<Type>(ptf, p, iF, mapper),
    procPatch_(refCast<const processorPointPatch>(ptf.patch()))
{}


template<class Type>
processorPointPatchField<Type>::processorPointPatchField
(
    const processorPointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    coupledPointPatchField<Type>(ptf, iF),
    procPatch_(refCast<const processorPointPatch>(ptf.patch()))
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class Type>
processorPointPatchField<Type>::~processorPointPatchField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void processorPointPatchField<Type>::initSwapAddSeparated
(
    const Pstream::commsTypes commsType,
    Field<Type>& pField
)
const
{
//    if (Pstream::parRun())
//    {
//        // Get internal field into correct order for opposite side
//        Field<Type> pf
//        (
//            this->patchInternalField
//            (
//                pField,
//                procPatch_.reverseMeshPoints()
//            )
//        );
//
//        OPstream::write
//        (
//            commsType,
//            procPatch_.neighbProcNo(),
//            reinterpret_cast<const char*>(pf.begin()),
//            pf.byteSize(),
//            procPatch_.tag()
//        );
//    }
}


template<class Type>
void processorPointPatchField<Type>::swapAddSeparated
(
    const Pstream::commsTypes commsType,
    Field<Type>& pField
) const
{
//    if (Pstream::parRun())
//    {
//        Field<Type> pnf(this->size());
//
//        IPstream::read
//        (
//            commsType,
//            procPatch_.neighbProcNo(),
//            reinterpret_cast<char*>(pnf.begin()),
//            pnf.byteSize(),
//            procPatch_.tag()
//        );
//
//        if (doTransform())
//        {
//            const processorPolyPatch& ppp = procPatch_.procPolyPatch();
//            const tensor& forwardT = ppp.forwardT();
//
//            transform(pnf, forwardT, pnf);
//        }
//
//        addToInternalField(pField, pnf, procPatch_.separatedPoints());
//    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
