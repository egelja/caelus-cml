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

Class
    CML::simpleRegIOobject

Description
    Abstract base class for registered object with I/O. Used in debug symbol
    registration.

SourceFiles

\*---------------------------------------------------------------------------*/

#ifndef simpleRegIOobject_HPP
#define simpleRegIOobject_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                         Class simpleRegIOobject Declaration
\*---------------------------------------------------------------------------*/

class simpleRegIOobject
{
public:

    // Constructors

        //- Construct from objectregistry inserter and name
        simpleRegIOobject
        (
            void (*fn)(const char* name, simpleRegIOobject*),
            const char* name
        )
        {
            (*fn)(name, this);
        }


    //- Destructor
    virtual ~simpleRegIOobject()
    {};


    // Member Functions

        //- Read
        virtual void readData(Istream&) = 0;

        //- Write
        virtual void writeData(Ostream&) const = 0;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
