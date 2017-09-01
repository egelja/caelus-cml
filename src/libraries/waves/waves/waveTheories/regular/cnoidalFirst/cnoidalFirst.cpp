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

#include "cnoidalFirst.hpp"
#include "addToRunTimeSelectionTable.hpp"

#include "gsl_sf_ellint.h"
#include "gsl_sf_elljac.h"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(cnoidalFirst, 0);
addToRunTimeSelectionTable(waveTheory, cnoidalFirst, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


cnoidalFirst::cnoidalFirst
(
    const word& subDictName,
    const fvMesh& mesh_
)
:
    waveTheory(subDictName, mesh_),
    H_(readScalar(coeffDict_.lookup("height"))),
    h_(readScalar(coeffDict_.lookup("depth"))),
    omega_(readScalar(coeffDict_.lookup("omega"))),

    propagationDirection_(vector(coeffDict_.lookup("direction"))),
    m_(readScalar(coeffDict_.lookup("m"))),
    length_(readScalar(coeffDict_.lookup("length"))),
    celerity_(readScalar(coeffDict_.lookup("celerity")))
{
    scalar Eelliptic = gsl_sf_ellint_Ecomp(CML::sqrt(m_), GSL_PREC_DOUBLE);

    period_    = 2.0*PI_/omega_;

    Kelliptic_ = gsl_sf_ellint_Kcomp( CML::sqrt(m_), GSL_PREC_DOUBLE);
    etaMin_    = ((1.0 - Eelliptic/Kelliptic_)/m_ - 1.0)*H_;

    propagationDirection_ /= CML::mag(propagationDirection_);

    Tsoft_ = coeffDict_.lookupOrDefault<scalar>("Tsoft",period_);

    checkWaveDirection(propagationDirection_);
}


void cnoidalFirst::printCoeffs()
{
    Info << "Loading wave theory: " << typeName << endl;
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //


scalar cnoidalFirst::factor(const scalar& time) const
{
    scalar factor(1.0);
    if (Tsoft_ > 0.0)
    {
        factor = CML::sin(2*PI_/(4.0*Tsoft_)*CML::min(Tsoft_, time));
    }

    return factor;
}


scalar cnoidalFirst::argument
(
    const point& x,
    const scalar& time
) const
{
    scalar arg = 2.0*Kelliptic_
        *(time/period_ - (propagationDirection_ & x)/length_);

    return arg;
}


scalar cnoidalFirst::eta_x
(
    const scalar& sn,
    const scalar& cn,
    const scalar& dn
) const
{
    scalar val( 4.0*H_*Kelliptic_/length_*( cn*dn*sn ));
    return val;
}


scalar cnoidalFirst::eta_xx
(
    const scalar& sn,
    const scalar& cn,
    const scalar& dn
) const
{
    scalar val(
                CML::pow(dn, 2.0)*CML::pow(sn, 2.0)
              + CML::pow(cn, 2.0)
              * (
                    - CML::pow(dn, 2.0) + m_*CML::pow(sn, 2.0)
                )
              );

    val *= 8.0*H_*CML::pow(Kelliptic_/length_, 2.0);

    return val;
}


scalar cnoidalFirst::eta_xxx
(
    const scalar& sn,
    const scalar& cn,
    const scalar& dn
) const
{
    scalar val(
                - cn*dn*sn
                * (
                      CML::pow(dn, 2.0)
                    + m_*(CML::pow(cn, 2.0) - CML::pow(sn, 2.0))
                  )
              );

    val *= 64.0*H_*CML::pow(Kelliptic_/length_, 3.0);

    return val;
}


// * * * * * * * * * * * * * Public Member Functions * * * * * * * * * * * * //

scalar cnoidalFirst::eta
(
    const point& x,
    const scalar& time
) const
{
    scalar arg(argument(x,time));

    scalar snn(0.0), cnn(0.0), dnn(0.0);
    gsl_sf_elljac_e( arg, m_, &snn, &cnn, &dnn);


    scalar eta = (etaMin_ + H_*CML::pow(cnn,2.0))*factor(time) + seaLevel_;

    return eta;
}


scalar cnoidalFirst::ddxPd
(
    const point& x,
    const scalar& time,
    const vector& unitVector
) const
{

    scalar Z(returnZ(x));
    scalar arg(argument(x,time));

    scalar snn(0.0), cnn(0.0), dnn(0.0);
    gsl_sf_elljac_e( arg, m_, &snn, &cnn, &dnn);

    scalar ddxPd(0);

    ddxPd  =   rhoWater_*CML::mag(g_)
             * (
                 eta_x(snn, cnn, dnn) + 1.0/2.0*CML::pow(h_, 2.0)
                *(1 - CML::pow((Z + h_)/h_, 2.0))*eta_xxx(snn, cnn, dnn)
               );

    ddxPd *= factor(time);

    return ddxPd;
}


vector cnoidalFirst::U
(
    const point& x,
    const scalar& time
) const
{
    scalar Z(returnZ(x));
    scalar arg(argument(x, time));

    scalar snn(0.0), cnn(0.0), dnn(0.0);
    gsl_sf_elljac_e( arg, m_, &snn, &cnn, &dnn);

    scalar etaVal = eta(x,time);

    scalar Uhorz =   celerity_
                   * (
                         etaVal/h_
                       - CML::pow(etaVal/h_, 2.0)
                       + 1.0/2.0 *
                         (
                            1.0/3.0
                          - CML::pow((Z + h_)/h_, 2.0)
                         )
                       * h_*eta_xx(snn, cnn, dnn)
                     );

    Uhorz       *= factor(time);

    scalar Uvert = - celerity_*(Z + h_)
                   * (
                        eta_x(snn, cnn, dnn)/h_*(1 - 2.0*etaVal/h_)
                      + 1.0/6.0*h_*eta_xxx(snn, cnn, dnn)
                      * (1 - CML::pow((Z + h_)/h_, 2.0))
                     );

    Uvert       *= factor(time);

    // Note "-" because of "g" working in the opposite direction
    return Uhorz*propagationDirection_ - Uvert*direction_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// ************************************************************************* //
