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
    CML::cyclicACMIFvsPatchField

Description
    CML::cyclicACMIFvsPatchField

SourceFiles
    cyclicACMIFvsPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef cyclicACMIFvsPatchField_H
#define cyclicACMIFvsPatchField_H

#include "coupledFvsPatchField.hpp"
#include "cyclicACMIFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class cyclicACMIFvsPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicACMIFvsPatchField
:
    public coupledFvsPatchField<Type>
{
    // Private data

        //- Local reference cast into the cyclic patch
        const cyclicACMIFvPatch& cyclicACMIPatch_;


public:

    //- Runtime type information
    TypeName(cyclicACMIFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicACMIFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct from patch, internal field and dictionary
        cyclicACMIFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const dictionary&
        );

        //- Construct by mapping given cyclicACMIFvsPatchField onto a new patch
        cyclicACMIFvsPatchField
        (
            const cyclicACMIFvsPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        cyclicACMIFvsPatchField
        (
            const cyclicACMIFvsPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvsPatchField<Type> > clone() const
        {
            return tmp<fvsPatchField<Type> >
            (
                new cyclicACMIFvsPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        cyclicACMIFvsPatchField
        (
            const cyclicACMIFvsPatchField<Type>&,
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
                new cyclicACMIFvsPatchField<Type>(*this, iF)
            );
        }

    // Member functions

        // Access

            //- Return true if running parallel
            virtual bool coupled() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::cyclicACMIFvsPatchField<Type>::cyclicACMIFvsPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF
)
:
    coupledFvsPatchField<Type>(p, iF),
    cyclicACMIPatch_(refCast<const cyclicACMIFvPatch>(p))
{}


template<class Type>
CML::cyclicACMIFvsPatchField<Type>::cyclicACMIFvsPatchField
(
    const cyclicACMIFvsPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    coupledFvsPatchField<Type>(ptf, p, iF, mapper),
    cyclicACMIPatch_(refCast<const cyclicACMIFvPatch>(p))
{
    if (!isA<cyclicACMIFvPatch>(this->patch()))
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
CML::cyclicACMIFvsPatchField<Type>::cyclicACMIFvsPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF,
    const dictionary& dict
)
:
    coupledFvsPatchField<Type>(p, iF, dict),
    cyclicACMIPatch_(refCast<const cyclicACMIFvPatch>(p))
{
    if (!isA<cyclicACMIFvPatch>(p))
    {
        FatalIOErrorInFunction(dict)
            << "patch " << this->patch().index() << " not cyclicACMI type. "
            << "Patch type = " << p.type()
            << exit(FatalIOError);
    }
}


template<class Type>
CML::cyclicACMIFvsPatchField<Type>::cyclicACMIFvsPatchField
(
    const cyclicACMIFvsPatchField<Type>& ptf
)
:
    coupledFvsPatchField<Type>(ptf),
    cyclicACMIPatch_(ptf.cyclicACMIPatch_)
{}


template<class Type>
CML::cyclicACMIFvsPatchField<Type>::cyclicACMIFvsPatchField
(
    const cyclicACMIFvsPatchField<Type>& ptf,
    const DimensionedField<Type, surfaceMesh>& iF
)
:
    coupledFvsPatchField<Type>(ptf, iF),
    cyclicACMIPatch_(ptf.cyclicACMIPatch_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
bool CML::cyclicACMIFvsPatchField<Type>::coupled() const
{
    if
    (
        Pstream::parRun()
     || (
            this->cyclicACMIPatch_.size()
         && this->cyclicACMIPatch_.cyclicACMIPatch().neighbPatch().size()
        )
    )
    {
        return true;
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
