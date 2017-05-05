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
    CML::sequential

Description
    CML::sequential


\*---------------------------------------------------------------------------*/

#ifndef sequential_H
#define sequential_H

#include "chemistrySolver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class sequential Declaration
\*---------------------------------------------------------------------------*/

template<class ODEChemistryType>
class sequential
:
    public chemistrySolver<ODEChemistryType>
{
    // Private data

        //- Coefficients dictionary
        dictionary coeffsDict_;


        // Model constants

            //- Chemistry time scale
            scalar cTauChem_;

            //- Equilibrium rate limiter flag (on/off)
            Switch eqRateLimiter_;


public:

    //- Runtime type information
    TypeName("sequential");


    // Constructors

        //- Construct from components
        sequential
        (
            const fvMesh& mesh,
            const word& ODEModelName,
            const word& thermoType
        );


    //- Destructor
    virtual ~sequential();


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

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ODEChemistryType>
CML::sequential<ODEChemistryType>::sequential
(
    const fvMesh& mesh,
    const word& ODEModelName,
    const word& thermoType
)
:
    chemistrySolver<ODEChemistryType>(mesh, ODEModelName, thermoType),
    coeffsDict_(this->subDict("sequentialCoeffs")),
    cTauChem_(readScalar(coeffsDict_.lookup("cTauChem"))),
    eqRateLimiter_(coeffsDict_.lookup("equilibriumRateLimiter"))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ODEChemistryType>
CML::sequential<ODEChemistryType>::~sequential()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ODEChemistryType>
CML::scalar CML::sequential<ODEChemistryType>::solve
(
    scalarField &c,
    const scalar T,
    const scalar p,
    const scalar t0,
    const scalar dt
) const
{
    scalar tChemInv = SMALL;

    scalar pf, cf, pb, cb;
    label lRef, rRef;

    forAll(this->reactions(), i)
    {
        scalar omega = this->omegaI(i, c, T, p, pf, cf, lRef, pb, cb, rRef);

        if (eqRateLimiter_)
        {
            if (omega < 0.0)
            {
                omega /= 1.0 + pb*dt;
            }
            else
            {
                omega /= 1.0 + pf*dt;
            }
        }

        tChemInv = max(tChemInv, mag(omega));

        this->updateConcsInReactionI(i, dt, omega, c);
    }

    return cTauChem_/tChemInv;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
