/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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
    CML::spectralMethodsLeastSquaresBased

Description
    Helper functions for spectral analysis using a least-squares based
    approach.

SourceFiles
    spectralMethodsLeastSquaresBased.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef spectralMethodsLeastSquaresBased_HPP
#define spectralMethodsLeastSquaresBased_HPP

#include "fvCFD.hpp"
#include "complexExp.hpp"
#include "scalarMatrices.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class spectralMethodsLeastSquaresBased
{

private:

    // Private Member Functions

        void computePowerSpectrum
        (
            const scalarField&,
            const scalarField&,
            const label&,
            const scalar&,
            scalarField&
        );

        //- Disallow default bitwise copy construct
        spectralMethodsLeastSquaresBased
        (
            const spectralMethodsLeastSquaresBased&
        );

        //- Disallow default bitwise assignment
        void operator=(const spectralMethodsLeastSquaresBased&);

    // Private member data

public:

    //- Runtime type information
    TypeName("spectralMethodsLeastSquaresBased");

    // Constructors

        //- Construct from components
        spectralMethodsLeastSquaresBased
        (
            const Time&,
            const dictionary&
        );

    // Destructor

        virtual ~spectralMethodsLeastSquaresBased();

    // Member Functions

        //- Solves a least squares problem, with the columns given in the
        //  List<scalarField> and the right hand side in the scalarField.
        //  The solution is returned in the right hand side field
        void solve
        (
            const List<scalarField>&,
            scalarField&
        );

        scalarField frequencies
        (
            const label&
        );

        scalarField powerSpectra
        (
               const scalarField&,
            const scalarField&,
            const label&,
            const scalar&
        );

        List<vectorField> powerSpectra
        (
               const scalarField&,
            const List<vectorField>&,
            const label&,
            const scalar&
        );

        List<scalarField> powerSpectra
        (
               const scalarField&,
            const List<scalarField>&,
            const label& N,
            const scalar&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
