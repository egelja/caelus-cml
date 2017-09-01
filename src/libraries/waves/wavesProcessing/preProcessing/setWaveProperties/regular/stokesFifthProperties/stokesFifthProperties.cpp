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

#include "stokesFifthProperties.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(stokesFifthProperties, 0);
addToRunTimeSelectionTable
(
    setWaveProperties,
    stokesFifthProperties,
    setWaveProperties
);

// * * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * //


scalar stokesFifthProperties::eval(scalar& k)
{
    scalar S  = 1.0/CML::cosh(2*k * depth_);
    scalar C0 = CML::sqrt(CML::tanh(k*depth_));
    scalar C2 = CML::sqrt(CML::tanh(k*depth_))*(2.0 + 7.0*CML::pow(S, 2.0))
        /( 4.0*CML::pow(1.0 - S, 2.0) );
    scalar C4 = CML::sqrt(CML::tanh(k*depth_))
        *(4.0 + 32.0*S - 116.0*CML::pow(S, 2.0) - 400.0*CML::pow(S, 3.0)
        - 71.0*CML::pow(S, 4.0) + 146.0*CML::pow(S, 5.0))
        /(32.0*CML::pow(1.0 - S, 5.0) );
    scalar D2 = - CML::sqrt(1.0/CML::tanh(k*depth_))/2.0;
    scalar D4 = CML::sqrt(1.0/CML::tanh(k*depth_))
        *(2.0 + 4.0*S + CML::pow(S,2.0) + 2.0*CML::pow(S,3.0))
        /(8.0*CML::pow(1.0 - S,3.0));

    return CML::sqrt(k/G_)*Q_ - 2.0*PI_/(period_*CML::sqrt(G_*k))
         + C0 + CML::pow((k*height_/2.0),2.0)*(C2 + D2/(k*depth_))
         + CML::pow(k*height_/2.0, 4.0)*(C4 + D4/(k*depth_));
}


scalar stokesFifthProperties::waveNumber()
{
    scalar lower(1.0e-7);

    scalar upper = CML::max
        (
            4.0*PI_/(period_*CML::sqrt(CML::mag(G_)*depth_)),
            2.0*PI_/(CML::pow(period_, 2.0))
        );

    scalar middle(0.5*(lower + upper));

    scalar valLower(eval( lower )), valMiddle(eval(middle));

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
        }

        middle = 0.5*(lower + upper);

        valMiddle = eval(middle);

        if (CML::mag(valMiddle) < 1.0e-13)
        {
            break;
        }
    }

    return middle;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


stokesFifthProperties::stokesFifthProperties
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
    height_ = readScalar(dict.lookup("height"));
    Q_      = readScalar(dict.lookup("stokesDrift"));

    omega_  = 2.0*PI_/period_ ;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void stokesFifthProperties::set(Ostream& os)
{
    scalar k = waveNumber();

    // Write the beginning of the sub-dictionary
    writeBeginning(os);

    // Write the already given parameters
    writeGiven(os, "waveType");

    writeGiven(os, "height");
    writeGiven(os, "period");

    writeGiven(os, "depth");
    writeGiven(os, "stokesDrift");
    writeGiven(os, "direction");

    if (dict_.found("Tsoft"))
    {
        writeGiven(os, "Tsoft");
    }

    writeGiven(os, "phi");

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


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
