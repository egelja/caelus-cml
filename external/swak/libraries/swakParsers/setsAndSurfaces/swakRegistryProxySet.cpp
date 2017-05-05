/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2010 OpenCFD Ltd.
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

#include "swakRegistryProxySet.hpp"
#include "dictionary.hpp"
#include "volFields.hpp"
#include "volPointInterpolation.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvMesh.hpp"

#include "SetsRepository.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(swakRegistryProxySet, 0);
    addToRunTimeSelectionTable(sampledSet, swakRegistryProxySet, word);
}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// void CML::swakRegistryProxySet::createGeometry()
// {
//     if (debug)
//     {
//         Pout<< "swakRegistryProxySet::createGeometry() - doing nothing"
//             << endl;
//     }
// }

CML::sampledSet &CML::swakRegistryProxySet::realSet()
{
    return SetsRepository::getRepository(
        mesh()
    ).getSet(
        setName_,
        static_cast<const fvMesh&>(mesh())
    );
}

const CML::sampledSet &CML::swakRegistryProxySet::realSet() const
{
    return SetsRepository::getRepository(
        mesh()
    ).getSet(
        setName_,
        static_cast<const fvMesh&>(mesh())
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::swakRegistryProxySet::swakRegistryProxySet
(
    const word& name,
    const polyMesh& mesh,
    const meshSearch& search,
    const dictionary& dict
)
:
    sampledSet(
        name,
        mesh,
        search,
        dict
    ),
    setName_(dict.lookup("setName"))
{
    setSamples(
        realSet(),
        realSet().cells(),
        realSet().faces(),
        realSet().segments(),
        realSet().curveDist()
    );
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::swakRegistryProxySet::~swakRegistryProxySet()
{}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// ************************************************************************* //
