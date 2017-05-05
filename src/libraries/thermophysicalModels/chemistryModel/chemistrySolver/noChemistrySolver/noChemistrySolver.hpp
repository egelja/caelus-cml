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
    CML::noChemistrySolver

Description
    Dummy chemistry solver for 'none' option


\*---------------------------------------------------------------------------*/

#ifndef noChemistySolver_H
#define noChemistySolver_H

#include "chemistrySolver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class noChemistrySolver Declaration
\*---------------------------------------------------------------------------*/

template<class ODEChemistryType>
class noChemistrySolver
:
    public chemistrySolver<ODEChemistryType>
{

public:

    //- Runtime type information
    TypeName("none");


    // Constructors


        //- Construct from components
        noChemistrySolver
        (
            const fvMesh& mesh,
            const word& ODEModelName,
            const word& thermoType
        );


    //- Destructor
    virtual ~noChemistrySolver();


    // Member Functions

        //- Update the concentrations and return the chemical time
        virtual scalar solve
        (
            scalarField &c,
            const scalar T,
            const scalar p,
            const scalar t0,
            const scalar dt
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ODEChemistryType>
CML::noChemistrySolver<ODEChemistryType>::noChemistrySolver
(
    const fvMesh& mesh,
    const word& ODEModelName,
    const word& thermoType
)
:
    chemistrySolver<ODEChemistryType>(mesh, ODEModelName, thermoType)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ODEChemistryType>
CML::noChemistrySolver<ODEChemistryType>::~noChemistrySolver()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ODEChemistryType>
CML::scalar CML::noChemistrySolver<ODEChemistryType>::solve
(
    scalarField&,
    const scalar,
    const scalar,
    const scalar,
    const scalar
) const
{
    return GREAT;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
