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
    CML::focusingPhase

Description
    Set phases such that the irregular wave train focuses at a given time at
    a given location.

SourceFiles
    focusingPhase.cpp

Author
    Niels Gjøl Jacobsen, Deltares.


\*---------------------------------------------------------------------------*/

#ifndef focusingPhase_HPP
#define focusingPhase_HPP

#include "phases.hpp"
#include <time.h>

namespace CML
{

class focusingPhase
:
    public phases
{
private:

        scalar focusTime_;
        vector focusPoint_;

public:

        //- Runtime type information
        TypeName("focusingPhase");

        focusingPhase
        (
            const Time&,
            dictionary&
        );

        virtual scalar phase(const scalar&, const vector&);
};

}

#endif

// ************************************************************************* //
