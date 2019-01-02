/*---------------------------------------------------------------------------*\
Copyright (C) 2016 Applied CCM
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
    CML::viscosityModels::CarreauYasudaCross

Description
    An incompressible non-Newtonian viscosity model based on Carreau-Yasuda-Cross.

    References:
    \verbatim
        "Rheological equations from molecular network theories"
        P. Carreau
        Trans. Soc. Rheol, Vol. 16, 99, 1972.
    \endverbatim

    \verbatim
        "Shear-flow properties of concentrated-solutions of linear and star branched polystyrenes"
        R. A. K Yasuda
        R. Cohen
        Rheol. Acta rnal, Vol. 20, 163, 1981.
    \endverbatim

    \verbatim
        "Rheology of non-Newtonian fluids: a new flow equation for pseudo-plastic systems"
        M. Cross
        J. Colloid, Vol. 20, 417, 1958.
    \endverbatim

SourceFiles
    CarreauYasudaCross.cpp

\*---------------------------------------------------------------------------*/

#ifndef CarreauYasudaCross_H
#define CarreauYasudaCross_H

#include "viscosityModel.hpp"
#include "dimensionedScalar.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace viscosityModels
{

/*---------------------------------------------------------------------------*\
                           Class CarreauYasudaCross Declaration
\*---------------------------------------------------------------------------*/

class CarreauYasudaCross
:
    public viscosityModel
{
    // Private data

        dictionary CarreauYasudaCrossCoeffs_;

        dimensionedScalar mu0_;
        dimensionedScalar muInf_;
        dimensionedScalar lambda_;
        dimensionedScalar n_;
        dimensionedScalar a_;
        dimensionedScalar rhoRef_;

        volScalarField nu_;


    // Private Member Functions

        //- Calculate and return the laminar viscosity
        tmp<volScalarField> calcNu() const;


public:

    //- Runtime type information
    TypeName("CarreauYasudaCross");


    // Constructors

        //- construct from components
        CarreauYasudaCross
        (
            const word& name,
            const dictionary& viscosityProperties,
            const volVectorField& U,
            const surfaceScalarField& phi
        );


    //- Destructor
    ~CarreauYasudaCross()
    {}


    // Member Functions

        //- Return the laminar viscosity
        tmp<volScalarField> nu() const
        {
            return nu_;
        }

        //- Correct the laminar viscosity
        void correct();

        //- Read transportProperties dictionary
        bool read(const dictionary& viscosityProperties);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace viscosityModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
