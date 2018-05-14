/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "Constant.hpp"
#include "Uniform.hpp"
#include "ZeroConstant.hpp"
#include "OneConstant.hpp"
#include "PolynomialEntry.hpp"
#include "Sine.hpp"
#include "Square.hpp"
#include "CSV.hpp"
#include "Table.hpp"
#include "TableFile.hpp"
#include "Scale.hpp"

#include "fieldTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeDataEntrys(Type)                                                   \
    makeDataEntry(Type);                                                       \
    makeDataEntryType(Constant, Type);                                         \
    makeDataEntryType(Uniform, Type);                                          \
    makeDataEntryType(ZeroConstant, Type);                                     \
    makeDataEntryType(OneConstant, Type);                                      \
    makeDataEntryType(Polynomial, Type);                                       \
    makeDataEntryType(Sine, Type);                                             \
    makeDataEntryType(Square, Type);                                           \
    makeDataEntryType(CSV, Type);                                              \
    makeDataEntryType(Table, Type);                                            \
    makeDataEntryType(TableFile, Type);                                        \
    makeDataEntryType(Scale, Type);

namespace CML
{
    makeDataEntry(label);
    makeDataEntryType(Constant, label);

    makeDataEntrys(scalar);
    makeDataEntrys(vector);
    makeDataEntrys(sphericalTensor);
    makeDataEntrys(symmTensor);
    makeDataEntrys(tensor);
}


// ************************************************************************* //
