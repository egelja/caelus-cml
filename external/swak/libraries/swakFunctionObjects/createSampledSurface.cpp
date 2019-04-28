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
    2010, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>
    2013 Bruno Santos <wyldckat@gmail.com>

\*---------------------------------------------------------------------------*/

#include "createSampledSurface.hpp"

#include "SurfacesRepository.hpp"

namespace CML {
    defineTypeNameAndDebug(createSampledSurface,0);
}

CML::createSampledSurface::createSampledSurface
(
    const word& name,
    const objectRegistry& obr,
    const dictionary& dict,
    const bool loadFromFiles
):
    active_(true),
    obr_(obr)
{
    if (!isA<fvMesh>(obr))
    {
        active_=false;
        WarningInFunction
            << "Not a fvMesh. Nothing I can do"
            << endl;
    }
    read(dict);
    execute();
}

CML::createSampledSurface::~createSampledSurface()
{}

void CML::createSampledSurface::timeSet()
{
    // Do nothing
}

void CML::createSampledSurface::read(const dictionary& dict)
{
    if(active_) {
        SurfacesRepository::getRepository(obr_).getSurface(
            dict,
            dynamic_cast<const fvMesh &>(obr_)
        );
    }
}

void CML::createSampledSurface::execute()
{
}


void CML::createSampledSurface::end()
{
}

void CML::createSampledSurface::write()
{
}

void CML::createSampledSurface::clearData()
{
}

// ************************************************************************* //
