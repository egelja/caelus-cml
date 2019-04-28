/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2013 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
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
    SpalartAllmarasIDDES LES turbulence model for incompressible flows

    Reference:
    "A hybrid RANS-LES approach with delayed-DES and wall-modelled LES
    Capabilities", M. L. Shur, P. R. Spalart, K. Kh. Strelets, A. K. Travin,
    International Journal of Heat Transfer and Fluid Flow, 29 (2008), 
    pp. 1638-1649

Author: A. Jemcov

\*---------------------------------------------------------------------------*/

#ifndef compressibleSpalartAllmarasIDDES_HPP
#define compressibleSpalartAllmarasIDDES_HPP

#include "compressibleSpalartAllmarasDES.hpp"

namespace CML
{
namespace compressible
{
namespace LESModels
{

class SpalartAllmarasIDDES
:
    public SpalartAllmarasDES
{
    // Private data

        // Model constants

            autoPtr<CML::LESdelta> hmax_;
            autoPtr<CML::LESdelta> IDDESDelta_;
            dimensionedScalar fwStar_;
            dimensionedScalar cl_;
            dimensionedScalar ct_;


    // Private Member Functions

        tmp<volScalarField> alpha() const;
        tmp<volScalarField> ft(const volScalarField& S) const;
        tmp<volScalarField> fl(const volScalarField& S) const;

        tmp<volScalarField> rd
        (
            const volScalarField& visc,
            const volScalarField& S
        ) const;

        //- Delay function
        tmp<volScalarField> fd(const volScalarField& S) const;

        // Disallow default bitwise copy construct and assignment
        SpalartAllmarasIDDES(const SpalartAllmarasIDDES&);
        SpalartAllmarasIDDES& operator=(const SpalartAllmarasIDDES&);


protected:

    // Protected Member Functions

        //- Length scale
        virtual tmp<volScalarField> dTilda(const volScalarField& S) const;

        virtual tmp<volScalarField> S(const volTensorField& gradU) const;


public:

    //- Runtime type information
    TypeName("SpalartAllmarasIDDES");


    // Constructors

        //- Construct from components
        SpalartAllmarasIDDES
        (
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const fluidThermo& thermophysicalModel,
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    //- Destructor
    virtual ~SpalartAllmarasIDDES()
    {}


    // Member Functions

        //- Access function to filter width
        virtual const volScalarField& delta() const
        {
            return IDDESDelta_();
        }

        //- Read LESProperties dictionary
        virtual bool read();
};


}
}
}

#endif

