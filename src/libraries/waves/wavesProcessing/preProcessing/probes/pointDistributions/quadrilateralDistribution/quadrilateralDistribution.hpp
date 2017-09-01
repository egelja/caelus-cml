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
    CML::quadrilateralDistribution

Description
    A quadrilateral point distribution with stretching along both of the sides
    of the quadrilateral.

SourceFiles
    quadrilateralDistribution.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef quadrilateralDistribution_HPP
#define quadrilateralDistribution_HPP

#include "pointDistributions.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class quadrilateralDistribution
:
    public pointDistributions
{

protected:

    // Protected data

    // Protected member functions

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        quadrilateralDistribution(const quadrilateralDistribution&);

        //- Disallow default bitwise assignment
        void operator=(const quadrilateralDistribution&);

    // Private member data

public:

    //- Runtime type information
    TypeName("quadrilateralDistribution");

    // Constructors

        //- Construct from components
        quadrilateralDistribution
        (
            const fvMesh&,
            const dictionary&
        );


    // Destructor

        virtual ~quadrilateralDistribution();

    // Member Functions
        virtual pointField evaluate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
