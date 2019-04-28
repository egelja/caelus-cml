/*---------------------------------------------------------------------------*\
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
    CML::compressibilityModels::linear

Description
    linear compressibility model.

SourceFiles
    linearCompressibility.cpp

\*---------------------------------------------------------------------------*/

#ifndef linearCompressibility_HPP
#define linearCompressibility_HPP

#include "barotropicCompressibilityModel.hpp"
#include "dimensionedScalar.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressibilityModels
{

/*---------------------------------------------------------------------------*\
                           Class linear Declaration
\*---------------------------------------------------------------------------*/

class linear
:
    public barotropicCompressibilityModel
{

    dimensionedScalar psiv_;
    dimensionedScalar psil_;


public:

    //- Runtime type information
    TypeName("linear");


    //- Construct from components
    linear
    (
        const dictionary& compressibilityProperties,
        const volScalarField& gamma,
        const word& psiName = "psi"
    );


    //- Destructor
    ~linear()
    {}


    // Member Functions

    //- Correct the linear compressibility
    void correct();

    //- Read transportProperties dictionary
    bool read(const dictionary& compressibilityProperties);

};


} // End namespace compressibilityModels
} // End namespace CML


#endif
