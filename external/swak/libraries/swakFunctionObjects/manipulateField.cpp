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

Contributors/Copyright:
    2010-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
    2013 Bruno Santos <wyldckat@gmail.com>

\*---------------------------------------------------------------------------*/

#include "manipulateField.hpp"

#include "FieldValueExpressionDriver.hpp"

namespace CML {
    defineTypeNameAndDebug(manipulateField,0);
}

CML::manipulateField::manipulateField
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    active_(true),
    writeManipulated_(false),
    obr_(obr),
    dict_(dict)
{
    if (!isA<fvMesh>(obr_))
    {
        active_=false;
        WarningInFunction
            << "Not a fvMesh. Nothing I can do"
            << endl;
    }
    read(dict);
    write();
}

CML::manipulateField::~manipulateField()
{}

template<class TData,class TMask>
void CML::manipulateField::manipulate(
    const TData &data,
    const TMask &mask,
    const word entity
)
{
    TData &original=const_cast<TData &>(obr_.lookupObject<TData>(name_));
    label cnt=0;
    forAll(original,cellI) {
        if(mask[cellI]>SMALL) {
            cnt++;
            original[cellI]=data[cellI];
        }
    }

    reduce(cnt,plusOp<label>());
    Info << "Manipulated field " << name_ << " in " << cnt
        << " " << entity << " with the expression " << expression_ << endl;
    original.correctBoundaryConditions();

    if(
        obr_.time().outputTime()
        &&
        original.writeOpt()==IOobject::AUTO_WRITE
    ) {
        if(this->writeManipulated_) {
            Info << "Rewriting manipulated field " << original.name() << endl;

            original.write();
        } else {
            Info << "Manipulated field " << original.name()
                << " not rewritten. Set 'writeManipulated'" << endl;
        }
    }
}

template<class TData,class TMask>
void CML::manipulateField::manipulateSurface(
    const TData &data,
    const TMask &mask
)
{
    TData &original=const_cast<TData &>(obr_.lookupObject<TData>(name_));
    label cnt=0;
    forAll(original,cellI) {
        if(mask[cellI]>SMALL) {
            cnt++;
            original[cellI]=data[cellI];
        }
    }

    reduce(cnt,plusOp<label>());
    Info << "Manipulated field " << name_ << " on " << cnt
        << " faces with the expression " << expression_ << endl;

    // this does not work for surface fields
    //    original.correctBoundaryConditions();

    if(
        obr_.time().outputTime()
        &&
        original.writeOpt()==IOobject::AUTO_WRITE
    ) {
        if(this->writeManipulated_) {
            Info << "Rewriting manipulated field " << original.name() << endl;

            original.write();
        } else {
            Info << "Manipulated field " << original.name()
                << " not rewritten. Set 'writeManipulated'" << endl;
        }
    }
}

void CML::manipulateField::timeSet()
{
    // Do nothing
}

void CML::manipulateField::read(const dictionary& dict)
{
    if(active_) {
        name_=word(dict.lookup("fieldName"));
        expression_=exprString(
            dict.lookup("expression"),
            dict
        );
        maskExpression_=exprString(
            dict.lookup("mask"),
            dict
        );
        writeManipulated_=dict.lookupOrDefault<bool>("writeManipulated",false);

        const fvMesh& mesh = refCast<const fvMesh>(obr_);

        driver_.set(
            new FieldValueExpressionDriver(
                mesh.time().timeName(),
                mesh.time(),
                mesh,
                false, // no caching. No need
                true,  // search fields in memory
                false  // don't look up files in memory
            )
        );

        driver_->readVariablesAndTables(dict_);

        // this might not work when rereading ... but what is consistent in that case?
        driver_->createWriterAndRead(name_+"_"+type());
    }
}

void CML::manipulateField::write()
{
    if(active_) {
        FieldValueExpressionDriver &driver=driver_();

        driver.clearVariables();

        driver.parse(maskExpression_);

        if(!driver.isLogical()) {
            FatalErrorInFunction
                << maskExpression_ << " does not evaluate to a logical expression"
                << endl
                << exit(FatalError);
        }

        if(driver.resultIsTyp<volScalarField>(true)) {
            volScalarField conditionField(driver.getResult<volScalarField>());

            driver.parse(expression_);

            if(driver.resultIsTyp<volVectorField>()) {
                manipulate(
                    driver.getResult<volVectorField>(),
                    conditionField
                );

            } else if(driver.resultIsTyp<volScalarField>()) {
                manipulate(
                    driver.getResult<volScalarField>(),
                    conditionField
                );
            } else if(driver.resultIsTyp<volTensorField>()) {
                manipulate(
                    driver.getResult<volTensorField>(),
                    conditionField
                );
            } else if(driver.resultIsTyp<volSymmTensorField>()) {
                manipulate(
                    driver.getResult<volSymmTensorField>(),
                    conditionField
                );
            } else if(driver.resultIsTyp<volSphericalTensorField>()) {
                manipulate(
                    driver.getResult<volSphericalTensorField>(),
                    conditionField
                );
            } else {
                WarningInFunction
                    << "Expression '" << expression_
                    << "' evaluated to an unsupported type "
                    << driver.typ() << " that is incompatible with a mask defined on cells"
                    << endl;
            }
        } else if(driver.resultIsTyp<surfaceScalarField>(true)) {
            surfaceScalarField conditionField(driver.getResult<surfaceScalarField>());

            driver.parse(expression_);

            if(driver.resultIsTyp<surfaceVectorField>()) {
                manipulateSurface(
                    driver.getResult<surfaceVectorField>(),
                    conditionField
                );

            } else if(driver.resultIsTyp<surfaceScalarField>()) {
                manipulateSurface(
                    driver.getResult<surfaceScalarField>(),
                    conditionField
                );
            } else if(driver.resultIsTyp<surfaceTensorField>()) {
                manipulateSurface(
                    driver.getResult<surfaceTensorField>(),
                    conditionField
                );
            } else if(driver.resultIsTyp<surfaceSymmTensorField>()) {
                manipulateSurface(
                    driver.getResult<surfaceSymmTensorField>(),
                    conditionField
                );
            } else if(driver.resultIsTyp<surfaceSphericalTensorField>()) {
                manipulateSurface(
                    driver.getResult<surfaceSphericalTensorField>(),
                    conditionField
                );
            } else {
                WarningInFunction
                    << "Expression '" << expression_
                    << "' evaluated to an unsupported type "
                    << driver.typ() << " that is incompatible with a mask defined on faces"
                    << endl;
            }
        } else if(driver.resultIsTyp<pointScalarField>(true)) {
            pointScalarField conditionField(driver.getResult<pointScalarField>());

            driver.parse(expression_);

            if(driver.resultIsTyp<pointVectorField>()) {
                manipulate(
                    driver.getResult<pointVectorField>(),
                    conditionField,
                    "points"
                );

            } else if(driver.resultIsTyp<pointScalarField>()) {
                manipulate(
                    driver.getResult<pointScalarField>(),
                    conditionField,
                    "points"
                );
            } else if(driver.resultIsTyp<pointTensorField>()) {
                manipulate(
                    driver.getResult<pointTensorField>(),
                    conditionField,
                    "points"
                );
            } else if(driver.resultIsTyp<pointSymmTensorField>()) {
                manipulate(
                    driver.getResult<pointSymmTensorField>(),
                    conditionField,
                    "points"
                );
            } else if(driver.resultIsTyp<pointSphericalTensorField>()) {
                manipulate(
                    driver.getResult<pointSphericalTensorField>(),
                    conditionField,
                    "points"
                );
            } else {
                WarningInFunction
                    << "Expression '" << expression_
                    << "' evaluated to an unsupported type "
                    << driver.typ() << " that is incompatible with a mask defined on faces"
                    << endl;
            }
        } else {
                WarningInFunction
                    << "Mask " << maskExpression_ << " evaluates to type "
                    << driver.typ() << " which is an unsupported field type for logical"
                    << endl;
        }
    }

    driver_->tryWrite();
}


void CML::manipulateField::end()
{
    execute();
}

void CML::manipulateField::execute()
{
}

// ************************************************************************* //
