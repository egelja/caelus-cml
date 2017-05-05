/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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
#include "Pstream.hpp"

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
    // Private data

        //- Initial random number seed
        label seed_;

        //- List of scalar samples
        scalarList samples_;

        //- Current sample marker
        label sampleI_;

        //- Indicator, which tells if there is a stored gaussian sample
        bool hasGaussSample_;

        //- Stored sample value
        scalar gaussSample_;


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

            // Random numbers

                //- Return a sample whose components lie in the range 0-1
                template<class Type>
                Type sample01();

                //- Return a sample whose components are normally distributed
                //  with zero mean and unity variance N(0, 1)
                template<class Type>
                Type GaussNormal();

                //- Return a sample between start and end
                template<class Type>
                Type position(const Type& start, const Type& end);

                //- Randomise value in the range 0-1
                template<class Type>
                void randomise01(Type& value);


            // Global random numbers - consistent across all processors

                //- Return a sample whose components lie in the range 0-1
                template<class Type>
                Type globalSample01();

                //- Return a sample whose components are normally distributed
                //  with zero mean and unity variance N(0, 1)
                template<class Type>
                Type globalGaussNormal();

                //- Return a sample between start and end
                template<class Type>
                Type globalPosition(const Type& start, const Type& end);

                //- Randomise value in the range 0-1
                template<class Type>
                void globalRandomise01(Type& value);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Template specialisations

template<>
scalar cachedRandom::sample01<scalar>();

template<>
label cachedRandom::sample01<label>();

template<>
scalar cachedRandom::GaussNormal<scalar>();

template<>
label cachedRandom::GaussNormal<label>();

template<>
scalar cachedRandom::position<scalar>
(
    const scalar& start,
    const scalar& end
);

template<>
label cachedRandom::position<label>(const label& start, const label& end);

template<>
scalar cachedRandom::globalSample01<scalar>();

template<>
label cachedRandom::globalSample01<label>();

template<>
scalar cachedRandom::globalGaussNormal<scalar>();

template<>
label cachedRandom::globalGaussNormal<label>();

template<>
scalar cachedRandom::globalPosition<scalar>
(
    const scalar& start,
    const scalar& end
);

template<>
label cachedRandom::globalPosition<label>(const label& start, const label& end);


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
Type CML::cachedRandom::GaussNormal()
{
    Type value;
    for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; cmpt++)
    {
        value.component(cmpt) = GaussNormal<scalar>();
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


template<class Type>
Type CML::cachedRandom::globalSample01()
{
    Type value = -GREAT*pTraits<Type>::one;

    if (Pstream::master())
    {
        value = sample01<Type>();
    }

    reduce(value, maxOp<Type>());

    return value;
}


template<class Type>
Type CML::cachedRandom::globalGaussNormal()
{
    Type value = -GREAT*pTraits<Type>::one;

    if (Pstream::master())
    {
        value = GaussNormal<Type>();
    }

    reduce(value, maxOp<Type>());

    return value;
}


template<class Type>
Type CML::cachedRandom::globalPosition(const Type& start, const Type& end)
{
    Type value = -GREAT*pTraits<Type>::one;

    if (Pstream::master())
    {
        value = position<Type>(start, end);
    }

    reduce(value, maxOp<Type>());

    return value;
}


template<class Type>
void CML::cachedRandom::globalRandomise01(Type& value)
{
    value = -GREAT*pTraits<Type>::one;

    if (Pstream::master())
    {
        value = sample01<Type>();
    }

    reduce(value, maxOp<Type>());
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
