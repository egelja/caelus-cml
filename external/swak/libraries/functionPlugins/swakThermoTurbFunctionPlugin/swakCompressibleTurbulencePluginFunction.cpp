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

#include "swakCompressibleTurbulencePluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "HashPtrTable.hpp"
#include "compressibleLESModel.hpp"
#include "compressibleRASModel.hpp"

#include "addToRunTimeSelectionTable.hpp"

namespace CML {

defineTypeNameAndDebug(swakCompressibleTurbulencePluginFunction,0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

swakCompressibleTurbulencePluginFunction::swakCompressibleTurbulencePluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name,
    const word &returnValueType
):
    swakThermophysicalPluginFunction<swakFluidThermoType>(
        parentDriver,
        name,
        returnValueType
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

    const compressible::turbulenceModel &swakCompressibleTurbulencePluginFunction::turbInternal(
    const fvMesh &reg
)
{
    static HashPtrTable<compressible::turbulenceModel> turb_;

    if(reg.foundObject<compressible::turbulenceModel>("turbulenceProperties")) {
        if(debug) {
            Info << "swakCompressibleTurbulencePluginFunction::turbInternal: "
                << "turbulence already in memory" << endl;
        }
        // Somebody else already registered this
        return reg.lookupObject<compressible::turbulenceModel>("turbulenceProperties");
    }
    if(reg.foundObject<compressible::LESModel>("LESProperties")) {
        if(debug) {
            Info << "swakCompressibleTurbulencePluginFunction::turbInternal: "
                << "LES already in memory" << endl;
        }
        // Somebody else already registered this
        return reg.lookupObject<compressible::LESModel>("LESProperties");
    }
    if(reg.foundObject<compressible::RASModel>("RASProperties")) {
        if(debug) {
            Info << "swakCompressibleTurbulencePluginFunction::turbInternal: "
                << "RAS already in memory" << endl;
        }
        // Somebody else already registered this
        return reg.lookupObject<compressible::RASModel>("RASProperties");
    }

    if(!turb_.found(reg.name())) {
        if(debug) {
            Info << "swakCompressibleTurbulencePluginFunction::turbInternal: "
                << "not yet in memory for " << reg.name() << endl;
        }

        turb_.set(
            reg.name(),
            compressible::turbulenceModel::New(
                // reg.lookupObject<volScalarField>("rho"),
                thermoInternal(reg).rho(),
                reg.lookupObject<volVectorField>("U"),
                reg.lookupObject<surfaceScalarField>("phi"),
                thermoInternal(reg)
            ).ptr()
        );
    }

    return *(turb_[reg.name()]);
}

    const compressible::turbulenceModel &swakCompressibleTurbulencePluginFunction::turb()
{
    return turbInternal(mesh());
}

// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //

#define concreteTurbFunction(funcName,resultType)                  \
class swakCompressibleTurbulencePluginFunction_ ## funcName        \
: public swakCompressibleTurbulencePluginFunction                  \
{                                                                  \
public:                                                            \
    TypeName("swakCompressibleTurbulencePluginFunction_" #funcName);       \
    swakCompressibleTurbulencePluginFunction_ ## funcName (        \
        const FieldValueExpressionDriver &parentDriver,            \
        const word &name                                           \
    ): swakCompressibleTurbulencePluginFunction(                   \
        parentDriver,                                              \
        name,                                                      \
        #resultType                                                \
    ) {}                                                           \
    void doEvaluation() {                                          \
        result().setObjectResult(                                  \
            autoPtr<resultType>(                                   \
                new resultType(                                    \
                    turb().funcName()                              \
                )                                                  \
            )                                                      \
        );                                                         \
    }                                                              \
};                                                                 \
defineTypeNameAndDebug(swakCompressibleTurbulencePluginFunction_ ## funcName,0);  \
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakCompressibleTurbulencePluginFunction_ ## funcName,name,rhoTurb_ ## funcName);

concreteTurbFunction(mut,volScalarField);
concreteTurbFunction(muEff,volScalarField);
concreteTurbFunction(alphaEff,volScalarField);
concreteTurbFunction(k,volScalarField);
concreteTurbFunction(epsilon,volScalarField);
concreteTurbFunction(R,volSymmTensorField);
concreteTurbFunction(devRhoReff,volSymmTensorField);

concreteTurbFunction(kappaEff,volScalarField);

} // namespace

// ************************************************************************* //
