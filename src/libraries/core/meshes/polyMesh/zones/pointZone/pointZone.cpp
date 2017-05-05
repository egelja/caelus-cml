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

#include "pointZone.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "pointZoneMesh.hpp"
#include "polyMesh.hpp"
#include "primitiveMesh.hpp"
#include "demandDrivenData.hpp"
#include "syncTools.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(pointZone, 0);
    defineRunTimeSelectionTable(pointZone, dictionary);
    addToRunTimeSelectionTable(pointZone, pointZone, dictionary);
}

const char* const CML::pointZone::labelsName = "pointLabels";

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::pointZone::pointZone
(
    const word& name,
    const labelUList& addr,
    const label index,
    const pointZoneMesh& zm
)
:
    zone(name, addr, index),
    zoneMesh_(zm)
{}


CML::pointZone::pointZone
(
    const word& name,
    const Xfer<labelList>& addr,
    const label index,
    const pointZoneMesh& zm
)
:
    zone(name, addr, index),
    zoneMesh_(zm)
{}


CML::pointZone::pointZone
(
    const word& name,
    const dictionary& dict,
    const label index,
    const pointZoneMesh& zm
)
:
    zone(name, dict, this->labelsName, index),
    zoneMesh_(zm)
{}


CML::pointZone::pointZone
(
    const pointZone& pz,
    const labelUList& addr,
    const label index,
    const pointZoneMesh& zm
)
:
    zone(pz, addr, index),
    zoneMesh_(zm)
{}


CML::pointZone::pointZone
(
    const pointZone& pz,
    const Xfer<labelList>& addr,
    const label index,
    const pointZoneMesh& zm
)
:
    zone(pz, addr, index),
    zoneMesh_(zm)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::pointZone::~pointZone()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::pointZoneMesh& CML::pointZone::zoneMesh() const
{
    return zoneMesh_;
}


CML::label CML::pointZone::whichPoint(const label globalPointID) const
{
    return zone::localID(globalPointID);
}


bool CML::pointZone::checkDefinition(const bool report) const
{
    return zone::checkDefinition(zoneMesh_.mesh().points().size(), report);
}


bool CML::pointZone::checkParallelSync(const bool report) const
{
    const polyMesh& mesh = zoneMesh().mesh();

    labelList maxZone(mesh.nPoints(), -1);
    labelList minZone(mesh.nPoints(), labelMax);
    forAll(*this, i)
    {
        label pointI = operator[](i);
        maxZone[pointI] = index();
        minZone[pointI] = index();
    }
    syncTools::syncPointList(mesh, maxZone, maxEqOp<label>(), label(-1));
    syncTools::syncPointList(mesh, minZone, minEqOp<label>(), labelMax);

    bool error = false;

    forAll(maxZone, pointI)
    {
        // Check point in same (or no) zone on all processors
        if
        (
            (
                maxZone[pointI] != -1
             || minZone[pointI] != labelMax
            )
         && (maxZone[pointI] != minZone[pointI])
        )
        {
            if (report && !error)
            {
                Info<< " ***Problem with pointZone " << index()
                    << " named " << name()
                    << ". Point " << pointI
                    << " at " << mesh.points()[pointI]
                    << " is in zone "
                    << (minZone[pointI] == labelMax ? -1 : minZone[pointI])
                    << " on some processors and in zone "
                    << maxZone[pointI]
                    << " on some other processors." << nl
                    << "(suppressing further warnings)"
                    << endl;
            }
            error = true;
        }
    }

    return error;
}


void CML::pointZone::writeDict(Ostream& os) const
{
    os  << nl << name_ << nl << token::BEGIN_BLOCK << nl
        << "    type " << type() << token::END_STATEMENT << nl;

    writeEntry(this->labelsName, os);

    os  << token::END_BLOCK << endl;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::pointZone::operator=(const pointZone& zn)
{
    clearAddressing();
    labelList::operator=(zn);
}


void CML::pointZone::operator=(const labelUList& addr)
{
    clearAddressing();
    labelList::operator=(addr);
}


void CML::pointZone::operator=(const Xfer<labelList>& addr)
{
    clearAddressing();
    labelList::operator=(addr);
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const pointZone& zn)
{
    zn.write(os);
    os.check("Ostream& operator<<(Ostream&, const pointZone&");
    return os;
}


// ************************************************************************* //
