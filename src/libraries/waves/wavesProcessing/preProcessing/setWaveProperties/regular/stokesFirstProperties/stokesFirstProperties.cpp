/*---------------------------------------------------------------------------*\
Copyright  Niels Gjøl Jacobsen, Technical University of Denmark.
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

#include "stokesFirstProperties.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(stokesFirstProperties, 0);
addToRunTimeSelectionTable
(
    setWaveProperties,
    stokesFirstProperties,
    setWaveProperties
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


stokesFirstProperties::stokesFirstProperties
(
    const Time& rT,
    dictionary& dict
)
:
    setWaveProperties(rT, dict, false)
{
//    Info << "\nConstructing: " << this->type() << "(Dummy)"<< endl;

    period_ = 0.0;
    depth_  = 0.0;
    omega_  = 0.0;
}


stokesFirstProperties::stokesFirstProperties
(
    const Time& rT,
    dictionary& dict,
    bool write
)
:
    setWaveProperties(rT, dict, write)
{
    Info << "\nConstructing: " << this->type() << endl;

    period_ = readScalar(dict.lookup("period"));
    depth_  = readScalar(dict.lookup("depth"));
    omega_  = 2.0*PI_/period_ ;
}


stokesFirstProperties::stokesFirstProperties
(
    const Time& rT,
    dictionary& dict,
    bool write,
    word string
)
:
    setWaveProperties(rT, dict, write)
{
    Info << "\nConstructing: " << this->type()
         << " (Used by another wave theory)";

    period_ = readScalar(dict.lookup("period"+string));
    depth_  = readScalar(dict.lookup("depth"));
    omega_  = 2.0*PI_/period_;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void stokesFirstProperties::set(Ostream& os)
{
    scalar k = linearWaveNumber();

    // Write the beginning of the sub-dictionary
    writeBeginning(os);

    // Write the already given parameters
    writeGiven(os, "waveType");

    if (dict_.found("Tsoft"))
    {
        writeGiven(os, "Tsoft");
    }

    writeGiven(os, "depth");
    writeGiven(os, "period");
    writeGiven(os, "direction");
    writeGiven(os, "phi");
    writeGiven(os, "height");

    if (write_)
    {
        vector direction(vector(dict_.lookup("direction")));
        direction /= CML::mag(direction);
        direction *= k;

        writeDerived(os, "waveNumber", direction);
        writeDerived(os, "omega", omega_);
    }

    // Write the relaxation zone
    writeRelaxationZone(os);

    // Write the closing bracket
    writeEnding(os);
}


scalar stokesFirstProperties::linearWaveNumber() const
{
    scalar lower(0.0);

    scalar upper = CML::max
        (
            4.0*PI_/(period_*CML::sqrt(CML::mag(G_)*depth_)),
            2.0*PI_/(CML::pow(period_, 2.0))
        );

    scalar middle(0.5*(lower + upper));

    scalar tanhMax(100);

    scalar valLower = CML::pow(omega_, 2.0)
        - CML::mag(G_)*lower*CML::tanh(CML::min(lower*depth_, tanhMax));
    scalar valUpper = CML::pow(omega_, 2.0)
        - CML::mag(G_)*upper*CML::tanh(CML::min(upper*depth_, tanhMax));
    scalar valMiddle = CML::pow(omega_, 2.0)
        - CML::mag(G_)*middle*CML::tanh(CML::min(middle*depth_, tanhMax));

    while (true)
    {
        if (CML::sign(valLower) == CML::sign(valMiddle))
        {
            lower    = middle;
            valLower = valMiddle;
        }
        else
        {
            upper    = middle;
            valUpper = valMiddle;
        }

        middle = 0.5*(lower + upper);

        valMiddle = CML::pow(omega_, 2.0)
            - CML::mag(G_)*middle
            *CML::tanh(CML::min(middle*depth_, tanhMax));

        if
        (
            CML::mag(valMiddle) < 1.0e-13 ||
            CML::mag(valLower - valUpper)/middle < 1.0e-13
        )
        {
            break;
        }
    }

    return middle;
}


// Note that the frequency is NOT the cyclic frequency
scalar stokesFirstProperties::linearWaveNumber
(
    const scalar& depth,
    const scalar& frequency
)
{
    depth_ = depth;
    omega_ = 2.0*PI_*frequency;
    period_ = 1.0/frequency;

    return linearWaveNumber();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
