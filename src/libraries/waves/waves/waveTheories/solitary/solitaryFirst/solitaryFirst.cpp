/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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

\*---------------------------------------------------------------------------*/

#include "solitaryFirst.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(solitaryFirst, 0);
addToRunTimeSelectionTable(waveTheory, solitaryFirst, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


solitaryFirst::solitaryFirst
(
    const word& subDictName,
    const fvMesh& mesh_
)
:
    waveTheory(subDictName, mesh_),
    H_(readScalar(coeffDict_.lookup("height"))),
    h_(readScalar(coeffDict_.lookup("depth"))),
    propagationDirection_(vector(coeffDict_.lookup("direction"))),
    x0_( vector(coeffDict_.lookup("x0")))
{
    G_ = CML::mag(g_);
    c_     = CML::sqrt(G_*(H_ + h_));

    propagationDirection_ /= CML::mag(propagationDirection_);

    checkWaveDirection(propagationDirection_);
}


void solitaryFirst::printCoeffs()
{
    Info << "Loading wave theory: " << typeName << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


scalar solitaryFirst::factor(const scalar& time) const
{
    // Dummy, as it does not make sense to ramp up a solitary wave

    return 0.0;
}


scalar solitaryFirst::eta
(
    const point& x,
    const scalar& time
) const
{
    scalar eta = H_*1.0/CML::pow(
        cosh(CML::sqrt(3.0*H_/(4.0*CML::pow(h_, 3.0)))
       *(c_*time - ((x - x0_) & propagationDirection_))) ,2.0 ) + seaLevel_;
    return eta;
}


scalar solitaryFirst::ddxPd
(
    const point& x,
    const scalar& time,
    const vector& unitVector
) const
{
    // A quite nasty expression!

    return 0.0;
}


scalar solitaryFirst::p
(
    const point& x,
    const scalar& time
) const
{
    scalar Z(returnZ(x));

    scalar result( G_*H_*rhoWater_ );

    result /= (CML::pow(CML::cosh(CML::sqrt(3*H_/(4.0*CML::pow(h_, 3.0)))
        *(CML::sqrt(G_*(h_ + H_))*time - ((x - x0_) & propagationDirection_)
        )), 4.0 ));

    result *= (2.0*(CML::pow(h_, 3.0) + 6.0*h_*H_*Z + 3.0*H_*CML::pow(Z, 2.0))
        + ( 2.0*CML::pow(h_, 3.0) - 6.0*h_*H_*Z - 3.0*H_*CML::pow(Z, 2.0))
        * CML::cosh(CML::sqrt(3.0*H_/CML::pow(h_, 3.0))*
          (CML::sqrt(G_*(h_ + H_))*time - ((x - x0_) & propagationDirection_))
          ));

    result /= (4.0*CML::pow(h_, 3.0));

    result += rhoWater_*G_*seaLevel_;

    return result;
}


vector solitaryFirst::U
(
    const point& x,
    const scalar& time
) const
{
    scalar Z(returnZ(x));

    scalar Uhorz(0.0);

    Uhorz = 1.0/(4.0*CML::pow(h_, 4.0) )*CML::sqrt(G_*h_)*H_
        *(
             2*CML::pow(h_, 3.0) + CML::pow(h_, 2.0)*H_
           + 12.0*h_*H_*Z + 6.0*H_*CML::pow(Z, 2.0)
           + (
                 2*CML::pow(h_, 3.0) - CML::pow(h_, 2.0)*H_
               - 6.0*h_*H_*Z - 3.0*H_*CML::pow(Z, 2.0)
             )
             *CML::cosh
             (
                 CML::sqrt(3*H_/CML::pow(h_, 3.0))
                *(
                     CML::sqrt(G_*(h_ + H_))*time
                   - ((x - x0_) & propagationDirection_)
                 )
             )
        )
        /CML::pow
        (
            CML::cosh(CML::sqrt(3*H_/(4.0*CML::pow(h_, 3.0)))
           *(
                CML::sqrt(G_*(h_ + H_))*time
              - ((x - x0_) & propagationDirection_))
            ), 4.0
        );

    scalar Uvert(0.0);

    Uvert = 1.0/(4.0*CML::sqrt(G_*h_))*CML::sqrt(3.0)*G_
        *CML::pow(H_/CML::pow(h_,3.0), 1.5)*(h_ + Z)
        *(
             2*CML::pow(h_, 3.0) - 7.0*CML::pow(h_, 2.0)*H_
           + 10.0*h_*H_*Z + 5.0*H_*CML::pow(Z, 2.0)
           +(
                2*CML::pow(h_, 3.0) + CML::pow(h_, 2.0)*H_
              - 2.0*h_*H_*Z - H_*CML::pow(Z, 2.0)
            )
           *CML::cosh
            (
                CML::sqrt(3*H_/CML::pow(h_, 3.0))*
                (
                    CML::sqrt(G_*(h_ + H_))*time
                  - ((x - x0_) & propagationDirection_)
                )
            )
        )
        /CML::pow
        (
            CML::cosh(CML::sqrt(3*H_/(4.0*CML::pow(h_, 3.0)))
          *(
               CML::sqrt(G_*(h_ + H_))*time
             - ((x - x0_) & propagationDirection_)
           )
        ), 4.0 )
        *CML::tanh(CML::sqrt(3*H_/(4.0*CML::pow(h_, 3.0)))
        *(
             CML::sqrt(G_*(h_ + H_))*time
           - ((x - x0_) & propagationDirection_)
         ));

    return Uhorz*propagationDirection_ - Uvert*direction_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// ************************************************************************* //
