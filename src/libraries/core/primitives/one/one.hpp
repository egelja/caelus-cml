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

namespace CML
{

class one
{
public:

    //- Construct null
    one() {}
};

template <typename Type>
inline Type const& operator*(Type const& t, one const&)
{
    return t;
}

template <typename Type>
inline Type const& operator*(one const&, Type const& t)
{
    return t;
}

template <typename Type>
inline Type operator/(one const&, Type const& t)
{
    return scalar(1)/t;
}


template <typename Type>
inline Type const& operator/(Type const& t, one const&)
{
    return t;
}

} // End namespace CML

#endif

