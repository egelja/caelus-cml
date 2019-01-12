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
    2012-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "swakPsiChemistryModelPluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "HashPtrTable.hpp"
#include "swakThermoTypes.hpp"

#include "addToRunTimeSelectionTable.hpp"

#include "swakVersion.hpp"
#include "DebugOStream.hpp"

namespace CML {

defineTypeNameAndDebug(swakPsiChemistryModelPluginFunction,0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

swakPsiChemistryModelPluginFunction::swakPsiChemistryModelPluginFunction(
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

const  swakPsiChemistryModelPluginFunction::ChemistryModelType &swakPsiChemistryModelPluginFunction::chemistryInternal(
    const fvMesh &reg
)
{
    static HashPtrTable<swakPsiChemistryModelPluginFunction::ChemistryModelType> chemistry_;

    typedef swakPsiChemistryModelPluginFunction::ChemistryModelType::reactionThermo rThermo;
    static HashPtrTable<rThermo> thermo_;

    if(reg.foundObject< swakPsiChemistryModelPluginFunction::ChemistryModelType>("chemistryProperties")) {
        if(debug) {
            Info << "swakPsiChemistryModelPluginFunction::chemistryInternal: "
                << "already in memory" << endl;
        }
        // Somebody else already registered this
        return reg.lookupObject< swakPsiChemistryModelPluginFunction::ChemistryModelType>("chemistryProperties");
    }
    if(!chemistry_.found(reg.name())) {
        if(debug) {
            Info << "swakPsiChemistryModelPluginFunction::chemistryInternal: "
                << "not yet in memory for " << reg.name() << endl;
        }

        // Create it ourself because nobody registered it
        if(reg.foundObject<rThermo>("thermophysicalProperties")) {
            chemistry_.set(
                reg.name(),
                swakPsiChemistryModelPluginFunction::ChemistryModelType::New(
                    const_cast<rThermo&>(
                        reg.lookupObject<rThermo>("thermophysicalProperties")
                    )
                ).ptr()
            );
        } else {
            thermo_.set(
                reg.name(),
                rThermo::New(
                    reg
                ).ptr()
            );
            chemistry_.set(
                reg.name(),
                swakPsiChemistryModelPluginFunction::ChemistryModelType::New(
                    const_cast<rThermo&>(
                        *thermo_[reg.name()]
                    )
                ).ptr()
            );
        }

        Info << "Created chemistry model. Calculating to get values ..."
            << endl;

        chemistry_[reg.name()]->solve(
            reg.time().deltaT().value()
        );
        //        chemistry_[reg.name()]->calculate();
    }

    return *(chemistry_[reg.name()]);
}

void swakPsiChemistryModelPluginFunction::updateChemistry(const scalar dt)
{
    const_cast< swakPsiChemistryModelPluginFunction::ChemistryModelType&>(
        chemistry()
    ).solve(
        dt
    );
}

tmp<volScalarField> swakPsiChemistryModelPluginFunction::wrapDimField(
        const DimensionedField<scalar,volMesh> &dimField
)
{
    tmp<volScalarField> result(
            new volScalarField(
                IOobject(
                    dimField.name(),
                    mesh().time().timeName(),
                    mesh(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh(),
                dimensionedScalar(dimField.name(),dimField.dimensions(),0),
                "zeroGradient"
            )
    );
    result->internalField() = dimField;

    return result;
}

const  swakPsiChemistryModelPluginFunction::ChemistryModelType &swakPsiChemistryModelPluginFunction::chemistry()
{
    return chemistryInternal(mesh());
}

// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //

#define concreteChemistryFunction(funcName,resultType)                \
class swakPsiChemistryModelPluginFunction_ ## funcName                \
: public swakPsiChemistryModelPluginFunction                          \
{                                                                  \
public:                                                            \
    TypeName("swakPsiChemistryModelPluginFunction_" #funcName);       \
    swakPsiChemistryModelPluginFunction_ ## funcName (                \
        const FieldValueExpressionDriver &parentDriver,            \
        const word &name                                           \
    ): swakPsiChemistryModelPluginFunction(                           \
        parentDriver,                                              \
        name,                                                      \
        #resultType                                                \
    ) {}                                                           \
    void doEvaluation() {                                          \
        result().setObjectResult(                                  \
            autoPtr<resultType>(                                   \
                new resultType(                                    \
                    chemistry().funcName()                            \
                )                                                  \
            )                                                      \
        );                                                         \
    }                                                              \
};                                                                 \
defineTypeNameAndDebug(swakPsiChemistryModelPluginFunction_ ## funcName,0);  \
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakPsiChemistryModelPluginFunction_ ## funcName,name,psiChem_ ## funcName);

concreteChemistryFunction(tc,volScalarField);
concreteChemistryFunction(Qdot,volScalarField);

class swakPsiChemistryModelPluginFunction_RR
: public swakPsiChemistryModelPluginFunction
{
    word speciesName_;

public:
    TypeName("swakPsiChemistryModelPluginFunction_RR");
    swakPsiChemistryModelPluginFunction_RR (
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    ): swakPsiChemistryModelPluginFunction(
        parentDriver,
        name,
        "volScalarField",
        "speciesName primitive word"
    ) {}

    void doEvaluation() {
        label specI=chemistry().thermo().composition().species()[speciesName_];

        result().setObjectResult(
            autoPtr<volScalarField>(
                wrapDimField(
                    chemistry().RR(specI)
                ).ptr()
            )
        );
    }

    void setArgument(label index,const word &s) {
        assert(index==0);

        speciesName_=s;
    }
};
defineTypeNameAndDebug(swakPsiChemistryModelPluginFunction_RR,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakPsiChemistryModelPluginFunction_RR,name,psiChem_RR);

class swakPsiChemistryModelPluginFunction_RRError
: public swakPsiChemistryModelPluginFunction
{
public:
    TypeName("swakPsiChemistryModelPluginFunction_RRError");
    swakPsiChemistryModelPluginFunction_RRError (
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    ): swakPsiChemistryModelPluginFunction(
        parentDriver,
        name,
        "volScalarField"
    ) {}

    void doEvaluation() {
        autoPtr<volScalarField> pSum(
            new volScalarField(
                0*wrapDimField(chemistry().RR(0))
            )
        );

        volScalarField &summe=pSum();
        for(
            label specI=0;
            specI<chemistry().thermo().composition().species().size();
            specI++
        ) {
            summe+=wrapDimField(chemistry().RR(specI));
        }

        result().setObjectResult(
            pSum
        );
    }
};

defineTypeNameAndDebug(swakPsiChemistryModelPluginFunction_RRError,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakPsiChemistryModelPluginFunction_RRError,name,psiChem_RRError);

class swakPsiChemistryModelPluginFunction_RRSumPositive
: public swakPsiChemistryModelPluginFunction
{
public:
    TypeName("swakPsiChemistryModelPluginFunction_RRSumPositive");
    swakPsiChemistryModelPluginFunction_RRSumPositive (
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    ): swakPsiChemistryModelPluginFunction(
        parentDriver,
        name,
        "volScalarField"
    ) {}

    void doEvaluation() {
        autoPtr<volScalarField> pSum(
            new volScalarField(
                0*wrapDimField(chemistry().RR(0))
            )
        );

        volScalarField &summe=pSum();
        for(
            label specI=0;
            specI<chemistry().thermo().composition().species().size();
            specI++
        ) {
            const volScalarField &RR=wrapDimField(chemistry().RR(specI));
            forAll(summe,cellI) {
                if(RR[cellI]>0) {
                    summe[cellI]+=RR[cellI];
                }
            }
        }
        summe.correctBoundaryConditions();

        result().setObjectResult(
            pSum
        );
    }
};

defineTypeNameAndDebug(swakPsiChemistryModelPluginFunction_RRSumPositive,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakPsiChemistryModelPluginFunction_RRSumPositive,name,psiChem_RRSumPositive);

class swakPsiChemistryModelPluginFunction_updateChemistry
: public swakPsiChemistryModelPluginFunction
{
    scalar timeStep_;

public:
    TypeName("swakPsiChemistryModelPluginFunction_updateChemistry");
    swakPsiChemistryModelPluginFunction_updateChemistry (
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    ): swakPsiChemistryModelPluginFunction(
        parentDriver,
        name,
        "volScalarField",
        "timestep primitive scalar"
    ) {}

    void doEvaluation() {
        updateChemistry(timeStep_);

        result().setObjectResult(
            autoPtr<volScalarField>(
                new volScalarField(
                    IOobject(
                        "dummyField",
                        mesh().time().timeName(),
                        mesh(),
                        IOobject::NO_READ,
                        IOobject::NO_WRITE
                    ),
                    mesh(),
                    dimensionedScalar("dtChem",dimless,0),
                    "zeroGradient"
                )
            )
        );
    }

    void setArgument(label index,const scalar &s) {
        assert(index==0);

        timeStep_=s;
    }
};
defineTypeNameAndDebug(swakPsiChemistryModelPluginFunction_updateChemistry,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakPsiChemistryModelPluginFunction_updateChemistry,name,psiChem_updateChemistry);

class swakPsiChemistryModelPluginFunction_deltaTChem
: public swakPsiChemistryModelPluginFunction
{
public:
    TypeName("swakPsiChemistryModelPluginFunction_deltaTChem");
    swakPsiChemistryModelPluginFunction_deltaTChem (
        const FieldValueExpressionDriver &parentDriver,
        const word &name
    ): swakPsiChemistryModelPluginFunction(
        parentDriver,
        name,
        "volScalarField"
    ) {}

    void doEvaluation() {
        const scalarField &dtChem=chemistry().deltaTChem();

        autoPtr<volScalarField> val(
            new volScalarField(
                IOobject(
                    "deltaTChem",
                    mesh().time().timeName(),
                    mesh(),
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh(),
                dimensionedScalar("dtChem",dimless,0),
                "zeroGradient"
            )
        );
        val->internalField()=dtChem;

        result().setObjectResult(
            val
        );
    }
};
defineTypeNameAndDebug(swakPsiChemistryModelPluginFunction_deltaTChem,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakPsiChemistryModelPluginFunction_deltaTChem,name,psiChem_deltaTChem);

} // namespace

// ************************************************************************* //
