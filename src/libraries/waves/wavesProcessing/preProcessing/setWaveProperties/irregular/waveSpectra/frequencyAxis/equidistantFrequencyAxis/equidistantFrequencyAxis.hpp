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

Class
    CML::equidistantFrequencyAxis

Description
    Set random frequencyAxis for the irregular wave train.

SourceFiles
    equidistantFrequencyAxis.cpp

Author
    Niels Gjøl Jacobsen, Deltares.


\*---------------------------------------------------------------------------*/

#ifndef equidistantFrequencyAxis_HPP
#define equidistantFrequencyAxis_HPP

#include "frequencyAxis.hpp"
#include <time.h>

namespace CML
{

class equidistantFrequencyAxis
:
    public frequencyAxis
{
private:

public:

        //- Runtime type information
        TypeName("equidistantFrequencyAxis");

        equidistantFrequencyAxis
        (
            const Time&,
            dictionary&
        );

        virtual scalarField freqAxis
        (
            const scalarField&,
            const scalarField&,
            const label&
        ) const;

        scalarField freqAxis
        (
            const label&
        ) const;

};

}

#endif

// ************************************************************************* //
