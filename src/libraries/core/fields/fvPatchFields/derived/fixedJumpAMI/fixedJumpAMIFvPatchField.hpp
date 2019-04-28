/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
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
    CML::fixedJumpAMIFvPatchField

Description
    This boundary condition provides a jump condition, across non-conformal
    cyclic path-pairs, employing an arbitraryMeshInterface (AMI).

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
        type            fixedJumpAMI;
        patchType       cyclic;
        jump            uniform 10;
    }
    \endverbatim

    The above example shows the use of a fixed jump of '10'.

Note
     The underlying \c patchType should be set to \c cyclicAMI

See also
    CML::jumpCyclicAMIFvPatchField


\*---------------------------------------------------------------------------*/

#ifndef fixedJumpAMIFvPatchField_HPP
#define fixedJumpAMIFvPatchField_HPP

#include "jumpCyclicAMIFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class fixedJumpAMIFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class fixedJumpAMIFvPatchField
:
    public jumpCyclicAMIFvPatchField<Type>
{

protected:

    // Protected data

        //- "jump" field
        Field<Type> jump_;


public:

    //- Runtime type information
    TypeName("fixedJumpAMI");


    // Constructors

        //- Construct from patch and internal field
        fixedJumpAMIFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        fixedJumpAMIFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given fixedJumpAMIFvPatchField onto a
        //  new patch
        fixedJumpAMIFvPatchField
        (
            const fixedJumpAMIFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        fixedJumpAMIFvPatchField
        (
            const fixedJumpAMIFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new fixedJumpAMIFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        fixedJumpAMIFvPatchField
        (
            const fixedJumpAMIFvPatchField<Type>&,
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
                new fixedJumpAMIFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Access

            //- Return the "jump" across the patch
            virtual tmp<Field<Type> > jump() const;


        // Mapping functions

            //- Map (and resize as needed) from self given a mapping object
            virtual void autoMap(const fvPatchFieldMapper&);

            //- Reverse map the given fvPatchField onto this fvPatchField
            virtual void rmap(const fvPatchField<Type>&, const labelList&);


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::fixedJumpAMIFvPatchField<Type>::fixedJumpAMIFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    jumpCyclicAMIFvPatchField<Type>(p, iF),
    jump_(this->size(), Zero)
{}


template<class Type>
CML::fixedJumpAMIFvPatchField<Type>::fixedJumpAMIFvPatchField
(
    const fixedJumpAMIFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    jumpCyclicAMIFvPatchField<Type>(ptf, p, iF, mapper),
    jump_(ptf.jump_, mapper)
{}


template<class Type>
CML::fixedJumpAMIFvPatchField<Type>::fixedJumpAMIFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    jumpCyclicAMIFvPatchField<Type>(p, iF),
    jump_(p.size(), Zero)
{
    if (this->cyclicAMIPatch().owner())
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
CML::fixedJumpAMIFvPatchField<Type>::fixedJumpAMIFvPatchField
(
    const fixedJumpAMIFvPatchField<Type>& ptf
)
:
    jumpCyclicAMIFvPatchField<Type>(ptf),
    jump_(ptf.jump_)
{}


template<class Type>
CML::fixedJumpAMIFvPatchField<Type>::fixedJumpAMIFvPatchField
(
    const fixedJumpAMIFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    jumpCyclicAMIFvPatchField<Type>(ptf, iF),
    jump_(ptf.jump_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::Field<Type> > CML::fixedJumpAMIFvPatchField<Type>::jump() const
{
    if (this->cyclicAMIPatch().owner())
    {
        return jump_;
    }
    else
    {
        const fixedJumpAMIFvPatchField& nbrPatch =
            refCast<const fixedJumpAMIFvPatchField<Type> >
            (
                this->neighbourPatchField()
            );

        if (this->cyclicAMIPatch().applyLowWeightCorrection())
        {
            return this->cyclicAMIPatch().interpolate
            (
                nbrPatch.jump(),
                Field<Type>(this->size(), Zero)
            );
        }
        else
        {
            return this->cyclicAMIPatch().interpolate(nbrPatch.jump());
        }
    }
}


template<class Type>
void CML::fixedJumpAMIFvPatchField<Type>::autoMap
(
    const fvPatchFieldMapper& m
)
{
    jumpCyclicAMIFvPatchField<Type>::autoMap(m);
    jump_.autoMap(m);
}


template<class Type>
void CML::fixedJumpAMIFvPatchField<Type>::rmap
(
    const fvPatchField<Type>& ptf,
    const labelList& addr
)
{
    jumpCyclicAMIFvPatchField<Type>::rmap(ptf, addr);

    const fixedJumpAMIFvPatchField<Type>& tiptf =
        refCast<const fixedJumpAMIFvPatchField<Type> >(ptf);
    jump_.rmap(tiptf.jump_, addr);
}


template<class Type>
void CML::fixedJumpAMIFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    os.writeKeyword("patchType") << this->interfaceFieldType()
        << token::END_STATEMENT << nl;

    if (this->cyclicAMIPatch().owner())
    {
        jump_.writeEntry("jump", os);
    }

    this->writeEntry("value", os);
}


#endif
