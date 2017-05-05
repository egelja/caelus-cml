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

#include "reactingParcelInjectionData.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::reactingParcelInjectionData::reactingParcelInjectionData(Istream& is)
:
    thermoParcelInjectionData(is)
{
    is.check("reading Y's");
    is >> Y_;

    is.check("reactingParcelInjectionData(Istream& is)");
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<
(
    Ostream& os,
    const reactingParcelInjectionData& data
)
{
    os << static_cast<const thermoParcelInjectionData&>(data);

    os << data.Y_;

    return os;
}


CML::Istream& CML::operator>>(Istream& is, reactingParcelInjectionData& data)
{
    is >> static_cast<thermoParcelInjectionData&>(data);

    is.check("reading Y's");
    is >> data.Y_;

    is.check("operator(Istream&, reactingParcelInjectionData&)");

    return is;
}


// ************************************************************************* //
