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
    CML::tensor

Description
    Tensor of scalars.

SourceFiles
    tensor.cpp

\*---------------------------------------------------------------------------*/

#ifndef tensor_HPP
#define tensor_HPP

#include "Tensor_.hpp"
#include "vector.hpp"
#include "sphericalTensor.hpp"
#include "symmTensor.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

typedef Tensor<scalar> tensor;

vector eigenValues(const tensor& T);
vector eigenVector
(
    const tensor& T,
    const scalar lambda,
    const vector& direction1,
    const vector& direction2
);
tensor eigenVectors(const tensor& T, const vector& lambdas);
tensor eigenVectors(const tensor& T);

vector eigenValues(const symmTensor& T);
vector eigenVector
(
    const symmTensor& T,
    const scalar lambda,
    const vector& direction1,
    const vector& direction2
);
tensor eigenVectors(const symmTensor& T, const vector& lambdas);
tensor eigenVectors(const symmTensor& T);

//- Data associated with tensor type are contiguous
template<>
inline bool contiguous<tensor>() {return true;}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
