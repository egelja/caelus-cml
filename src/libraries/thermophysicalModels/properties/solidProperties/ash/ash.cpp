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

#include "ash.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(ash, 0);
    addToRunTimeSelectionTable(solidProperties, ash,);
    addToRunTimeSelectionTable(solidProperties, ash, Istream);
    addToRunTimeSelectionTable(solidProperties, ash, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::ash::ash()
:
    solidProperties(2010, 710, 0.04, 0.0, 1.0)
{
    if (debug)
    {
        WarningIn("ash::ash()")
            << "Properties of ash need to be checked!!!"
            << endl;
    }
}


CML::ash::ash(const solidProperties& s)
:
    solidProperties(s)
{}


CML::ash::ash(Istream& is)
:
    solidProperties(is)
{}


CML::ash::ash(const dictionary& dict)
:
    solidProperties(dict)
{}


CML::ash::ash(const ash& s)
:
    solidProperties(s)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::ash::writeData(Ostream& os) const
{
    solidProperties::writeData(os);
}


// * * * * * * * * * * * * * * IOStream operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const ash& s)
{
    s.writeData(os);
    return os;
}


// ************************************************************************* //
