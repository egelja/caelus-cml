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
    CML::basicMixture

Description
    CML::basicMixture

SourceFiles
    basicMixture.cpp

\*---------------------------------------------------------------------------*/

#ifndef basicMixture_HPP
#define basicMixture_HPP

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;
class dictionary;

/*---------------------------------------------------------------------------*\
                        Class basicMixture Declaration
\*---------------------------------------------------------------------------*/

class basicMixture
{

public:

    //- The base class of the mixture
    typedef basicMixture basicMixtureType;


    //- Construct from dictionary, mesh and phase name
    basicMixture(const dictionary&, const fvMesh&, const word&)
    {}

};


} // End namespace CML


#endif
