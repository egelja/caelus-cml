/*---------------------------------------------------------------------------*\
Copyright (C) 2013 OpenFOAM Foundation
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

#include "ensightPTraits.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const char* const CML::ensightPTraits<CML::scalar>::typeName =
    CML::pTraits<CML::scalar>::typeName;

const char* const CML::ensightPTraits<CML::vector>::typeName =
    CML::pTraits<CML::vector>::typeName;

const char* const CML::ensightPTraits<CML::sphericalTensor>::typeName =
    CML::pTraits<CML::scalar>::typeName;

const char* const CML::ensightPTraits<CML::symmTensor>::typeName =
    "tensor symm";

const char* const CML::ensightPTraits<CML::tensor>::typeName =
    "tensor asym";


// ************************************************************************* //
