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
    CML::TroeFallOffFunction

Description
    The Troe fall-off function


\*---------------------------------------------------------------------------*/

#ifndef TroeFallOffFunction_HPP
#define TroeFallOffFunction_HPP

#include "scalar.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class TroeFallOffFunction;
Ostream& operator<<(Ostream&, const TroeFallOffFunction&);


/*---------------------------------------------------------------------------*\
                     Class TroeFallOffFunction Declaration
\*---------------------------------------------------------------------------*/

class TroeFallOffFunction
{

    scalar alpha_;
    scalar Tsss_, Ts_, Tss_;


public:


    //- Construct from components
    inline TroeFallOffFunction
    (
        const scalar alpha,
        const scalar Tsss,
        const scalar Ts,
        const scalar Tss
    )
    :
        alpha_(alpha),
        Tsss_(Tsss),
        Ts_(Ts),
        Tss_(Tss)
    {}

    //- Construct from dictionary
    inline TroeFallOffFunction(const dictionary& dict)
    :
        alpha_(readScalar(dict.lookup("alpha"))),
        Tsss_(readScalar(dict.lookup("Tsss"))),
        Ts_(readScalar(dict.lookup("Ts"))),
        Tss_(readScalar(dict.lookup("Tss")))
    {}


    // Member Functions

    //- Return the type name
    static word type()
    {
        return "Troe";
    }

    inline scalar operator()
    (
        const scalar T,
        const scalar Pr
    ) const
    {
        scalar logFcent = log10
        (
            max
            (
                (1 - alpha_)*exp(-T/Tsss_) + alpha_*exp(-T/Ts_) + exp(-Tss_/T),
                SMALL
            )
        );

        scalar c = -0.4 - 0.67*logFcent;
        static const scalar d = 0.14;
        scalar n = 0.75 - 1.27*logFcent;

        scalar logPr = log10(max(Pr, SMALL));
        return pow(10.0, logFcent/(1.0 + sqr((logPr + c)/(n - d*(logPr + c)))));
    }

    inline scalar ddT
    (
        const scalar Pr,
        const scalar F,
        const scalar dPrdT,
        const scalar T
    ) const
    {
        scalar logPr = log10(max(Pr, SMALL));
        scalar logTen = log(10.0);
        scalar Fcent =
        (
            max
            (
                (1 - alpha_)*exp(-T/Tsss_) + alpha_*exp(-T/Ts_) + exp(-Tss_/T),
                SMALL
            )
        );
        scalar logFcent = log10(Fcent);

        scalar dFcentdT =
        (
            (alpha_ - 1)*exp(-T/Tsss_)/Tsss_
          - alpha_*exp(-T/Ts_)/Ts_
          + Tss_*exp(-Tss_/T)/sqr(T)
        );

        scalar d = 0.14;
        scalar dlogFcentdT = dFcentdT/Fcent/logTen;
        scalar c = -0.4 - 0.67*logFcent;
        scalar dcdT = -0.67*dlogFcentdT;
        scalar n = 0.75 - 1.27*logFcent;
        scalar dndT = -1.27*dlogFcentdT;

        scalar dlogPrdT = dPrdT/Pr/logTen;

        scalar dParentdT =
            2.0*(logPr + c)/sqr(n - d*(logPr + c))
           *(
                (dlogPrdT + dcdT)
              - (logPr + c)*(dndT - d*(dlogPrdT + dcdT))/(n - d*(logPr + c))
            );

        return
        (
            F*logTen
           *(
                dlogFcentdT/(1.0 + sqr((logPr + c)/(n - d*(logPr + c))))
              - logFcent*dParentdT/sqr(1.0 + sqr((logPr + c)/(n - d*(logPr + c))))
            )
        );
    }

    inline scalar ddc
    (
        const scalar Pr,
        const scalar F,
        const scalar dPrdc,
        const scalar T
    ) const
    {
        scalar logPr = log10(max(Pr, SMALL));
        scalar logTen = log(10.0);
        scalar logFcent = log10
        (
            max
            (
                (1 - alpha_)*exp(-T/Tsss_) + alpha_*exp(-T/Ts_) + exp(-Tss_/T),
                SMALL
            )
        );

        scalar dlogPrdc = dPrdc/Pr/logTen;
        scalar d = 0.14;
        scalar c = -0.4 - 0.67*logFcent;
        scalar n = 0.75 - 1.27*logFcent;

        scalar dParentdc =
            2.0*(logPr + c)/sqr(n - d*(logPr + c))
           *(
                (dlogPrdc)
              - (logPr + c)*(-d*(dlogPrdc))/(n - d*(logPr + c))
            );

        return
        (
            F*logTen
           *(
              - logFcent*dParentdc/sqr(1.0 + sqr((logPr + c)/(n - d*(logPr + c))))
            )
        );
    }

    //- Write to stream
    inline void write(Ostream& os) const
    {
        os.writeKeyword("alpha") << alpha_ << token::END_STATEMENT << nl;
        os.writeKeyword("Tsss") << Tsss_ << token::END_STATEMENT << nl;
        os.writeKeyword("Ts") << Ts_ << token::END_STATEMENT << nl;
        os.writeKeyword("Tss") << Tss_ << token::END_STATEMENT << nl;
    }


    // Ostream Operator

    friend Ostream& operator<<
    (
        Ostream& os,
        const TroeFallOffFunction& tfof
    )
    {
        tfof.write(os);
        return os;
    }

};


} // End namespace CML


#endif
