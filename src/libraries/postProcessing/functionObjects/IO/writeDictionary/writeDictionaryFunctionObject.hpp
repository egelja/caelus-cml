/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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

Typedef
    CML::writeDictionaryFunctionObject

Description
    FunctionObject wrapper around writeDictionary to allow them to be
    created via the functions entry within controlDict.

SourceFiles
    writeDictionaryFunctionObject.cpp

\*---------------------------------------------------------------------------*/

#ifndef writeDictionaryFunctionObject_H
#define writeDictionaryFunctionObject_H

#include "writeDictionary.hpp"
#include "OutputFilterFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    typedef OutputFilterFunctionObject<writeDictionary>
        writeDictionaryFunctionObject;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
