/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::fv::IOoptionList

Description
    IOoptionList

SourceFiles
    IOoptionList.cpp

\*---------------------------------------------------------------------------*/


#ifndef IOoptionList_HPP
#define IOoptionList_HPP

#include "fvOptionList.hpp"
#include "IOdictionary.hpp"
#include "autoPtr.hpp"

namespace CML
{
namespace fv
{

class IOoptionList
:
    public IOdictionary,
    public optionList
{
private:

    // Private Member Functions

        //- Create IO object if dictionary is present
        IOobject createIOobject(const fvMesh& mesh) const;

        //- Disallow default bitwise copy construct
        IOoptionList(const IOoptionList&);

        //- Disallow default bitwise assignment
        void operator=(const IOoptionList&);


public:

    // Constructors

        //- Construct from components with list of field names
        IOoptionList(const fvMesh& mesh);


        //- Destructor
        virtual ~IOoptionList()
        {}


    // Member Functions

        //- Read dictionary
        virtual bool read();
};


}
}

#endif


