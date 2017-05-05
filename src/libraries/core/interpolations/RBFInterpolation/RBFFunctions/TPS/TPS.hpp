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
    TPS

Description
    TPS radial basis function

Author
    Frank Bos, TU Delft.

SourceFiles
    TPS.cpp

\*---------------------------------------------------------------------------*/

#ifndef TPS_HPP
#define TPS_HPP

#include "RBFFunction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                              Class TPS Declaration
\*---------------------------------------------------------------------------*/

class TPS
:
    public RBFFunction
{
    // Private data

        //- Radius
        scalar radius_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        TPS(const TPS&);

        //- Disallow default bitwise assignment
        void operator=(const TPS&);


public:

    //- Runtime type information
    TypeName("TPS");

    // Constructors

        //- Construct given radius
        TPS(const scalar radius);

        //- Construct from dictionary
        TPS(const dictionary& dict);

        virtual autoPtr<RBFFunction> clone() const
        {
            return autoPtr<RBFFunction>(new TPS(this->radius_));
        }


    // Destructor

        virtual ~TPS();


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
