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

#ifndef makeCombustionTypes_H
#define makeCombustionTypes_H

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeCombustionTypesThermo(CombModel, Comb, Thermo)                    \
                                                                              \
    typedef CML::combustionModels::CombModel                                  \
        <CML::combustionModels::Comb, CML::Thermo>                            \
        CombModel##Comb##Thermo;                                              \
                                                                              \
                                                                              \
    defineTemplateTypeNameAndDebugWithName                                    \
    (                                                                         \
        CombModel##Comb##Thermo,                                              \
        #CombModel"<"#Comb","#Thermo">",                                      \
        0                                                                     \
    );                                                                        \
                                                                              \
    namespace CML                                                             \
    {                                                                         \
        namespace combustionModels                                            \
        {                                                                     \
            typedef CombModel<Comb, Thermo> CombModel##Comb##Thermo;          \
            addToRunTimeSelectionTable                                        \
            (                                                                 \
                Comb,                                                         \
                CombModel##Comb##Thermo,                                      \
                dictionary                                                    \
            );                                                                \
        }                                                                     \
    }                                                                         \

#define makeCombustionTypes(CombModel, CombThermoType)                        \
                                                                              \
    typedef CML::combustionModels::CombModel                                  \
        <CML::combustionModels::CombThermoType>                               \
        CombModel##CombThermoType;                                            \
                                                                              \
    defineTemplateTypeNameAndDebugWithName                                    \
    (                                                                         \
        CombModel##CombThermoType,                                            \
        #CombModel"<"#CombThermoType">",                                      \
        0                                                                     \
    );                                                                        \
                                                                              \
    namespace CML                                                             \
    {                                                                         \
        namespace combustionModels                                            \
        {                                                                     \
            typedef CombModel<CombThermoType> CombModel##CombThermoType;      \
            addToRunTimeSelectionTable                                        \
            (                                                                 \
                CombThermoType,                                               \
                CombModel##CombThermoType,                                    \
                dictionary                                                    \
            );                                                                \
        }                                                                     \
    }                                                                         \

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
