/*---------------------------------------------------------------------------*\
Copyright Niels G. Jacobsen Technical University of Denmark
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

Typedef
    CML::overtoppingFunctionObject

Description
    Make the overtopping method available for run-time sampling. It is not
    recommend to be used as a post-processing utility, as the outputted
    data will most probably not capture the overtopping events.

SourceFiles
    overtoppingFunctionObject.cpp

\*---------------------------------------------------------------------------*/

#ifndef overtoppingFunctionObject_HPP
#define overtoppingFunctionObject_HPP

#include "overtopping.hpp"
#include "OutputFilterFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    typedef OutputFilterFunctionObject<overtopping>
        overtoppingFunctionObject;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
