/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2016 Applied CCM
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
    CML::one

Description
    A class representing the concept of 1 (scalar(1.0)) used to avoid
    unnecessary manipulations for objects which are known to be one at
    compile-time.

\*---------------------------------------------------------------------------*/

#ifndef one_HPP_
#define one_HPP_

#include "label.hpp"

namespace CML
{

class one
{
public:

    typedef one value_type;

    // Constructors

        //- Construct null
        one()
        {}


    // Member operators

        //- Return 1 for label
        inline operator label() const
        {
            return 1;
        }

        //- Return 1 for float
        inline operator float() const
        {
            return 1;
        }

        //- Return 1 for double
        inline operator double() const
        {
            return 1;
        }
};

template<typename arg2>
class innerProduct<one, arg2>
{
public:

    typedef arg2 type;
};

inline const one& operator*(const one& o, const one&)
{
    return o;
}

template <typename Type>
inline const Type& operator*(const Type& t, const one&)
{
    return t;
}

template <typename Type>
inline const Type& operator*(const one&, const Type& t)
{
    return t;
}

template<typename Type>
inline const Type& operator&(const one&, const Type& t)
{
    return t;
}

inline const one& operator/(const one& o, const one&)
{
    return o;
}

template <typename Type>
inline Type operator/(const one&, const Type& t)
{
    return scalar(1)/t;
}


template <typename Type>
inline const Type& operator/(const Type& t, const one&)
{
    return t;
}

} // End namespace CML

#endif

