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
    CML::spectralMethodsFFTBased

Description
    Helper functions for FFT-based analysis.

SourceFiles
    spectralMethodsFFTBased.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef spectralMethodsFFTBased_HPP
#define spectralMethodsFFTBased_HPP

#include "fvCFD.hpp"
#include "complexExp.hpp"

#include <math.h>
#include "gsl_errno.h"
#include "gsl_fft_real.h"
#include "gsl_fft_halfcomplex.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class spectralMethodsFFTBased
{

private:
    // Private member data
        const dictionary& dict_;

        label bins_;
        label step_;
        label sweeps_;
        label sweepCount_;

        // Initialise and allocate GSL-FFT
        gsl_fft_real_wavetable *real;
        gsl_fft_real_workspace *work;

    // Private Member Functions
        void checkBins();

        void powerSpectrum
        (
            const scalarField&,
            const scalar&,
            scalarField&
        );

        void powerSpectrum
        (
            const vectorField&,
            const scalar&,
            vectorField&
        );

        Field<complex> computeFourierTransform
        (
            const scalarField&
        );

        //- Disallow default bitwise copy construct
        spectralMethodsFFTBased(const spectralMethodsFFTBased&);

        //- Disallow default bitwise assignment
        void operator=(const spectralMethodsFFTBased&);

public:

    //- Runtime type information
    TypeName("spectralMethodsFFTBased");

    // Constructors

        //- Construct from components
        spectralMethodsFFTBased
        (
            const Time&,
            const dictionary&
        );

    // Destructor

        virtual ~spectralMethodsFFTBased();

    // Member Functions
        Field<complex> fft
        (
            const scalarField&
        );

        List<Field<complex> > fft
        (
            const List<scalarField>&
        );

        List<scalarField> powerSpectra
        (
            const List<scalarField>&,
            const scalar&
        );

        List<vectorField> powerSpectra
        (
            const List<vectorField>&,
            const scalar&
        );

        scalarField frequencies
        (
            const scalar& deltaT
        );

        void initSweep
        (
            const scalarField&
        );

        void resetSweep();

        inline label nSweeps() const {return sweeps_;};

        inline label nBins() const {return bins_;};
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
