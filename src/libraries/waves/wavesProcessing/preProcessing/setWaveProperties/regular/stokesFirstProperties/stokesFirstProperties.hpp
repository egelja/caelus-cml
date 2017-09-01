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
    CML::stokesFirstProperties

Description

    A description of the general wave framework is given in

    @article { jacobsenFuhrmanFredsoe2011,
        Author = {Jacobsen, N G and Fuhrman, D R and Freds\o{}e, J},
        title = {{A Wave Generation Toolbox for the Open-Source CFD Library: OpenFoam\textregistered{}}},
        Journal = {{Int. J. for Numer. Meth. Fluids}},
        Year = {2012},
        Volume = {70},
        Number = {9},
        Pages = {1073-1088},
        DOI = {{10.1002/fld.2726}},
    }

SourceFiles
    setWaveProperties.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef stokesFirstProperties_HPP
#define stokesFirstProperties_HPP


#include "setWaveProperties.hpp"

namespace CML
{

class stokesFirstProperties
:
    public setWaveProperties
{
private:

    scalar period_;
    scalar omega_;
    scalar depth_;

public:

        //- Runtime type information
        TypeName("stokesFirstProperties");

        stokesFirstProperties
        (
            const Time&,
            dictionary&
        );

        stokesFirstProperties
        (
            const Time&,
            dictionary&,
            bool
        );

        stokesFirstProperties
        (
            const Time&,
            dictionary&,
            bool,
            word
        );

        // Method

        virtual void set(Ostream&);

        scalar linearWaveNumber() const;

        scalar linearWaveNumber(const scalar&, const scalar&);

        const scalar& omega() const {return omega_;};
};

}

#endif

// ************************************************************************* //
