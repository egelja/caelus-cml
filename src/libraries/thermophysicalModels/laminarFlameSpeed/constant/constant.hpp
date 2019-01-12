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
    CML::laminarFlameSpeedModels::constant

Description
    Constant laminar flame speed model.

SourceFiles
    selectLaminarFlameSpeed.cpp

\*---------------------------------------------------------------------------*/

#ifndef constant_HPP
#define constant_HPP

#include "laminarFlameSpeed.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace laminarFlameSpeedModels
{

/*---------------------------------------------------------------------------*\
                           Class constant Declaration
\*---------------------------------------------------------------------------*/

class constant
:
    public laminarFlameSpeed
{

private:

    // Const laminar flame speed
    dimensionedScalar Su_;


    //- Construct as copy (not implemented)
    constant(const constant&);
    void operator=(const constant&);


public:

    //- Runtime type information
    TypeName("constant");


    //- Construct from dictionary and psiuReactionThermo
    constant
    (
        const dictionary&,
        const psiuReactionThermo&
    );


    //- Destructor
    virtual ~constant()
    {}


    // Member functions

    //- Return the laminar flame speed [m/s]
    tmp<volScalarField> operator()() const;

};


} // End laminarFlameSpeedModels
} // End namespace CML


#endif
