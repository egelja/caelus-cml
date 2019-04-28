/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
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

Class
    CML::uniformField

Description
    A class representing the concept of a uniform field which stores only
    the single value and providing the operator[] to return it.

\*---------------------------------------------------------------------------*/

#ifndef uniformField_HPP
#define uniformField_HPP

#include "label.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class uniformField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class uniformField
{
    Type value_;

public:

    //- Construct given value
    inline uniformField(const Type& value)
    :
        value_(value)
    {}


    // Member Operators

    inline operator Type() const
    {
        return value_;
    }

    inline Type operator[](const label) const
    {
        return value_;
    }

    inline uniformField field() const
    {
        return UniformField(value_);
    }

};


} // End namespace CML


#endif
