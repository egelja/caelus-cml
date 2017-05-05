/*---------------------------------------------------------------------------*\
Copyright 2009 TU Delft
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Class
    W2

Description
    W2 radial basis function

Author
    Frank Bos, TU Delft.

SourceFiles
    W2.cpp

\*---------------------------------------------------------------------------*/

#ifndef W2_HPP
#define W2_HPP

#include "RBFFunction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                              Class W2 Declaration
\*---------------------------------------------------------------------------*/

class W2
:
    public RBFFunction
{
    // Private data

        //- Radius
        scalar radius_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        W2(const W2&);

        //- Disallow default bitwise assignment
        void operator=(const W2&);


public:

    //- Runtime type information
    TypeName("W2");

    // Constructors

        //- Construct given radius
        W2(const scalar radius);

        //- Construct from dictionary
        W2(const dictionary& dict);

        //- Create and return a clone
        virtual autoPtr<RBFFunction> clone() const
        {
            return autoPtr<RBFFunction>(new W2(this->radius_));
        }


    // Destructor

        virtual ~W2();


    // Member Functions

        //- Return weights given points
        virtual tmp<scalarField> weights
        (
            const vectorField& points,
            const vector& controlPoint
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
