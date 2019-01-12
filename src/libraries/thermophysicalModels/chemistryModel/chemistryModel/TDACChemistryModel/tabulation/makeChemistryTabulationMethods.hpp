/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
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

#ifndef makeChemistryTabulationMethods_HPP
#define makeChemistryTabulationMethods_HPP

#include "chemistryTabulationMethod.hpp"
#include "noChemistryTabulation.hpp"
#include "ISAT.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeChemistryTabulationMethod(SS, Comp, Thermo)                        \
                                                                               \
    typedef chemistryTabulationMethods::SS<Comp, Thermo>                       \
        chemistryTabulationMethod##SS##Comp##Thermo;                           \
                                                                               \
    defineTemplateTypeNameAndDebugWithName                                     \
    (                                                                          \
        chemistryTabulationMethod##SS##Comp##Thermo,                           \
        (#SS"<" + word(Comp::typeName_())                                      \
      + "," + Thermo::typeName() + ">").c_str(),                               \
        0                                                                      \
    );                                                                         \
                                                                               \
    chemistryTabulationMethod<Comp, Thermo>::                                  \
        adddictionaryConstructorToTable                                        \
        <chemistryTabulationMethod##SS##Comp##Thermo>                          \
        add##chemistryTabulationMethods##SS##Comp##Thermo##ConstructorToTable_;


#define makeChemistryTabulationMethods(CompChemModel, Thermo)                  \
                                                                               \
    typedef chemistryTabulationMethod<CompChemModel, Thermo>                   \
        chemistryTabulationMethod##CompChemModel##Thermo;                      \
                                                                               \
    defineTemplateTypeNameAndDebugWithName                                     \
    (                                                                          \
        chemistryTabulationMethod##CompChemModel##Thermo,                      \
        (word(chemistryTabulationMethod##CompChemModel##Thermo::typeName_()) + \
        '<' + word(CompChemModel::typeName_()) + "," + Thermo::typeName() + '>'\
        ).c_str(),                                                             \
        0                                                                      \
    );                                                                         \
                                                                               \
    defineTemplateRunTimeSelectionTable                                        \
    (                                                                          \
        chemistryTabulationMethod##CompChemModel##Thermo,                      \
        dictionary                                                             \
    );                                                                         \
                                                                               \
    makeChemistryTabulationMethod                                              \
    (                                                                          \
        none,                                                                  \
        CompChemModel,                                                         \
        Thermo                                                                 \
    );                                                                         \
                                                                               \
    makeChemistryTabulationMethod                                              \
    (                                                                          \
        ISAT,                                                                  \
        CompChemModel,                                                         \
        Thermo                                                                 \
    );


#endif
