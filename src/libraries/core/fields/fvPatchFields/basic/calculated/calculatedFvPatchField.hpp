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
    CML::calculatedFvPatchField

Description
    CML::calculatedFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef calculatedFvPatchField_H
#define calculatedFvPatchField_H

#include "fvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class calculatedFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class calculatedFvPatchField
:
    public fvPatchField<Type>
{

public:

    //- Runtime type information
    TypeName("calculated");


    // Constructors

        //- Construct from patch and internal field
        calculatedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        calculatedFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&,
            const bool valueRequired=false
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        calculatedFvPatchField
        (
            const calculatedFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        calculatedFvPatchField
        (
            const calculatedFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new calculatedFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        calculatedFvPatchField
        (
            const calculatedFvPatchField<Type>&,
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
                new calculatedFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return true if this patch field fixes a value.
            //  Needed to check if a level has to be specified while solving
            //  Poissons equations.
            virtual bool fixesValue() const
            {
                return true;
            }


        // Evaluation functions

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type> > valueInternalCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the value of this patchField with given weights
            virtual tmp<Field<Type> > valueBoundaryCoeffs
            (
                const tmp<scalarField>&
            ) const;

            //- Return the matrix diagonal coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            tmp<Field<Type> > gradientInternalCoeffs() const;

            //- Return the matrix source coefficients corresponding to the
            //  evaluation of the gradient of this patchField
            tmp<Field<Type> > gradientBoundaryCoeffs() const;


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvPatchFieldMapper.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
const word& fvPatchField<Type>::calculatedType()
{
    return calculatedFvPatchField<Type>::typeName;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
calculatedFvPatchField<Type>::calculatedFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fvPatchField<Type>(p, iF)
{}


template<class Type>
calculatedFvPatchField<Type>::calculatedFvPatchField
(
    const calculatedFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fvPatchField<Type>(ptf, p, iF, mapper)
{}


template<class Type>
calculatedFvPatchField<Type>::calculatedFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict,
    const bool valueRequired
)
:
    fvPatchField<Type>(p, iF, dict, valueRequired)
{}


template<class Type>
calculatedFvPatchField<Type>::calculatedFvPatchField
(
    const calculatedFvPatchField<Type>& ptf
)
:
    fvPatchField<Type>(ptf)
{}


template<class Type>
calculatedFvPatchField<Type>::calculatedFvPatchField
(
    const calculatedFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fvPatchField<Type>(ptf, iF)
{}


template<class Type>
tmp<fvPatchField<Type> > fvPatchField<Type>::NewCalculatedType
(
    const fvPatch& p
)
{
    typename patchConstructorTable::iterator patchTypeCstrIter =
        patchConstructorTablePtr_->find(p.type());

    if (patchTypeCstrIter != patchConstructorTablePtr_->end())
    {
        return patchTypeCstrIter()
        (
            p,
            DimensionedField<Type, volMesh>::null()
        );
    }
    else
    {
        return tmp<fvPatchField<Type> >
        (
            new calculatedFvPatchField<Type>
            (
                p,
                DimensionedField<Type, volMesh>::null()
            )
        );
    }
}


template<class Type>
template<class Type2>
tmp<fvPatchField<Type> > fvPatchField<Type>::NewCalculatedType
(
    const fvPatchField<Type2>& pf
)
{
    return NewCalculatedType(pf.patch());
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<Field<Type> > calculatedFvPatchField<Type>::valueInternalCoeffs
(
    const tmp<scalarField>&
) const
{
    FatalErrorInFunction
        << "\n    "
           "valueInternalCoeffs cannot be called for a calculatedFvPatchField"
        << "\n    on patch " << this->patch().name()
        << " of field " << this->dimensionedInternalField().name()
        << " in file " << this->dimensionedInternalField().objectPath()
        << "\n    You are probably trying to solve for a field with a "
           "default boundary condition."
        << exit(FatalError);

    return *this;
}


template<class Type>
tmp<Field<Type> > calculatedFvPatchField<Type>::valueBoundaryCoeffs
(
    const tmp<scalarField>&
) const
{
    FatalErrorInFunction
        << "\n    "
           "valueBoundaryCoeffs cannot be called for a calculatedFvPatchField"
        << "\n    on patch " << this->patch().name()
        << " of field " << this->dimensionedInternalField().name()
        << " in file " << this->dimensionedInternalField().objectPath()
        << "\n    You are probably trying to solve for a field with a "
           "default boundary condition."
        << exit(FatalError);

    return *this;
}

template<class Type>
tmp<Field<Type> > calculatedFvPatchField<Type>::gradientInternalCoeffs() const
{
    FatalErrorInFunction
        << "\n    "
           "gradientInternalCoeffs cannot be called for a "
           "calculatedFvPatchField"
        << "\n    on patch " << this->patch().name()
        << " of field " << this->dimensionedInternalField().name()
        << " in file " << this->dimensionedInternalField().objectPath()
        << "\n    You are probably trying to solve for a field with a "
           "default boundary condition."
        << exit(FatalError);

    return *this;
}

template<class Type>
tmp<Field<Type> > calculatedFvPatchField<Type>::gradientBoundaryCoeffs() const
{
    FatalErrorInFunction
        << "\n    "
           "gradientBoundaryCoeffs cannot be called for a "
           "calculatedFvPatchField"
        << "\n    on patch " << this->patch().name()
        << " of field " << this->dimensionedInternalField().name()
        << " in file " << this->dimensionedInternalField().objectPath()
        << "\n    You are probably trying to solve for a field with a "
           "default boundary condition."
        << exit(FatalError);

    return *this;
}


// Write
template<class Type>
void calculatedFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
