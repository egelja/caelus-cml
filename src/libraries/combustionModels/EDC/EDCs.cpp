/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "makeCombustionTypes.hpp"

#include "psiReactionThermo.hpp"
#include "rhoReactionThermo.hpp"
#include "EDC.hpp"

// * * * * * * * * * * * * * Static Member Data  * * * * * * * * * * * * * * //

template<>
const char* CML::NamedEnum
<
    CML::combustionModels::EDCversions,
    4
>::names[] =
{
    "v1981",
    "v1996",
    "v2005",
    "v2016"
};

const CML::NamedEnum<CML::combustionModels::EDCversions, 4>
    CML::combustionModels::EDCversionNames;

const CML::combustionModels::EDCversions
CML::combustionModels::EDCdefaultVersion
(
    CML::combustionModels::EDCversions::v2005
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

makeCombustionTypes(EDC, psiReactionThermo);
makeCombustionTypes(EDC, rhoReactionThermo);

}
