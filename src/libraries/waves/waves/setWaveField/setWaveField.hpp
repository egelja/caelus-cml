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
    CML::setWaveField

Description
    Implementation of a method for setting an arbitrary wave theory as initial
    conditions in the computational domain. Change in bed level is not taken
    into consideration.

    A description of the general wave framework is given in

    @article { jacobsenFuhrmanFredsoe2011,
        Author = {Jacobsen, N G and Fuhrman, D R and Freds\o{}e, J},
        title = {{A Wave Generation Toolbox for the Open-Source CFD Library: OpenFoam\textregistered{}}},
        Journal = {{Int. J. for Numer. Meth. Fluids}},
        Year = {2012},
        Volume = {70},
        Number = {9},
        Pages = {1073-1088},
        DOI = {{10.1002/fld.2726}},
    }

SourceFiles
    setWaveField.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef setWaveField_HPP
#define setWaveField_HPP

#include "convexPolyhedral.hpp"
#include "waveTheory.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class setWaveField Declaration
\*---------------------------------------------------------------------------*/

class setWaveField
:
    public convexPolyhedral
{
private:

    // Private data

        const fvMesh& mesh_;

        volVectorField& U_;
        volScalarField& alpha_;
        volScalarField& p_;

        word name_;
        autoPtr<waveTheories::waveTheory> waveProps_;


    // Private member functions

        //- Returns a scalarField of the signed distance to an arbitrary
        //  surface
            virtual void signedPointToSurfaceDistance
            (
                const pointField&,
                scalarField&
            );

        //- Returns a scalar of the signed distance to an arbitrary surface
            virtual scalar signedPointToSurfaceDistance
            (
                const point&
            ) const;

public:

    //- Runtime type information
    TypeName("setWaveField");

    // Constructors

        //- Construct from components
        setWaveField
        (
            const fvMesh& mesh,
            volVectorField& U,
            volScalarField& alpha,
            volScalarField& p
        );

        setWaveField
        (
            const fvMesh& mesh,
            const word,
            volVectorField& U,
            volScalarField& alpha,
            volScalarField& p
        );

    // Member Functions

        void correct();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
