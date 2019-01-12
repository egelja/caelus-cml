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
    CML::groovyBCFixedValueFvPatchField

Description
    CML::groovyBCFixedValueFvPatchField

SourceFiles
    groovyBCFixedValueFvPatchField.cpp

Contributors/Copyright:
    2009-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef groovyBCFixedValueFvPatchField_H
#define groovyBCFixedValueFvPatchField_H

#include "fixedValueFvPatchField.hpp"

#include "PatchValueExpressionDriver.hpp"

#include "groovyBCCommon.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class groovyBCFixedValueFvPatch Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class groovyBCFixedValueFvPatchField
:
    public fixedValueFvPatchField<Type>,
    public groovyBCCommon<Type>
{

protected:

    // Protected data

    PatchValueExpressionDriver driver_;

public:

    //- Runtime type information
    TypeName("groovyBCFixedValue");


    // Constructors

    //- Construct from patch and internal field
    groovyBCFixedValueFvPatchField
    (
        const fvPatch&,
        const DimensionedField<Type, volMesh>&
    );

    //- Construct from patch, internal field and dictionary
    groovyBCFixedValueFvPatchField
    (
        const fvPatch&,
        const DimensionedField<Type, volMesh>&,
        const dictionary&
    );

    //- Construct by mapping given groovyBCFixedValueFvPatchField onto a new patch
    groovyBCFixedValueFvPatchField
    (
        const groovyBCFixedValueFvPatchField<Type>&,
        const fvPatch&,
        const DimensionedField<Type, volMesh>&,
        const fvPatchFieldMapper&
    );

    //- Construct as copy
    groovyBCFixedValueFvPatchField
    (
        const groovyBCFixedValueFvPatchField<Type>&
    );

    //- Construct and return a clone
    virtual tmp<fvPatchField<Type> > clone() const
        {
            return tmp<fvPatchField<Type> >
                (
                    new groovyBCFixedValueFvPatchField<Type>(*this)
                );
        }

    //- Construct as copy setting internal field reference
    groovyBCFixedValueFvPatchField
    (
        const groovyBCFixedValueFvPatchField<Type>&,
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
                    new groovyBCFixedValueFvPatchField<Type>(*this, iF)
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
groovyBCFixedValueFvPatchField<Type>::groovyBCFixedValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(p, iF),
    groovyBCCommon<Type>(false),
    driver_(this->patch())
{
    if(debug) {
        Info << "groovyBCFixedValueFvPatchField<Type>::groovyBCFixedValueFvPatchField 1" << endl;
    }
}


template<class Type>
groovyBCFixedValueFvPatchField<Type>::groovyBCFixedValueFvPatchField
(
    const groovyBCFixedValueFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fixedValueFvPatchField<Type>(ptf, p, iF, mapper),
    groovyBCCommon<Type>(ptf),
    driver_(this->patch(),ptf.driver_)
{
    if(debug) {
        Info << "groovyBCFixedValueFvPatchField<Type>::groovyBCFixedValueFvPatchField 2" << endl;
    }
}


template<class Type>
groovyBCFixedValueFvPatchField<Type>::groovyBCFixedValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    fixedValueFvPatchField<Type>(p, iF),
    groovyBCCommon<Type>(dict,false),
    driver_(dict,this->patch())
{
    if(debug) {
        Info << "groovyBCFixedValueFvPatchField<Type>::groovyBCFixedValueFvPatchField 3" << endl;
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
        (*this)==pTraits<Type>::zero;

        WarningInFunction
            << "No value defined for " << this->dimensionedInternalField().name()
            << " on " << this->patch().name() << " therefore would be undefined "
            << pTraits<Type>::zero
            << endl;
    }

    if(this->evaluateDuringConstruction()) {
        // make sure that this works with potentialSolver or other solvers that don't evaluate the BCs
        this->evaluate();
    } else {
        // original does nothing
    }
}


template<class Type>
groovyBCFixedValueFvPatchField<Type>::groovyBCFixedValueFvPatchField
(
    const groovyBCFixedValueFvPatchField<Type>& ptf
)
:
    fixedValueFvPatchField<Type>(ptf),
    groovyBCCommon<Type>(ptf),
    driver_(this->patch(),ptf.driver_)
{
    if(debug) {
        Info << "groovyBCFixedValueFvPatchField<Type>::groovyBCFixedValueFvPatchField 4" << endl;
    }
}


template<class Type>
groovyBCFixedValueFvPatchField<Type>::groovyBCFixedValueFvPatchField
(
    const groovyBCFixedValueFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    fixedValueFvPatchField<Type>(ptf, iF),
    groovyBCCommon<Type>(ptf),
    driver_(this->patch(),ptf.driver_)
{
    if(debug) {
        Info << "groovyBCFixedValueFvPatchField<Type>::groovyBCFixedValueFvPatchField 5" << endl;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void groovyBCFixedValueFvPatchField<Type>::updateCoeffs()
{
    if(debug) {
        Info << "groovyBCFixedValueFvPatchField<Type>::updateCoeffs" << endl;
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
        Info << "groovyBCFixedValueFvPatchField<Type>::updateCoeffs - updating" << endl;
    }

    driver_.clearVariables();

    (*this) == driver_.evaluate<Type>(this->valueExpression_);

    fixedValueFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void groovyBCFixedValueFvPatchField<Type>::write(Ostream& os) const
{
    if(debug) {
        Info << "groovyBCFixedValueFvPatchField<Type>::write" << endl;
    }
    fixedValueFvPatchField<Type>::write(os);
    groovyBCCommon<Type>::write(os);

    driver_.writeCommon(os,this->debug_ || debug);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    #endif

// ************************************************************************* //
