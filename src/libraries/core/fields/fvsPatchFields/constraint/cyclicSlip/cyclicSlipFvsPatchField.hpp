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
    CML::cyclicSlipFvsPatchField

Description
    CML::cyclicSlipFvsPatchField


\*---------------------------------------------------------------------------*/

#ifndef cyclicSlipFvsPatchField_H
#define cyclicSlipFvsPatchField_H

#include "cyclicFvsPatchField.hpp"
#include "cyclicSlipFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class cyclicSlipFvsPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class cyclicSlipFvsPatchField
:
    public cyclicFvsPatchField<Type>
{

public:

    //- Runtime type information
    TypeName(cyclicSlipFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        cyclicSlipFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&
        );

        //- Construct from patch, internal field and dictionary
        cyclicSlipFvsPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const dictionary&
        );

        //- Construct by mapping given cyclicSlipFvsPatchField onto a new patch
        cyclicSlipFvsPatchField
        (
            const cyclicSlipFvsPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, surfaceMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        cyclicSlipFvsPatchField
        (
            const cyclicSlipFvsPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvsPatchField<Type> > clone() const
        {
            return tmp<fvsPatchField<Type> >
            (
                new cyclicSlipFvsPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        cyclicSlipFvsPatchField
        (
            const cyclicSlipFvsPatchField<Type>&,
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
                new cyclicSlipFvsPatchField<Type>(*this, iF)
            );
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
cyclicSlipFvsPatchField<Type>::cyclicSlipFvsPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF
)
:
    cyclicFvsPatchField<Type>(p, iF)
{}


template<class Type>
cyclicSlipFvsPatchField<Type>::cyclicSlipFvsPatchField
(
    const cyclicSlipFvsPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    cyclicFvsPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
cyclicSlipFvsPatchField<Type>::cyclicSlipFvsPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, surfaceMesh>& iF,
    const dictionary& dict
)
:
    cyclicFvsPatchField<Type>(p, iF, dict)
{}


template<class Type>
cyclicSlipFvsPatchField<Type>::cyclicSlipFvsPatchField
(
    const cyclicSlipFvsPatchField<Type>& ptf
)
:
    cyclicFvsPatchField<Type>(ptf)
{}


template<class Type>
cyclicSlipFvsPatchField<Type>::cyclicSlipFvsPatchField
(
    const cyclicSlipFvsPatchField<Type>& ptf,
    const DimensionedField<Type, surfaceMesh>& iF
)
:
    cyclicFvsPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
