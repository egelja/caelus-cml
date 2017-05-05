/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
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
    CML::groovyFixedNormalSlipPointPatchField

Description
    CML::groovyFixedNormalSlipPointPatchField

SourceFiles
    groovyFixedNormalSlipPointPatchField.cpp

Contributors/Copyright:
    2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef groovyFixedNormalSlipPointPatchField_H
#define groovyFixedNormalSlipPointPatchField_H

#include "slipPointPatchField.hpp"
#define SlipPointPatchFieldType slipPointPatchField<Type>

#include "PatchValueExpressionDriver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class groovyTotalPressurePointPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class groovyFixedNormalSlipPointPatchField
:
    public SlipPointPatchFieldType
{
    // Private data

    exprString fixedValueExpression_;

    exprString normalExpression_;

    PatchValueExpressionDriver driver_;

public:

    //- Runtime type information
    TypeName("groovyFixedNormalSlip");


    // Constructors

        //- Construct from patch and internal field
        groovyFixedNormalSlipPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&
        );

        //- Construct from patch, internal field and dictionary
        groovyFixedNormalSlipPointPatchField
        (
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const dictionary&
        );

        //- Construct by mapping given groovyFixedNormalSlipPointPatchField
        //  onto a new patch
        groovyFixedNormalSlipPointPatchField
        (
            const groovyFixedNormalSlipPointPatchField&,
            const pointPatch&,
            const DimensionedField<Type, pointMesh>&,
            const pointPatchFieldMapper&
        );

        //- Construct as copy
        groovyFixedNormalSlipPointPatchField
        (
            const groovyFixedNormalSlipPointPatchField&
        );

        //- Construct and return a clone
        virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
            (
                new groovyFixedNormalSlipPointPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        groovyFixedNormalSlipPointPatchField
        (
            const groovyFixedNormalSlipPointPatchField&,
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
                new groovyFixedNormalSlipPointPatchField<Type>(*this, iF)
            );
        }

        // Mapping functions

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

#include "addToRunTimeSelectionTable.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

#include "groovyBCCommon.hpp"

#include "pointPatchFieldMapper.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::groovyFixedNormalSlipPointPatchField<Type>::groovyFixedNormalSlipPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    SlipPointPatchFieldType(p, iF),
    fixedValueExpression_(""),
    normalExpression_("toPoint(normal())"),
    driver_(groovyBCCommon<Type>::getFvPatch(this->patch()))
{}


template<class Type>
CML::groovyFixedNormalSlipPointPatchField<Type>::groovyFixedNormalSlipPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    SlipPointPatchFieldType(p, iF),
    fixedValueExpression_(
	dict.lookup("fixedValueExpression"),
	dict
    ),
    normalExpression_(
	dict.lookup("normalExpression"),
	dict
    ),
    driver_(dict,groovyBCCommon<Type>::getFvPatch(this->patch()))
{
}


template<class Type>
CML::groovyFixedNormalSlipPointPatchField<Type>::groovyFixedNormalSlipPointPatchField
(
    const groovyFixedNormalSlipPointPatchField& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    SlipPointPatchFieldType(ptf, p, iF, mapper),
    fixedValueExpression_(ptf.fixedValueExpression_),
    normalExpression_(ptf.normalExpression_),
    driver_(groovyBCCommon<Type>::getFvPatch(this->patch()),ptf.driver_)
{}


template<class Type>
CML::groovyFixedNormalSlipPointPatchField<Type>::groovyFixedNormalSlipPointPatchField
(
    const groovyFixedNormalSlipPointPatchField& tppsf
)
:
    SlipPointPatchFieldType(tppsf),
    fixedValueExpression_(tppsf.fixedValueExpression_),
    normalExpression_(tppsf.normalExpression_),
    driver_(groovyBCCommon<Type>::getFvPatch(this->patch()),tppsf.driver_)
{}


template<class Type>
CML::groovyFixedNormalSlipPointPatchField<Type>::groovyFixedNormalSlipPointPatchField
(
    const groovyFixedNormalSlipPointPatchField& tppsf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    SlipPointPatchFieldType(tppsf, iF),
    fixedValueExpression_(tppsf.fixedValueExpression_),
    normalExpression_(tppsf.normalExpression_),
    driver_(groovyBCCommon<Type>::getFvPatch(this->patch()),tppsf.driver_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::groovyFixedNormalSlipPointPatchField<Type>::evaluate
(
    const Pstream::commsTypes commsType
)
{
    driver_.clearVariables();

    Field<vector> n(driver_.evaluate<vector>(this->normalExpression_,true));
    n/=mag(n); // normalize
    Field<Type> val(driver_.evaluate<Type>(this->fixedValueExpression_,true));

    tmp<Field<Type> > tvalues=transform(I - n*n, this->patchInternalField())+transform(n*n,val);

    // Get internal field to insert values into
    Field<Type>& iF = const_cast<Field<Type>&>(this->internalField());

    this->setInInternalField(iF, tvalues());
}

template<class Type>
void CML::groovyFixedNormalSlipPointPatchField<Type>::write(Ostream& os) const
{
    SlipPointPatchFieldType::write(os);

    os.writeKeyword("fixedValueExpression")
        << fixedValueExpression_ << token::END_STATEMENT << nl;

    os.writeKeyword("normalExpression")
        << normalExpression_ << token::END_STATEMENT << nl;

    os.writeKeyword("value")
        << this->patchInternalField() << token::END_STATEMENT << nl;

    if(this->fixedValueExpression_!="") {
        os.writeKeyword("fixedValue")
            << const_cast<PatchValueExpressionDriver&>(driver_).evaluate<Type>(this->fixedValueExpression_,true)
                << token::END_STATEMENT << nl;
    }
    os.writeKeyword("normal")
        << const_cast<PatchValueExpressionDriver&>(driver_).evaluate<Type>(this->normalExpression_,true)
            << token::END_STATEMENT << nl;

    driver_.writeCommon(os,debug);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
