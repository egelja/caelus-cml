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

Global
    CML::addToStaticMemberFunctionSelectionTable

Description
    Macros for easy insertion into member function selection tables

\*---------------------------------------------------------------------------*/

#ifndef addToStaticMemberFunctionSelectionTable_H
#define addToStaticMemberFunctionSelectionTable_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


// add to hash-table of functions with 'lookup' as the key
#define addNamedToStaticMemberFunctionSelectionTable\
(baseType,thisType,memberFunction,argNames,lookup,functionPtr)                \
                                                                              \
    /* Add the thisType memberFunction to the table, find by lookup name */   \
    baseType::add##memberFunction##argNames##                                 \
    StaticMemberFunctionToTable<thisType>                                     \
    add_##lookup##_##thisType##memberFunction##argNames##                     \
    StaticMemberFunctionTo##baseType##Table_(#lookup, functionPtr)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
