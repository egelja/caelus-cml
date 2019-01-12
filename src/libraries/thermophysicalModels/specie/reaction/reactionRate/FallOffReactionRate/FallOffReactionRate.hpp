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
    CML::FallOffReactionRate

Description
    General class for handling unimolecular/recombination fall-off reactions.


\*---------------------------------------------------------------------------*/

#ifndef FallOffReactionRate_HPP
#define FallOffReactionRate_HPP

#include "thirdBodyEfficiencies.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class ReactionRate, class FallOffFunction>
class FallOffReactionRate;

template<class ReactionRate, class FallOffFunction>
inline Ostream& operator<<
(
    Ostream&,
    const FallOffReactionRate<ReactionRate, FallOffFunction>&
);


/*---------------------------------------------------------------------------*\
                    Class FallOffReactionRate Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionRate, class FallOffFunction>
class FallOffReactionRate
{

    ReactionRate k0_;
    ReactionRate kInf_;
    FallOffFunction F_;
    thirdBodyEfficiencies thirdBodyEfficiencies_;
    List<Tuple2<label, scalar> > beta_;


public:


    //- Construct from components
    inline FallOffReactionRate
    (
        const ReactionRate& k0,
        const ReactionRate& kInf,
        const FallOffFunction& F,
        const thirdBodyEfficiencies& tbes
    )
    :
        k0_(k0),
        kInf_(kInf),
        F_(F),
        thirdBodyEfficiencies_(tbes)
    {
        forAll(tbes, i)
        {
            beta_.append(Tuple2<label, scalar>(i, tbes[i]));
        }
    }

    //- Construct from dictionary
    inline FallOffReactionRate
    (
        const speciesTable& species,
        const dictionary& dict
    )
    :
        k0_(species, dict.subDict("k0")),
        kInf_(species, dict.subDict("kInf")),
        F_(dict.subDict("F")),
        thirdBodyEfficiencies_(species, dict.subDict("thirdBodyEfficiencies"))
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
        return ReactionRate::type() + FallOffFunction::type() + "FallOff";
    }

    inline scalar operator()
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        const scalar k0 = k0_(p, T, c);
        const scalar kInf = kInf_(p, T, c);
        const scalar Pr = k0*thirdBodyEfficiencies_.M(c)/kInf;

        return kInf*(Pr/(1 + Pr))*F_(T, Pr);
    }

    inline scalar ddT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        const scalar k0 = k0_(p, T, c);
        const scalar kInf = kInf_(p, T, c);
        const scalar Pr = k0*thirdBodyEfficiencies_.M(c)/kInf;

        return (Pr/(1 + Pr))*F_(T, Pr)*kInf_.ddT(p, T, c);
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
        const scalar M = thirdBodyEfficiencies_.M(c);

        if (M > SMALL)
        {
            const scalar k0 = k0_(p, T, c);
            const scalar kInf = kInf_(p, T, c);
            const scalar Pr = k0*M/kInf;
            const scalar F = F_(T, Pr);

            forAll(beta_, i)
            {
                const scalar dPrdci = -beta_[i].second()*k0/kInf;
                const scalar dFdci = F_.ddc(Pr, F, dPrdci, T);
                dcidc[i] = (dPrdci/(Pr*(1 + Pr)) + dFdci/F);
            }
        }
        else
        {
            forAll(beta_, i)
            {
                dcidc[i] = 0;
            }
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
        const scalar M = thirdBodyEfficiencies_.M(c);

        if (M > SMALL)
        {
            const scalar k0 = k0_(p, T, c);
            const scalar kInf = kInf_(p, T, c);

            const scalar Pr = k0*thirdBodyEfficiencies_.M(c)/kInf;
            const scalar F = F_(T, Pr);
            const scalar dPrdT =
                Pr*(k0_.ddT(p, T, c)/k0 - kInf_.ddT(p, T, c)/kInf - 1/T);
            const scalar dFdT = F_.ddT(Pr, F, dPrdT, T);

            return (dPrdT/(Pr*(1 + Pr)) + dFdT/F);
        }
        else
        {
            return 0;
        }
    }

    //- Write to stream
    inline void write(Ostream& os) const
    {
        os  << indent << "k0" << nl;
        os  << indent << token::BEGIN_BLOCK << nl;
        os  << incrIndent;
        k0_.write(os);
        os  << decrIndent;
        os  << indent << token::END_BLOCK << nl;

        os  << indent << "kInf" << nl;
        os  << indent << token::BEGIN_BLOCK << nl;
        os  << incrIndent;
        kInf_.write(os);
        os  << decrIndent;
        os  << indent << token::END_BLOCK << nl;

        os  << indent << "F" << nl;
        os  << indent << token::BEGIN_BLOCK << nl;
        os  << incrIndent;
        F_.write(os);
        os  << decrIndent;
        os  << indent << token::END_BLOCK << nl;

        os  << indent << "thirdBodyEfficiencies" << nl;
        os  << indent << token::BEGIN_BLOCK << nl;
        os  << incrIndent;
        thirdBodyEfficiencies_.write(os);
        os  << decrIndent;
        os  << indent << token::END_BLOCK << nl;
    }


    // Ostream Operator

    friend Ostream& operator<<
    (
        Ostream& os,
        const FallOffReactionRate<ReactionRate, FallOffFunction>& forr
    )
    {
        forr.write(os);
        return os;
    }
};


} // End namespace CML


#endif
