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
    CML::powerSeriesReactionRate

Description
    Power series reaction rate.


\*---------------------------------------------------------------------------*/

#ifndef powerSeriesReactionRate_HPP
#define powerSeriesReactionRate_HPP

#include "scalarField.hpp"
#include "typeInfo.hpp"
#include "FixedList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class powerSeriesReactionRate;

Ostream& operator<<(Ostream&, const powerSeriesReactionRate&);


/*---------------------------------------------------------------------------*\
                   Class powerSeriesReactionRate Declaration
\*---------------------------------------------------------------------------*/

class powerSeriesReactionRate
{

    scalar A_;
    scalar beta_;
    scalar Ta_;

    static const label nCoeff_ = 4;
    FixedList<scalar, nCoeff_> coeffs_;


public:


    //- Construct from components
    inline powerSeriesReactionRate
    (
        const scalar A,
        const scalar beta,
        const scalar Ta,
        const FixedList<scalar, nCoeff_> coeffs
    )
    :
        A_(A),
        beta_(beta),
        Ta_(Ta),
        coeffs_(coeffs)
    {}

    //- Construct from dictionary
    inline powerSeriesReactionRate
    (
        const speciesTable& species,
        const dictionary& dict
    )
    :
        A_(readScalar(dict.lookup("A"))),
        beta_(readScalar(dict.lookup("beta"))),
        Ta_(readScalar(dict.lookup("Ta"))),
        coeffs_(dict.lookup("coeffs"))
    {}


    // Member Functions

    //- Return the type name
    static word type()
    {
        return "powerSeries";
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

        forAll(coeffs_, n)
        {
            expArg += coeffs_[n]/pow(T, n + 1);
        }

        lta *= exp(expArg);

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

        forAll(coeffs_, n)
        {
            scalar cT = coeffs_[n]/pow(T, n + 1);
            expArg += cT;
            deriv -= (n + 1)*cT;
        }

        lta *= exp(expArg);

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
        os.writeKeyword("coeffs") << coeffs_ << token::END_STATEMENT << nl;
    }


    // Ostream Operator

    inline friend Ostream& operator<<
    (
        Ostream& os,
        const powerSeriesReactionRate& psrr
    )
    {
    psrr.write(os);
    return os;
   }

};


} // End namespace CML


#endif
