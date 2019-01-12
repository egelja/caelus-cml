/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2015 OpenFOAM Foundation
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

#include "meshToMeshMethod.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::autoPtr<CML::meshToMeshMethod> CML::meshToMeshMethod::New
(
    const word& methodName,
    const polyMesh& src,
    const polyMesh& tgt
)
{
    if (debug)
    {
        Info<< "Selecting AMIMethod " << methodName << endl;
    }

    componentsConstructorTable::iterator cstrIter =
        componentsConstructorTablePtr_->find(methodName);

    if (cstrIter == componentsConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown meshToMesh type "
            << methodName << nl << nl
            << "Valid meshToMesh types are:" << nl
            << componentsConstructorTablePtr_->sortedToc() << exit(FatalError);
    }

    return autoPtr<meshToMeshMethod>(cstrIter()(src, tgt));
}


// ************************************************************************* //
