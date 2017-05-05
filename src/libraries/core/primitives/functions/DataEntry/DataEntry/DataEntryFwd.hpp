/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#ifndef DataEntryFws_H
#define DataEntryFws_H

#include "DataEntry.hpp"
#include "vector.hpp"
#include "symmTensor.hpp"
#include "sphericalTensor.hpp"
#include "tensor.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    typedef DataEntry<label> labelDataEntry;
    typedef DataEntry<scalar> scalarDataEntry;
    typedef DataEntry<vector> vectorDataEntry;
    typedef DataEntry<symmTensor> symmTensorDataEntry;
    typedef DataEntry<sphericalTensor> sphericalTensorDataEntry;
    typedef DataEntry<tensor> tensorDataEntry;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
