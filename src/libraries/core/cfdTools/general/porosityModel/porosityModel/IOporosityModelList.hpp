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
    CML::IOporosityModelList

Description
    List of porosity models with IO functionality

SourceFiles
    IOporosityModelList.cpp

\*---------------------------------------------------------------------------*/

#ifndef IOporosityModelList_H
#define IOporosityModelList_H

#include "IOdictionary.hpp"
#include "porosityModelList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class IOporosityModelList Declaration
\*---------------------------------------------------------------------------*/

class IOporosityModelList
:
    public IOdictionary,
    public porosityModelList
{
private:

    // Private Member Functions

        //- Create IO object if dictionary is present
        IOobject createIOobject(const fvMesh& mesh) const;

        //- Disallow default bitwise copy construct
        IOporosityModelList(const IOporosityModelList&);

        //- Disallow default bitwise assignment
        void operator=(const IOporosityModelList&);


public:

    // Constructors

        //- Construct from mesh
        IOporosityModelList(const fvMesh& mesh);


        //- Destructor
        virtual ~IOporosityModelList()
        {}


    // Member Functions

        //- Read dictionary
        virtual bool read();
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
