/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    defineRunTimeSelectionTable(solidProperties, dictionary);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidProperties::solidProperties
(
    scalar rho,
    scalar Cp,
    scalar kappa,
    scalar Hf,
    scalar emissivity
)
:
    rho_(rho),
    Cp_(Cp),
    kappa_(kappa),
    Hf_(Hf),
    emissivity_(emissivity)
{}


CML::solidProperties::solidProperties(const dictionary& dict)
:
    rho_(readScalar(dict.lookup("rho"))),
    Cp_(readScalar(dict.lookup("Cp"))),
    kappa_
    (
        dict.found("K")
      ? readScalar(dict.lookup("K"))
      : readScalar(dict.lookup("kappa"))
    ),
    Hf_(readScalar(dict.lookup("Hf"))),
    emissivity_(readScalar(dict.lookup("emissivity")))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::solidProperties::readIfPresent(const dictionary& dict)
{
    dict.readIfPresent("rho", rho_);
    dict.readIfPresent("Cp", Cp_);
    dict.readIfPresent("K", kappa_);
    dict.readIfPresent("kappa", kappa_);
    dict.readIfPresent("Hf_", Hf_);
    dict.readIfPresent("emissivity", emissivity_);
}


void CML::solidProperties::writeData(Ostream& os) const
{
    os  << rho_ << token::SPACE
        << Cp_ << token::SPACE
        << kappa_ << token::SPACE
        << Hf_ << token::SPACE
        << emissivity_;
}


// * * * * * * * * * * * * * * IOStream operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const solidProperties& s)
{
    s.writeData(os);
    return os;
}
