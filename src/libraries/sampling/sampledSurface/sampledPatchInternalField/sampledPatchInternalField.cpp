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

#include "sampledPatchInternalField.hpp"
#include "dictionary.hpp"
#include "polyMesh.hpp"
#include "polyPatch.hpp"
#include "volFields.hpp"

#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(sampledPatchInternalField, 0);
    addNamedToRunTimeSelectionTable
    (
        sampledSurface,
        sampledPatchInternalField,
        word,
        patchInternalField
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sampledPatchInternalField::sampledPatchInternalField
(
    const word& name,
    const polyMesh& mesh,
    const dictionary& dict
)
:
    sampledPatch(name, mesh, dict),
    mappers_(patchIDs().size())
{
    mappedPatchBase::offsetMode mode = mappedPatchBase::NORMAL;
    if (dict.found("offsetMode"))
    {
        mode = mappedPatchBase::offsetModeNames_.read
        (
            dict.lookup("offsetMode")
        );
    }

    switch (mode)
    {
        case mappedPatchBase::NORMAL:
        {
            const scalar distance = readScalar(dict.lookup("distance"));
            forAll(patchIDs(), i)
            {
                mappers_.set
                (
                    i,
                    new mappedPatchBase
                    (
                        mesh.boundaryMesh()[patchIDs()[i]],
                        mesh.name(),                        // sampleRegion
                        mappedPatchBase::NEARESTCELL,       // sampleMode
                        word::null,                         // samplePatch
                        -distance                  // sample inside my domain
                    )
                );
            }
        }
        break;

        case mappedPatchBase::UNIFORM:
        {
            const point offset(dict.lookup("offset"));
            forAll(patchIDs(), i)
            {
                mappers_.set
                (
                    i,
                    new mappedPatchBase
                    (
                        mesh.boundaryMesh()[patchIDs()[i]],
                        mesh.name(),                        // sampleRegion
                        mappedPatchBase::NEARESTCELL,       // sampleMode
                        word::null,                         // samplePatch
                        offset                  // sample inside my domain
                    )
                );
            }
        }
        break;

        case mappedPatchBase::NONUNIFORM:
        {
            const pointField offsets(dict.lookup("offsets"));
            forAll(patchIDs(), i)
            {
                mappers_.set
                (
                    i,
                    new mappedPatchBase
                    (
                        mesh.boundaryMesh()[patchIDs()[i]],
                        mesh.name(),                        // sampleRegion
                        mappedPatchBase::NEARESTCELL,       // sampleMode
                        word::null,                         // samplePatch
                        offsets                  // sample inside my domain
                    )
                );
            }
        }
        break;
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sampledPatchInternalField::~sampledPatchInternalField()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::tmp<CML::scalarField> CML::sampledPatchInternalField::sample
(
    const volScalarField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::vectorField> CML::sampledPatchInternalField::sample
(
    const volVectorField& vField
) const
{
    return sampleField(vField);
}

CML::tmp<CML::sphericalTensorField> CML::sampledPatchInternalField::sample
(
    const volSphericalTensorField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::symmTensorField> CML::sampledPatchInternalField::sample
(
    const volSymmTensorField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::tensorField> CML::sampledPatchInternalField::sample
(
    const volTensorField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::scalarField> CML::sampledPatchInternalField::interpolate
(
    const interpolation<scalar>& interpolator
) const
{
    return interpolateField(interpolator);
}


CML::tmp<CML::vectorField> CML::sampledPatchInternalField::interpolate
(
    const interpolation<vector>& interpolator
) const
{
    return interpolateField(interpolator);
}


CML::tmp<CML::sphericalTensorField>
CML::sampledPatchInternalField::interpolate
(
    const interpolation<sphericalTensor>& interpolator
) const
{
    return interpolateField(interpolator);
}


CML::tmp<CML::symmTensorField> CML::sampledPatchInternalField::interpolate
(
    const interpolation<symmTensor>& interpolator
) const
{
    return interpolateField(interpolator);
}


CML::tmp<CML::tensorField> CML::sampledPatchInternalField::interpolate
(
    const interpolation<tensor>& interpolator
) const
{
    return interpolateField(interpolator);
}


void CML::sampledPatchInternalField::print(Ostream& os) const
{
    os  << "sampledPatchInternalField: " << name() << " :"
        << "  patches:" << patchNames()
        << "  faces:" << faces().size()
        << "  points:" << points().size();
}


// ************************************************************************* //
