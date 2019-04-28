/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

#ifndef makeReactionThermo_HPP
#define makeReactionThermo_HPP

#include "addToRunTimeSelectionTable.hpp"
#include "makeThermo.hpp"
#include "SpecieMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define defineThermoPhysicsReactionThermo(BaseReactionThermo,CThermo,Mixture,ThermoPhys) \
                                                                               \
    typedef CThermo                                                            \
    <                                                                          \
        BaseReactionThermo,                                                    \
        SpecieMixture                                                          \
        <                                                                      \
            Mixture                                                            \
            <                                                                  \
                ThermoPhys                                                     \
            >                                                                  \
        >                                                                      \
    > CThermo##Mixture##ThermoPhys;                                            \
                                                                               \
    defineTemplateTypeNameAndDebugWithName                                     \
    (                                                                          \
        CThermo##Mixture##ThermoPhys,                                          \
        (#CThermo"<" + Mixture<ThermoPhys>::typeName() + ">").c_str(),         \
        0                                                                      \
    )


#define makeThermoPhysicsReactionThermos(BaseThermo,BaseReactionThermo,CThermo,Mixture,ThermoPhys) \
                                                                               \
    defineThermoPhysicsReactionThermo                                          \
    (                                                                          \
        BaseReactionThermo,                                                    \
        CThermo,                                                               \
        Mixture,                                                               \
        ThermoPhys                                                             \
    );                                                                         \
                                                                               \
    addThermoPhysicsThermo(basicThermo, CThermo##Mixture##ThermoPhys);         \
    addThermoPhysicsThermo(fluidThermo, CThermo##Mixture##ThermoPhys);         \
    addThermoPhysicsThermo(BaseThermo, CThermo##Mixture##ThermoPhys);          \
    addThermoPhysicsThermo(BaseReactionThermo, CThermo##Mixture##ThermoPhys)


#define makeThermoPhysicsReactionThermo(BaseReactionThermo,CThermo,Mixture,ThermoPhys) \
                                                                               \
    defineThermoPhysicsReactionThermo                                          \
    (                                                                          \
        BaseReactionThermo,                                                    \
        CThermo,                                                               \
        Mixture,                                                               \
        ThermoPhys                                                             \
    );                                                                         \
                                                                               \
    addThermoPhysicsThermo(BaseReactionThermo, CThermo##Mixture##ThermoPhys)


#define makeReactionThermos(BaseThermo,BaseReactionThermo,CThermo,Mixture,Transport,Type,Thermo,EqnOfState,Specie) \
                                                                               \
    typedefThermoPhysics(Transport,Type,Thermo,EqnOfState,Specie);             \
                                                                               \
    makeThermoPhysicsReactionThermos                                           \
    (                                                                          \
        BaseThermo,                                                            \
        BaseReactionThermo,                                                    \
        CThermo,                                                               \
        Mixture,                                                               \
        Transport##Type##Thermo##EqnOfState##Specie                            \
    )


#define makeReactionThermo(BaseReactionThermo,CThermo,Mixture,Transport,Type,Thermo,EqnOfState,Specie) \
                                                                               \
    typedefThermoPhysics(Transport,Type,Thermo,EqnOfState,Specie);             \
                                                                               \
    makeThermoPhysicsReactionThermo                                            \
    (                                                                          \
        BaseReactionThermo,                                                    \
        CThermo,                                                               \
        Mixture,                                                               \
        Transport##Type##Thermo##EqnOfState##Specie                            \
    )


#endif
