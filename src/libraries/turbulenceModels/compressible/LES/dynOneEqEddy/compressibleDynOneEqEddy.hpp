/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::compressible::LESModels::dynOneEqEddy

Description
    Localised Dynamic One Equation Eddy Viscosity Model for compressible
    flows

    Eddy viscosity SGS model using a modeled balance equation to simulate
    the behaviour of k, hence
    \verbatim
        d/dt(rho*k) + div(rho*U*k) - div(muEff*grad(k))
        =
        -rho*B*L - ce*rho*k^3/2/delta
    and
        B = 2/3*rho*k*I - 2*muSgs*dev(D)
        Beff = 2/3*rho*k*I - 2*muEff*dev(D)
    where
        muSgs = rho*cD*delta^2*||D||
        muEff = muSgs + mu
    \endverbatim

    A dynamic procedure is here applied to evaluate ck and ce
    \verbatim
        ck=<L.M>/<M.M>
    and
        ce=<e*m>/<m*m>
    where
       K = 0.5*(F(U.U) - F(U).F(U))
       L = (F(U*U) - F(U)*F(U) - 0.33*K*I)
       M = delta*(F(sqrt(k)*D) - 2*sqrt(K + filter(k))*F(D))
       m = pow(K + F(k), 3.0/2.0)/(2*delta) - F(pow(k, 3.0/2.0))/delta
       e = 2*delta*ck*(F(sqrt(k)*(D && D)) - 2*sqrt(K + F(k))*(F(D) && F(D)))/
    \endverbatim

SourceFiles
    compressibleDynOneEqEddy.cpp

\*---------------------------------------------------------------------------*/

#ifndef compressibleDynOneEqEddy_H
#define compressibleDynOneEqEddy_H

#include "compressibleGenEddyVisc.hpp"
#include "simpleFilter.hpp"
#include "LESfilter.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{
namespace LESModels
{

/*---------------------------------------------------------------------------*\
                           Class dynOneEqEddy Declaration
\*---------------------------------------------------------------------------*/

class dynOneEqEddy
:
    public GenEddyVisc
{
    // Private data

        volScalarField k_;

        simpleFilter simpleFilter_;
        autoPtr<LESfilter> filterPtr_;
        LESfilter& filter_;


    // Private Member Functions

        //- Update sub-grid scale fields
        void updateSubGridScaleFields
        (
            const volSymmTensorField& D,
            const volScalarField& KK
        );

        //- Calculate ck, ce by filtering the velocity field U.
        volScalarField ck
        (
            const volSymmTensorField&,
            const volScalarField&
        ) const;

        volScalarField ce
        (
            const volSymmTensorField&,
            const volScalarField&
        ) const;

        // Disallow default bitwise copy construct and assignment
        dynOneEqEddy(const dynOneEqEddy&);
        dynOneEqEddy& operator=(const dynOneEqEddy&);


public:

    //- Runtime type information
    TypeName("dynOneEqEddy");

    // Constructors

        //- Construct from components
        dynOneEqEddy
        (
            const volScalarField& rho,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const fluidThermo& thermophysicalModel,
            const word& turbulenceModelName = turbulenceModel::typeName,
            const word& modelName = typeName
        );


    //- Destructor
    virtual ~dynOneEqEddy()
    {}


    // Member Functions

        //- Return SGS kinetic energy
        virtual tmp<volScalarField> k() const
        {
            return k_;
        }

        //- Return the effective diffusivity for k
        tmp<volScalarField> DkEff() const
        {
            return tmp<volScalarField>
            (
                new volScalarField("DkEff", muSgs_ + mu())
            );
        }

        //- Correct Eddy-Viscosity and related properties
        virtual void correct(const tmp<volTensorField>& gradU);

        //- Read LESProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace LESModels
} // End namespace incompressible
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
