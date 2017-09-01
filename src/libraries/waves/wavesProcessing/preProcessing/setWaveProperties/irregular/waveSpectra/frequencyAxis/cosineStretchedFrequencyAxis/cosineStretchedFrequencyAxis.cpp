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

#include "cosineStretchedFrequencyAxis.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(cosineStretchedFrequencyAxis, 0);
addToRunTimeSelectionTable(frequencyAxis, cosineStretchedFrequencyAxis, frequencyAxis);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


cosineStretchedFrequencyAxis::cosineStretchedFrequencyAxis
(
    const Time& rT,
    dictionary& dict
)
:
    frequencyAxis(rT, dict)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


scalarField cosineStretchedFrequencyAxis::freqAxis
(
    const scalarField&,
    const scalarField&,
    const label& N
) const
{
    scalarField freq(N + 1, 0.0);

    scalar fp = 1.0/readScalar(dict_.lookup("Tp"));

    // Calculate the number of upper and lower frequencies
    label Nlow(ceil((fp - fl_)/(fu_ - fp)*(N + 1)));
    label Nhigh(N + 1 - Nlow);

    for (int i = 0; i < Nlow; i++)
    {
        freq[i] = (fp - fl_)*CML::sin(2*M_PI/(4.0*Nlow)*i) + fl_;
    }

    for (int i = 0; i <= Nhigh; i++)
    {
        freq[Nlow - 1 + i] =
            (fu_ - fp)*(- CML::cos(2*M_PI/(4*Nhigh)*i) + 1) + fp;
    }

    return freq;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
