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

#include "CaCO3.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(CaCO3, 0);
    addToRunTimeSelectionTable(solidProperties, CaCO3,);
    addToRunTimeSelectionTable(solidProperties, CaCO3, Istream);
    addToRunTimeSelectionTable(solidProperties, CaCO3, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::CaCO3::CaCO3()
:
    solidProperties(2710, 850, 1.3, 0.0, 1.0)
{
    if (debug)
    {
        WarningIn("CaCO3::CaCO3()")
            << "Properties of CaCO3 need to be checked!!!"
            << endl;
    }
}


CML::CaCO3::CaCO3(const solidProperties& s)
:
    solidProperties(s)
{}


CML::CaCO3::CaCO3(Istream& is)
:
    solidProperties(is)
{}


CML::CaCO3::CaCO3(const dictionary& dict)
:
    solidProperties(dict)
{}


CML::CaCO3::CaCO3(const CaCO3& s)
:
    solidProperties(s)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::CaCO3::writeData(Ostream& os) const
{
    solidProperties::writeData(os);
}


// * * * * * * * * * * * * * * IOStream operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const CaCO3& s)
{
    s.writeData(os);
    return os;
}


// ************************************************************************* //
