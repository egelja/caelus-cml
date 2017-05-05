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

#include "lcsEnthalpySourcePluginFunction.hpp"

#include "addToRunTimeSelectionTable.hpp"

#include "swakCloudTypes.hpp"

#include "basicReactingCloud.hpp"
#include "basicReactingMultiphaseCloud.hpp"

namespace CML {

defineTypeNameAndDebug(lcsEnthalpySourcePluginFunction,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,lcsEnthalpySourcePluginFunction , name, lcsEnthalpySource);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

lcsEnthalpySourcePluginFunction::lcsEnthalpySourcePluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name
):
    LagrangianCloudSourcePluginFunction(
        parentDriver,
        name,
        "volScalarField"
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

autoPtr<lcsEnthalpySourcePluginFunction::dimScalarField>
lcsEnthalpySourcePluginFunction::internalEvaluate()
{
    // pick up the first fitting class
    tryCall(dimScalarField,swakFluidThermoCloudType,thermoCloud,hsTrans());
    tryCall(dimScalarField,basicReactingCloud,reactingCloud,hsTrans());
    tryCall(dimScalarField,basicReactingMultiphaseCloud,reactingMultiphaseCloud,hsTrans());

    return autoPtr<dimScalarField>();
}

void lcsEnthalpySourcePluginFunction::doEvaluation()
{
    autoPtr<dimScalarField> pSh=internalEvaluate();

    noCloudFound(pSh);

    const dimScalarField &Sh=pSh();

    autoPtr<volScalarField> pSource(
        new volScalarField(
            IOobject(
                cloudName()+"EnthalpySource",
                mesh().time().timeName(),
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh(),
            Sh.dimensions()/(dimTime*dimVolume),
            "zeroGradient"
        )
    );

    pSource->internalField()=Sh.field()/(mesh().V()*mesh().time().deltaT().value());

    result().setObjectResult(pSource);
}

// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //


} // namespace

// ************************************************************************* //
