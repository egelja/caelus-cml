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
    IMQB

Description
    IMQB radial basis function

Author
    Frank Bos, TU Delft.
    Matteo Lombardi, Ecole Polytechnique Fédérale de Lausanne.
 
SourceFiles
    IMQB.cpp

\*---------------------------------------------------------------------------*/

#ifndef IMQB_HPP
#define IMQB_HPP

#include "RBFFunction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                              Class IMQB Declaration
\*---------------------------------------------------------------------------*/

class IMQB
:
    public RBFFunction
{
    // Private data

        //- Radius
        scalar radius_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        IMQB(const IMQB&);

        //- Disallow default bitwise assignment
        void operator=(const IMQB&);


public:

    //- Runtime type information
    TypeName("IMQB");

    // Constructors

        //- Construct given radius
        IMQB(const scalar radius);

        //- Construct from dictionary
        IMQB(const dictionary& dict);

        //- Create and return a clone
        virtual autoPtr<RBFFunction> clone() const
        {
            return autoPtr<RBFFunction>(new IMQB(this->radius_));
        }


    // Destructor

        virtual ~IMQB();


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
