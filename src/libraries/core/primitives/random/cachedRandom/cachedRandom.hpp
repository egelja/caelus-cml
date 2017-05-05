/*---------------------------------------------------------------------------*\
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
    CML::cachedRandom

Description
    Random number generator.

    Pre-computes and caches samples on construction, so that when sample01()
    is called, the function simply returns the next (pre-computed) sample. On
    reaching the last sample, the sample sequence is repeated.

    Constructed using a seed and sample count. If the supplied count is
    negative, no caching is performed, and a new sample is generated on each
    call to sample01().

    Note: the copy constructor cannot be used if count = -1.

SourceFiles
    cachedRandomI.hpp
    cachedRandom.cpp
    cachedRandomTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef cachedRandom_H
#define cachedRandom_H

#include "scalarList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class cachedRandom;

/*---------------------------------------------------------------------------*\
                       Class cachedRandom Declaration
\*---------------------------------------------------------------------------*/

class cachedRandom
{
private:

    // Private data

        //- Initial random number seed
        label seed_;

        //- List of scalar samples
        scalarList samples_;

        //- Current sample marker
        label sampleI_;


    // Private Member Functions

        //- Returns the current sample
        scalar scalar01();


public:


    // Constructors

        //- Construct given seed and sample count
        cachedRandom(const label seed, const label count);

        //- Copy constructor with optional reset of sampleI
        cachedRandom(const cachedRandom& cr, const bool reset = false);


    // Destructor
    ~cachedRandom();


    // Member functions

        // Access

            //- Return const access to the initial random number seed
            inline label seed() const;

            //- Return const access to the list of samples
            inline const scalarList& samples() const;

            //- Return the current sample marker
            inline label sampleI() const;


        // Manipulation

            //- Return non-const access to the sample marker
            inline label& sampleI();


        // Evaluation

            //- Return a sample whose components lie in the range 0-1
            template<class Type>
            Type sample01();

            //- Return a sample between start and end
            template<class Type>
            Type position(const Type& start, const Type& end);

            //- Randomise value in the range 0-1
            template<class Type>
            void randomise01(Type& value);


        // Operators

            //- Assignment operator
            void operator=(const cachedRandom& cr);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Template specialisations

template<>
label cachedRandom::sample01<label>();

template<>
scalar cachedRandom::sample01<scalar>();

template<>
label cachedRandom::position<label>(const label& start, const label& end);

template<>
scalar cachedRandom::position<scalar>
(
    const scalar& start,
    const scalar& end
);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline CML::label CML::cachedRandom::seed() const
{
    return seed_;
}


inline const CML::scalarList& CML::cachedRandom::samples() const
{
    return samples_;
}


inline CML::label CML::cachedRandom::sampleI() const
{
    return sampleI_;
}


inline CML::label& CML::cachedRandom::sampleI()
{
    return sampleI_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Type CML::cachedRandom::sample01()
{
    Type value;
    for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; cmpt++)
    {
        value.component(cmpt) = scalar01();
    }

    return value;
}


template<class Type>
Type CML::cachedRandom::position(const Type& start, const Type& end)
{
    Type value(start);
    for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; cmpt++)
    {
        value.component(cmpt) +=
            scalar01()*(end.component(cmpt) - start.component(cmpt));
    }

    return value;
}


template<class Type>
void CML::cachedRandom::randomise01(Type& value)
{
    value = sample01<Type>();
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
