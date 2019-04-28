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
    CML::groovyBCFvPatchField

Description
    CML::groovyBCFvPatchField

SourceFiles
    groovyBCFvPatchField.cpp

Contributors/Copyright:
    2009-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef groovyBCFvPatchField_H
#define groovyBCFvPatchField_H

#include "mixedFvPatchField.hpp"

#include "PatchValueExpressionDriver.hpp"

#include "groovyBCCommon.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class groovyBCFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class groovyBCFvPatchField
:
    public mixedFvPatchField<Type>,
    public groovyBCCommon<Type>
{

protected:

    // Protected data

    PatchValueExpressionDriver driver_;

public:

    //- Runtime type information
    TypeName("groovyBC");


    // Constructors

    //- Construct from patch and internal field
    groovyBCFvPatchField
    (
        const fvPatch&,
        const DimensionedField<Type, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    groovyBCFvPatchField
    (
        const fvPatch&,
        const DimensionedField<Type, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given groovyBCFvPatchField onto a new patch
    groovyBCFvPatchField
    (
        const groovyBCFvPatchField<Type>&,
        const fvPatch&,
        const DimensionedField<Type, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    groovyBCFvPatchField
    (
        const groovyBCFvPatchField<Type>&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
                (
                    new groovyBCFvPatchField<Type>(*this)
                );
        }

    //- Construct as copy setting internal field reference
    groovyBCFvPatchField
    (
        const groovyBCFvPatchField<Type>&,
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
                    new groovyBCFvPatchField<Type>(*this, iF)
                );
        }


    // Member functions

    //- Update the coefficients associated with the patch field
    virtual void updateCoeffs();

    //- Write
    virtual void write(Ostream&) const;
};


    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
groovyBCFvPatchField<Type>::groovyBCFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    mixedFvPatchField<Type>(p, iF),
    groovyBCCommon<Type>(true),
    driver_(this->patch())
{
    if(debug) {
        Info << "groovyBCFvPatchField<Type>::groovyBCFvPatchField 1" << endl;
    }

    this->refValue() = pTraits<Type>::zero;
    this->refGrad() = pTraits<Type>::zero;
    this->valueFraction() = 1;
}


template<class Type>
groovyBCFvPatchField<Type>::groovyBCFvPatchField
(
    const groovyBCFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchField<Type>(ptf, p, iF, mapper),
    groovyBCCommon<Type>(ptf),
    driver_(this->patch(),ptf.driver_)
{
    if(debug) {
        Info << "groovyBCFvPatchField<Type>::groovyBCFvPatchField 2" << endl;
    }
}


template<class Type>
groovyBCFvPatchField<Type>::groovyBCFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchField<Type>(p, iF),
    groovyBCCommon<Type>(dict,true),
    driver_(dict,this->patch())
{
    if(debug) {
        Info << "groovyBCFvPatchField<Type>::groovyBCFvPatchField 3" << endl;
    }

    driver_.readVariablesAndTables(dict);

    if (dict.found("refValue")) {
        this->refValue() = Field<Type>("refValue", dict, p.size());
    } else {
        this->refValue() = pTraits<Type>::zero;
    }

    if (dict.found("value"))
    {
        fvPatchField<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
	if (!dict.found("refValue")) {
 	    // make sure that refValue has a sensible value for the "update" below
	    this->refValue() = Field<Type>("value", dict, p.size());
	}
    }
    else
    {
        fvPatchField<Type>::operator=(this->refValue());
        WarningInFunction
            << "No value defined for " << this->dimensionedInternalField().name()
            << " on " << this->patch().name() << " therefore using "
            << this->refValue()
            << endl;
    }

    if (dict.found("refGradient")) {
        this->refGrad() = Field<Type>("refGradient", dict, p.size());
    } else {
        this->refGrad() = pTraits<Type>::zero;
    }

    if (dict.found("valueFraction")) {
        this->valueFraction() = Field<scalar>("valueFraction", dict, p.size());
    } else {
        this->valueFraction() = 1;
    }

    if(this->evaluateDuringConstruction()) {
        // make sure that this works with potentialSolver or other solvers that don't evaluate the BCs
        this->evaluate();
    } else {
        // emulate mixedFvPatchField<Type>::evaluate, but avoid calling "our" updateCoeffs
        if (!this->updated())
        {
            this->mixedFvPatchField<Type>::updateCoeffs();
        }

        Field<Type>::operator=
            (
                this->valueFraction()*this->refValue()
                +
                (1.0 - this->valueFraction())*
                (
                    this->patchInternalField()
                    + this->refGrad()/this->patch().deltaCoeffs()
                )
            );

        fvPatchField<Type>::evaluate();
    }
}


template<class Type>
groovyBCFvPatchField<Type>::groovyBCFvPatchField
(
    const groovyBCFvPatchField<Type>& ptf
)
:
    mixedFvPatchField<Type>(ptf),
    groovyBCCommon<Type>(ptf),
    driver_(this->patch(),ptf.driver_)
{
    if(debug) {
        Info << "groovyBCFvPatchField<Type>::groovyBCFvPatchField 4" << endl;
    }
}


template<class Type>
groovyBCFvPatchField<Type>::groovyBCFvPatchField
(
    const groovyBCFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    mixedFvPatchField<Type>(ptf, iF),
    groovyBCCommon<Type>(ptf),
    driver_(this->patch(),ptf.driver_)
{
    if(debug) {
        Info << "groovyBCFvPatchField<Type>::groovyBCFvPatchField 5" << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void groovyBCFvPatchField<Type>::updateCoeffs()
{
    if(debug) {
        Info << "groovyBCFvPatchField<Type>::updateCoeffs" << endl;
        Info << "Value: " << this->valueExpression_ << endl;
        Info << "Gradient: " << this->gradientExpression_ << endl;
        Info << "Fraction: " << this->fractionExpression_ << endl;
        Info << "Variables: ";
        driver_.writeVariableStrings(Info) << endl;
    }
    if (this->updated())
    {
        return;
    }

    if(debug) {
        Info << "groovyBCFvPatchField<Type>::updateCoeffs - updating" << endl;
    }

    driver_.clearVariables();

    this->refValue() = driver_.evaluate<Type>(this->valueExpression_);
    this->refGrad() = driver_.evaluate<Type>(this->gradientExpression_);
    this->valueFraction() = driver_.evaluate<scalar>(this->fractionExpression_);

    mixedFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void groovyBCFvPatchField<Type>::write(Ostream& os) const
{
    if(debug) {
        Info << "groovyBCFvPatchField<Type>::write" << endl;
    }
    mixedFvPatchField<Type>::write(os);
    groovyBCCommon<Type>::write(os);

    driver_.writeCommon(os,this->debug_ || debug);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    #endif

// ************************************************************************* //
