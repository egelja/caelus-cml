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
    CML::thirdBodyArrheniusReactionRate

Description
    Arrhenius reaction rate enhanced by third-body interation.


\*---------------------------------------------------------------------------*/

#ifndef thirdBodyArrheniusReactionRate_HPP
#define thirdBodyArrheniusReactionRate_HPP

#include "ArrheniusReactionRate.hpp"
#include "thirdBodyEfficiencies.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class thirdBodyArrheniusReactionRate;

Ostream& operator<<(Ostream&, const thirdBodyArrheniusReactionRate&);


/*---------------------------------------------------------------------------*\
              Class thirdBodyArrheniusReactionRate Declaration
\*---------------------------------------------------------------------------*/

class thirdBodyArrheniusReactionRate
:
    ArrheniusReactionRate
{

    thirdBodyEfficiencies thirdBodyEfficiencies_;
    List<Tuple2<label, scalar> > beta_;


public:


    //- Construct from components
    inline thirdBodyArrheniusReactionRate
    (
        const scalar A,
        const scalar beta,
        const scalar Ta,
        const thirdBodyEfficiencies& tbes
    )
    :
        ArrheniusReactionRate(A, beta, Ta),
        thirdBodyEfficiencies_(tbes)
    {
        forAll(tbes, i)
        {
            beta_.append(Tuple2<label, scalar>(i, tbes[i]));
        }
    }

    //- Construct from dictionary
    inline thirdBodyArrheniusReactionRate
    (
        const speciesTable& species,
        const dictionary& dict
    )
    :
        ArrheniusReactionRate
        (
            species,
            dict
        ),
        thirdBodyEfficiencies_(species, dict)
    {
        forAll(thirdBodyEfficiencies_, i)
        {
            beta_.append
            (
                Tuple2<label, scalar>
                (
                    i,
                    thirdBodyEfficiencies_[i]
                )
            );
        }
    }


    // Member Functions

    //- Return the type name
    static word type()
    {
        return "thirdBodyArrhenius";
    }

    inline scalar operator()
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        return
            thirdBodyEfficiencies_.M(c)
           *ArrheniusReactionRate::operator()(p, T, c);
    }

    inline scalar ddT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        return
            thirdBodyEfficiencies_.M(c)
           *ArrheniusReactionRate::ddT(p, T, c);
    }

    //- Third-body efficiencies (beta = 1-alpha)
    //  non-empty only for third-body reactions
    //  with enhanced molecularity (alpha != 1)
    inline const List<Tuple2<label, scalar> >& beta() const
    {
        return beta_;
    }

    //- Species concentration derivative of the pressure dependent term
    inline void dcidc
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarField& dcidc
    ) const
    {
        scalar M = thirdBodyEfficiencies_.M(c);
        forAll(beta_, i)
        {
            dcidc[i] = beta_[i].second()/max(M, SMALL);
        }
    }

    //- Temperature derivative of the pressure dependent term
    inline scalar dcidT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        return -1.0/T;
    }

    //- Write to stream
    inline void write(Ostream& os) const
    {
        ArrheniusReactionRate::write(os);
        thirdBodyEfficiencies_.write(os);
    }


    // Ostream Operator

    inline friend Ostream& operator<<
    (
        Ostream& os,
        const thirdBodyArrheniusReactionRate& arr
    )
    {
        arr.write(os);
        return os;
    }
};


} // End namespace CML


#endif
