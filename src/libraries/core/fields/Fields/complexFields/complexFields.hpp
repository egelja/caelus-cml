/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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

Typedef
    CML::complexField

Description
    Specialisation of Field\<T\> for complex.

Typedef
    CML::complexVectorField

Description
    Specialisation of Field\<T\> for complexVector.

SourceFiles
    complexFields.cpp

\*---------------------------------------------------------------------------*/

#ifndef complexFields_H
#define complexFields_H

#include "complex.hpp"
#include "complexVector.hpp"
#include "primitiveFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

typedef Field<complex> complexField;

complexField ComplexField(const UList<scalar>&, const UList<scalar>&);
complexField ReComplexField(const UList<scalar>&);
complexField ImComplexField(const UList<scalar>&);
scalarField Re(const UList<complex>&);
scalarField Im(const UList<complex>&);
scalarField ReImSum(const UList<complex>&);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

typedef Field<complexVector> complexVectorField;

complexVectorField ComplexField(const UList<vector>&, const UList<vector>&);
complexVectorField ReComplexField(const UList<vector>&);
complexVectorField ImComplexField(const UList<vector>&);
vectorField Re(const UList<complexVector>&);
vectorField Im(const UList<complexVector>&);
vectorField ReImSum(const UList<complexVector>&);

complexVectorField operator^
(
    const UList<vector>&,
    const UList<complexVector>&
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
