/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::laminarFlameSpeedModels::GuldersEGR

Description
    Laminar flame speed obtained from Gulder's correlation with EGR modelling.

SourceFiles
    GuldersEGR.cpp

\*---------------------------------------------------------------------------*/

#ifndef GuldersEGR_HPP
#define GuldersEGR_HPP

#include "laminarFlameSpeed.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace laminarFlameSpeedModels
{

/*---------------------------------------------------------------------------*\
                           Class GuldersEGR Declaration
\*---------------------------------------------------------------------------*/

class GuldersEGR
:
    public laminarFlameSpeed
{

    dictionary coeffsDict_;

    scalar W_;
    scalar eta_;
    scalar xi_;
    scalar f_;
    scalar alpha_;
    scalar beta_;


    inline scalar SuRef
    (
        scalar phi
    ) const
    {
        if (phi > SMALL)
        {
            return W_*pow(phi, eta_)*exp(-xi_*sqr(phi - 1.075));
        }
        else
        {
            return 0.0;
        }
    }

    inline scalar Su0pTphi
    (
        scalar p,
        scalar Tu,
        scalar phi,
        scalar Yres
    ) const
    {
        static const scalar Tref = 300.0;
        static const scalar pRef = 1.013e5;

        return SuRef(phi)*pow((Tu/Tref), alpha_)*pow((p/pRef), beta_)*(1 - f_*Yres);
    }

    tmp<volScalarField> Su0pTphi
    (
        const volScalarField& p,
        const volScalarField& Tu,
        scalar phi
    ) const;

    tmp<volScalarField> Su0pTphi
    (
        const volScalarField& p,
        const volScalarField& Tu,
        const volScalarField& phi,
        const volScalarField& egr
    ) const;

    //- Construct as copy (not implemented)
    GuldersEGR(const GuldersEGR&);

    void operator=(const GuldersEGR&);


public:

    //- Runtime type information
    TypeName("GuldersEGR");


    //- Construct from dictionary and psiuReactionThermo
    GuldersEGR
    (
        const dictionary&,
        const psiuReactionThermo&
    );


    //- Destructor
    virtual ~GuldersEGR()
    {}


    // Member functions

    //- Return the laminar flame speed [m/s]
    tmp<volScalarField> operator()() const;

};


} // End laminarFlameSpeedModels
} // End namespace CML


#endif
