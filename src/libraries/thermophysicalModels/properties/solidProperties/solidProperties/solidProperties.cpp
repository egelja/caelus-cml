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

#include "solidProperties.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(solidProperties, 0);
    defineRunTimeSelectionTable(solidProperties,);
    defineRunTimeSelectionTable(solidProperties, Istream);
    defineRunTimeSelectionTable(solidProperties, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidProperties::solidProperties
(
    scalar rho,
    scalar Cp,
    scalar K,
    scalar Hf,
    scalar emissivity
)
:
    rho_(rho),
    Cp_(Cp),
    K_(K),
    Hf_(Hf),
    emissivity_(emissivity)
{}


CML::solidProperties::solidProperties(Istream& is)
:
    rho_(readScalar(is)),
    Cp_(readScalar(is)),
    K_(readScalar(is)),
    Hf_(readScalar(is)),
    emissivity_(readScalar(is))
{}


CML::solidProperties::solidProperties(const dictionary& dict)
:
    rho_(readScalar(dict.lookup("rho"))),
    Cp_(readScalar(dict.lookup("Cp"))),
    K_(readScalar(dict.lookup("K"))),
    Hf_(readScalar(dict.lookup("Hf"))),
    emissivity_(readScalar(dict.lookup("emissivity")))
{}


CML::solidProperties::solidProperties(const solidProperties& s)
:
    rho_(s.rho_),
    Cp_(s.Cp_),
    K_(s.K_),
    Hf_(s.Hf_),
    emissivity_(s.emissivity_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::solidProperties::writeData(Ostream& os) const
{
    os  << rho_ << token::SPACE
        << Cp_ << token::SPACE
        << K_ << token::SPACE
        << Hf_ << token::SPACE
        << emissivity_;
}


// * * * * * * * * * * * * * * IOStream operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const solidProperties& s)
{
    s.writeData(os);
    return os;
}


// ************************************************************************* //
