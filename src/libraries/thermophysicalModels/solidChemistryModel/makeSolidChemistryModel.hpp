/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
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

Description
    Macros for instantiating solid chemistry models

\*---------------------------------------------------------------------------*/

#ifndef makeSolidChemistryModel_HPP
#define makeSolidChemistryModel_HPP

#include "solidChemistryModel.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeSolidChemistryModel(sChemistry, SS, Comp, SThermo)                 \
                                                                               \
    typedef CML::sChemistry<CML::Comp, CML::SThermo>                           \
        sChemistry##Comp##SThermo;                                             \
                                                                               \
    defineTemplateTypeNameAndDebugWithName                                     \
    (                                                                          \
        sChemistry##Comp##SThermo,                                             \
        (CML::word(sChemistry##Comp##SThermo::typeName_()) + "<"#Comp","       \
        + SThermo::typeName() + ">").c_str(),                                  \
        0                                                                      \
    );


#define makeSolidGasChemistryModel(sChemistry, SS, Comp, SThermo, GThermo)     \
                                                                               \
    typedef CML::SS<CML::Comp, CML::SThermo, CML::GThermo>                     \
        SS##Comp##SThermo##GThermo;                                            \
                                                                               \
    defineTemplateTypeNameAndDebugWithName                                     \
    (                                                                          \
        SS##Comp##SThermo##GThermo,                                            \
        (CML::word(SS##Comp##SThermo##GThermo::typeName_()) + "<"#Comp","      \
        + SThermo::typeName() + "," + GThermo::typeName() + ">").c_str(),      \
        0                                                                      \
    );


#endif
