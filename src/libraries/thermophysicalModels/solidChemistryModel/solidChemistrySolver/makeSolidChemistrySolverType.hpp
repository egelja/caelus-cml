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

Description
    Macros for instantiating solid chemistry models based on compressibility
    and transport types

\*---------------------------------------------------------------------------*/

#ifndef makeSolidChemistrySolverType_H
#define makeSolidChemistrySolverType_H

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeSolidChemistrySolverType(SS, ODEChem, Comp, SThermo, GThermo)     \
                                                                              \
    typedef SS<ODEChem<Comp, SThermo, GThermo> >                              \
        SS##ODEChem##Comp##SThermo##GThermo;                                  \
                                                                              \
    defineTemplateTypeNameAndDebugWithName                                    \
    (                                                                         \
        SS##ODEChem##Comp##SThermo##GThermo,                                  \
        #SS"<"#ODEChem"<"#Comp","#SThermo","#GThermo">>",                     \
        0                                                                     \
    );                                                                        \
                                                                              \
    addToRunTimeSelectionTable                                                \
    (                                                                         \
        Comp,                                                                 \
        SS##ODEChem##Comp##SThermo##GThermo,                                  \
        fvMesh                                                                \
    );


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
