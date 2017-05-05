/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2012 OpenFOAM Foundation
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

#ifndef makeReactingParcelInjectionModels_H
#define makeReactingParcelInjectionModels_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "CellZoneInjection.hpp"
#include "ConeInjection.hpp"
#include "ConeNozzleInjection.hpp"
#include "FieldActivatedInjection.hpp"
#include "ManualInjection.hpp"
#include "NoInjection.hpp"
#include "PatchInjection.hpp"
#include "PatchFlowRateInjection.hpp"
#include "TranCellZoneInjection.hpp"
#include "ReactingLookupTableInjection.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeReactingParcelInjectionModels(CloudType)                          \
                                                                              \
    makeInjectionModel(CloudType);                                            \
    makeInjectionModelType(CellZoneInjection, CloudType);                     \
    makeInjectionModelType(ConeInjection, CloudType);                         \
    makeInjectionModelType(ConeNozzleInjection, CloudType);                   \
    makeInjectionModelType(FieldActivatedInjection, CloudType);               \
    makeInjectionModelType(ManualInjection, CloudType);                       \
    makeInjectionModelType(NoInjection, CloudType);                           \
    makeInjectionModelType(PatchInjection, CloudType);                        \
    makeInjectionModelType(PatchFlowRateInjection, CloudType);                \
    makeInjectionModelType(TranCellZoneInjection, CloudType);                 \
    makeInjectionModelType(ReactingLookupTableInjection, CloudType);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
