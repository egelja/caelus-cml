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
    CML::groovyBCPointPatchField

Description

SourceFiles
    groovyBCPointPatchField.C

Contributors/Copyright:
    2010-2011, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef groovyBCPointPatchField_H
#define groovyBCPointPatchField_H

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

#include "mixedPointPatchField.hpp"
#define mixedPointPatchFieldType mixedPointPatchField<Type>

#include "PatchValueExpressionDriver.hpp"

#include "groovyBCCommon.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class groovyBCPointPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class groovyBCPointPatchField
:
    public mixedPointPatchFieldType,
    public groovyBCCommon<Type>
{

protected:

    // Protected data

    PatchValueExpressionDriver driver_;

    string nullValue();

public:

    //- Runtime type information
    TypeName("groovyBC");


    // Constructors

    //- Construct from patch and internal field
    groovyBCPointPatchField
    (
        const pointPatch&,
        const DimensionedField<Type, pointMesh>&
    );

    //- Construct from patch, internal field and dictionary
    groovyBCPointPatchField
    (
        const pointPatch&,
        const DimensionedField<Type, pointMesh>&,
        const dictionary&
    );

    //- Construct by mapping given patchField<Type> onto a new patch
    groovyBCPointPatchField
    (
        const groovyBCPointPatchField<Type>&,
        const pointPatch&,
        const DimensionedField<Type, pointMesh>&,
        const pointPatchFieldMapper&
    );

    //- Construct and return a clone
    virtual autoPtr<pointPatchField<Type> > clone() const
        {
            return autoPtr<pointPatchField<Type> >
                (
                    new groovyBCPointPatchField<Type>
                    (
                        *this
                    )
                );
        }

    //- Construct as copy setting internal field reference
    groovyBCPointPatchField
    (
        const groovyBCPointPatchField<Type>&,
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
                    new groovyBCPointPatchField<Type>
                    (
                        *this,
                        iF
                    )
                );
        }


    // Member functions

    // Evaluation functions

    //- Update the patch field
    virtual void updateCoeffs();


    //- Write
    virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "pointPatchFieldMapper.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
groovyBCPointPatchField<Type>::groovyBCPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF
)
:
    mixedPointPatchFieldType(p, iF),
    groovyBCCommon<Type>(false,true),
    driver_(groovyBCCommon<Type>::getFvPatch(this->patch()))
{
    this->refValue() = pTraits<Type>::zero;
    this->valueFraction() = 0.0;
}


template<class Type>
groovyBCPointPatchField<Type>::groovyBCPointPatchField
(
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const dictionary& dict
)
:
    mixedPointPatchFieldType(p, iF),
    groovyBCCommon<Type>(dict,false,true),
    driver_(dict,groovyBCCommon<Type>::getFvPatch(this->patch()))
{
    driver_.readVariablesAndTables(dict);

    if (dict.found("refValue")) {
        this->refValue() = Field<Type>("refValue", dict, p.size());
    } else {
        this->refValue() = pTraits<Type>::zero;
    }

    if (dict.found("value"))
    {
        Field<Type>::operator=
        (
            Field<Type>("value", dict, p.size())
        );
    }
    else
    {
        Field<Type>::operator=(this->refValue());

        WarningInFunction
            << "No value defined for " << this->dimensionedInternalField().name()
            << " on " << this->patch().name() << " therefore using "
            << this->refValue()
            << endl;
    }

    //    this->refGrad() = pTraits<Type>::zero;

    if (dict.found("valueFraction")) {
        this->valueFraction() = Field<scalar>("valueFraction", dict, p.size());
    } else {
        this->valueFraction() = 1;
    }

    if(this->evaluateDuringConstruction()) {
        // make sure that this works with potentialSolver or other solvers that don't evaluate the BCs
        this->evaluate();
    } else {
        // mixed-BC DOES NOT call evaluate during construction
    }
}


template<class Type>
groovyBCPointPatchField<Type>::groovyBCPointPatchField
(
    const groovyBCPointPatchField<Type>& ptf,
    const pointPatch& p,
    const DimensionedField<Type, pointMesh>& iF,
    const pointPatchFieldMapper& mapper
)
:
    mixedPointPatchFieldType
    (
        ptf,
        p,
        iF,
        mapper
    ),
    groovyBCCommon<Type>(ptf),
    driver_(groovyBCCommon<Type>::getFvPatch(this->patch()),ptf.driver_)
{
}


template<class Type>
groovyBCPointPatchField<Type>::groovyBCPointPatchField
(
    const groovyBCPointPatchField<Type>& ptf,
    const DimensionedField<Type, pointMesh>& iF
)
:
    mixedPointPatchFieldType(ptf, iF),
    groovyBCCommon<Type>(ptf),
    driver_(groovyBCCommon<Type>::getFvPatch(this->patch()),ptf.driver_)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// Evaluate patch field
template<class Type>
void groovyBCPointPatchField<Type>::updateCoeffs()
{
    if(debug)
    {
        Info << "groovyBCFvPatchField<Type>::updateCoeffs" << endl;
        Info << "Value: " << this->valueExpression_ << endl;
        //        Info << "Gradient: " << gradientExpression_ << endl;
        Info << "Fraction: " << this->fractionExpression_ << endl;
        Info << "Variables: ";
        driver_.writeVariableStrings(Info)  << endl;
    }
//     if (this->updated())
//     {
//         return;
//     }

    driver_.clearVariables();

    this->refValue() = driver_.evaluate<Type>(this->valueExpression_,true);
    this->valueFraction() = driver_.evaluate<scalar>(this->fractionExpression_,true);
    //    this->refGrad() = driver_.evaluate<Type>(gradientExpression_,true);

   mixedPointPatchFieldType::updateCoeffs();
}


// Write
template<class Type>
void groovyBCPointPatchField<Type>::write(Ostream& os) const
{
    mixedPointPatchFieldType::write(os);
    groovyBCCommon<Type>::write(os);

    this->writeEntry("value", os);

    driver_.writeCommon(os,this->debug_ || debug);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
