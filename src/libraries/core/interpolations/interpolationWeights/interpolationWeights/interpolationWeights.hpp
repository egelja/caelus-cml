/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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
    CML::interpolationWeights

Description
    Abstract base class for interpolating in 1D

SourceFiles
    interpolationWeights.cpp

\*---------------------------------------------------------------------------*/

#ifndef interpolationWeights_H
#define interpolationWeights_H

#include "scalarField.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "pointField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;
class objectRegistry;

/*---------------------------------------------------------------------------*\
                       Class interpolationWeights Declaration
\*---------------------------------------------------------------------------*/

class interpolationWeights
{

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        interpolationWeights(const interpolationWeights&);

        //- Disallow default bitwise assignment
        void operator=(const interpolationWeights&);

protected:

        const scalarField& samples_;

public:

    //- Runtime type information
    TypeName("interpolationWeights");


    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            interpolationWeights,
            word,
            (
                const scalarField& samples
            ),
            (samples)
        );


    // Constructors

        //- Construct from components
        interpolationWeights(const scalarField& samples);


    // Selectors

        //- Return a reference to the selected interpolationWeights
        static autoPtr<interpolationWeights> New
        (
            const word& type,
            const scalarField& samples
        );


    //- Destructor
    virtual ~interpolationWeights();


    // Member Functions

        //- Calculate weights and indices to calculate t from samples.
        //  Returns true if indices changed.
        virtual bool valueWeights
        (
            const scalar t,
            labelList& indices,
            scalarField& weights
        ) const = 0;

        //- Calculate weights and indices to calculate integrand of t1..t2
        //  from samples. Returns true if indices changed.
        virtual bool integrationWeights
        (
            const scalar t1,
            const scalar t2,
            labelList& indices,
            scalarField& weights
        ) const = 0;

        //- Helper: weighted sum
        template<class ListType1, class ListType2>
        static typename outerProduct
        <
            typename ListType1::value_type,
            typename ListType2::value_type
        >::type
        weightedSum(const ListType1& f1, const ListType2& f2);

};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "ListOps.hpp"
#include "IOobject.hpp"
#include "HashSet.hpp"
#include "objectRegistry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ListType1, class ListType2>
typename CML::outerProduct
<
    typename ListType1::value_type,
    typename ListType2::value_type
>::type
CML::interpolationWeights::weightedSum
(
    const ListType1& f1,
    const ListType2& f2
)
{
    typedef typename outerProduct
    <
        typename ListType1::value_type,
        typename ListType2::value_type
    >::type returnType;

    if (f1.size())
    {
        returnType SumProd = f1[0]*f2[0];
        for (label i = 1; i < f1.size(); ++i)
        {
            SumProd += f1[i]*f2[i];
        }
        return SumProd;
    }
    else
    {
        return pTraits<returnType>::zero;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
