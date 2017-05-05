/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2010 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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

Contributors/Copyright:
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "CloudProxyForReactingMultiphaseParcel.hpp"

#include "addToRunTimeSelectionTable.hpp"

#include "swakCloudTypes.hpp"

#include "basicReactingMultiphaseCloud.hpp"
#include "basicReactingCloud.hpp"

#include "basicKinematicCloud.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{

    addCloudProxyToTable(CloudProxyForKinematicParcel,basicKinematicCloud);

    addCloudProxyToTable(CloudProxyForThermoParcel,swakFluidThermoCloudType);

    addCloudProxyToTable(CloudProxyForReactingParcel,basicReactingCloud);

    addCloudProxyToTable(CloudProxyForReactingMultiphaseParcel,basicReactingMultiphaseCloud);

} // namespace end

// ************************************************************************* //
