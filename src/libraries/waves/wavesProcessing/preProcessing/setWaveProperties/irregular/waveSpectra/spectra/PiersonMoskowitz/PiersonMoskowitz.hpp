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
    CML::PiersonMoskowitz

Description

SourceFiles
    PiersonMoskowitz.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef PiersonMoskowitz_HPP
#define PiersonMoskowitz_HPP

#include "stokesFirstProperties.hpp"
#include "waveSpectra.hpp"

namespace CML
{

class PiersonMoskowitz
:
    public waveSpectra
{
private:

        scalarField spectralValue
        (
            const scalar&,
            const scalar&,
            const scalarField&
        ) const;

public:

        //- Runtime type information
        TypeName("PiersonMoskowitz");

        PiersonMoskowitz
        (
            const Time&,
            dictionary&,
            scalarField&,
            scalarField&,
            scalarField&,
            vectorField&
        );

        virtual void set(Ostream&);

        virtual wordList list();
};

}

#endif

// ************************************************************************* //
