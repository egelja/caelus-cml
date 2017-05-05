/*---------------------------------------------------------------------------*\
Copyright (C) 2013 OpenFOAM Foundation
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

Class
    CML::ensightPTraits

Description
    Conversion of Caelus pTraits into the Ensight equivalent

\*---------------------------------------------------------------------------*/

#ifndef ensightPTraits_H
#define ensightPTraits_H

#include "pTraits.hpp"
#include "fieldTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class ensightPTraits Declaration
\*---------------------------------------------------------------------------*/

template<class PrimitiveType>
class ensightPTraits
{
public:

    // Static data members

        static const char* const typeName;

};


template<>
class ensightPTraits<scalar>
{
public:

    static const char* const typeName;
};

template<>
class ensightPTraits<vector>
{
public:

    static const char* const typeName;
};

template<>
class ensightPTraits<sphericalTensor>
{
public:

    static const char* const typeName;
};

template<>
class ensightPTraits<symmTensor>
{
public:

    static const char* const typeName;
};

template<>
class ensightPTraits<tensor>
{
public:

    static const char* const typeName;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
