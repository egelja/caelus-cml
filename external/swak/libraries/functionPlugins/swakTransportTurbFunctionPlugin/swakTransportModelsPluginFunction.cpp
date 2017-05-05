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

#include "swakTransportModelsPluginFunction.hpp"
#include "FieldValueExpressionDriver.hpp"

#include "HashPtrTable.hpp"
#include "singlePhaseTransportModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

namespace CML {

defineTypeNameAndDebug(swakTransportModelsPluginFunction,0);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

swakTransportModelsPluginFunction::swakTransportModelsPluginFunction(
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

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const transportModel &swakTransportModelsPluginFunction::transportInternal(
    const fvMesh &reg
)
{
    static HashPtrTable<transportModel> trans_;

    if(reg.foundObject<transportModel>("transportProperties")) {
        if(debug) {
            Info << "swakTransportModelsPluginFunction::transportInternal: "
                << "already in memory" << endl;
        }
        // Somebody else already registered this
        return reg.lookupObject<transportModel>("transportProperties");
    }
    if(!trans_.found(reg.name())) {
        if(debug) {
            Info << "swakTransportModelsPluginFunction::transportInternal: "
                << "not yet in memory for " << reg.name() << endl;
        }


        // Create it ourself because nobody registered it
        trans_.set(
            reg.name(),
            new singlePhaseTransportModel(
                reg.lookupObject<volVectorField>("U"),
                reg.lookupObject<surfaceScalarField>("phi")
            )
        );
    }

    return *(trans_[reg.name()]);
}

const transportModel &swakTransportModelsPluginFunction::transport()
{
    return transportInternal(mesh());
}

// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //

#define concreteTransportFunction(funcName,resultType)                \
class swakTransportModelsPluginFunction_ ## funcName                \
: public swakTransportModelsPluginFunction                          \
{                                                                  \
public:                                                            \
    TypeName("swakTransportModelsPluginFunction_" #funcName);       \
    swakTransportModelsPluginFunction_ ## funcName (                \
        const FieldValueExpressionDriver &parentDriver,            \
        const word &name                                           \
    ): swakTransportModelsPluginFunction(                           \
        parentDriver,                                              \
        name,                                                      \
        #resultType                                                \
    ) {}                                                           \
    void doEvaluation() {                                          \
        result().setObjectResult(                                  \
            autoPtr<resultType>(                                   \
                new resultType(                                    \
                    transport().funcName()                         \
                )                                                  \
            )                                                      \
        );                                                         \
    }                                                              \
};                                                                 \
defineTypeNameAndDebug(swakTransportModelsPluginFunction_ ## funcName,0);  \
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,swakTransportModelsPluginFunction_ ## funcName,name,trans_ ## funcName);

concreteTransportFunction(nu,volScalarField);

} // namespace

// ************************************************************************* //
