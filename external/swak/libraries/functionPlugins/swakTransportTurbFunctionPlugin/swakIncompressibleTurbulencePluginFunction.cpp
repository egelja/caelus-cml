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

#include "swakIncompressibleTurbulencePluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "HashPtrTable.hpp"
#include "LESModel.hpp"
#include "RASModel.hpp"

#include "addToRunTimeSelectionTable.hpp"

namespace CML {

defineTypeNameAndDebug(swakIncompressibleTurbulencePluginFunction,0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

swakIncompressibleTurbulencePluginFunction::swakIncompressibleTurbulencePluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name,
    const word &returnValueType
):
    swakTransportModelsPluginFunction(
        parentDriver,
        name,
        returnValueType
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const incompressible::turbulenceModel &swakIncompressibleTurbulencePluginFunction::turbInternal(
    const fvMesh &reg
)
{
    static HashPtrTable<incompressible::turbulenceModel> turb_;

    if(reg.foundObject<incompressible::LESModel>("LESProperties")) {
        if(debug) {
            Info << "swakIncompressibleTurbulencePluginFunction::turbInternal: "
                << "LES already in memory" << endl;
        }
        // Somebody else already registered this
        return reg.lookupObject<incompressible::LESModel>("LESProperties");
    }
    if(reg.foundObject<incompressible::RASModel>("RASProperties")) {
        if(debug) {
            Info << "swakIncompressibleTurbulencePluginFunction::turbInternal: "
                << "RAS already in memory" << endl;
        }
        // Somebody else already registered this
        return reg.lookupObject<incompressible::RASModel>("RASProperties");
    }

    if(!turb_.found(reg.name())) {
        if(debug) {
            Info << "swakIncompressibleTurbulencePluginFunction::turbInternal: "
                << "not yet in memory for " << reg.name() << endl;
        }

        turb_.set(
            reg.name(),
            incompressible::turbulenceModel::New(
                reg.lookupObject<volVectorField>("U"),
                reg.lookupObject<surfaceScalarField>("phi"),
                const_cast<transportModel &>(transportInternal(reg))
            ).ptr()
        );
    }

    return *(turb_[reg.name()]);
}

const incompressible::turbulenceModel &swakIncompressibleTurbulencePluginFunction::turb()
{
    return turbInternal(mesh());
}

// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //

#define concreteTurbFunction(funcName,resultType)                  \
class swakIncompressibleTurbulencePluginFunction_ ## funcName      \
: public swakIncompressibleTurbulencePluginFunction                \
{                                                                  \
public:                                                            \
    TypeName("swakIncompressibleTurbulencePluginFunction_" #funcName);       \
    swakIncompressibleTurbulencePluginFunction_ ## funcName (      \
        const FieldValueExpressionDriver &parentDriver,            \
        const word &name                                           \
    ): swakIncompressibleTurbulencePluginFunction(                 \
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
defineTypeNameAndDebug(swakIncompressibleTurbulencePluginFunction_ ## funcName,0);  \
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakIncompressibleTurbulencePluginFunction_ ## funcName,name,turb_ ## funcName);

concreteTurbFunction(nut,volScalarField);
concreteTurbFunction(nuEff,volScalarField);
concreteTurbFunction(k,volScalarField);
concreteTurbFunction(epsilon,volScalarField);
concreteTurbFunction(R,volSymmTensorField);
concreteTurbFunction(devReff,volSymmTensorField);


} // namespace

// ************************************************************************* //
