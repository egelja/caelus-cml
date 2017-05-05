/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
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

#include "residuals.hpp"
#include "volFields.hpp"
#include "dictionary.hpp"
#include "Time.hpp"
#include "ListOps.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void CML::residuals::writeResidual
(
    const word& fieldName
)
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    if (obr_.foundObject<fieldType>(fieldName))
    {
        const fieldType& field = obr_.lookupObject<fieldType>(fieldName);
        const fvMesh& mesh = field.mesh();
        const CML::dictionary& solverDict = mesh.solverPerformanceDict();

        if (solverDict.found(fieldName))
        {
            const List<solverPerformance> sp(solverDict.lookup(fieldName));
            const scalar residual = sp.first().initialResidual();
            file() << token::TAB << residual;
        }
    }
}


// ************************************************************************* //
