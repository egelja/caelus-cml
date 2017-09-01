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

#include "irregular.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace waveTheories
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

defineTypeNameAndDebug(irregular, 0);
addToRunTimeSelectionTable(waveTheory, irregular, dictionary);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


irregular::irregular
(
    const word& subDictName,
    const fvMesh& mesh_
)
:
    waveTheory(subDictName, mesh_),
    N_(readScalar(coeffDict_.lookup("N"))),
    h_(readScalar(coeffDict_.lookup("depth"))),
    amp_("amplitude", coeffDict_, N_),
    omega_("frequency", coeffDict_, N_),
    phi_("phaselag", coeffDict_, N_),
    k_("waveNumber", coeffDict_, N_),
    K_(N_),
    compDir_(N_),
    period_(N_, 0),
    velAmp_(N_, 0),

    Tsoft_( readScalar(coeffDict_.lookup("Tsoft")))
{
    omega_ *= (2.0*PI_);

    // Compute the length of k_
    K_ = CML::mag(k_);

    compDir_ = k_ / K_;

    // Compute the period
    forAll (period_, index)
    {
        period_[index] = 2.0*PI_/omega_[index];
    }

    // Compute the velocity amplitude
    forAll (velAmp_, index)
    {
        velAmp_[index] = PI_*2.0*amp_[index]/period_[index]
            /CML::sinh(K_[index]*h_);
    }

    checkWaveDirection(k_);
}


void irregular::printCoeffs()
{
    Info << "Loading wave theory: " << typeName << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


scalar irregular::factor(const scalar& time) const
{
    scalar factor(1.0);
    if (0.0 < Tsoft_ && time < Tsoft_)
    {
        factor = CML::sin(2*PI_/(4.0*Tsoft_)*CML::min(Tsoft_, time));
    }

    return factor;
}


scalar irregular::eta
(
    const point& x,
    const scalar& time
) const
{
    scalar eta(0);

    forAll (amp_, index)
    {
        scalar arg = omega_[index]*time - (k_[index] & x) + phi_[index];
        eta += amp_[index]*CML::cos(arg);
    }
    eta *= factor(time);
    eta += seaLevel_;

    return eta;
}


scalar irregular::ddxPd
(
    const point& x,
    const scalar& time,
    const vector& unitVector
) const
{
    return 0.0;
}


vector irregular::U
(
    const point& x,
    const scalar& time
) const
{
    scalar Z(returnZ(x));

    vector U(vector::zero);

    forAll (amp_, index)
    {
        scalar arg0 = omega_[index]*time - (k_[index] & x) + phi_[index];
        scalar arg1 = K_[index]*(Z + h_);

        scalar Uhorz = velAmp_[index]*CML::cosh(arg1)*CML::cos(arg0);
        scalar Uvert = - velAmp_[index]*CML::sinh(arg1)*CML::sin(arg0);

        // Note "-" because of "g" working in the opposite direction
        U += Uhorz*compDir_[index] - Uvert*direction_;
    }

    U *= factor(time);

    return U;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace waveTheories
} // End namespace CML

// ************************************************************************* //
