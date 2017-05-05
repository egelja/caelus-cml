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
    CML::compressible::RASModels::kqRWallFunctionFvPatchField

Description
    Boundary condition for turbulence k, Q, and R when using wall functions.
    Simply acts as a zero gradient condition.


\*---------------------------------------------------------------------------*/

#ifndef compressiblekqRWallFunctionFvPatchField_H
#define compressiblekqRWallFunctionFvPatchField_H

#include "zeroGradientFvPatchField.hpp"
#include "fvPatchFieldMapper.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "wallFvPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{

/*---------------------------------------------------------------------------*\
                Class kqRWallFunctionFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class kqRWallFunctionFvPatchField
:
    public zeroGradientFvPatchField<Type>
{

    // Private Member Functions

        //- Check the type of the patch
        void checkType();


public:

    //- Runtime type information
    TypeName("compressible::kqRWallFunction");


    // Constructors

        //- Construct from patch and internal field
        kqRWallFunctionFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        kqRWallFunctionFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given
        // kqRWallFunctionFvPatchField
        //  onto a new patch
        kqRWallFunctionFvPatchField
        (
            const kqRWallFunctionFvPatchField&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        kqRWallFunctionFvPatchField
        (
            const kqRWallFunctionFvPatchField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new kqRWallFunctionFvPatchField(*this)
            );
        }

        //- Construct as copy setting internal field reference
        kqRWallFunctionFvPatchField
        (
            const kqRWallFunctionFvPatchField&,
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
                new kqRWallFunctionFvPatchField(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //- Evaluate the patchField
            virtual void evaluate
            (
				const Pstream::commsTypes commsType=Pstream::Pstream::blocking
            );


        // I-O

            //- Write
            virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
void kqRWallFunctionFvPatchField<Type>::checkType()
{
    if (!isA<wallFvPatch>(this->patch()))
    {
        FatalErrorIn("kqRWallFunctionFvPatchField::checkType()")
            << "Invalid wall function specification" << nl
            << "    Patch type for patch " << this->patch().name()
            << " must be wall" << nl
            << "    Current patch type is " << this->patch().type()
            << nl << endl
            << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
kqRWallFunctionFvPatchField<Type>::kqRWallFunctionFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    zeroGradientFvPatchField<Type>(p, iF)
{
    checkType();
}


template<class Type>
kqRWallFunctionFvPatchField<Type>::kqRWallFunctionFvPatchField
(
    const kqRWallFunctionFvPatchField& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    zeroGradientFvPatchField<Type>(ptf, p, iF, mapper)
{
    checkType();
}


template<class Type>
kqRWallFunctionFvPatchField<Type>::kqRWallFunctionFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    zeroGradientFvPatchField<Type>(p, iF, dict)
{
    checkType();
}


template<class Type>
kqRWallFunctionFvPatchField<Type>::kqRWallFunctionFvPatchField
(
    const kqRWallFunctionFvPatchField& tkqrwfpf
)
:
    zeroGradientFvPatchField<Type>(tkqrwfpf)
{
    checkType();
}


template<class Type>
kqRWallFunctionFvPatchField<Type>::kqRWallFunctionFvPatchField
(
    const kqRWallFunctionFvPatchField& tkqrwfpf,
    const DimensionedField<Type, volMesh>& iF
)
:
    zeroGradientFvPatchField<Type>(tkqrwfpf, iF)
{
    checkType();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void kqRWallFunctionFvPatchField<Type>::evaluate
(
    const Pstream::commsTypes commsType
)
{
    zeroGradientFvPatchField<Type>::evaluate(commsType);
}


template<class Type>
void kqRWallFunctionFvPatchField<Type>::write(Ostream& os) const
{
    zeroGradientFvPatchField<Type>::write(os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace compressible
} // End namespace CML


#endif

// ************************************************************************* //
