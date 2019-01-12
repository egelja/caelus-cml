/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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
    CML::surfZoneIOList

Description
    IOobject for a surfZoneList

SourceFiles
    surfZoneIOList.cpp

\*---------------------------------------------------------------------------*/

#ifndef surfZoneIOList_H
#define surfZoneIOList_H

#include "surfZoneList.hpp"
#include "regIOobject.hpp"
#include "faceList.hpp"
#include "className.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class surfZoneIOList;

Ostream& operator<<(Ostream&, const surfZoneIOList&);


/*---------------------------------------------------------------------------*\
                      Class surfZoneIOList Declaration
\*---------------------------------------------------------------------------*/

class surfZoneIOList
:
    public surfZoneList,
    public regIOobject
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        surfZoneIOList(const surfZoneIOList&);

        //- Disallow default bitwise assignment
        void operator=(const surfZoneIOList&);


public:

    //- Runtime type information
    TypeName("surfZoneList");


    // Constructors

        //- Construct from IOobject
        explicit surfZoneIOList(const IOobject&);

        //- Construct from IOobject and surfZoneList
        surfZoneIOList(const IOobject&, const surfZoneList&);

        //- Construct from IOobject and surfZoneList
        surfZoneIOList(const IOobject&, const Xfer<surfZoneList>&);


    //- Destructor
    ~surfZoneIOList();


    // Member Functions

        //- writeData member function required by regIOobject
        bool writeData(Ostream&) const;


    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const surfZoneIOList&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
