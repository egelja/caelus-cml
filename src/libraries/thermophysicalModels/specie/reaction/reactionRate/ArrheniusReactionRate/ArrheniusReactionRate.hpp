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
    CML::ArrheniusReactionRate

Description
    Arrhenius reaction rate given by:

        k = A * T^beta * exp(-Ta/T)


\*---------------------------------------------------------------------------*/

#ifndef ArrheniusReactionRate_HPP
#define ArrheniusReactionRate_HPP

#include "scalarField.hpp"
#include "typeInfo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class ArrheniusReactionRate;

Ostream& operator<<(Ostream&, const ArrheniusReactionRate&);


/*---------------------------------------------------------------------------*\
                    Class ArrheniusReactionRate Declaration
\*---------------------------------------------------------------------------*/

class ArrheniusReactionRate
{

    scalar A_;
    scalar beta_;
    scalar Ta_;


public:


    //- Construct from components
    inline ArrheniusReactionRate
    (
        const scalar A,
        const scalar beta,
        const scalar Ta
    )
    :
        A_(A),
        beta_(beta),
        Ta_(Ta)
    {}

    //- Construct from dictionary
    inline ArrheniusReactionRate
    (
        const speciesTable& species,
        const dictionary& dict
    )
    :
        A_(readScalar(dict.lookup("A"))),
        beta_(readScalar(dict.lookup("beta"))),
        Ta_(readScalar(dict.lookup("Ta")))
    {}


    // Member Functions

    //- Return the type name
    static word type()
    {
        return "Arrhenius";
    }

    inline scalar operator()
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        scalar ak = A_;

        if (mag(beta_) > VSMALL)
        {
            ak *= pow(T, beta_);
        }

        if (mag(Ta_) > VSMALL)
        {
            ak *= exp(-Ta_/T);
        }

        return ak;
    }

    inline scalar ddT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        scalar ak = A_;

        if (mag(beta_) > VSMALL)
        {
            ak *= pow(T, beta_);
        }

        if (mag(Ta_) > VSMALL)
        {
            ak *= exp(-Ta_/T);
        }

        return ak*(beta_+Ta_/T)/T;
    }

    //- Third-body efficiencies (beta = 1-alpha)
    //  non-empty only for third-body reactions
    //  with enhanced molecularity (alpha != 1)
    inline const List<Tuple2<label, scalar> >& beta() const
    {
        return NullSingletonRef<List<Tuple2<label, scalar> > >();
    }

    //- Species concentration derivative of the pressure dependent term
    //  By default this value is 1 as it multiplies the third-body term
    inline void dcidc
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarField& dcidc
    ) const
    {}

    //- Temperature derivative of the pressure dependent term
    //  By default this value is 0 since ddT of molecularity is approx.0
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
    }


    // Ostream Operator

    friend Ostream& operator<<
    (
        Ostream& os,
        const ArrheniusReactionRate& arr
    )
    {
        os  << token::BEGIN_LIST
            << arr.A_ << token::SPACE << arr.beta_ << token::SPACE << arr.Ta_
            << token::END_LIST;
        return os;
    }
};


} // End namespace CML


#endif
