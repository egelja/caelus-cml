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

Namespace
    CML::laminarFlameSpeedModels

Description
    Namespace for laminar flame speed models


Class
    CML::laminarFlameSpeed

Description
    Abstract class for laminar flame speed

SourceFiles
    laminarFlameSpeed.cpp
    laminarFlameSpeedNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef laminarFlameSpeed_HPP
#define laminarFlameSpeed_HPP

#include "psiuReactionThermo.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class laminarFlameSpeed Declaration
\*---------------------------------------------------------------------------*/

class laminarFlameSpeed
{

protected:

    const psiuReactionThermo& psiuReactionThermo_;

    //- Fuel
    word fuel_;

    //- Equivalence ratio of a homogeneous mixture
    scalar equivalenceRatio_;


private:

    //- Construct as copy (not implemented)
    laminarFlameSpeed(const laminarFlameSpeed&);
    void operator=(const laminarFlameSpeed&);


public:

    //- Runtime type information
    TypeName("laminarFlameSpeed");


    // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        laminarFlameSpeed,
        dictionary,
        (
            const dictionary& dict,
            const psiuReactionThermo& ct
        ),
        (dict, ct)
    );


    //- Construct from dictionary and psiuReactionThermo
    laminarFlameSpeed
    (
        const dictionary&,
        const psiuReactionThermo&
    );


    // Selector
    static autoPtr<laminarFlameSpeed> New
    (
        const psiuReactionThermo&
    );


    //- Destructor
    virtual ~laminarFlameSpeed()
    {}


    // Member functions

    //- Return the laminar flame speed [m/s]
    virtual tmp<volScalarField> operator()() const = 0;

};


} // End namespace CML


#endif
