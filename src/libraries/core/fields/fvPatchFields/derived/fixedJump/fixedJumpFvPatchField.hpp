/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::fixedJumpFvPatchField

Description
    This boundary condition provides a jump condition, using the \c cyclic
    condition as a base.

    The jump is specified as a fixed value field, applied as an offset to the
    'owner' patch.

Usage
    \table
        Property     | Description             | Required    | Default value
        patchType    | underlying patch type should be \c cyclic| yes |
        jump         | current jump value      | yes         |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            fixedJump;
        patchType       cyclic;
        jump            uniform 10;
    }
    \endverbatim

    The above example shows the use of a fixed jump of '10'.

Note
     The underlying \c patchType should be set to \c cyclic

See also
    CML::jumpCyclicFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef fixedJumpFvPatchField_HPP
#define fixedJumpFvPatchField_HPP

#include "jumpCyclicFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class fixedJumpFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fixedJumpFvPatchField
:
    public jumpCyclicFvPatchField<Type>
{

protected:

    // Protected data

        //- "jump" field
        Field<Type> jump_;


public:

    //- Runtime type information
    TypeName("fixedJump");

    // Constructors

        //- Construct from patch and internal field
        fixedJumpFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixedJumpFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given fixedJumpFvPatchField onto a
        //  new patch
        fixedJumpFvPatchField
        (
            const fixedJumpFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        fixedJumpFvPatchField
        (
            const fixedJumpFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new fixedJumpFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        fixedJumpFvPatchField
        (
            const fixedJumpFvPatchField<Type>&,
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
                new fixedJumpFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return the "jump" across the patch
            virtual tmp<Field<Type> > jump() const;


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap
            (
                const fvPatchField<Type>&,
                const labelList&
            );


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::fixedJumpFvPatchField<Type>::fixedJumpFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    jumpCyclicFvPatchField<Type>(p, iF),
    jump_(this->size(), pTraits<Type>::zero)
{}


template<class Type>
CML::fixedJumpFvPatchField<Type>::fixedJumpFvPatchField
(
    const fixedJumpFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    jumpCyclicFvPatchField<Type>(ptf, p, iF, mapper),
    jump_(ptf.jump_, mapper)
{}


template<class Type>
CML::fixedJumpFvPatchField<Type>::fixedJumpFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    jumpCyclicFvPatchField<Type>(p, iF),
    jump_(p.size(), Zero)
{
    if (this->cyclicPatch().owner())
    {
        jump_ = Field<Type>("jump", dict, p.size());
    }

    if (dict.found("value"))
    {
        fvPatchField<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else
    {
        this->evaluate(Pstream::blocking);
    }
}


template<class Type>
CML::fixedJumpFvPatchField<Type>::fixedJumpFvPatchField
(
    const fixedJumpFvPatchField<Type>& ptf
)
:
    jumpCyclicFvPatchField<Type>(ptf),
    jump_(ptf.jump_)
{}


template<class Type>
CML::fixedJumpFvPatchField<Type>::fixedJumpFvPatchField
(
    const fixedJumpFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    jumpCyclicFvPatchField<Type>(ptf, iF),
    jump_(ptf.jump_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::Field<Type> > CML::fixedJumpFvPatchField<Type>::jump() const
{
    if (this->cyclicPatch().owner())
    {
        return jump_;
    }
    else
    {
        return refCast<const fixedJumpFvPatchField<Type> >
        (
            this->neighbourPatchField()
        ).jump();
    }
}


template<class Type>
void CML::fixedJumpFvPatchField<Type>::autoMap
(
    const fvPatchFieldMapper& m
)
{
    jumpCyclicFvPatchField<Type>::autoMap(m);
    jump_.autoMap(m);
}


template<class Type>
void CML::fixedJumpFvPatchField<Type>::rmap
(
    const fvPatchField<Type>& ptf,
    const labelList& addr
)
{
    jumpCyclicFvPatchField<Type>::rmap(ptf, addr);

    const fixedJumpFvPatchField<Type>& tiptf =
        refCast<const fixedJumpFvPatchField<Type> >(ptf);
    jump_.rmap(tiptf.jump_, addr);
}


template<class Type>
void CML::fixedJumpFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    os.writeKeyword("patchType") << this->interfaceFieldType()
        << token::END_STATEMENT << nl;

    if (this->cyclicPatch().owner())
    {
        jump_.writeEntry("jump", os);
    }

    this->writeEntry("value", os);
}


#endif
