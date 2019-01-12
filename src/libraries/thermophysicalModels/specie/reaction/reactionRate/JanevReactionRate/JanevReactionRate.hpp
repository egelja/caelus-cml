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
    CML::JanevReactionRate

Description
    Janev, Langer, Evans and Post reaction rate.

\*---------------------------------------------------------------------------*/

#ifndef JanevReactionRate_HPP
#define JanevReactionRate_HPP

#include "scalarField.hpp"
#include "typeInfo.hpp"
#include "FixedList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class JanevReactionRate;

Ostream& operator<<(Ostream&, const JanevReactionRate&);


/*---------------------------------------------------------------------------*\
                     Class JanevReactionRate Declaration
\*---------------------------------------------------------------------------*/

class JanevReactionRate
{

    scalar A_;
    scalar beta_;
    scalar Ta_;

    static const label nb_ = 9;
    FixedList<scalar, nb_> b_;


public:


    //- Construct from components
    inline JanevReactionRate
    (
        const scalar A,
        const scalar beta,
        const scalar Ta,
        const FixedList<scalar, nb_> b
    )
    :
        A_(A),
        beta_(beta),
        Ta_(Ta),
        b_(b)
    {}

    //- Construct from dictionary
    inline JanevReactionRate
    (
        const speciesTable& species,
        const dictionary& dict
    )
    :
        A_(readScalar(dict.lookup("A"))),
        beta_(readScalar(dict.lookup("beta"))),
        Ta_(readScalar(dict.lookup("Ta"))),
        b_(dict.lookup("b"))
    {}


    // Member Functions

    //- Return the type name
    static word type()
    {
        return "Janev";
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

        scalar lnT = log(T);

        for (int n=0; n<nb_; n++)
        {
            expArg += b_[n]*pow(lnT, n);
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

        if (mag(Ta_) > VSMALL)
        {
            expArg -= Ta_/T;
        }

        scalar lnT = log(T);

        for (int n=0; n<nb_; n++)
        {
            expArg += b_[n]*pow(lnT, n);
        }

        scalar deriv = b_[1];

        for (int n=2; n<nb_; n++)
        {
            deriv += n*b_[n]*pow(lnT, n-1);
        }

        lta *= exp(expArg);

        return lta*(beta_+Ta_/T+deriv)/T;
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
        os.writeKeyword("A") << A_ << nl;
        os.writeKeyword("beta") << beta_ << nl;
        os.writeKeyword("Ta") << Ta_ << nl;
        os.writeKeyword("b") << b_ << nl;
    }


    // Ostream Operator
    inline friend Ostream& operator<<
    (
        Ostream& os,
        const JanevReactionRate& jrr
    )
    {
        jrr.write(os);
        return os;
    }
};


} // End namespace CML


#endif
