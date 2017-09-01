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
    CML::probeGauges

Description
    Writing in input file for probes based on a given point distribution.

SourceFiles
    probeGauges.cpp

Author
    Niels Gjøl Jacobsen, Deltares.


\*---------------------------------------------------------------------------*/

#ifndef probeGauges_HPP
#define probeGauges_HPP

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

class probeGauges
{
private:

    // Private data
        const fvMesh& mesh_;
        const dictionary& gaugeDict_;

    // Private Member Functions
        void writeVTKFormat
        (
            const word&,
            const pointField&
        );

        //- Disallow default bitwise copy construct
        probeGauges(const probeGauges&);

        //- Disallow default bitwise assignment
        void operator=(const probeGauges&);

public:

    //- Runtime type information
    TypeName("probeGauges");

    // Constructors

        //- Construct from components
        probeGauges
        (
            const fvMesh&,
            const dictionary&
        );

    // Destructor

        virtual ~probeGauges() {};

    // Member Functions

        // Access
        void evaluate(const word&);
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
