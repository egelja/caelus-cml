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
    CML::LandauTellerReactionRate

Description
    Landau-Teller reaction rate.

\*---------------------------------------------------------------------------*/

#ifndef LandauTellerReactionRate_HPP
#define LandauTellerReactionRate_HPP

#include "scalarField.hpp"
#include "typeInfo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class LandauTellerReactionRate;

Ostream& operator<<(Ostream&, const LandauTellerReactionRate&);


/*---------------------------------------------------------------------------*\
                   Class LandauTellerReactionRate Declaration
\*---------------------------------------------------------------------------*/

class LandauTellerReactionRate
{

    scalar A_;
    scalar beta_;
    scalar Ta_;
    scalar B_;
    scalar C_;


public:


    //- Construct from components
    inline LandauTellerReactionRate
    (
        const scalar A,
        const scalar beta,
        const scalar Ta,
        const scalar B,
        const scalar C
    )
    :
        A_(A),
        beta_(beta),
        Ta_(Ta),
        B_(B),
        C_(C)
    {}

    //- Construct from dictionary
    inline LandauTellerReactionRate
    (
        const speciesTable& species,
        const dictionary& dict
    )
    :
        A_(readScalar(dict.lookup("A"))),
        beta_(readScalar(dict.lookup("beta"))),
        Ta_(readScalar(dict.lookup("Ta"))),
        B_(readScalar(dict.lookup("B"))),
        C_(readScalar(dict.lookup("C")))
    {}


    // Member Functions

    //- Return the type name
    static word type()
    {
        return "LandauTeller";
    }

    inline scalar operator()
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        scalar lta = A_;

        if (mag(beta_) > VSMALL)
        {
            lta *= pow(T, beta_);
        }

        scalar expArg = 0.0;

        if (mag(Ta_) > VSMALL)
        {
            expArg -= Ta_/T;
        }

        if (mag(B_) > VSMALL)
        {
            expArg += B_/cbrt(T);
        }

        if (mag(C_) > VSMALL)
        {
            expArg += C_/pow(T, 2.0/3.0);
        }

        if (mag(expArg) > VSMALL)
        {
            lta *= exp(expArg);
        }

        return lta;
    }

    inline scalar ddT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        scalar lta = A_;

        if (mag(beta_) > VSMALL)
        {
            lta *= pow(T, beta_);
        }

        scalar expArg = 0;
        scalar deriv = 0;

        if (mag(Ta_) > VSMALL)
        {
            scalar TaT = Ta_/T;
            expArg -= TaT;
            deriv += TaT;
        }

        if (mag(B_) > VSMALL)
        {
            scalar BT = B_/cbrt(T);
            expArg += BT;
            deriv -= BT/3;
        }

        if (mag(C_) > VSMALL)
        {
            scalar CT = C_/pow(T, 2.0/3.0);
            expArg += CT;
            deriv -= 2*CT/3;
        }

        if (mag(expArg) > VSMALL)
        {
            lta *= exp(expArg);
        }

        return lta*(beta_+deriv)/T;
    }

    //- Third-body efficiencies (beta = 1-alpha)
    //  non-empty only for third-body reactions
    //  with enhanced molecularity (alpha != 1)
    inline const List<Tuple2<label, scalar> >& beta() const
    {
        return NullSingletonRef<List<Tuple2<label, scalar> > >();
    }

    //- Species concentration derivative of the pressure dependent term
    inline void dcidc
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarField& dcidc
    ) const
    {}

    //- Temperature derivative of the pressure dependent term
    inline scalar dcidT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        return 0;
    }

    //- Write to stream
    inline void write(Ostream& os) const
    {
        os.writeKeyword("A") << A_ << token::END_STATEMENT << nl;
        os.writeKeyword("beta") << beta_ << token::END_STATEMENT << nl;
        os.writeKeyword("Ta") << Ta_ << token::END_STATEMENT << nl;
        os.writeKeyword("B") << B_ << token::END_STATEMENT << nl;
        os.writeKeyword("C") << C_ << token::END_STATEMENT << nl;
    }


    // Ostream Operator

    inline friend Ostream& operator<<
    (
        Ostream& os,
        const LandauTellerReactionRate& arr
    )
    {
        arr.write(os);
        return os;
    }

};


} // End namespace CML


#endif
