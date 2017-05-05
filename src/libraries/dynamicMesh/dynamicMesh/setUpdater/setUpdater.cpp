/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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

\*---------------------------------------------------------------------------*/

#include "setUpdater.hpp"
#include "polyTopoChanger.hpp"
#include "polyTopoChange.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "mapPolyMesh.hpp"
#include "cellSet.hpp"
#include "faceSet.hpp"
#include "pointSet.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(setUpdater, 0);
    addToRunTimeSelectionTable
    (
        polyMeshModifier,
        setUpdater,
        dictionary
    );
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from dictionary
CML::setUpdater::setUpdater
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyTopoChanger& mme
)
:
    polyMeshModifier(name, index, mme, Switch(dict.lookup("active")))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::setUpdater::~setUpdater()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::setUpdater::changeTopology() const
{
    // I am never cause of changeTopo
    return false;
}


void CML::setUpdater::setRefinement(polyTopoChange&) const
{}


void CML::setUpdater::modifyMotionPoints(pointField&) const
{}


void CML::setUpdater::updateMesh(const mapPolyMesh& morphMap)
{
    // Mesh has changed topologically. Update all sets.
    if (debug)
    {
        Pout<< "setUpdater::updateMesh(const mapPolyMesh& morphMap)"
            << endl;
    }

    updateSets<cellSet>(morphMap);
    updateSets<faceSet>(morphMap);
    updateSets<pointSet>(morphMap);
}


void CML::setUpdater::write(Ostream& os) const
{
    os  << nl << type() << nl;
}


void CML::setUpdater::writeDict(Ostream& os) const
{
    os  << nl << name() << nl << token::BEGIN_BLOCK << nl
        << "    type " << type()
        << token::END_STATEMENT << nl
        << "    active " << active()
        << token::END_STATEMENT << nl
        << token::END_BLOCK << endl;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //


// ************************************************************************* //
