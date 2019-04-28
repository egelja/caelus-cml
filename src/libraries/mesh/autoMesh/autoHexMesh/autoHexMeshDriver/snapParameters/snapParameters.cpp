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

#include "snapParameters.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from dictionary
CML::snapParameters::snapParameters(const dictionary& dict)
:
    nSmoothPatch_(readLabel(dict.lookup("nSmoothPatch"))),
    snapTol_(readScalar(dict.lookup("tolerance"))),
    nSmoothDispl_(readLabel(dict.lookup("nSolveIter"))),
    nSnap_(readLabel(dict.lookup("nRelaxIter"))),
    nFeatureSnap_(dict.lookupOrDefault("nFeatureSnapIter", -1)),
    explicitFeatureSnap_(dict.lookupOrDefault("explicitFeatureSnap", true)),
    implicitFeatureSnap_(dict.lookupOrDefault("implicitFeatureSnap", false)),
    multiRegionFeatureSnap_
    (
        dict.lookupOrDefault("multiRegionFeatureSnap", false)
    )
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


// ************************************************************************* //
