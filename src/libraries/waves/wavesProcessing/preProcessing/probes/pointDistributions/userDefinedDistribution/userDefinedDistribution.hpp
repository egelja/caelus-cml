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
    CML::userDefinedDistribution

Description
    The user can define the location of the individual wave gauges. Useful
    for comparison with experimental data sets.

SourceFiles
    userDefinedDistribution.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef userDefinedDistribution_HPP
#define userDefinedDistribution_HPP

#include "pointDistributions.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class userDefinedDistribution
:
    public pointDistributions
{

protected:

    // Protected data

    // Protected member functions

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        userDefinedDistribution(const userDefinedDistribution&);

        //- Disallow default bitwise assignment
        void operator=(const userDefinedDistribution&);

    // Private member data

public:

    //- Runtime type information
    TypeName("userDefinedDistribution");

    // Constructors

        //- Construct from components
        userDefinedDistribution
        (
            const fvMesh&,
            const dictionary&
        );


    // Destructor

        virtual ~userDefinedDistribution();

    // Member Functions
        virtual pointField evaluate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
