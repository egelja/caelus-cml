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
    CML::processorCyclicFvsPatchField

Description
    CML::processorCyclicFvsPatchField

SourceFiles
    processorCyclicFvsPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef processorCyclicFvsPatchField_H
#define processorCyclicFvsPatchField_H

#include "coupledFvsPatchField.hpp"
#include "processorCyclicFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class processorCyclicFvsPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class processorCyclicFvsPatchField
:
    public coupledFvsPatchField<Type>
{
    // Private data

        //- Local reference cast into the processor patch
        const processorCyclicFvPatch& procPatch_;


public:

    //- Runtime type information
    TypeName(processorCyclicFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        processorCyclicFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct from patch and internal field and patch field
        processorCyclicFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const Field<Type>&
        );

        //- Construct from patch, internal field and dictionary
        processorCyclicFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const dictionary&
        );

        //- Construct by mapping given processorCyclicFvsPatchField onto a
        //  new patch
        processorCyclicFvsPatchField
        (
            const processorCyclicFvsPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        processorCyclicFvsPatchField(const processorCyclicFvsPatchField<Type>&);

        //- Construct and return a clone
        virtual tmp<fvsPatchField<Type> > clone() const
        {
            return tmp<fvsPatchField<Type> >
            (
                new processorCyclicFvsPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        processorCyclicFvsPatchField
        (
            const processorCyclicFvsPatchField<Type>&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvsPatchField<Type> > clone
        (
            const DimensionedField<Type, surfaceMesh>& iF
        ) const
        {
            return tmp<fvsPatchField<Type> >
            (
                new processorCyclicFvsPatchField<Type>(*this, iF)
            );
        }


    // Destructor

        ~processorCyclicFvsPatchField();


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
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors * * * * * * * * * * * * * * //

template<class Type>
processorCyclicFvsPatchField<Type>::processorCyclicFvsPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF
)
:
    coupledFvsPatchField<Type>(p, iF),
    procPatch_(refCast<const processorCyclicFvPatch>(p))
{}


template<class Type>
processorCyclicFvsPatchField<Type>::processorCyclicFvsPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF,
    const Field<Type>& f
)
:
    coupledFvsPatchField<Type>(p, iF, f),
    procPatch_(refCast<const processorCyclicFvPatch>(p))
{}


// Construct by mapping given processorCyclicFvsPatchField<Type>
template<class Type>
processorCyclicFvsPatchField<Type>::processorCyclicFvsPatchField
(
    const processorCyclicFvsPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    coupledFvsPatchField<Type>(ptf, p, iF, mapper),
    procPatch_(refCast<const processorCyclicFvPatch>(p))
{
    if (!isType<processorCyclicFvPatch>(this->patch()))
    {
        FatalErrorInFunction
            << "Field type does not correspond to patch type for patch "
            << this->patch().index() << "." << endl
            << "Field type: " << typeName << endl
            << "Patch type: " << this->patch().type()
            << exit(FatalError);
    }
}


template<class Type>
processorCyclicFvsPatchField<Type>::processorCyclicFvsPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF,
    const dictionary& dict
)
:
    coupledFvsPatchField<Type>(p, iF, dict),
    procPatch_(refCast<const processorCyclicFvPatch>(p))
{
    if (!isType<processorCyclicFvPatch>(p))
    {
        FatalIOErrorInFunction(dict)
            << "patch " << this->patch().index() << " not processor type. "
            << "Patch type = " << p.type()
            << exit(FatalIOError);
    }
}


template<class Type>
processorCyclicFvsPatchField<Type>::processorCyclicFvsPatchField
(
    const processorCyclicFvsPatchField<Type>& ptf
)
:
    coupledFvsPatchField<Type>(ptf),
    procPatch_(refCast<const processorCyclicFvPatch>(ptf.patch()))
{}


template<class Type>
processorCyclicFvsPatchField<Type>::processorCyclicFvsPatchField
(
    const processorCyclicFvsPatchField<Type>& ptf,
    const DimensionedField<Type, surfaceMesh>& iF
)
:
    coupledFvsPatchField<Type>(ptf, iF),
    procPatch_(refCast<const processorCyclicFvPatch>(ptf.patch()))
{}


// * * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * //

template<class Type>
processorCyclicFvsPatchField<Type>::~processorCyclicFvsPatchField()
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
