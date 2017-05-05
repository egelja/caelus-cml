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

Class
    CML::nil

Description
    A zero-sized class without any storage. Used, for example, in HashSet.

Note
    A zero-sized class actually does still require at least 1 byte storage.

\*---------------------------------------------------------------------------*/

#ifndef nil_H
#define nil_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class Istream;
class Ostream;

// Forward declaration of friend functions and operators

class nil;

Istream& operator>>(Istream&, nil&);
Ostream& operator<<(Ostream&, const nil&);


/*---------------------------------------------------------------------------*\
                             Class nil Declaration
\*---------------------------------------------------------------------------*/

class nil
{

public:

    // Constructors

        //- Construct null
        nil()
        {}

        //- Construct from Istream
        nil(Istream&)
        {}


    // IOstream Operators

        friend Istream& operator>>(Istream& is, nil&)
        {
            return is;
        }

        friend Ostream& operator<<(Ostream& os, const nil&)
        {
            return os;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
