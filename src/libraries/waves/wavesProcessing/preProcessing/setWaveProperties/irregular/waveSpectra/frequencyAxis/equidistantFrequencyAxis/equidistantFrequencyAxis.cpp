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

#include "equidistantFrequencyAxis.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

defineTypeNameAndDebug(equidistantFrequencyAxis, 0);
addToRunTimeSelectionTable(frequencyAxis, equidistantFrequencyAxis, frequencyAxis);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


equidistantFrequencyAxis::equidistantFrequencyAxis
(
    const Time& rT,
    dictionary& dict
)
:
    frequencyAxis(rT, dict)
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


scalarField equidistantFrequencyAxis::freqAxis
(
    const scalarField&,
    const scalarField&,
    const label& N
) const
{
    scalarField freq(N + 1, 0.0);

    scalar df = (fu_ - fl_)/static_cast<scalar>(N);

    for (int i = 0; i < N + 1; i++)
    {
        freq[i] = static_cast<scalar>(i)*df + fl_;
    }

    return freq;
}


scalarField equidistantFrequencyAxis::freqAxis(const label& N) const
{
    scalarField dummy(0);

    return freqAxis(dummy, dummy, N);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
