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
    CML::nonuniformTransformCyclicFvPatchField

Description
    CML::nonuniformTransformCyclicFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef nonuniformTransformCyclicFvPatchField_H
#define nonuniformTransformCyclicFvPatchField_H

#include "cyclicFvPatchField.hpp"
#include "nonuniformTransformCyclicFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
              Class nonuniformTransformCyclicFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class nonuniformTransformCyclicFvPatchField
:
    public cyclicFvPatchField<Type>
{
    // Private data

public:

    //- Runtime type information
    TypeName(nonuniformTransformCyclicFvPatch::typeName_());


    // Constructors

        //- Construct from patch and internal field
        nonuniformTransformCyclicFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        nonuniformTransformCyclicFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        //  nonuniformTransformCyclicFvPatchField onto a new patch
        nonuniformTransformCyclicFvPatchField
        (
            const nonuniformTransformCyclicFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        nonuniformTransformCyclicFvPatchField
        (
            const nonuniformTransformCyclicFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new nonuniformTransformCyclicFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        nonuniformTransformCyclicFvPatchField
        (
            const nonuniformTransformCyclicFvPatchField<Type>&,
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
                new nonuniformTransformCyclicFvPatchField<Type>(*this, iF)
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
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    cyclicFvPatchField<Type>(p, iF)
{}


template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
    const nonuniformTransformCyclicFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    cyclicFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    cyclicFvPatchField<Type>(p, iF, dict)
{}


template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
    const nonuniformTransformCyclicFvPatchField<Type>& ptf
)
:
    cyclicFvPatchField<Type>(ptf)
{}


template<class Type>
nonuniformTransformCyclicFvPatchField<Type>::
nonuniformTransformCyclicFvPatchField
(
    const nonuniformTransformCyclicFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    cyclicFvPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
