/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

#ifndef makeFvOption_HPP
#define makeFvOption_HPP

#include "fvOption.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeFvOption(Option, Type)                                            \
                                                                              \
    defineTemplateTypeNameAndDebugWithName                                    \
    (                                                                         \
        Option<Type>,                                                         \
        #Type#Option,                                                         \
        0                                                                     \
    );                                                                        \
                                                                              \
    option::adddictionaryConstructorToTable<Option<Type> >                    \
        add##Option##Type##dictionary##ConstructorTooptionTable_


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
