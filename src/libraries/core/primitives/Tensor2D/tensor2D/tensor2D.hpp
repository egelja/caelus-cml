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
    CML::tensor2D

Description
    Tensor2D or scalars.

SourceFiles
    tensor2D.cpp

\*---------------------------------------------------------------------------*/

#ifndef tensor2D_H
#define tensor2D_H

#include "Tensor2D_.hpp"
#include "vector2D.hpp"
#include "contiguous.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

typedef Tensor2D<scalar> tensor2D;

vector2D eigenValues(const tensor2D& t);
vector2D eigenVector
(
    const tensor2D& t,
    const scalar lambda,
    const vector2D& direction1
);
tensor2D eigenVectors(const tensor2D& t, const vector2D& lambdas);
tensor2D eigenVectors(const tensor2D& t);

//- Data associated with tensor2D type are contiguous
template<>
inline bool contiguous<tensor2D>() {return true;}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "sphericalTensor2D.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
