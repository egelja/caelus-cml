/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2010 OpenCFD Ltd.
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
    CML::groovyFixedNormalSlipFvPatchField

Description
    CML::groovyFixedNormalSlipFvPatchField

SourceFiles
    groovyFixedNormalSlipFvPatchField.cpp

Contributors/Copyright:
    2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef groovyFixedNormalSlipFvPatchField_H
#define groovyFixedNormalSlipFvPatchField_H

#include "fixedNormalSlipFvPatchField.hpp"

#include "PatchValueExpressionDriver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class groovyTotalPressureFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class groovyFixedNormalSlipFvPatchField
:
    public fixedNormalSlipFvPatchField<Type>
{
    // Private data

    exprString fixedValueExpression_;

    PatchValueExpressionDriver driver_;

public:

    //- Runtime type information
    TypeName("groovyFixedNormalSlip");


    // Constructors

        //- Construct from patch and internal field
        groovyFixedNormalSlipFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        groovyFixedNormalSlipFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given groovyFixedNormalSlipFvPatchField
        //  onto a new patch
        groovyFixedNormalSlipFvPatchField
        (
            const groovyFixedNormalSlipFvPatchField&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        groovyFixedNormalSlipFvPatchField
        (
            const groovyFixedNormalSlipFvPatchField&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
            (
                new groovyFixedNormalSlipFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        groovyFixedNormalSlipFvPatchField
        (
            const groovyFixedNormalSlipFvPatchField&,
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
                new groovyFixedNormalSlipFvPatchField<Type>(*this, iF)
            );
        }

        // Mapping functions

        // Evaluation functions

            //- Update the coefficients associated with the patch field
            virtual void updateCoeffs();


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "addToRunTimeSelectionTable.hpp"
#include "fvPatchFieldMapper.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

#include "groovyBCCommon.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::groovyFixedNormalSlipFvPatchField<Type>::groovyFixedNormalSlipFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedNormalSlipFvPatchField<Type>(p, iF),
    fixedValueExpression_(groovyBCCommon<Type>::nullValue()),
    driver_(this->patch())
{}


template<class Type>
CML::groovyFixedNormalSlipFvPatchField<Type>::groovyFixedNormalSlipFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fixedNormalSlipFvPatchField<Type>(p, iF),
    fixedValueExpression_(
        dict.lookup("fixedValueExpression"),
        dict
    ),
    driver_(dict,this->patch())
{
    this->evaluate();
}


template<class Type>
CML::groovyFixedNormalSlipFvPatchField<Type>::groovyFixedNormalSlipFvPatchField
(
    const groovyFixedNormalSlipFvPatchField& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedNormalSlipFvPatchField<Type>(ptf, p, iF, mapper),
    fixedValueExpression_(ptf.fixedValueExpression_),
    driver_(this->patch(),ptf.driver_)
{}


template<class Type>
CML::groovyFixedNormalSlipFvPatchField<Type>::groovyFixedNormalSlipFvPatchField
(
    const groovyFixedNormalSlipFvPatchField& tppsf
)
:
    fixedNormalSlipFvPatchField<Type>(tppsf),
    fixedValueExpression_(tppsf.fixedValueExpression_),
    driver_(this->patch(),tppsf.driver_)
{}


template<class Type>
CML::groovyFixedNormalSlipFvPatchField<Type>::groovyFixedNormalSlipFvPatchField
(
    const groovyFixedNormalSlipFvPatchField& tppsf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedNormalSlipFvPatchField<Type>(tppsf, iF),
    fixedValueExpression_(tppsf.fixedValueExpression_),
    driver_(this->patch(),tppsf.driver_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::groovyFixedNormalSlipFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    driver_.clearVariables();

    this->fixedValue()=driver_.evaluate<Type>(this->fixedValueExpression_);

    fixedNormalSlipFvPatchField<Type>::updateCoeffs();
}

template<class Type>
void CML::groovyFixedNormalSlipFvPatchField<Type>::write(Ostream& os) const
{
    fixedNormalSlipFvPatchField<Type>::write(os);

    this->writeEntry("value", os);

    os.writeKeyword("fixedValueExpression")
        << fixedValueExpression_ << token::END_STATEMENT << nl;

    driver_.writeCommon(os,debug);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
