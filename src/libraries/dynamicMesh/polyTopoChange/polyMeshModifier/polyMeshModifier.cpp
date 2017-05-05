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

Description
    Virtual base class for mesh modifiers.

\*---------------------------------------------------------------------------*/

#include "polyMeshModifier.hpp"
#include "dictionary.hpp"
#include "polyTopoChanger.hpp"
#include "polyMesh.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(polyMeshModifier, 0);

    defineRunTimeSelectionTable(polyMeshModifier, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
CML::polyMeshModifier::polyMeshModifier
(
    const word& name,
    const label index,
    const polyTopoChanger& mme,
    const bool act
)
:
    name_(name),
    index_(index),
    topoChanger_(mme),
    active_(act)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::polyMeshModifier::~polyMeshModifier()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::polyTopoChanger& CML::polyMeshModifier::topoChanger() const
{
    return topoChanger_;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const polyMeshModifier& pmm)
{
    pmm.write(os);
    os.check("Ostream& operator<<(Ostream& f, const polyMeshModifier& pmm)");
    return os;
}


// ************************************************************************* //
