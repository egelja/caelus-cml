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
    CML::surfacePatchIOList

Description
    IOobject for a surfacePatchList

SourceFiles
    surfacePatchIOList.cpp

\*---------------------------------------------------------------------------*/

#ifndef surfacePatchIOList_H
#define surfacePatchIOList_H

#include "surfacePatchList.hpp"
#include "regIOobject.hpp"
#include "faceList.hpp"
#include "className.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class surfacePatchIOList;

Ostream& operator<<(Ostream&, const surfacePatchIOList&);


/*---------------------------------------------------------------------------*\
                           Class surfacePatchIOList Declaration
\*---------------------------------------------------------------------------*/

class surfacePatchIOList
:
    public surfacePatchList,
    public regIOobject
{
    // Private data


    // Private Member Functions

        //- Disallow default bitwise copy construct
        surfacePatchIOList(const surfacePatchIOList&);

        //- Disallow default bitwise assignment
        void operator=(const surfacePatchIOList&);


public:

    //- Runtime type information
    TypeName("surfacePatchIOList");


    // Static data members

        //- Static data someStaticData

    // Constructors

        //- Construct from IOobject
        explicit surfacePatchIOList(const IOobject& io);

        //- Construct from IOobject
        surfacePatchIOList(const IOobject& io, const surfacePatchList&);

    //- Destructor
    ~surfacePatchIOList();


    // Member Functions

        //- writeData member function required by regIOobject
        bool writeData(Ostream&) const;


    // Member Operators

    // Friend Functions

    // Friend Operators

    // IOstream Operators

        friend Ostream& operator<<(Ostream&, const surfacePatchIOList&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
