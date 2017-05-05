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
    CML::slipFvPatchField

Description
    CML::slipFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef slipFvPatchField_H
#define slipFvPatchField_H

#include "basicSymmetryFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class slipFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class slipFvPatchField
:
    public basicSymmetryFvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("slip");


    // Constructors

        //- Construct from patch and internal field
        slipFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        slipFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given slipFvPatchField onto a new patch
        slipFvPatchField
        (
            const slipFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        slipFvPatchField
        (
            const slipFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new slipFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        slipFvPatchField
        (
            const slipFvPatchField<Type>&,
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
                new slipFvPatchField<Type>(*this, iF)
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
slipFvPatchField<Type>::slipFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
   basicSymmetryFvPatchField<Type>(p, iF)
{}


template<class Type>
slipFvPatchField<Type>::slipFvPatchField
(
    const slipFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    basicSymmetryFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
slipFvPatchField<Type>::slipFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    basicSymmetryFvPatchField<Type>(p, iF, dict)
{}


template<class Type>
slipFvPatchField<Type>::slipFvPatchField
(
    const slipFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    basicSymmetryFvPatchField<Type>(ptf, iF)
{}


template<class Type>
slipFvPatchField<Type>::slipFvPatchField
(
    const slipFvPatchField<Type>& ptf
)
:
    basicSymmetryFvPatchField<Type>(ptf)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
