/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright  held by original author
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
    2013 Bruno Santos <wyldckat@gmail.com>

\*---------------------------------------------------------------------------*/

#include "addForeignMeshes.hpp"

#include "CommonValueExpressionDriver.hpp"

namespace CML {
    defineTypeNameAndDebug(addForeignMeshes,0);
}

CML::addForeignMeshes::addForeignMeshes
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
)
{
    read(dict);
    execute();
}

CML::addForeignMeshes::~addForeignMeshes()
{}

void CML::addForeignMeshes::timeSet()
{
    // Do nothing
}

void CML::addForeignMeshes::read(const dictionary& dict)
{
    CommonValueExpressionDriver::readForeignMeshInfo(dict,true);
}

void CML::addForeignMeshes::execute()
{
}


void CML::addForeignMeshes::end()
{
}

void CML::addForeignMeshes::write()
{
}

void CML::addForeignMeshes::clearData()
{
}

// ************************************************************************* //
