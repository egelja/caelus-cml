/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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
    CML::interpolateForcesAndMoments

Description
    Reads a given force-stream and returns the contribution from the pressure
    induced forces and moments. The viscous part is currently omitted.

    The data is interpolated onto an equidistant time axis.

    There is an assumption of one-patch per force file only.

SourceFiles
    interpolateForcesAndMoments.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef interpolateForcesAndMoments_HPP
#define interpolateForcesAndMoments_HPP

#include "postProcessingWaves.hpp"
#include "rawForcesAndMoments.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class interpolateForcesAndMoments
:
    public postProcessingWaves
{

protected:

    // Protected data

    // Protected member functions

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        interpolateForcesAndMoments(const interpolateForcesAndMoments&);

        //- Disallow default bitwise assignment
        void operator=(const interpolateForcesAndMoments&);

    // Private member data

public:

    //- Runtime type information
    TypeName("interpolateForcesAndMoments");

    // Constructors

        //- Construct from components
        interpolateForcesAndMoments
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~interpolateForcesAndMoments();

    // Member Functions
        virtual void evaluate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
