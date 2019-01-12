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

#include "expressionField.hpp"

#include "FieldValueExpressionDriver.hpp"

namespace CML {
    defineTypeNameAndDebug(expressionField,0);
}

CML::expressionField::expressionField
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
:
    active_(true),
    obr_(obr),
    dict_(dict),
    dimensions_(dimless),
    setDimensions_(false)
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

CML::expressionField::~expressionField()
{}

template<class T>
void CML::expressionField::storeField(
    const T &data
)
{
    if(field_.empty()) {
        field_.reset(
            new T(
                IOobject(
                    name_,
                    obr_.time().timeName(),
                    obr_,
                    IOobject::NO_READ,
                    autowrite_ ? IOobject::AUTO_WRITE : IOobject::NO_WRITE
                ),
                data
            )
        );
    } else {
        if(setDimensions_) {
            dynamic_cast<T &>(field_()).dimensions().reset(data.dimensions());
        }
        dynamic_cast<T &>(field_())==data;
    }

    if(setDimensions_) {
        dynamic_cast<T &>(field_()).dimensions().reset(dimensions_);
    }

    if(
        this->autowrite_
        &&
        this->obr_.time().outputTime()
    ) {
        field_->write();
    }
}

void CML::expressionField::timeSet()
{
    // Do nothing
}

void CML::expressionField::read(const dictionary& dict)
{
    if(active_) {
        name_=word(dict.lookup("fieldName"));
        expression_=exprString(
            dict.lookup("expression"),
            dict
        );
        autowrite_=Switch(dict.lookup("autowrite"));
        if(dict.found("dimension")) {
            dimensions_.reset(dict.lookup("dimension"));
            setDimensions_=true;
        } else {
            WarningInFunction
                << "No entry 'dimension' in " << dict.name() << " for field " << name_ << endl
                << "Not resetting the dimensions of the field" << nl
                << endl;
            dimensions_.reset(dimless);
            setDimensions_=false;
        }

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

void CML::expressionField::write()
{
    if(active_) {
        Info << "Creating expression field " << name_ << " ..." << flush;

        FieldValueExpressionDriver &driver=driver_();

        bool oldDimsetDebug=dimensionSet::debug;
        dimensionSet::debug=false;

        driver.clearVariables();

        driver.parse(expression_);

        dimensionSet::debug=oldDimsetDebug;

        Info << " type:" << driver.getResultType() << endl;

        if(driver.resultIsTyp<volVectorField>()) {
            storeField(
                driver.getResult<volVectorField>()
            );
        } else if(driver.resultIsTyp<volScalarField>()) {
            storeField(
                driver.getResult<volScalarField>()
            );
        } else if(driver.resultIsTyp<volTensorField>()) {
            storeField(
                driver.getResult<volTensorField>()
            );
        } else if(driver.resultIsTyp<volSymmTensorField>()) {
            storeField(
                driver.getResult<volSymmTensorField>()
            );
        } else if(driver.resultIsTyp<volSphericalTensorField>()) {
            storeField(
                driver.getResult<volSphericalTensorField>()
            );
        } else if(driver.resultIsTyp<surfaceVectorField>()) {
            storeField(
                driver.getResult<surfaceVectorField>()
            );
        } else if(driver.resultIsTyp<surfaceScalarField>()) {
            storeField(
                driver.getResult<surfaceScalarField>()
            );
        } else if(driver.resultIsTyp<surfaceTensorField>()) {
            storeField(
                driver.getResult<surfaceTensorField>()
            );
        } else if(driver.resultIsTyp<surfaceSymmTensorField>()) {
            storeField(
                driver.getResult<surfaceSymmTensorField>()
            );
        } else if(driver.resultIsTyp<surfaceSphericalTensorField>()) {
            storeField(
                driver.getResult<surfaceSphericalTensorField>()
            );
        } else if(driver.resultIsTyp<pointVectorField>()) {
            storeField(
                driver.getResult<pointVectorField>()
            );
        } else if(driver.resultIsTyp<pointScalarField>()) {
            storeField(
                driver.getResult<pointScalarField>()
            );
        } else if(driver.resultIsTyp<pointTensorField>()) {
            storeField(
                driver.getResult<pointTensorField>()
            );
        } else if(driver.resultIsTyp<pointSymmTensorField>()) {
            storeField(
                driver.getResult<pointSymmTensorField>()
            );
        } else if(driver.resultIsTyp<pointSphericalTensorField>()) {
            storeField(
                driver.getResult<pointSphericalTensorField>()
            );
        } else {
            WarningInFunction
                << "Expression '" << expression_
                << "' evaluated to an unsupported type "
                << driver.typ()
                << endl;
        }
    }

    driver_->tryWrite();
}


void CML::expressionField::end()
{
    execute();
}

void CML::expressionField::execute()
{
}

void CML::expressionField::clearData()
{
    field_.clear();
}

// ************************************************************************* //
