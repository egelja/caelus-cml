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

#include "label.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class zero Declaration
\*---------------------------------------------------------------------------*/

class zero
{
public:

    typedef zero value_type;

    // Constructors

        //- Construct null
        zero()
        {}


    // Member operators

        //- Return 0 for bool
        inline operator bool() const
        {
            return 0;
        }

        //- Return 0 for label
        inline operator label() const
        {
            return 0;
        }

        //- Return 0 for float
        inline operator float() const
        {
            return 0;
        }

        //- Return 0 for double
        inline operator double() const
        {
            return 0;
        }
};


// Global zero
static const zero Zero;

inline zero operator+(const zero&, const zero&)
{
    return Zero;
}

template <typename Type>
inline const Type& operator+(const Type& t, const zero&)
{
    return t;
}

template <typename Type>
inline const Type& operator+(const zero&, const Type& t)
{
    return t;
}

inline zero operator-(const zero&, const zero&)
{
    return Zero;
}

template <typename Type>
inline const Type& operator-(const Type& t, const zero&)
{
    return t;
}

template <typename Type>
inline Type operator-(const zero&, const Type& t)
{
    return -t;
}

inline zero operator*(const zero&, const zero&)
{
    return Zero;
}

template <typename Type>
inline zero operator*(const Type& t, const zero&)
{
    return zero();
}

template <typename Type>
inline zero operator*(const zero&, const Type& t)
{
    return zero();
}

template <typename Type>
inline zero operator/(const zero&, const Type& t)
{
    return zero();
}

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
