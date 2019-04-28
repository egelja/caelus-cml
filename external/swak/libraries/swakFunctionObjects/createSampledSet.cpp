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
    2011, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
    2013 Bruno Santos <wyldckat@gmail.com>

\*---------------------------------------------------------------------------*/

#include "createSampledSet.hpp"

#include "SetsRepository.hpp"

#include "polyMesh.hpp"

namespace CML {
    defineTypeNameAndDebug(createSampledSet,0);
}

CML::createSampledSet::createSampledSet
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
):
    active_(true),
    obr_(obr)
{
    if (!isA<polyMesh>(obr))
    {
        active_=false;
        WarningInFunction
            << "Not a polyMesh. Nothing I can do"
            << endl;
    }
    read(dict);
    execute();
}

CML::createSampledSet::~createSampledSet()
{}

void CML::createSampledSet::timeSet()
{
    // Do nothing
}

void CML::createSampledSet::read(const dictionary& dict)
{
    if(active_) {
        SetsRepository::getRepository(obr_).getSet(
            dict,
            dynamic_cast<const polyMesh &>(obr_)
        );
    }
}

void CML::createSampledSet::execute()
{
}


void CML::createSampledSet::end()
{
}

void CML::createSampledSet::write()
{
}

void CML::createSampledSet::clearData()
{
}

// ************************************************************************* //
