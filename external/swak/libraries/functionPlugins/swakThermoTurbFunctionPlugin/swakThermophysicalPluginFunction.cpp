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

#include "swakThermophysicalPluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "HashPtrTable.hpp"

#include "addToRunTimeSelectionTable.hpp"

#include "solidThermo.hpp"
#include "swakVersion.hpp"
#include "DebugOStream.hpp"

namespace CML {
defineTemplateTypeNameAndDebug(swakThermophysicalPluginFunction<swakFluidThermoType>,0);
defineTemplateTypeNameAndDebug(swakThermophysicalPluginFunction<solidThermo>,0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
swakThermophysicalPluginFunction<ThermoType>::swakThermophysicalPluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name,
    const word &returnValueType
):
    FieldValuePluginFunction(
        parentDriver,
        name,
        returnValueType,
        string("")
    )
{
}

template class swakThermophysicalPluginFunction<swakFluidThermoType>;
template class swakThermophysicalPluginFunction<solidThermo>;

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
const ThermoType& swakThermophysicalPluginFunction<ThermoType>::thermoInternal(
    const fvMesh& reg
)
{
    static HashPtrTable<ThermoType> thermo_;

    if(reg.foundObject<ThermoType>("thermophysicalProperties")) {
        if(debug) {
            Info << "swakThermophysicalPluginFunction::thermoInternal: "
                << "already in memory" << endl;
        }
        // Somebody else already registered this
        return reg.lookupObject<ThermoType>("thermophysicalProperties");
    }
    if(!thermo_.found(reg.name())) {
        if(debug) {
            Info << "swakThermophysicalPluginFunction::thermoInternal: "
                << "not yet in memory for " << reg.name() << endl;
        }

        bool usePsi=true;
        bool found=false;

        {
            // make sure it is gone before we create the object
            IOdictionary dict
                (
                    IOobject
                    (
                        "thermophysicalProperties",
                        reg.time().constant(),
                        reg,
                        IOobject::MUST_READ,
                        IOobject::NO_WRITE
                    )
                );

            word thermoTypeName;
            if (dict.isDict("thermoType")) {
                const dictionary& thermoTypeDict(dict.subDict("thermoType"));

                // Construct the name of the thermo package from the components
                thermoTypeName =
                    word(thermoTypeDict.lookup("type")) + '<'
                    + word(thermoTypeDict.lookup("mixture")) + '<'
                    + word(thermoTypeDict.lookup("transport")) + '<'
                    + word(thermoTypeDict.lookup("thermo")) + '<'
                    + word(thermoTypeDict.lookup("equationOfState")) + '<'
                    + word(thermoTypeDict.lookup("specie")) + ">>,"
                    + word(thermoTypeDict.lookup("energy")) + ">>>";
            } else {
                thermoTypeName=word(dict["thermoType"]);
            }

            swakRhoThermoType::fvMeshConstructorTable::iterator cstrIter =
                swakRhoThermoType::fvMeshConstructorTablePtr_->find(
                    thermoTypeName
                );
            if (cstrIter != swakRhoThermoType::fvMeshConstructorTablePtr_->end())
            {
                if(debug) {
                    Info << thermoTypeName << " is a rhoThermo-type";
                }
                usePsi=false;
                found=true;
            } else if(debug) {
                Info << "No " << thermoTypeName << " in rhoThermo-types "
                    << swakRhoThermoType::fvMeshConstructorTablePtr_->sortedToc()
                    << endl;
            }
            if(usePsi) {
                swakPsiThermoType::fvMeshConstructorTable::iterator cstrIter =
                    swakPsiThermoType::fvMeshConstructorTablePtr_->find(
                        thermoTypeName
                    );
                if(cstrIter != swakPsiThermoType::fvMeshConstructorTablePtr_->end())
                {
                    if(debug) {
                        Info << thermoTypeName << " is a psiThermo-type";
                    }
                    found=true;
                } else if(debug) {
                    Info << "No " << thermoTypeName << " in psiThermo-types "
                    << swakPsiThermoType::fvMeshConstructorTablePtr_->sortedToc()
                    << endl;
                }
            }
        }

        if(!found) {
            thermo_.set(
                reg.name(),
                ThermoType::New(reg).ptr()
            );
        } else {
            // Create it ourself because nobody registered it
            if(usePsi) {
                thermo_.set(
                    reg.name(),
                    swakPsiThermoType::New(reg).ptr()
                );
            } else {
                thermo_.set(
                    reg.name(),
                    swakRhoThermoType::New(reg).ptr()
                );
            }
        }
    }

    return *(thermo_[reg.name()]);
}

template<>
const solidThermo &swakThermophysicalPluginFunction<solidThermo>::thermoInternal(
    const fvMesh &reg
)
{
    static HashPtrTable<solidThermo> thermo_;

    if(reg.foundObject<solidThermo>("thermophysicalProperties")) {
        if(debug) {
            Info << "swakThermophysicalPluginFunction::thermoInternal: "
                << "already in memory" << endl;
        }
        // Somebody else already registered this
        return reg.lookupObject<solidThermo>("thermophysicalProperties");
    }
    if(!thermo_.found(reg.name())) {
        if(debug) {
            Info << "swakThermophysicalPluginFunction::thermoInternal: "
                << "not yet in memory for " << reg.name() << endl;
        }

        {
            // make sure it is gone before we create the object
            IOdictionary dict
                (
                    IOobject
                    (
                        "thermophysicalProperties",
                        reg.time().constant(),
                        reg,
                        IOobject::MUST_READ,
                        IOobject::NO_WRITE
                    )
                );

            word thermoTypeName;
            if (dict.isDict("thermoType")) {
                const dictionary& thermoTypeDict(dict.subDict("thermoType"));

                // Construct the name of the thermo package from the components
                thermoTypeName =
                    word(thermoTypeDict.lookup("type")) + '<'
                    + word(thermoTypeDict.lookup("mixture")) + '<'
                    + word(thermoTypeDict.lookup("transport")) + '<'
                    + word(thermoTypeDict.lookup("thermo")) + '<'
                    + word(thermoTypeDict.lookup("equationOfState")) + '<'
                    + word(thermoTypeDict.lookup("specie")) + ">>,"
                    + word(thermoTypeDict.lookup("energy")) + ">>>";
            } else {
                thermoTypeName=word(dict["thermoType"]);
            }

            solidThermo::fvMeshConstructorTable::iterator cstrIter =
                solidThermo::fvMeshConstructorTablePtr_->find(
                    thermoTypeName
                );
            if (cstrIter != solidThermo::fvMeshConstructorTablePtr_->end())
            {
                if(debug) {
                    Info << thermoTypeName << " is a solidThermo-type";
                }
            } else if(debug) {
                Info << "No " << thermoTypeName << " in solidThermo-types "
                    << solidThermo::fvMeshConstructorTablePtr_->sortedToc()
                    << endl;
            }

        }

        // Create it ourself because nobody registered it
        thermo_.set(
            reg.name(),
            solidThermo::New(reg).ptr()
        );
    }

    return *(thermo_[reg.name()]);
}

template<class ThermoType>
const ThermoType &swakThermophysicalPluginFunction<ThermoType>::thermo()
{
    return thermoInternal(mesh());
}

// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //

#define concreteThermoFunction(funcName,resultType,tthermo)        \
class swakThermophysicalPluginFunction_ ## funcName                \
: public swakThermophysicalPluginFunction<tthermo>                 \
{                                                                  \
public:                                                            \
    TypeName("swakThermophysicalPluginFunction_" #funcName);       \
    swakThermophysicalPluginFunction_ ## funcName (                \
        const FieldValueExpressionDriver &parentDriver,            \
        const word &name                                           \
    ): swakThermophysicalPluginFunction<tthermo>(                  \
        parentDriver,                                              \
        name,                                                      \
        #resultType                                                \
    ) {}                                                           \
    void doEvaluation() {                                          \
        result().setObjectResult(                                  \
            autoPtr<resultType>(                                   \
                new resultType(                                    \
                    thermo().funcName()                            \
                )                                                  \
            )                                                      \
        );                                                         \
    }                                                              \
};                                                                 \
defineTypeNameAndDebug(swakThermophysicalPluginFunction_ ## funcName,0);  \
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakThermophysicalPluginFunction_ ## funcName,name,thermo_ ## funcName);

concreteThermoFunction(p,volScalarField,swakFluidThermoType);
concreteThermoFunction(rho,volScalarField,swakFluidThermoType);
concreteThermoFunction(psi,volScalarField,swakFluidThermoType);
concreteThermoFunction(he,volScalarField,swakFluidThermoType);
concreteThermoFunction(Kappa,volVectorField,solidThermo);
concreteThermoFunction(hc,volScalarField,swakFluidThermoType);
concreteThermoFunction(T,volScalarField,swakFluidThermoType);
concreteThermoFunction(Cp,volScalarField,swakFluidThermoType);
concreteThermoFunction(Cv,volScalarField,swakFluidThermoType);
concreteThermoFunction(mu,volScalarField,swakFluidThermoType);
concreteThermoFunction(alpha,volScalarField,swakFluidThermoType);

concreteThermoFunction(gamma,volScalarField,swakFluidThermoType);
concreteThermoFunction(Cpv,volScalarField,swakFluidThermoType);
concreteThermoFunction(CpByCpv,volScalarField,swakFluidThermoType);
concreteThermoFunction(kappa,volScalarField,swakFluidThermoType);

} // namespace

// ************************************************************************* //
