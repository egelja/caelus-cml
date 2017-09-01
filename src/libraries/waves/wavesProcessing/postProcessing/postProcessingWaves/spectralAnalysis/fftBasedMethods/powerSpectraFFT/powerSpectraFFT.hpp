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
    CML::powerSpectraFFT

Description
    Computes the power spectra for both scalar and vector quantities using a
    FFT routine.

SourceFiles
    powerSpectraFFT.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef powerSpectraFFT_HPP
#define powerSpectraFFT_HPP

#include "postProcessingWaves.hpp"

#include "spectralMethodsFFTBased.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class powerSpectraFFT
:
    public postProcessingWaves
{

protected:

    // Protected data
        IOdictionary dataDict_;

        labelList indices_;
        scalar deltaT_;

    // Protected member functions

private:

    // Private Member Functions
        void evaluateScalar();

        void evaluateVector();

        void writeScalar
        (
            const scalarField&,
            const List<scalarField>&
        );

        void writeVector
        (
            const scalarField&,
            const List<vectorField>&
        );

        //- Disallow default bitwise copy construct
        powerSpectraFFT(const powerSpectraFFT&);

        //- Disallow default bitwise assignment
        void operator=(const powerSpectraFFT&);

    // Private member data

public:

    //- Runtime type information
    TypeName("powerSpectraFFT");

    // Constructors

        //- Construct from components
        powerSpectraFFT
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~powerSpectraFFT();

    // Member Functions
        virtual void evaluate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
