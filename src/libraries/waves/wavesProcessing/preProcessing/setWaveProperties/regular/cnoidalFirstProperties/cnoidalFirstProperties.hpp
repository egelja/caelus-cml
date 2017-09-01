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
    CML::cnoidalFirstProperties

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
    cnoidalFirstProperties.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef cnoidalFirstProperties_HPP
#define cnoidalFirstProperties_HPP

#include "setWaveProperties.hpp"

namespace CML
{

class cnoidalFirstProperties
:
    public setWaveProperties
{
private:

    scalar T_;
    scalar d_;
    scalar H_;

    double solve( );

public:

    //- Runtime type information
        TypeName("cnoidalFirstProperties");

        cnoidalFirstProperties
        (
            const Time&,
            dictionary&,
            bool
        );

    // Method

        virtual void set(Ostream&);
};

struct cnoidalFirstParams
{
    double depth_;
    double height_;
    double period_;
    double g_;
};

double lowerMBound_f (double m, void *params);

double cnoidalFirst_f (double m, void *params);

}

#endif

// ************************************************************************* //
