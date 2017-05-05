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

Namespace
    CML::fvc

Description
    Namespace of functions to calculate explicit derivatives.

\*---------------------------------------------------------------------------*/

#ifndef fvc_H
#define fvc_H

#include "fv.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "surfaceInterpolate.hpp"
#include "fvcVolumeIntegrate.hpp"
#include "fvcSurfaceIntegrate.hpp"
#include "fvcAverage.hpp"
#include "fvcReconstruct.hpp"
#include "fvcDdt.hpp"
#include "fvcDDt_subst.hpp"
#include "fvcD2dt2.hpp"
#include "fvcDiv.hpp"
#include "fvcFlux.hpp"
#include "fvcGrad.hpp"
#include "fvcMagSqrGradGrad.hpp"
#include "fvcSnGrad.hpp"
#include "fvcCurl.hpp"
#include "fvcLaplacian.hpp"
#include "fvcSup.hpp"
#include "fvcMeshPhi.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
