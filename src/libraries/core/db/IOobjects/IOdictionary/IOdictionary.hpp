/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::IOdictionary

Description
    IOdictionary is derived from dictionary and IOobject to give the dictionary
    automatic IO functionality via the objectRegistry.  To facilitate IO,
    IOdictioanry is provided with a constructor from IOobject and writeData and
    write functions.

SourceFiles
    IOdictionary.cpp
    IOdictionaryIO.cpp

\*---------------------------------------------------------------------------*/

#ifndef IOdictionary_H
#define IOdictionary_H

#include "dictionary.hpp"
#include "regIOobject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class IOdictionary Declaration
\*---------------------------------------------------------------------------*/

class IOdictionary
:
    public regIOobject,
    public dictionary
{
    // Private data

        static bool writeDictionaries;


    // Private Member Functions

        //- Read dictionary from file
        void readFile(const bool);


public:

    TypeName("dictionary");


    // Constructors

        //- Construct given an IOobject
        IOdictionary(const IOobject&);

        //- Construct given an IOobject and dictionary
        IOdictionary(const IOobject&, const dictionary&);

        //- Construct given an IOobject and Istream
        IOdictionary(const IOobject&, Istream&);


    //- Destructor
    virtual ~IOdictionary();


    // Member functions

        //- Name function is needed to disambiguate those inherited
        //  from regIOobject and dictionary
        const word& name() const;

        //- ReadData function required for regIOobject read operation
        bool readData(Istream&);

        //- WriteData function required for regIOobject write operation
        bool writeData(Ostream&) const;


    // Member operators

        //- Assignment of other IOdictionary's entries to this IOdictionary
        void operator=(const IOdictionary&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
