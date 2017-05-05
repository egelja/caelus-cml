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

#ifndef makeChemistrySolverTypes_H
#define makeChemistrySolverTypes_H

#include "chemistrySolver.hpp"

#include "ODEChemistryModel.hpp"

#include "noChemistrySolver.hpp"
#include "EulerImplicit.hpp"
#include "ode_.hpp"
#include "sequential.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeChemistrySolverTypes(CompChemModel,Thermo)                        \
                                                                              \
    typedef ODEChemistryModel<CompChemModel, Thermo> CompChemModel##Thermo;   \
                                                                              \
    makeChemistrySolver(CompChemModel##Thermo);                               \
                                                                              \
    makeChemistrySolverType                                                   \
    (                                                                         \
        noChemistrySolver,                                                    \
        ODEChemistryModel,                                                    \
        CompChemModel,                                                        \
        Thermo                                                                \
    );                                                                        \
                                                                              \
    makeChemistrySolverType                                                   \
    (                                                                         \
        EulerImplicit,                                                        \
        ODEChemistryModel,                                                    \
        CompChemModel,                                                        \
        Thermo                                                                \
    );                                                                        \
                                                                              \
    makeChemistrySolverType                                                   \
    (                                                                         \
        ode,                                                                  \
        ODEChemistryModel,                                                    \
        CompChemModel,                                                        \
        Thermo                                                                \
    );                                                                        \
                                                                              \
    makeChemistrySolverType                                                   \
    (                                                                         \
        sequential,                                                           \
        ODEChemistryModel,                                                    \
        CompChemModel,                                                        \
        Thermo                                                                \
    );


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
