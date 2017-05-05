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
    CML::fixedNormalSlipPointPatchField

Description
    slip with user-specified normal

SourceFiles
    fixedNormalSlipPointPatchField.cpp

\*---------------------------------------------------------------------------*/

#ifndef fixedNormalSlipPointPatchField_H
#define fixedNormalSlipPointPatchField_H

#include "slipPointPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Type>
class fixedNormalSlipPointPatchField;

template<class Type>
Ostream& operator<<
(
    Ostream&,
    const fixedNormalSlipPointPatchField<Type>&
);


/*---------------------------------------------------------------------------*\
                        Class fixedNormalSlipPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fixedNormalSlipPointPatchField
:
    public slipPointPatchField<Type>
{
    // Private data

        //- User specified normal
        vector n_;


public:

    //- Runtime type information
    TypeName("fixedNormalSlip");


    // Constructors

        //- Construct from patch and internal field
        fixedNormalSlipPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixedNormalSlipPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given patchField<Type> onto a new patch
        fixedNormalSlipPointPatchField
        (
            const fixedNormalSlipPointPatchField<Type>&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new fixedNormalSlipPointPatchField<Type>
                (
                    *this
                )
            );
        }

        //- Construct as copy setting internal field reference
        fixedNormalSlipPointPatchField
        (
            const fixedNormalSlipPointPatchField<Type>&,
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
                new fixedNormalSlipPointPatchField<Type>
                (
                    *this,
                    iF
                )
            );
        }

    // Member functions

        // Evaluation functions

            //- Update the patch field
            virtual void evaluate
            (
                const Pstream::commsTypes commsType=Pstream::blocking
            );


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::fixedNormalSlipPointPatchField<Type>::fixedNormalSlipPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    slipPointPatchField<Type>(p, iF),
    n_(vector::max)
{}


template<class Type>
CML::fixedNormalSlipPointPatchField<Type>::fixedNormalSlipPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    slipPointPatchField<Type>(p, iF, dict),
    n_(dict.lookup("n"))
{}


template<class Type>
CML::fixedNormalSlipPointPatchField<Type>::fixedNormalSlipPointPatchField
(
    const fixedNormalSlipPointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    slipPointPatchField<Type>(ptf, p, iF, mapper),
    n_(ptf.n_)
{}


template<class Type>
CML::fixedNormalSlipPointPatchField<Type>::fixedNormalSlipPointPatchField
(
    const fixedNormalSlipPointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    slipPointPatchField<Type>(ptf, iF),
    n_(ptf.n_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::fixedNormalSlipPointPatchField<Type>::evaluate
(
    const Pstream::commsTypes
)
{
    tmp<Field<Type> > tvalues =
        transform(I - n_*n_, this->patchInternalField());

    // Get internal field to insert values into
    Field<Type>& iF = const_cast<Field<Type>&>(this->internalField());

    this->setInInternalField(iF, tvalues());
}


template<class Type>
void CML::fixedNormalSlipPointPatchField<Type>::write(Ostream& os) const
{
    slipPointPatchField<Type>::write(os);
    os.writeKeyword("n")
        << n_ << token::END_STATEMENT << nl;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
