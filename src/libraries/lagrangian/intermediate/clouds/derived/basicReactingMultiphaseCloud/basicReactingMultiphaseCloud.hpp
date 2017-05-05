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

Class
    CML::basicReactingMultiphaseCloud

Description
    Cloud class to introduce multi-phase reacting parcels

\*---------------------------------------------------------------------------*/

#ifndef basicReactingMultiphaseCloud_H
#define basicReactingMultiphaseCloud_H

#include "Cloud.hpp"
#include "KinematicCloud_.hpp"
#include "ThermoCloud_.hpp"
#include "ReactingCloud_.hpp"
#include "ReactingMultiphaseCloud_.hpp"
#include "basicReactingMultiphaseParcel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    typedef ReactingMultiphaseCloud
    <
        ReactingCloud
        <
            ThermoCloud
            <
                KinematicCloud
                <
                    Cloud
                    <
                        basicReactingMultiphaseParcel
                    >
                >
            >
        >
    > basicReactingMultiphaseCloud;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
