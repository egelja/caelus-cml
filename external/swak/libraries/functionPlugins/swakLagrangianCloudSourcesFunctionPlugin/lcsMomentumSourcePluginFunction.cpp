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

#include "lcsMomentumSourcePluginFunction.hpp"

#include "addToRunTimeSelectionTable.hpp"

#include "swakCloudTypes.hpp"

#include "basicKinematicCloud.hpp"
#include "basicReactingCloud.hpp"
#include "basicReactingMultiphaseCloud.hpp"

namespace CML {

defineTypeNameAndDebug(lcsMomentumSourcePluginFunction,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,lcsMomentumSourcePluginFunction , name, lcsMomentumSource);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

lcsMomentumSourcePluginFunction::lcsMomentumSourcePluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name
):
    LagrangianCloudSourcePluginFunction(
        parentDriver,
        name,
        "volVectorField"
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

autoPtr<lcsMomentumSourcePluginFunction::dimVectorField>
lcsMomentumSourcePluginFunction::internalEvaluate()
{
    // pick up the first fitting class
    tryCall(dimVectorField,basicKinematicCloud,kinematicCloud,UTrans());
    tryCall(dimVectorField,swakFluidThermoCloudType,thermoCloud,UTrans());
    tryCall(dimVectorField,basicReactingCloud,reactingCloud,UTrans());
    tryCall(dimVectorField,basicReactingMultiphaseCloud,reactingMultiphaseCloud,UTrans());

    return autoPtr<dimVectorField>();
}

void lcsMomentumSourcePluginFunction::doEvaluation()
{
    autoPtr<dimVectorField> pSU=internalEvaluate();

    noCloudFound(pSU);

    const dimVectorField &SU=pSU();

    autoPtr<volVectorField> pSource(
        new volVectorField(
            IOobject(
                cloudName()+"MomentumSource",
                mesh().time().timeName(),
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh(),
            SU.dimensions()/(dimTime*dimVolume),
            "zeroGradient"
        )
    );

    pSource->internalField()=SU.field()/(mesh().time().deltaT().value()*mesh().V());

    result().setObjectResult(pSource);
}

// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //


} // namespace

// ************************************************************************* //
