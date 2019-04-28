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
    CML::noChemistrySolver

Description
    Dummy chemistry solver for 'none' option


\*---------------------------------------------------------------------------*/

#ifndef noChemistrySolver_HPP
#define noChemistrySolver_HPP

#include "chemistrySolver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class noChemistrySolver Declaration
\*---------------------------------------------------------------------------*/

template<class ChemistryModel>
class noChemistrySolver
:
    public chemistrySolver<ChemistryModel>
{

public:

    //- Runtime type information
    TypeName("none");


    //- Construct from thermo
    noChemistrySolver(typename ChemistryModel::reactionThermo& thermo);


    //- Destructor
    virtual ~noChemistrySolver()
    {}


    // Member Functions

    //- Update the concentrations and return the chemical time
    virtual void solve
    (
        scalarField& c,
        scalar& T,
        scalar& p,
        scalar& deltaT,
        scalar& subDeltaT
    ) const;

};


} // End namespace CML


#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ChemistryModel>
CML::noChemistrySolver<ChemistryModel>::noChemistrySolver
(
    typename ChemistryModel::reactionThermo& thermo
)
:
    chemistrySolver<ChemistryModel>(thermo)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ChemistryModel>
void CML::noChemistrySolver<ChemistryModel>::solve
(
    scalarField&,
    scalar&,
    scalar&,
    scalar&,
    scalar&
) const
{}

#endif
