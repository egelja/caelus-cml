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
    CML::waveGauges

Description
    Writing wave gauges depending on a given point distribution.

SourceFiles
    waveGauges.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef waveGauges_HPP
#define waveGauges_HPP

#include "fvMesh.hpp"
#include "fvCFD.hpp"
#include "volFields.hpp"
#include "IOdictionary.hpp"
#include "OFstream.hpp"
#include "uniformDimensionedFields.hpp"

#include "pointDistributions.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class porosityCoefficient Declaration
\*---------------------------------------------------------------------------*/

class waveGauges
{
private:

    // Private data
        const fvMesh& mesh_;
        const dictionary& gaugeDict_;

    // Private Member Functions
        void writeVTKFormat
        (
            const word&,
            const pointField&,
            const point&
        );

        //- Disallow default bitwise copy construct
        waveGauges(const waveGauges&);

        //- Disallow default bitwise assignment
        void operator=(const waveGauges&);

public:

    //- Runtime type information
    TypeName("waveGauges");

    // Constructors

        //- Construct from components
        waveGauges
        (
            const fvMesh&,
            const dictionary&
        );

    // Destructor

        virtual ~waveGauges() {};

    // Member Functions

        // Access
        void evaluate( const word& );
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
