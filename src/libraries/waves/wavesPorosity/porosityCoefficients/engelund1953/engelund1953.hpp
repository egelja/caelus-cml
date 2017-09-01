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
    CML::engelund1953

Description
    Porosity coefficients following the work by Engelund, F. (1953).

SourceFiles
    engelund1953.C

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef engelund1953_HPP
#define engelund1953_HPP

#include "porosityCoefficient.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class engelund1953
:
    public porosityCoefficient
{

protected:

    // Protected data

    // Protected member functions

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        engelund1953(const engelund1953&);

        //- Disallow default bitwise assignment
        void operator=(const engelund1953&);

    // Private member data

public:

    //- Runtime type information
    TypeName("engelund1953");

    // Constructors

        //- Construct from components
        engelund1953
        (
            const dictionary &
        );


    // Destructor

        virtual ~engelund1953();

    // Member Functions

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
