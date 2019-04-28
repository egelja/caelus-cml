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

#ifndef makeCombustionTypes_H
#define makeCombustionTypes_H

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeCombustion(Comp)                                                   \
                                                                               \
    typedef CombustionModel<Comp> CombustionModel##Comp;                       \
                                                                               \
    defineTemplateTypeNameAndDebugWithName                                     \
    (                                                                          \
        CombustionModel##Comp,                                                 \
        (                                                                      \
            word(CombustionModel##Comp::typeName_()) + "<" + Comp::typeName    \
          + ">"                                                                \
        ).c_str(),                                                             \
        0                                                                      \
    );                                                                         \
                                                                               \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        CombustionModel##Comp,                                                 \
        dictionary                                                             \
    );


#define makeCombustionTypesThermo(CombModel, Comp, Thermo)                     \
                                                                               \
    typedef combustionModels::CombModel<Comp, Thermo>                          \
        CombModel##Comp##Thermo;                                               \
                                                                               \
    defineTemplateTypeNameAndDebugWithName                                     \
    (                                                                          \
        CombModel##Comp##Thermo,                                               \
        (                                                                      \
            word(CombModel##Comp##Thermo::typeName_()) + "<" + Comp::typeName  \
          + "," + Thermo::typeName() + ">"                                     \
        ).c_str(),                                                             \
        0                                                                      \
    );                                                                         \
                                                                               \
    CombustionModel<Comp>::                                                    \
        add##dictionary##ConstructorToTable<CombModel##Comp##Thermo>           \
        add##CombModel##Comp##Thermo##dictionary##ConstructorTo##\
CombustionModel##Comp##Table_;


#define makeCombustionTypes(CombModel, Comp)                                   \
                                                                               \
    typedef combustionModels::CombModel<Comp> CombModel##Comp;                 \
                                                                               \
    defineTemplateTypeNameAndDebugWithName                                     \
    (                                                                          \
        CombModel##Comp,                                                       \
        (                                                                      \
            word(CombModel##Comp::typeName_()) + "<" + Comp::typeName + ">"    \
        ).c_str(),                                                             \
        0                                                                      \
    );                                                                         \
                                                                               \
    CombustionModel<Comp>::                                                    \
        add##dictionary##ConstructorToTable<CombModel##Comp>                   \
        add##CombModel##Comp##dictionary##ConstructorTo##CombustionModel##Comp\
##Table_;


} // End namespace CML


#endif
