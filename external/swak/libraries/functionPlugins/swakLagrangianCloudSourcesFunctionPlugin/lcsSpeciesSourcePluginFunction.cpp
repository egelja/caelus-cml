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

#include "lcsSpeciesSourcePluginFunction.hpp"

#include "addToRunTimeSelectionTable.hpp"

#include "swakCloudTypes.hpp"

#include "basicReactingCloud.hpp"
#include "basicReactingMultiphaseCloud.hpp"

namespace CML {

defineTypeNameAndDebug(lcsSpeciesSourcePluginFunction,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,lcsSpeciesSourcePluginFunction , name, lcsSpeciesSource);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

lcsSpeciesSourcePluginFunction::lcsSpeciesSourcePluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name
):
    LagrangianCloudSourcePluginFunction(
        parentDriver,
        name,
        "volScalarField",
        "speciesName primitive word"
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

autoPtr<lcsSpeciesSourcePluginFunction::dimScalarField>
lcsSpeciesSourcePluginFunction::internalEvaluate(const label speciesIndex)
{
    // pick up the first fitting class
    tryCall(dimScalarField,basicReactingCloud,reactingCloud,Srho(speciesIndex));
    tryCall(dimScalarField,basicReactingMultiphaseCloud,reactingMultiphaseCloud,Srho(speciesIndex));

    return autoPtr<dimScalarField>();
}

label lcsSpeciesSourcePluginFunction::getIndex(wordList &speciesList)
{
    getSpeciesIndex(basicReactingCloud,reactingCloud);
    getSpeciesIndex(basicReactingMultiphaseCloud,reactingMultiphaseCloud);

    return -1;
}

void lcsSpeciesSourcePluginFunction::doEvaluation()
{
    wordList speciesList;
    label speciesIndex=getIndex(speciesList);

    if(speciesList.size()==0) {
        listAvailableClouds(Info);
        FatalErrorInFunction
            << "No species list found (probably wrong cloud type)"
            << endl
            << "Supported cloud types (typeid): "                   \
            << supportedTypes() << endl                             \
            << exit(FatalError);
    }

    if(speciesIndex<0) {
        listAvailableClouds(Info);
        FatalErrorInFunction
            << "Species " << speciesName_ << " not found in gas composition "
            << speciesList
            << endl
            << "Supported cloud types (typeid): "                   \
            << supportedTypes() << endl                             \
            << exit(FatalError);

    }

    autoPtr<dimScalarField> pSrho=internalEvaluate(speciesIndex);;

    noCloudFound(pSrho);

    const dimScalarField &Srho=pSrho();

    autoPtr<volScalarField> pSource(
        new volScalarField(
            IOobject(
                cloudName()+"SpeciesSource",
                mesh().time().timeName(),
                mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh(),
            Srho.dimensions(),
            "zeroGradient"
        )
    );

    pSource->internalField()=Srho.field();

    result().setObjectResult(pSource);
}

void lcsSpeciesSourcePluginFunction::setArgument(
    label index,
    const word &name
)
{
    if(index==0) {
        LagrangianCloudSourcePluginFunction::setArgument(
            index,
            name
        );
        return;
    }
    assert(index==1);
    speciesName_=name;
}



// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //


} // namespace

// ************************************************************************* //
