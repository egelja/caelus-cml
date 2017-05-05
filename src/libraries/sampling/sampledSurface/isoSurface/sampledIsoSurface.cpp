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

#include "sampledIsoSurface.hpp"
#include "dictionary.hpp"
#include "volFields.hpp"
#include "volPointInterpolation.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(sampledIsoSurface, 0);
    addNamedToRunTimeSelectionTable
    (
        sampledSurface,
        sampledIsoSurface,
        word,
        isoSurface
    );
}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::sampledIsoSurface::getIsoFields() const
{
    const fvMesh& fvm = static_cast<const fvMesh&>(mesh());

    // Get volField
    // ~~~~~~~~~~~~

    if (fvm.foundObject<volScalarField>(isoField_))
    {
        if (debug)
        {
            Info<< "sampledIsoSurface::getIsoFields() : lookup volField "
                << isoField_ << endl;
        }
        storedVolFieldPtr_.clear();
        volFieldPtr_ = &fvm.lookupObject<volScalarField>(isoField_);
    }
    else
    {
        // Bit of a hack. Read field and store.

        if (debug)
        {
            Info<< "sampledIsoSurface::getIsoFields() : checking "
                << isoField_ << " for same time " << fvm.time().timeName()
                << endl;
        }

        if
        (
            storedVolFieldPtr_.empty()
         || (fvm.time().timeName() != storedVolFieldPtr_().instance())
        )
        {
            if (debug)
            {
                Info<< "sampledIsoSurface::getIsoFields() : reading volField "
                    << isoField_ << " from time " << fvm.time().timeName()
                    << endl;
            }

            IOobject vfHeader
            (
                isoField_,
                fvm.time().timeName(),
                fvm,
                IOobject::MUST_READ,
                IOobject::NO_WRITE,
                false
            );

            if (vfHeader.headerOk())
            {
                storedVolFieldPtr_.reset
                (
                    new volScalarField
                    (
                        vfHeader,
                        fvm
                    )
                );
                volFieldPtr_ = storedVolFieldPtr_.operator->();
            }
            else
            {
                FatalErrorIn("sampledIsoSurface::getIsoFields()")
                << "Cannot find isosurface field " << isoField_
                << " in database or directory " << vfHeader.path()
                << exit(FatalError);
            }
        }
    }


    // Get pointField
    // ~~~~~~~~~~~~~~

    if (!subMeshPtr_.valid())
    {
        word pointFldName = "volPointInterpolate(" + isoField_ + ')';

        if (fvm.foundObject<pointScalarField>(pointFldName))
        {
            if (debug)
            {
                Info<< "sampledIsoSurface::getIsoFields() : lookup pointField "
                    << pointFldName << endl;
            }
            pointFieldPtr_ = &fvm.lookupObject<pointScalarField>(pointFldName);
        }
        else
        {
            // Not in registry. Interpolate.

            if (debug)
            {
                Info<< "sampledIsoSurface::getIsoFields() : "
                    << "checking pointField " << pointFldName
                    << " for same time " << fvm.time().timeName()
                    << endl;
            }

            if
            (
                storedPointFieldPtr_.empty()
             || (fvm.time().timeName() != storedPointFieldPtr_().instance())
            )
            {
                if (debug)
                {
                    Info<< "sampledIsoSurface::getIsoFields() :"
                        << " interpolating volField " << volFieldPtr_->name()
                        << " to get pointField " << pointFldName << endl;
                }

                storedPointFieldPtr_.reset
                (
                    volPointInterpolation::New(fvm)
                    .interpolate(*volFieldPtr_).ptr()
                );
                storedPointFieldPtr_->checkOut();
                pointFieldPtr_ = storedPointFieldPtr_.operator->();
            }
        }


        // If averaging redo the volField. Can only be done now since needs the
        // point field.
        if (average_)
        {
            storedVolFieldPtr_.reset
            (
                pointAverage(*pointFieldPtr_).ptr()
            );
            volFieldPtr_ = storedVolFieldPtr_.operator->();
        }


        if (debug)
        {
            Info<< "sampledIsoSurface::getIsoFields() : volField "
                << volFieldPtr_->name() << " min:" << min(*volFieldPtr_).value()
                << " max:" << max(*volFieldPtr_).value() << endl;
            Info<< "sampledIsoSurface::getIsoFields() : pointField "
                << pointFieldPtr_->name()
                << " min:" << gMin(pointFieldPtr_->internalField())
                << " max:" << gMax(pointFieldPtr_->internalField()) << endl;
        }
    }
    else
    {
        // Get subMesh variants
        const fvMesh& subFvm = subMeshPtr_().subMesh();

        // Either lookup on the submesh or subset the whole-mesh volField

        if (subFvm.foundObject<volScalarField>(isoField_))
        {
            if (debug)
            {
                Info<< "sampledIsoSurface::getIsoFields() :"
                    << " submesh lookup volField "
                    << isoField_ << endl;
            }
            storedVolSubFieldPtr_.clear();
            volSubFieldPtr_ = &subFvm.lookupObject<volScalarField>(isoField_);
        }
        else
        {
            if (debug)
            {
                Info<< "sampledIsoSurface::getIsoFields() : "
                    << "subsetting volField " << isoField_ << endl;
            }
            storedVolSubFieldPtr_.reset
            (
                subMeshPtr_().interpolate
                (
                    *volFieldPtr_
                ).ptr()
            );
            storedVolSubFieldPtr_->checkOut();
            volSubFieldPtr_ = storedVolSubFieldPtr_.operator->();
        }


        // Pointfield on submesh

        word pointFldName =
            "volPointInterpolate("
          + volSubFieldPtr_->name()
          + ')';

        if (subFvm.foundObject<pointScalarField>(pointFldName))
        {
            if (debug)
            {
                Info<< "sampledIsoSurface::getIsoFields() :"
                    << " submesh lookup pointField " << pointFldName << endl;
            }
            storedPointSubFieldPtr_.clear();
            pointSubFieldPtr_ = &subFvm.lookupObject<pointScalarField>
            (
                pointFldName
            );
        }
        else
        {
            if (debug)
            {
                Info<< "sampledIsoSurface::getIsoFields() :"
                    << " interpolating submesh volField "
                    << volSubFieldPtr_->name()
                    << " to get submesh pointField " << pointFldName << endl;
            }
            storedPointSubFieldPtr_.reset
            (
                volPointInterpolation::New
                (
                    subFvm
                ).interpolate(*volSubFieldPtr_).ptr()
            );
            storedPointSubFieldPtr_->checkOut();
            pointSubFieldPtr_ = storedPointSubFieldPtr_.operator->();
        }


        // If averaging redo the volField. Can only be done now since needs the
        // point field.
        if (average_)
        {
            storedVolSubFieldPtr_.reset
            (
                pointAverage(*pointSubFieldPtr_).ptr()
            );
            volSubFieldPtr_ = storedVolSubFieldPtr_.operator->();
        }


        if (debug)
        {
            Info<< "sampledIsoSurface::getIsoFields() : volSubField "
                << volSubFieldPtr_->name()
                << " min:" << min(*volSubFieldPtr_).value()
                << " max:" << max(*volSubFieldPtr_).value() << endl;
            Info<< "sampledIsoSurface::getIsoFields() : pointSubField "
                << pointSubFieldPtr_->name()
                << " min:" << gMin(pointSubFieldPtr_->internalField())
                << " max:" << gMax(pointSubFieldPtr_->internalField()) << endl;
        }
    }
}


bool CML::sampledIsoSurface::updateGeometry() const
{
    const fvMesh& fvm = static_cast<const fvMesh&>(mesh());

    // No update needed
    if (fvm.time().timeIndex() == prevTimeIndex_)
    {
        return false;
    }

    // Get any subMesh
    if (zoneID_.index() != -1 && !subMeshPtr_.valid())
    {
        const polyBoundaryMesh& patches = mesh().boundaryMesh();

        // Patch to put exposed internal faces into
        const label exposedPatchI = patches.findPatchID(exposedPatchName_);

        if (debug)
        {
            Info<< "Allocating subset of size "
                << mesh().cellZones()[zoneID_.index()].size()
                << " with exposed faces into patch "
                << patches[exposedPatchI].name() << endl;
        }

        subMeshPtr_.reset
        (
            new fvMeshSubset(fvm)
        );
        subMeshPtr_().setLargeCellSubset
        (
            labelHashSet(mesh().cellZones()[zoneID_.index()]),
            exposedPatchI
        );
    }


    prevTimeIndex_ = fvm.time().timeIndex();
    getIsoFields();

    // Clear any stored topo
    surfPtr_.clear();
    facesPtr_.clear();

    // Clear derived data
    clearGeom();

    if (subMeshPtr_.valid())
    {
        surfPtr_.reset
        (
            new isoSurface
            (
                *volSubFieldPtr_,
                *pointSubFieldPtr_,
                isoVal_,
                regularise_,
                mergeTol_
            )
        );
    }
    else
    {
        surfPtr_.reset
        (
            new isoSurface
            (
                *volFieldPtr_,
                *pointFieldPtr_,
                isoVal_,
                regularise_,
                mergeTol_
            )
        );
    }


    if (debug)
    {
        Pout<< "sampledIsoSurface::updateGeometry() : constructed iso:"
            << nl
            << "    regularise     : " << regularise_ << nl
            << "    average        : " << average_ << nl
            << "    isoField       : " << isoField_ << nl
            << "    isoValue       : " << isoVal_ << nl;
        if (subMeshPtr_.valid())
        {
            Pout<< "    zone size      : " << subMeshPtr_().subMesh().nCells()
                << nl;
        }
        Pout<< "    points         : " << points().size() << nl
            << "    tris           : " << surface().size() << nl
            << "    cut cells      : " << surface().meshCells().size()
            << endl;
    }

    return true;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sampledIsoSurface::sampledIsoSurface
(
    const word& name,
    const polyMesh& mesh,
    const dictionary& dict
)
:
    sampledSurface(name, mesh, dict),
    isoField_(dict.lookup("isoField")),
    isoVal_(readScalar(dict.lookup("isoValue"))),
    mergeTol_(dict.lookupOrDefault("mergeTol", 1e-6)),
    regularise_(dict.lookupOrDefault("regularise", true)),
    average_(dict.lookupOrDefault("average", false)),
    zoneID_(dict.lookupOrDefault("zone", word::null), mesh.cellZones()),
    exposedPatchName_(word::null),
    surfPtr_(NULL),
    facesPtr_(NULL),
    prevTimeIndex_(-1),
    storedVolFieldPtr_(NULL),
    volFieldPtr_(NULL),
    storedPointFieldPtr_(NULL),
    pointFieldPtr_(NULL)
{
    if (!sampledSurface::interpolate())
    {
        FatalIOErrorIn
        (
            "sampledIsoSurface::sampledIsoSurface"
            "(const word&, const polyMesh&, const dictionary&)",
            dict
        )   << "Non-interpolated iso surface not supported since triangles"
            << " span across cells." << exit(FatalIOError);
    }

    if (zoneID_.index() != -1)
    {
        dict.lookup("exposedPatchName") >> exposedPatchName_;

        if (mesh.boundaryMesh().findPatchID(exposedPatchName_) == -1)
        {
            FatalIOErrorIn
            (
                "sampledIsoSurface::sampledIsoSurface"
                "(const word&, const polyMesh&, const dictionary&)",
                dict
            )   << "Cannot find patch " << exposedPatchName_
                << " in which to put exposed faces." << endl
                << "Valid patches are " << mesh.boundaryMesh().names()
                << exit(FatalIOError);
        }

        if (debug && zoneID_.index() != -1)
        {
            Info<< "Restricting to cellZone " << zoneID_.name()
                << " with exposed internal faces into patch "
                << exposedPatchName_ << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sampledIsoSurface::~sampledIsoSurface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::sampledIsoSurface::needsUpdate() const
{
    const fvMesh& fvm = static_cast<const fvMesh&>(mesh());

    return fvm.time().timeIndex() != prevTimeIndex_;
}


bool CML::sampledIsoSurface::expire()
{
    surfPtr_.clear();
    facesPtr_.clear();
    subMeshPtr_.clear();

    // Clear derived data
    clearGeom();

    // already marked as expired
    if (prevTimeIndex_ == -1)
    {
        return false;
    }

    // force update
    prevTimeIndex_ = -1;
    return true;
}


bool CML::sampledIsoSurface::update()
{
    return updateGeometry();
}


CML::tmp<CML::scalarField> CML::sampledIsoSurface::sample
(
    const volScalarField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::vectorField> CML::sampledIsoSurface::sample
(
    const volVectorField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::sphericalTensorField> CML::sampledIsoSurface::sample
(
    const volSphericalTensorField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::symmTensorField> CML::sampledIsoSurface::sample
(
    const volSymmTensorField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::tensorField> CML::sampledIsoSurface::sample
(
    const volTensorField& vField
) const
{
    return sampleField(vField);
}


CML::tmp<CML::scalarField> CML::sampledIsoSurface::interpolate
(
    const interpolation<scalar>& interpolator
) const
{
    return interpolateField(interpolator);
}


CML::tmp<CML::vectorField> CML::sampledIsoSurface::interpolate
(
    const interpolation<vector>& interpolator
) const
{
    return interpolateField(interpolator);
}

CML::tmp<CML::sphericalTensorField> CML::sampledIsoSurface::interpolate
(
    const interpolation<sphericalTensor>& interpolator
) const
{
    return interpolateField(interpolator);
}


CML::tmp<CML::symmTensorField> CML::sampledIsoSurface::interpolate
(
    const interpolation<symmTensor>& interpolator
) const
{
    return interpolateField(interpolator);
}


CML::tmp<CML::tensorField> CML::sampledIsoSurface::interpolate
(
    const interpolation<tensor>& interpolator
) const
{
    return interpolateField(interpolator);
}


void CML::sampledIsoSurface::print(Ostream& os) const
{
    os  << "sampledIsoSurface: " << name() << " :"
        << "  field   :" << isoField_
        << "  value   :" << isoVal_;
}


// ************************************************************************* //
