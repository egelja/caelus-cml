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
    CML::chemistrySolver

Description
    An abstract base class for solving chemistry


\*---------------------------------------------------------------------------*/

#ifndef chemistrySolver_HPP
#define chemistrySolver_HPP

#include "IOdictionary.hpp"
#include "scalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class chemistrySolver Declaration
\*---------------------------------------------------------------------------*/

template<class ChemistryModel>
class chemistrySolver
:
    public ChemistryModel
{

public:

    //- Construct from thermo
    chemistrySolver(typename ChemistryModel::reactionThermo& thermo);


    //- Destructor
    virtual ~chemistrySolver()
    {}


    // Member Functions

    //- Update the concentrations and return the chemical time
    virtual void solve
    (
        scalarField &c,
        scalar& T,
        scalar& p,
        scalar& deltaT,
        scalar& subDeltaT
    ) const = 0;

};


} // End namespace CML



// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ChemistryModel>
CML::chemistrySolver<ChemistryModel>::chemistrySolver
(
    typename ChemistryModel::reactionThermo& thermo
)
:
    ChemistryModel(thermo)
{}


#endif
