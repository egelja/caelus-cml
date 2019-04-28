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
    CML::groovyBCDirectionFvPatchField

Description
    CML::groovyBCDirectionFvPatchField

SourceFiles
    groovyBCDirectionFvPatchField.cpp

Contributors/Copyright:
    2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef groovyBCDirectionFvPatchField_H
#define groovyBCDirectionFvPatchField_H

#include "directionMixedFvPatchField.hpp"

#include "PatchValueExpressionDriver.hpp"

#include "groovyBCCommon.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class groovyBCFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class groovyBCDirectionFvPatchField
:
    public directionMixedFvPatchField<Type>,
    public groovyBCCommon<Type>
{

protected:

    // Protected data

    PatchValueExpressionDriver driver_;

public:

    //- Runtime type information
    TypeName("groovyBCDirection");


    // Constructors

    //- Construct from patch and internal field
    groovyBCDirectionFvPatchField
    (
        const fvPatch&,
        const DimensionedField<Type, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    groovyBCDirectionFvPatchField
    (
        const fvPatch&,
        const DimensionedField<Type, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given groovyBCDirectionFvPatchField onto a new patch
    groovyBCDirectionFvPatchField
    (
        const groovyBCDirectionFvPatchField<Type>&,
        const fvPatch&,
        const DimensionedField<Type, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    groovyBCDirectionFvPatchField
    (
        const groovyBCDirectionFvPatchField<Type>&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
                (
                    new groovyBCDirectionFvPatchField<Type>(*this)
                );
        }

    //- Construct as copy setting internal field reference
    groovyBCDirectionFvPatchField
    (
        const groovyBCDirectionFvPatchField<Type>&,
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
                    new groovyBCDirectionFvPatchField<Type>(*this, iF)
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
groovyBCDirectionFvPatchField<Type>::groovyBCDirectionFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    directionMixedFvPatchField<Type>(p, iF),
    groovyBCCommon<Type>(
        true,
        false,
        "symmTensor(1,0,0,1,0,1)"
    ),
    driver_(this->patch())
{
    if(debug) {
        Info << "groovyBCDirectionFvPatchField<Type>::groovyBCDirectionFvPatchField 1" << endl;
    }

    this->refValue() = pTraits<Type>::zero;
    this->refGrad() = pTraits<Type>::zero;
}


template<class Type>
groovyBCDirectionFvPatchField<Type>::groovyBCDirectionFvPatchField
(
    const groovyBCDirectionFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    directionMixedFvPatchField<Type>(ptf, p, iF, mapper),
    groovyBCCommon<Type>(ptf),
    driver_(this->patch(),ptf.driver_)
{
    if(debug) {
        Info << "groovyBCDirectionFvPatchField<Type>::groovyBCDirectionFvPatchField 2" << endl;
    }
}


template<class Type>
groovyBCDirectionFvPatchField<Type>::groovyBCDirectionFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    directionMixedFvPatchField<Type>(p, iF),
    groovyBCCommon<Type>(
        dict,
        true,
        false,
        "symmTensor(1,0,0,1,0,1)"
    ),
    driver_(dict,this->patch())
{
    if(debug) {
        Info << "groovyBCDirectionFvPatchField<Type>::groovyBCDirectionFvPatchField 3" << endl;
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
        this->valueFraction() = Field<symmTensor>("valueFraction", dict, p.size());
    } else {
        this->valueFraction() = I;
    }

    if(this->evaluateDuringConstruction()) {
        // make sure that this works with potentialSolver or other solvers that don't evaluate the BCs
        this->evaluate();
    } else {
        // emulate the evaluate of the parent-class
        if (!this->updated())
        {
            this->directionMixedFvPatchField<Type>::updateCoeffs();
        }

        tmp<Field<Type> > normalValue = transform(this->valueFraction(), this->refValue());

        tmp<Field<Type> > gradValue =
            this->patchInternalField() + this->refGrad()/this->patch().deltaCoeffs();
        
        tmp<Field<Type> > transformGradValue =
            transform(I - this->valueFraction(), gradValue);
        
        Field<Type>::operator=(normalValue + transformGradValue);
        
        transformFvPatchField<Type>::evaluate();
    }
}


template<class Type>
groovyBCDirectionFvPatchField<Type>::groovyBCDirectionFvPatchField
(
    const groovyBCDirectionFvPatchField<Type>& ptf
)
:
    directionMixedFvPatchField<Type>(ptf),
    groovyBCCommon<Type>(ptf),
    driver_(this->patch(),ptf.driver_)
{
    if(debug) {
        Info << "groovyBCDirectionFvPatchField<Type>::groovyBCDirectionFvPatchField 4" << endl;
    }
}


template<class Type>
groovyBCDirectionFvPatchField<Type>::groovyBCDirectionFvPatchField
(
    const groovyBCDirectionFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    directionMixedFvPatchField<Type>(ptf, iF),
    groovyBCCommon<Type>(ptf),
    driver_(this->patch(),ptf.driver_)
{
    if(debug) {
        Info << "groovyBCDirectionFvPatchField<Type>::groovyBCDirectionFvPatchField 5" << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void groovyBCDirectionFvPatchField<Type>::updateCoeffs()
{
    if(debug) {
        Info << "groovyBCDirectionFvPatchField<Type>::updateCoeffs" << endl;
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
        Info << "groovyBCDirectionFvPatchField<Type>::updateCoeffs - updating" << endl;
    }

    driver_.clearVariables();

    this->refValue() = driver_.evaluate<Type>(this->valueExpression_);
    this->refGrad() = driver_.evaluate<Type>(this->gradientExpression_);
    this->valueFraction() = driver_.evaluate<symmTensor>(this->fractionExpression_);
    
    directionMixedFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void groovyBCDirectionFvPatchField<Type>::write(Ostream& os) const
{
    if(debug) {
        Info << "groovyBCDirectionFvPatchField<Type>::write" << endl;
    }
    directionMixedFvPatchField<Type>::write(os);
    groovyBCCommon<Type>::write(os);

    driver_.writeCommon(os,this->debug_ || debug);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    #endif

// ************************************************************************* //
