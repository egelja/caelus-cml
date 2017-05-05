/*---------------------------------------------------------------------------*\
Copyright 2009 TU Delft
Copyright 2011 Ecole Polytechnique Fédérale de Lausanne 
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
    Gauss

Description
    Gauss radial basis function

Author
    Frank Bos, TU Delft.
    Matteo Lombardi, Ecole Polytechnique Fédérale de Lausanne.

SourceFiles
    Gauss.cpp

\*---------------------------------------------------------------------------*/

#ifndef Gauss_HPP
#define Gauss_HPP

#include "RBFFunction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                              Class Gauss Declaration
\*---------------------------------------------------------------------------*/

class Gauss
:
    public RBFFunction
{
    // Private data

        //- Radius
        scalar radius_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        Gauss(const Gauss&);

        //- Disallow default bitwise assignment
        void operator=(const Gauss&);


public:

    //- Runtime type information
    TypeName("Gauss");

    // Constructors

        //- Construct given radius
        Gauss(const scalar radius);

        //- Construct from dictionary
        Gauss(const dictionary& dict);

        //- Create and return a clone
        virtual autoPtr<RBFFunction> clone() const
        {
            return autoPtr<RBFFunction>(new Gauss(this->radius_));
        }


    // Destructor

        virtual ~Gauss();


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
