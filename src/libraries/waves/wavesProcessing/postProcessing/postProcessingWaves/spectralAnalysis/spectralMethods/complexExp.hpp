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

Global
    The complex exponential.

SourceFiles
    complexExp.cpp

Author
    Niels G. Jacobsen, Technical University of Denmark

\*---------------------------------------------------------------------------*/

#ifndef complexExp_HPP
#define complexExp_HPP

#include "complex.hpp"

namespace CML
{
    //- Computes the complex exponentital
    CML::complex exp(const CML::complex&);

} // End namespace CML

#endif
