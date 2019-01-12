/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
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
    CML::radiation::noScatter

Description
    Dummy scatter model for 'none'

SourceFiles
    noScatter.cpp

\*---------------------------------------------------------------------------*/

#ifndef noScatter_HPP
#define noScatter_HPP

#include "scatterModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace radiation
{

/*---------------------------------------------------------------------------*\
                          Class noScatter Declaration
\*---------------------------------------------------------------------------*/

class noScatter
:
    public scatterModel
{

public:

    //- Runtime type information
    TypeName("none");


    //- Construct from components
    noScatter(const dictionary& dict, const fvMesh& mesh);


    //- Destructor
    virtual ~noScatter()
    {}


    // Member Functions

    //- Return scatter coefficient
    tmp<volScalarField> sigmaEff() const;

};


} // End namespace radiation
} // End namespace CML


#endif
