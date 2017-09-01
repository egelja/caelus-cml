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
    CML::interpolateVelocityProbes

Description
    Reads velocity probes and writes each of them into an Caelus-formatted
    data file.

    The data is interpolated onto an equidistant time axis.

SourceFiles
    interpolateVelocityProbes.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef interpolateVelocityProbes_HPP
#define interpolateVelocityProbes_HPP

#include "postProcessingWaves.hpp"
#include "rawVelocityProbes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class interpolateVelocityProbes
:
    public postProcessingWaves
{

protected:

    // Protected data

    // Protected member functions

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        interpolateVelocityProbes(const interpolateVelocityProbes&);

        //- Disallow default bitwise assignment
        void operator=(const interpolateVelocityProbes&);

    // Private member data

public:

    //- Runtime type information
    TypeName("interpolateVelocityProbes");

    // Constructors

        //- Construct from components
        interpolateVelocityProbes
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~interpolateVelocityProbes();

    // Member Functions
        virtual void evaluate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
