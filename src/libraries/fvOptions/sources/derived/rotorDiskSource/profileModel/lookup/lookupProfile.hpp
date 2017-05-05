/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::lookupProfile

Description
    Look-up based profile data - drag and lift coefficients are lineraly
    interpolated based on the supplied angle of attack

    Input in list format:

        data
        (
            (AOA1 Cd1 Cl2)
            (AOA2 Cd2 Cl2)
            ...
            (AOAN CdN CdN)
        );

    where:
        AOA = angle of attack [deg] converted to [rad] internally
        Cd  = drag coefficient
        Cl  = lift coefficient

SourceFiles
    lookupProfile.cpp

\*---------------------------------------------------------------------------*/

#ifndef lookupProfile_HPP
#define lookupProfile_HPP

#include "profileModel.hpp"
#include "List.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class lookupProfile Declaration
\*---------------------------------------------------------------------------*/

class lookupProfile
:
    public profileModel
{

protected:

    // Protected data

        //- List of angle-of-attack values [deg] on input, converted to [rad]
        List<scalar> AOA_;

        //- List of drag coefficient values
        List<scalar> Cd_;

        //- List of lift coefficient values
        List<scalar> Cl_;


    // Protected Member Functions

        //- Return the interpolation indices and gradient
        void interpolateWeights
        (
            const scalar& xIn,
            const List<scalar>& values,
            label& i1,
            label& i2,
            scalar& ddx
        ) const;


public:

    //- Runtime type information
    TypeName("lookup");

    //- Constructor
    lookupProfile(const dictionary& dict, const word& modelName);


    // Member functions

        // Evaluation

            //- Return the Cd and Cl for a given angle-of-attack
            virtual void Cdl(const scalar alpha, scalar& Cd, scalar& Cl) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
