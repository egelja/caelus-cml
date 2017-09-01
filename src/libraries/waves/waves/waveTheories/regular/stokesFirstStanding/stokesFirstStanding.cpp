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

#include "stokesFirstStanding.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(stokesFirstStanding, 0);
addToRunTimeSelectionTable(waveTheory, stokesFirstStanding, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


stokesFirstStanding::stokesFirstStanding
(
    const word& subDictName,
    const fvMesh& mesh_
)
:
    waveTheory(subDictName, mesh_),
    H_(readScalar(coeffDict_.lookup("height"))),
    h_(readScalar(coeffDict_.lookup("depth"))),
    omega_(readScalar(coeffDict_.lookup("omega"))),
    period_(2*PI_/omega_),
    phi_(readScalar(coeffDict_.lookup("phi"))),
    k_(vector(coeffDict_.lookup("waveNumber"))),
    K_(mag(k_)),

    Tsoft_(coeffDict_.lookupOrDefault<scalar>("Tsoft",period_))
{
    checkWaveDirection(k_);
}


void stokesFirstStanding::printCoeffs()
{
    Info << "Loading wave theory: " << typeName << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


scalar stokesFirstStanding::factor(const scalar& time) const
{
    scalar factor(1.0);
    if (Tsoft_ > 0.0)
    {
        factor = CML::sin(omega_/4.0*CML::min(Tsoft_, time));
    }

    return factor;
}


scalar stokesFirstStanding::eta
(
    const point& x,
    const scalar& time
) const
{
    scalar eta =
        (
            H_/2.0*CML::cos(omega_*time - (k_ & x) + phi_)
          + H_/2.0*CML::cos(omega_*time + (k_ & x) + phi_)
        )*factor(time) + seaLevel_;
    return eta;
}


scalar stokesFirstStanding::ddxPd
(
    const point& x,
    const scalar& time,
    const vector& unitVector
) const
{

    scalar Z(returnZ(x));
    scalar arg1(omega_*time - (k_ & x) + phi_);
    scalar arg2(omega_*time + (k_ & x) + phi_);

    scalar ddxPd(0);

    ddxPd = (
                rhoWater_*mag(g_)*K_*H_/2.0*CML::cosh(K_*(Z + h_))
               /CML::cosh(K_*h_)*CML::sin(arg1)
              - rhoWater_*mag(g_)*K_*H_/2.0*CML::cosh(K_*(Z + h_))
               /CML::cosh(K_*h_)*CML::sin(arg2)
            )*factor(time);

    return ddxPd;
}


vector stokesFirstStanding::U
(
    const point& x,
    const scalar& time
) const
{
    scalar Z(returnZ(x));

    scalar Uhorz = PI_*H_/period_ *
                   CML::cosh(K_*(Z + h_))/CML::sinh(K_*h_) *
                   CML::cos(omega_*time - (k_ & x) + phi_)
                 - PI_*H_/period_ *
                   CML::cosh(K_*(Z + h_))/CML::sinh(K_*h_) *
                   CML::cos(omega_*time + (k_ & x) + phi_);

    Uhorz *= factor(time);

    scalar Uvert = - PI_*H_/period_ *
                   CML::sinh(K_*(Z + h_))/CML::sinh(K_*h_) *
                   CML::sin(omega_*time - (k_ & x) + phi_)
                 - PI_*H_/period_ *
                   CML::sinh(K_*(Z + h_))/CML::sinh(K_*h_) *
                   CML::sin(omega_*time + (k_ & x) + phi_);

    Uvert *= factor(time);

    // Note "-" because of "g" working in the opposite direction
    return Uhorz*k_/K_ - Uvert*direction_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// ************************************************************************* //
