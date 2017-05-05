/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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
    CML::Random

Description
    Simple random number generator.

SourceFiles
    Random.cpp

\*---------------------------------------------------------------------------*/

#ifndef Random_H
#define Random_H

#include "vector.hpp"
#include "tensor.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class Random Declaration
\*---------------------------------------------------------------------------*/

class Random
{
    // Private data

        label Seed;


public:


    // Constructors

        //- Construct given seed
        Random(const label);


    // Member functions

        int bit();

        //- scalar [0..1] (so including 0,1)
        scalar scalar01();

        //- vector with every component scalar01
        vector vector01();

        //- sphericalTensor with every component scalar01
        sphericalTensor sphericalTensor01();

        //- symmTensor with every component scalar01
        symmTensor symmTensor01();

        //- tensor with every component scalar01
        tensor tensor01();

        //- label [lower..upper]
        label integer(const label lower, const label upper);

        vector position(const vector&, const vector&);

        void randomise(scalar&);
        void randomise(vector&);
        void randomise(sphericalTensor&);
        void randomise(symmTensor&);
        void randomise(tensor&);

        //- Return a normal Gaussian randon number
        //  with zero mean and unity variance N(0, 1)
        scalar GaussNormal();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
