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
#include "Time.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

template<class FieldType>
FieldType& CML::calcMag::magField
(
    const word& magName,
    const dimensionSet& dims
)
{
    const fvMesh& mesh = refCast<const fvMesh>(obr_);

    if (!mesh.foundObject<FieldType>(magName))
    {
        FieldType* magFieldPtr
        (
            new FieldType
            (
                IOobject
                (
                    magName,
                    mesh.time().timeName(),
                    mesh,
                    IOobject::NO_READ,
                    IOobject::NO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dims, 0.0)
            )
        );

        mesh.objectRegistry::store(magFieldPtr);
    }

    const FieldType& f = mesh.lookupObject<FieldType>(magName);

    return const_cast<FieldType&>(f);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::calcMag::calc
(
    const word& fieldName,
    const word& resultName,
    bool& processed
)
{
    typedef GeometricField<Type, fvPatchField, volMesh> vfType;
    typedef GeometricField<Type, fvsPatchField, surfaceMesh> sfType;

    const fvMesh& mesh = refCast<const fvMesh>(obr_);

    if (mesh.foundObject<vfType>(fieldName))
    {
        const vfType& vf = mesh.lookupObject<vfType>(fieldName);

        volScalarField& field =
            magField<volScalarField>(resultName_, vf.dimensions());

        field = mag(vf);

        processed = true;
    }
    else if (mesh.foundObject<sfType>(fieldName))
    {
        const sfType& sf = mesh.lookupObject<sfType>(fieldName);

        surfaceScalarField& field =
            magField<surfaceScalarField>(resultName_, sf.dimensions());

        field = mag(sf);

        processed = true;
    }
}


// ************************************************************************* //
