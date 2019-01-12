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
    CML::radiation::constantScatter

Description
    Constant radiation scatter coefficient

SourceFiles
    scatterModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef radiationConstantScatter_HPP
#define radiationConstantScatter_HPP

#include "scatterModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace radiation
{

/*---------------------------------------------------------------------------*\
                       Class constantScatter Declaration
\*---------------------------------------------------------------------------*/

class constantScatter
:
    public scatterModel
{

    //- Coefficients dictionary
    dictionary coeffsDict_;

    //- Scattering coefficient / [1/m]
    dimensionedScalar sigma_;

    //- Linear-anisotropic phase function coefficient / []
    //  -1 < C < 1
    //  - = backward scattering
    //  0 = isotropic scattering (reasonable default value)
    //  + = forward scattering
    dimensionedScalar C_;


public:

    //- Runtime type information
    TypeName("constantScatter");


    //- Construct from components
    constantScatter(const dictionary& dict, const fvMesh& mesh);


    //- Destructor
    virtual ~constantScatter()
    {}


    // Member Functions

    //- Return scatter coefficient
    tmp<volScalarField> sigmaEff() const;

};


} // End namespace radiation
} // End namespace CML


#endif
