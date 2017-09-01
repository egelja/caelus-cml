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
    CML::interpolateSurfaceElevation

Description
    Reads probed alpha1-values and writes data for each probe in an OF-
    formatted manner.

    The data is interpolated onto an equidistant time axis.

SourceFiles
    interpolateAlphaProbes.cpp

Author
    Bjarne Jensen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef interpolateAlphaProbes_HPP
#define interpolateAlphaProbes_HPP

#include "postProcessingWaves.hpp"
#include "rawAlphaProbes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class interpolateAlphaProbes
:
    public postProcessingWaves
{

protected:

    // Protected data

    // Protected member functions

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        interpolateAlphaProbes(const interpolateAlphaProbes&);

        //- Disallow default bitwise assignment
        void operator=(const interpolateAlphaProbes&);

    // Private member data

public:

    //- Runtime type information
    TypeName("interpolateAlphaProbes");

    // Constructors

        //- Construct from components
        interpolateAlphaProbes
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~interpolateAlphaProbes();

    // Member Functions
        virtual void evaluate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
