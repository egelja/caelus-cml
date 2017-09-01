/*---------------------------------------------------------------------------*\
Copyright held by original author
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
    CML::zeroCrossing

Description
    Perform an ensemble average of a given data stream.

SourceFiles
    zeroCrossing.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef zeroCrossing_HPP
#define zeroCrossing_HPP

#include "postProcessingWaves.hpp"

#include "spectralMethodsFFTBased.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class zeroCrossing
:
    public postProcessingWaves
{

protected:

    // Protected data
        word crossingType_;
        IOdictionary dataDict_;

        labelList indices_;
        scalar deltaT_;


    // Protected member functions

private:

    // Private Member Functions
        void evaluateScalar();

        void writeScalar
        (
            const List<scalarField>&,
            const List<scalarField>&,
            const List<scalarField>&,
            const List<scalarField>&,
            const word
        ) const;

        void crossingAnalysis
        (
            const scalarField&,
            const List<scalarField>&,
            List<scalarField>&,
            List<scalarField>&,
            List<scalarField>&,
            List<scalarField>&,
            const label
        ) const;


        //- Disallow default bitwise copy construct
        zeroCrossing(const zeroCrossing&);

        //- Disallow default bitwise assignment
        void operator=(const zeroCrossing&);

    // Private member data

public:

    //- Runtime type information
    TypeName("zeroCrossing");

    // Constructors

        //- Construct from components
        zeroCrossing
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~zeroCrossing();

    // Member Functions
        virtual void evaluate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
