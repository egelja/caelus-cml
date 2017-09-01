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
    CML::ensembleAverage

Description
    Perform an ensemble average of a given data stream.

SourceFiles
    ensembleAverage.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef ensembleAverage_HPP
#define ensembleAverage_HPP

#include "postProcessingWaves.hpp"

#include "spectralMethodsFFTBased.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class ensembleAverage
:
    public postProcessingWaves
{

protected:

    // Protected data
        IOdictionary dataDict_;

        labelList indices_;
        scalar deltaT_;
        scalar period_;
        label N_;

    // Protected member functions

private:

    // Private Member Functions
        void evaluateScalar();

        void evaluateVector();

        void writeScalar
        (
            const List<scalarField>&
        );

        void writeVector
        (
            const List<vectorField>&
        );

        //- Disallow default bitwise copy construct
        ensembleAverage(const ensembleAverage&);

        //- Disallow default bitwise assignment
        void operator=(const ensembleAverage&);

    // Private member data

public:

    //- Runtime type information
    TypeName("ensembleAverage");

    // Constructors

        //- Construct from components
        ensembleAverage
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~ensembleAverage();

    // Member Functions
        virtual void evaluate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
