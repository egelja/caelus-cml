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

\*---------------------------------------------------------------------------*/

#include "reactingMultiphaseParcelInjectionData.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline const CML::scalarList&
CML::reactingMultiphaseParcelInjectionData::YGas() const
{
    return YGas_;
}


inline const CML::scalarList&
CML::reactingMultiphaseParcelInjectionData::YLiquid() const
{
    return YLiquid_;
}


inline const CML::scalarList&
CML::reactingMultiphaseParcelInjectionData::YSolid() const
{
    return YSolid_;
}


inline CML::scalarList& CML::reactingMultiphaseParcelInjectionData::YGas()
{
    return YGas_;
}


inline CML::scalarList& CML::reactingMultiphaseParcelInjectionData::YLiquid()
{
    return YLiquid_;
}


inline CML::scalarList& CML::reactingMultiphaseParcelInjectionData::YSolid()
{
    return YSolid_;
}


// ************************************************************************* //
