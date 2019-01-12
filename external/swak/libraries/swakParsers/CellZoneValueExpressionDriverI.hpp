/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
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
    2010, 2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "surfaceMesh.hpp"
#include "fvsPatchField.hpp"
#include "pointPatchField.hpp"
#include "primitivePatchInterpolation.hpp"

#include <cassert>

namespace CML {
    label CellZoneValueExpressionDriver::size() const
    {
        return cellZone_.size();
    }

    label CellZoneValueExpressionDriver::pointSize() const
    {
        NotImplemented;
        return 0;
    }

    const fvMesh &CellZoneValueExpressionDriver::mesh() const
    {
        //        return dynamicCast<const fvMesh&>(cellZone_.zoneMesh().mesh()); // doesn't work with gcc 4.2
        return dynamic_cast<const fvMesh&>(cellZone_.zoneMesh().mesh());
    }

}


// ************************************************************************* //
