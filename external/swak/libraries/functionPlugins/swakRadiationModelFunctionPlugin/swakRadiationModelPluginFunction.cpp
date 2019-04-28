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

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "swakRadiationModelPluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "HashPtrTable.hpp"

#include "swakThermoTypes.hpp"

#include "addToRunTimeSelectionTable.hpp"

#include "sootModel.hpp"
#include "absorptionEmissionModel.hpp"

namespace CML {

defineTypeNameAndDebug(swakRadiationModelPluginFunction,0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

swakRadiationModelPluginFunction::swakRadiationModelPluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name,
    const word &returnValueType,
    const string &spec
):
    FieldValuePluginFunction(
        parentDriver,
        name,
        returnValueType,
        spec
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const radiation::radiationModel &swakRadiationModelPluginFunction::radiationInternal(
    const fvMesh &reg
)
{
    static HashPtrTable<radiation::radiationModel> radiation_;

    if(reg.foundObject<radiation::radiationModel>("radiationProperties")) {
        if(debug) {
            Info << "swakRadiationModelPluginFunction::radiationInternal: "
                << "already in memory" << endl;
        }
        // Somebody else already registered this
        return reg.lookupObject<radiation::radiationModel>("radiationProperties");
    }
    if(!radiation_.found(reg.name())) {
        if(debug) {
            Info << "swakRadiationModelPluginFunction::radiationInternal: "
                << "not yet in memory for " << reg.name() << endl;
        }

        // Create it ourself because nobody registered it
        radiation_.set(
                reg.name(),
                radiation::radiationModel::New(
                    reg.lookupObject<volScalarField>("T")
                ).ptr()
        );

        Info << "Created radiation model. Calculating to get values ..."
            << endl;
        radiation_[reg.name()]->correct();
    }

    return *(radiation_[reg.name()]);
}

const radiation::radiationModel &swakRadiationModelPluginFunction::radiation()
{
    return radiationInternal(mesh());
}

// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //

#define concreteRadiationFunction(funcName,resultType)                \
class swakRadiationModelPluginFunction_ ## funcName                \
: public swakRadiationModelPluginFunction                          \
{                                                                  \
public:                                                            \
    TypeName("swakRadiationModelPluginFunction_" #funcName);       \
    swakRadiationModelPluginFunction_ ## funcName (                \
        const FieldValueExpressionDriver &parentDriver,            \
        const word &name                                           \
    ): swakRadiationModelPluginFunction(                           \
        parentDriver,                                              \
        name,                                                      \
        #resultType                                                \
    ) {}                                                           \
    void doEvaluation() {                                          \
        result().setObjectResult(                                  \
            autoPtr<resultType>(                                   \
                new resultType(                                    \
                    #funcName "_rad",                               \
                    radiation().funcName()                          \
                )                                                  \
            )                                                      \
        );                                                         \
    }                                                              \
};                                                                 \
defineTypeNameAndDebug(swakRadiationModelPluginFunction_ ## funcName,0);  \
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakRadiationModelPluginFunction_ ## funcName,name,radiation_ ## funcName);

concreteRadiationFunction(Rp,volScalarField);

class swakRadiationModelPluginFunction_Ru
: public swakRadiationModelPluginFunction
{
public:
    TypeName("swakRadiationModelPluginFunction_Ru");
    swakRadiationModelPluginFunction_Ru (
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    ): swakRadiationModelPluginFunction(
        parentDriver,
        name,
        "volScalarField"
    ) {}

    void doEvaluation() {
        const DimensionedField<scalar,volMesh> &ruRad=radiation().Ru();

        autoPtr<volScalarField> val(
            new volScalarField(
                IOobject(
                    "radiationRu",
                    mesh().time().timeName(),
                    mesh(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh(),
                dimensionedScalar("Ru",ruRad.dimensions(),0),
                "zeroGradient"
            )
        );
        val->dimensionedInternalField()=ruRad;

        result().setObjectResult(
            val
        );
    }
};
defineTypeNameAndDebug(swakRadiationModelPluginFunction_Ru,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakRadiationModelPluginFunction_Ru,name,radiation_Ru);

class swakRadiationModelPluginFunction_radSource
: public swakRadiationModelPluginFunction
{
public:
    TypeName("swakRadiationModelPluginFunction_radSource");
    swakRadiationModelPluginFunction_radSource (
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    ): swakRadiationModelPluginFunction(
        parentDriver,
        name,
        "volScalarField"
    ) {}

    void doEvaluation() {
        const volScalarField T4(
            pow4(
                mesh().lookupObject<volScalarField>("T")
            )
        );

        autoPtr<volScalarField> val(
            new volScalarField(
                IOobject
                (
                    "radSource",
                    mesh().time().timeName(),
                    mesh(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                -T4*radiation().Rp(),
                "zeroGradient"
            )
        );

        val().internalField()+=radiation().Ru();
        val().correctBoundaryConditions();

        result().setObjectResult(
            val
        );
    }
};
defineTypeNameAndDebug(swakRadiationModelPluginFunction_radSource,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakRadiationModelPluginFunction_radSource,name,radiation_radSource);

    // Values of the sootModel
class swakRadiationModelPluginFunction_soot
: public swakRadiationModelPluginFunction
{
public:
    TypeName("swakRadiationModelPluginFunction_soot");
    swakRadiationModelPluginFunction_soot (
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    ): swakRadiationModelPluginFunction(
        parentDriver,
        name,
        "volScalarField"
    ) {}
    void doEvaluation() {
        result().setObjectResult(
            autoPtr<volScalarField>(
                new volScalarField(
                    "soot_rad",
                    radiation().soot().soot()
                )
            )
        );
    }
};
defineTypeNameAndDebug(swakRadiationModelPluginFunction_soot,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakRadiationModelPluginFunction_soot,name,radiation_soot);

} // namespace

// ************************************************************************* //
