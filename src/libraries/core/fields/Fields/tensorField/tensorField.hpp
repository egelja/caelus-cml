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
    CML::tensorField

Description
    Specialisation of Field\<T\> for tensor.

SourceFiles
    tensorField.cpp

\*---------------------------------------------------------------------------*/

#ifndef tensorField_H
#define tensorField_H

#include "scalarField.hpp"
#include "vectorField.hpp"
#include "sphericalTensorField.hpp"
#include "symmTensorField.hpp"
#include "tensor.hpp"

#define TEMPLATE
#include "FieldFunctionsM.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

typedef Field<tensor> tensorField;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

UNARY_FUNCTION(scalar, tensor, tr)
UNARY_FUNCTION(sphericalTensor, tensor, sph)
UNARY_FUNCTION(symmTensor, tensor, symm)
UNARY_FUNCTION(symmTensor, tensor, twoSymm)
UNARY_FUNCTION(tensor, tensor, skew)
UNARY_FUNCTION(tensor, tensor, dev)
UNARY_FUNCTION(tensor, tensor, dev2)
UNARY_FUNCTION(scalar, tensor, det)
UNARY_FUNCTION(tensor, tensor, cof)
UNARY_FUNCTION(tensor, tensor, inv)
UNARY_FUNCTION(vector, tensor, eigenValues)
UNARY_FUNCTION(tensor, tensor, eigenVectors)

UNARY_FUNCTION(vector, symmTensor, eigenValues)
UNARY_FUNCTION(tensor, symmTensor, eigenVectors)


// * * * * * * * * * * * * * * * global operators  * * * * * * * * * * * * * //

UNARY_OPERATOR(vector, tensor, *, hdual)
UNARY_OPERATOR(tensor, vector, *, hdual)

BINARY_OPERATOR(vector, vector, tensor, /, divide)
BINARY_TYPE_OPERATOR(vector, vector, tensor, /, divide)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "undefFieldFunctionsM.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
