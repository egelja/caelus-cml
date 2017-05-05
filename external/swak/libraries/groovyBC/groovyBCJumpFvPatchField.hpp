/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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
    CML::groovyBCJumpFvPatchField

Description
    CML::groovyBCJumpFvPatchField

SourceFiles
    groovyBCJumpFvPatchField.cpp

Contributors/Copyright:
    2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef groovyBCJumpFvPatchField_H
#define groovyBCJumpFvPatchField_H

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

#include "fixedJumpFvPatchField.hpp"

#include "PatchValueExpressionDriver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class groovyBCFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class groovyBCJumpFvPatchField
:
    public fixedJumpFvPatchField<Type>
{

protected:

    // Protected data

    PatchValueExpressionDriver driver_;

    exprString jumpExpression_;

public:

    //- Runtime type information
    TypeName("groovyBCJump");


    // Constructors

    //- Construct from patch and internal field
    groovyBCJumpFvPatchField
    (
        const fvPatch&,
        const DimensionedField<Type, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    groovyBCJumpFvPatchField
    (
        const fvPatch&,
        const DimensionedField<Type, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given groovyBCJumpFvPatchField onto a new patch
    groovyBCJumpFvPatchField
    (
        const groovyBCJumpFvPatchField<Type>&,
        const fvPatch&,
        const DimensionedField<Type, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    groovyBCJumpFvPatchField
    (
        const groovyBCJumpFvPatchField<Type>&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
                (
                    new groovyBCJumpFvPatchField<Type>(*this)
                );
        }

    //- Construct as copy setting internal field reference
    groovyBCJumpFvPatchField
    (
        const groovyBCJumpFvPatchField<Type>&,
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
                    new groovyBCJumpFvPatchField<Type>(*this, iF)
                );
        }


    // Evaluation functions

    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();

    // Member functions

    //- Write
    virtual void write(Ostream&) const;
};

//- Specialisation of the jump-condition for the pressure
template<>
void groovyBCJumpFvPatchField<scalar>::updateCoeffs();

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
groovyBCJumpFvPatchField<Type>::groovyBCJumpFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedJumpFvPatchField<Type>(p, iF),
    driver_(this->patch()),
    jumpExpression_("0")
{
    if(debug) {
        Info << "groovyBCJumpFvPatchField<Type>::groovyBCJumpFvPatchField 1" << endl;
    }
}


template<class Type>
groovyBCJumpFvPatchField<Type>::groovyBCJumpFvPatchField
(
    const groovyBCJumpFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedJumpFvPatchField<Type>(ptf, p, iF, mapper),
    driver_(this->patch(),ptf.driver_),
    jumpExpression_(ptf.jumpExpression_)
{
    if(debug) {
        Info << "groovyBCJumpFvPatchField<Type>::groovyBCJumpFvPatchField 2" << endl;
    }
}


template<class Type>
groovyBCJumpFvPatchField<Type>::groovyBCJumpFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fixedJumpFvPatchField<Type>(p, iF),
    driver_(dict,this->patch()),
    jumpExpression_(
        dict.lookup("jumpExpression"),
        dict
    )
{
    if(debug) {
        Info << "groovyBCJumpFvPatchField<Type>::groovyBCJumpFvPatchField 3" << endl;
    }

    driver_.readVariablesAndTables(dict);

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
groovyBCJumpFvPatchField<Type>::groovyBCJumpFvPatchField
(
    const groovyBCJumpFvPatchField<Type>& ptf
)
:
    fixedJumpFvPatchField<Type>(ptf),
    driver_(this->patch(),ptf.driver_),
    jumpExpression_(ptf.jumpExpression_)
{
    if(debug) {
        Info << "groovyBCJumpFvPatchField<Type>::groovyBCJumpFvPatchField 4" << endl;
    }
}


template<class Type>
groovyBCJumpFvPatchField<Type>::groovyBCJumpFvPatchField
(
    const groovyBCJumpFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedJumpFvPatchField<Type>(ptf, iF),
    driver_(this->patch(),ptf.driver_),
    jumpExpression_(ptf.jumpExpression_)
{
    if(debug) {
        Info << "groovyBCJumpFvPatchField<Type>::groovyBCJumpFvPatchField 5" << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void groovyBCJumpFvPatchField<Type>::write(Ostream& os) const
{
    if(debug) {
        Info << "groovyBCJumpFvPatchField<Type>::write" << endl;
    }
    fixedJumpFvPatchField<Type>::write(os);
    os.writeKeyword("jumpExpression")
        << jumpExpression_ << token::END_STATEMENT << nl;

    driver_.writeCommon(os,debug);

    this->writeEntry("value", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    #endif

// ************************************************************************* //
