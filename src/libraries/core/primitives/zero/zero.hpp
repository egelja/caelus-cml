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
    CML::zero

Description
    A class representing the concept of 0 used to avoid unnecessary
    manipulations for objects that are known to be zero at compile-time.

SourceFiles
    zeroI.hpp

\*---------------------------------------------------------------------------*/

#ifndef zero_HPP_
#define zero_HPP_

namespace CML
{
class zero
{
public:

    zero()
    {}
};


template <typename Type>
inline Type const& operator+(Type const& t, zero const&)
{
    return t;
}

template <typename Type>
inline Type const& operator+(zero const&, Type const& t)
{
    return t;
}

template <typename Type>
inline const Type& operator-(const Type& t, const zero&)
{
    return t;
}

template <typename Type>
inline Type operator-(zero const&, Type const& t)
{
    return -t;
}

template <typename Type>
inline zero operator*(Type const& t, zero const&)
{
    return zero();
}

template <typename Type>
inline zero operator*(zero const&, Type const& t)
{
    return zero();
}

template <typename Type>
inline zero operator/(zero const&, Type const& t)
{
    return zero();
}

} // End namespace CML


#endif


