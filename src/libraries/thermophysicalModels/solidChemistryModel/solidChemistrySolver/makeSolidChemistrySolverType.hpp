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
    Macros for instantiating solid chemistry models based on compressibility
    and transport types

\*---------------------------------------------------------------------------*/

#ifndef makeSolidChemistrySolverType_HPP
#define makeSolidChemistrySolverType_HPP

#include "addToRunTimeSelectionTable.hpp"

#include "noChemistrySolver.hpp"
#include "EulerImplicit.hpp"
#include "ode_.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeSolidChemistrySolverType(SS, Schem, Comp, SThermo, GThermo)        \
                                                                               \
    typedef SS<Schem<Comp, SThermo, GThermo>>                                  \
        SS##Schem##Comp##SThermo##GThermo;                                     \
                                                                               \
    defineTemplateTypeNameAndDebugWithName                                     \
    (                                                                          \
        SS##Schem##Comp##SThermo##GThermo,                                     \
        (#SS"<" + word(Schem<Comp, SThermo, GThermo>::typeName_())             \
      + "<"#Comp"," + SThermo::typeName()                                      \
      + ","  + GThermo::typeName() + ">>").c_str(),                            \
        0                                                                      \
    );                                                                         \
                                                                               \
    addToRunTimeSelectionTable                                                 \
    (                                                                          \
        Comp,                                                                  \
        SS##Schem##Comp##SThermo##GThermo,                                     \
        thermo                                                                 \
    );


#define makeSolidChemistrySolverTypes(SolidChem, Comp, SThermo, GThermo)       \
                                                                               \
    makeSolidChemistrySolverType                                               \
    (                                                                          \
        noChemistrySolver,                                                     \
        SolidChem,                                                             \
        Comp,                                                                  \
        SThermo,                                                               \
        GThermo                                                                \
    );                                                                         \
                                                                               \
    makeSolidChemistrySolverType                                               \
    (                                                                          \
        ode,                                                                   \
        SolidChem,                                                             \
        Comp,                                                                  \
        SThermo,                                                               \
        GThermo                                                                \
    );


} // End namespace CML


#endif
