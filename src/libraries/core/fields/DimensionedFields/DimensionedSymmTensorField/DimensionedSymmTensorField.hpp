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

InClass
    CML::DimensionedSymmTensorField

Description
    SymmTensor specific part of the implementation of DimensionedField.


\*---------------------------------------------------------------------------*/

#ifndef DimensionedSymmTensorField_H
#define DimensionedSymmTensorField_H

#include "DimensionedField.hpp"
#include "symmTensor.hpp"

#define TEMPLATE template<class GeoMesh>
#include "DimensionedFieldFunctionsM.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

UNARY_FUNCTION(symmTensor, vector, sqr, transform)

UNARY_FUNCTION(scalar, symmTensor, tr, transform)
UNARY_FUNCTION(sphericalTensor, symmTensor, sph, transform)
UNARY_FUNCTION(symmTensor, symmTensor, symm, transform)
UNARY_FUNCTION(symmTensor, symmTensor, twoSymm, transform)
UNARY_FUNCTION(symmTensor, symmTensor, dev, transform)
UNARY_FUNCTION(symmTensor, symmTensor, dev2, transform)
UNARY_FUNCTION(scalar, symmTensor, det, transform)
UNARY_FUNCTION(symmTensor, symmTensor, cof, cof)
UNARY_FUNCTION(symmTensor, symmTensor, inv, inv)


// * * * * * * * * * * * * * * * global operators  * * * * * * * * * * * * * //

UNARY_OPERATOR(vector, symmTensor, *, hdual, transform)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "undefFieldFunctionsM.hpp"

#include "symmTensorField.hpp"

#define TEMPLATE template<class GeoMesh>
#include "DimensionedFieldFunctionsM.hxx"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

UNARY_FUNCTION(symmTensor, vector, sqr, transform)

UNARY_FUNCTION(scalar, symmTensor, tr, transform)
UNARY_FUNCTION(sphericalTensor, symmTensor, sph, transform)
UNARY_FUNCTION(symmTensor, symmTensor, symm, transform)
UNARY_FUNCTION(symmTensor, symmTensor, twoSymm, transform)
UNARY_FUNCTION(symmTensor, symmTensor, dev, transform)
UNARY_FUNCTION(symmTensor, symmTensor, dev2, transform)
UNARY_FUNCTION(scalar, symmTensor, det, transform)
UNARY_FUNCTION(symmTensor, symmTensor, cof, cof)
UNARY_FUNCTION(symmTensor, symmTensor, inv, inv)


// * * * * * * * * * * * * * * * global operators  * * * * * * * * * * * * * //

UNARY_OPERATOR(vector, symmTensor, *, hdual, transform)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "undefFieldFunctionsM.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
