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
    CML::fixedInternalValueFvPatchField

Description
    This boundary condition provides a mechanism to set boundary (cell) values
    directly into a matrix, i.e. to set a constraint condition.  Default
    behaviour is to act as a zero gradient condition.

    \heading Patch usage

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            fixedInternalValue;
        value           uniform 0;              // place holder
    }
    \endverbatim

Note
    This is used as a base for conditions such as the turbulence \c epsilon
    wall function, which applies a near-wall constraint for high Reynolds
    number flows.

SeeAlso
    CML::epsilonWallFunctionFvPatchScalarField

SourceFiles
    fixedInternalValueFvPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef fixedInternalValueFvPatchField_H
#define fixedInternalValueFvPatchField_H

#include "zeroGradientFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
              Class fixedInternalValueFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fixedInternalValueFvPatchField
:
    public zeroGradientFvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("fixedInternalValue");


    // Constructors

        //- Construct from patch and internal field
        fixedInternalValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixedInternalValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping the given fixedInternalValueFvPatchField<Type>
        //  onto a new patch
        fixedInternalValueFvPatchField
        (
            const fixedInternalValueFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        fixedInternalValueFvPatchField
        (
            const fixedInternalValueFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new fixedInternalValueFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        fixedInternalValueFvPatchField
        (
            const fixedInternalValueFvPatchField<Type>&,
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
                new fixedInternalValueFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //-Manipulate a matrix
            virtual void manipulateMatrix(fvMatrix<Type>& matrix);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvPatchFieldMapper.hpp"
#include "fvMatrix.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::fixedInternalValueFvPatchField<Type>::fixedInternalValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    zeroGradientFvPatchField<Type>(p, iF)
{}


template<class Type>
CML::fixedInternalValueFvPatchField<Type>::fixedInternalValueFvPatchField
(
    const fixedInternalValueFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    zeroGradientFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
CML::fixedInternalValueFvPatchField<Type>::fixedInternalValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    zeroGradientFvPatchField<Type>(p, iF, dict)
{}


template<class Type>
CML::fixedInternalValueFvPatchField<Type>::fixedInternalValueFvPatchField
(
    const fixedInternalValueFvPatchField& fivpf
)
:
    zeroGradientFvPatchField<Type>(fivpf)
{}


template<class Type>
CML::fixedInternalValueFvPatchField<Type>::fixedInternalValueFvPatchField
(
    const fixedInternalValueFvPatchField& fivpf,
    const DimensionedField<Type, volMesh>& iF
)
:
    zeroGradientFvPatchField<Type>(fivpf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::fixedInternalValueFvPatchField<Type>::manipulateMatrix
(
    fvMatrix<Type>& matrix
)
{
    // Apply the patch internal field as a constraint in the matrix
    matrix.setValues(this->patch().faceCells(), this->patchInternalField());
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
