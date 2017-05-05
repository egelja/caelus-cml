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

#include "coalCloudSpeciesSourcePluginFunction.hpp"

#include "addToRunTimeSelectionTable.hpp"

#include "swakCloudTypes.hpp"

#include "coalCloud.hpp"

namespace CML {

defineTypeNameAndDebug(coalCloudSpeciesSourcePluginFunction,0);
addNamedToRunTimeSelectionTable(FieldValuePluginFunction,coalCloudSpeciesSourcePluginFunction , name, coalCloudSpeciesSource);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

coalCloudSpeciesSourcePluginFunction::coalCloudSpeciesSourcePluginFunction(
    const FieldValueExpressionDriver &parentDriver,
    const word &name
):
    lcsSpeciesSourcePluginFunction(
        parentDriver,
        name
    )
{
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

label coalCloudSpeciesSourcePluginFunction::getIndex(wordList &speciesList)
{
    getSpeciesIndex(coalCloud,reactingMultiphaseCloud);

    return lcsSpeciesSourcePluginFunction::getIndex(speciesList);
}

autoPtr<lcsSpeciesSourcePluginFunction::dimScalarField>
coalCloudSpeciesSourcePluginFunction::internalEvaluate(const label index)
{
    // pick up the first fitting class
    tryCall(dimScalarField,coalCloud,reactingMultiphaseCloud,Srho(index));

    return lcsSpeciesSourcePluginFunction::internalEvaluate(index);
}

// * * * * * * * * * * * * * * * Concrete implementations * * * * * * * * * //


} // namespace

// ************************************************************************* //
