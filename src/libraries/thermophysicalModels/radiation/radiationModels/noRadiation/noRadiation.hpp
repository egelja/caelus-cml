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
    CML::radiation::noRadiation

Description
    No radiation - does nothing to energy equation source terms
    (returns zeros)

SourceFiles
    noRadiation.cpp

\*---------------------------------------------------------------------------*/

#ifndef noRadiation_HPP
#define noRadiation_HPP

#include "radiationModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace radiation
{

/*---------------------------------------------------------------------------*\
                         Class noRadiation Declaration
\*---------------------------------------------------------------------------*/

class noRadiation
:
    public radiationModel
{

    //- Disallow default bitwise copy construct
    noRadiation(const noRadiation&);

    //- Disallow default bitwise assignment
    void operator=(const noRadiation&);


public:

    //- Runtime type information
    TypeName("none");


    //- Construct from components
    noRadiation(const volScalarField& T);

    //- Construct from components
    noRadiation(const dictionary& dict, const volScalarField& T);


    //- Destructor
    virtual ~noRadiation()
    {}


    // Member functions

    // Edit

    //- Main update/correction routine
    void correct();

    //- Solve radiation equation(s)
    void calculate();

    //- Read radiationProperties dictionary
    bool read();

    //- Source term component (for power of T^4)
    tmp<volScalarField> Rp() const;

    //- Source term component (constant)
    tmp<DimensionedField<scalar, volMesh> > Ru() const;

};


} // End namespace radiation
} // End namespace CML


#endif
