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
    CML::slipPointPatchField

Description
    CML::slipPointPatchField

SourceFiles
    slipPointPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef slipPointPatchField_H
#define slipPointPatchField_H

#include "basicSymmetryPointPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class slipPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class slipPointPatchField
:
    public basicSymmetryPointPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("slip");


    // Constructors

        //- Construct from patch and internal field
        slipPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        slipPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        slipPointPatchField
        (
            const slipPointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new slipPointPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        slipPointPatchField
        (
            const slipPointPatchField<Type>&,
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
                new slipPointPatchField<Type>
                (
                    *this,
                    iF
                )
            );
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
slipPointPatchField<Type>::slipPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    basicSymmetryPointPatchField<Type>(p, iF)
{}


template<class Type>
slipPointPatchField<Type>::slipPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    basicSymmetryPointPatchField<Type>(p, iF, dict)
{}


template<class Type>
slipPointPatchField<Type>::slipPointPatchField
(
    const slipPointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    basicSymmetryPointPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
slipPointPatchField<Type>::slipPointPatchField
(
    const slipPointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    basicSymmetryPointPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
