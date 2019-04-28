/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::extrapolatedCalculatedFvPatchField

Description
    This boundary condition applies a zero-gradient condition from the patch
    internal field onto the patch faces when evaluated but may also be
    assigned. snGrad returns the patch gradient evaluated from the current
    internal and patch field values rather than returning zero.

    \heading Patch usage

    Example of the boundary condition specification:
    \verbatim
    myPatch
    {
        type            extrapolatedCalculated;
    }
    \endverbatim

\*---------------------------------------------------------------------------*/

#ifndef extrapolatedCalculatedFvPatchField_HPP
#define extrapolatedCalculatedFvPatchField_HPP

#include "calculatedFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class extrapolatedCalculatedFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class extrapolatedCalculatedFvPatchField
:
    public calculatedFvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("extrapolatedCalculated");


    // Constructors

        //- Construct from patch and internal field
        extrapolatedCalculatedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        extrapolatedCalculatedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&,
            const bool valueRequired=false
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        extrapolatedCalculatedFvPatchField
        (
            const extrapolatedCalculatedFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        extrapolatedCalculatedFvPatchField
        (
            const extrapolatedCalculatedFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type>> clone() const
        {
            return tmp<fvPatchField<Type>>
            (
                new extrapolatedCalculatedFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        extrapolatedCalculatedFvPatchField
        (
            const extrapolatedCalculatedFvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<Type>>
            (
                new extrapolatedCalculatedFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Evaluation functions

            //- Evaluate the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            );
};


} // End namespace CML


#include "fvPatchFieldMapper.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::extrapolatedCalculatedFvPatchField<Type>::
extrapolatedCalculatedFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    calculatedFvPatchField<Type>(p, iF)
{}


template<class Type>
CML::extrapolatedCalculatedFvPatchField<Type>::
extrapolatedCalculatedFvPatchField
(
    const extrapolatedCalculatedFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    calculatedFvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
CML::extrapolatedCalculatedFvPatchField<Type>::
extrapolatedCalculatedFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict,
    const bool valueRequired
)
:
    calculatedFvPatchField<Type>(p, iF, dict, valueRequired)
{}


template<class Type>
CML::extrapolatedCalculatedFvPatchField<Type>::
extrapolatedCalculatedFvPatchField
(
    const extrapolatedCalculatedFvPatchField<Type>& ptf
)
:
    calculatedFvPatchField<Type>(ptf)
{}


template<class Type>
CML::extrapolatedCalculatedFvPatchField<Type>::
extrapolatedCalculatedFvPatchField
(
    const extrapolatedCalculatedFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    calculatedFvPatchField<Type>(ptf, iF)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::extrapolatedCalculatedFvPatchField<Type>::evaluate
(
    const Pstream::commsTypes
)
{
    if (!this->updated())
    {
        this->updateCoeffs();
    }

    calculatedFvPatchField<Type>::operator==(this->patchInternalField());
    calculatedFvPatchField<Type>::evaluate();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

