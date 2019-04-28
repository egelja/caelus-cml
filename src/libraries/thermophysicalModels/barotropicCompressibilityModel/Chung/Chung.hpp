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
    CML::compressibilityModels::Chung

Description
    Chung compressibility model.

SourceFiles
    Chung.cpp

\*---------------------------------------------------------------------------*/

#ifndef Chung_HPP
#define Chung_HPP

#include "barotropicCompressibilityModel.hpp"
#include "dimensionedScalar.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressibilityModels
{

/*---------------------------------------------------------------------------*\
                           Class Chung Declaration
\*---------------------------------------------------------------------------*/

class Chung
:
    public barotropicCompressibilityModel
{

    dimensionedScalar psiv_;
    dimensionedScalar psil_;

    dimensionedScalar rhovSat_;
    dimensionedScalar rholSat_;


public:

    //- Runtime type information
    TypeName("Chung");


    //- Construct from components
    Chung
    (
        const dictionary& compressibilityProperties,
        const volScalarField& gamma,
        const word& psiName = "psi"
    );


    //- Destructor
    ~Chung()
    {}


    // Member Functions

    //- Correct the Chung compressibility
    void correct();

    //- Read transportProperties dictionary
    bool read(const dictionary& compressibilityProperties);

};


} // End namespace compressibilityModels
} // End namespace CML


#endif
