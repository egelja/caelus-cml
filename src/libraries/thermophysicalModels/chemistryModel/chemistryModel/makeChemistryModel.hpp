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

Description
    Macros for instantiating chemistry models based on compressibility and
    transport types

\*---------------------------------------------------------------------------*/

#ifndef makeChemistryModel_HPP
#define makeChemistryModel_HPP

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeChemistryModel(Comp)                                               \
                                                                               \
    typedef BasicChemistryModel<Comp> BasicChemistryModel##Comp;               \
                                                                               \
    defineTemplateTypeNameAndDebugWithName                                     \
    (                                                                          \
        BasicChemistryModel##Comp,                                             \
        "BasicChemistryModel<"#Comp">",                                        \
        0                                                                      \
    );                                                                         \
                                                                               \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        BasicChemistryModel##Comp,                                             \
        thermo                                                                 \
    );


#define makeChemistryModelType(SS, Comp, Thermo)                               \
                                                                               \
    typedef SS<Comp, Thermo> SS##Comp##Thermo;                                 \
                                                                               \
    defineTemplateTypeNameAndDebugWithName                                     \
    (                                                                          \
        SS##Comp##Thermo,                                                      \
        (#SS"<"#Comp"," + Thermo::typeName() + ">").c_str(),                   \
        0                                                                      \
    );


} // End namespace CML


#endif
