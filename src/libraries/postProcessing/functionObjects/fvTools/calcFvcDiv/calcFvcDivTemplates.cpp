/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2013 OpenFOAM Foundation
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

#include "fvMesh.hpp"
#include "fvcDiv.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class FieldType>
void CML::calcFvcDiv::calcDiv
(
    const word& fieldName,
    const word& resultName,
    bool& processed
)
{
    const fvMesh& mesh = refCast<const fvMesh>(obr_);

    if (mesh.foundObject<FieldType>(fieldName))
    {
        const FieldType& vf = mesh.lookupObject<FieldType>(fieldName);

        volScalarField& field = divField(resultName, vf.dimensions());

        field = fvc::div(vf);

        processed = true;
    }
}


// ************************************************************************* //
