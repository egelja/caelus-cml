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
    CML::radiation::noSoot

Description
    noSoot


SourceFiles
    noSoot.cpp

\*---------------------------------------------------------------------------*/

#ifndef noSoot_HPP
#define noSoot_HPP


#include "sootModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace radiation
{

/*---------------------------------------------------------------------------*\
                    Class noSoot Declaration
\*---------------------------------------------------------------------------*/
class noSoot
:
    public sootModel
{

public:

    //- Runtime type information
    TypeName("none");


    //- Construct from components
    noSoot(const dictionary& dict, const fvMesh& mesh, const word&);


    //- Destructor
    virtual ~noSoot()
    {}


    // Member Functions

    //- Main update/correction routine
    void correct();

    //- Return Ysoot
    const volScalarField& soot() const;

};


} // End namespace CML
} // End namespace radiation


#endif
