/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::oneField

Description
    A class representing the concept of a field of 1 used to avoid unnecessary
    manipulations for objects which are known to be one at compile-time.

    Used for example as the density argument to a function written for
    compressible to be used for incompressible flow.

\*---------------------------------------------------------------------------*/

#ifndef oneField_H
#define oneField_H

#include "one.hpp"
#include "scalar.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class oneField Declaration
\*---------------------------------------------------------------------------*/

class oneField
:
    public one
{

public:

    // Public typedefs

        typedef oneField FieldType;


    // Constructors

        //- Construct null
        oneField()
        {}


    // Member Operators

        inline one operator[](const label) const;

        inline oneField field() const;
};


inline const oneField& operator*(const oneField&, const oneField&);

inline const oneField& operator/(const oneField&, const oneField&);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#   include "oneFieldI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
