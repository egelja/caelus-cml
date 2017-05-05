/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2010 OpenCFD Ltd.
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
    2012-2013 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#include "swakRegistryProxySurface.hpp"
#include "dictionary.hpp"
#include "volFields.hpp"
#include "volPointInterpolation.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "fvMesh.hpp"
//#include "isoSurface.hpp"
// #include "isoSurfaceCell.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(swakRegistryProxySurface, 0);
    addToRunTimeSelectionTable(sampledSurface, swakRegistryProxySurface, word);
}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// void CML::swakRegistryProxySurface::createGeometry()
// {
//     if (debug)
//     {
//         Pout<< "swakRegistryProxySurface::createGeometry() - doing nothing"
//             << endl;
//     }
// }

CML::sampledSurface &CML::swakRegistryProxySurface::realSurface()
{
    return SurfacesRepository::getRepository(
        mesh()
    ).getSurface(
        surfaceName_,
        static_cast<const fvMesh&>(mesh())
    );
}

const CML::sampledSurface &CML::swakRegistryProxySurface::realSurface() const
{
    return SurfacesRepository::getRepository(
        mesh()
    ).getSurface(
        surfaceName_,
        static_cast<const fvMesh&>(mesh())
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::swakRegistryProxySurface::swakRegistryProxySurface
(
    const word& name,
    const polyMesh& mesh,
    const dictionary& dict
)
:
    sampledSurface(name, mesh, dict),
    surfaceName_(dict.lookup("surfaceName"))
{
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::swakRegistryProxySurface::~swakRegistryProxySurface()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::swakRegistryProxySurface::needsUpdate() const
{
    bool originalUpdate=realSurface().needsUpdate();

    if(debug) {
        Pout << "CML::swakRegistryProxySurface::needsUpdate(): " << originalUpdate << endl;
    }

    return originalUpdate;
}


bool CML::swakRegistryProxySurface::expire()
{
    bool originalExpire=realSurface().expire();

    if(debug) {
        Pout << "CML::swakRegistryProxySurface::expire(): " << originalExpire << endl;
    }

    return originalExpire;
}


bool CML::swakRegistryProxySurface::update()
{
    bool originalUpdate=realSurface().update();

    if(
        this->Sf().size() != realSurface().Sf().size()
        ||
        this->Cf().size() != realSurface().Cf().size()
        ||
        this->magSf().size() != realSurface().magSf().size()
    ) {
        if(debug) {
            Pout << "CML::swakRegistryProxySurface::update(): Clearin Geometry" << endl;
        }
        clearGeom();
    }

    if(debug) {
        Pout << "CML::swakRegistryProxySurface::update(): " << originalUpdate << endl;
    }
    return originalUpdate;
}


CML::tmp<CML::scalarField>
CML::swakRegistryProxySurface::sample
(
    const volScalarField& vField
) const
{
    return realSurface().sample(vField);
}


CML::tmp<CML::vectorField>
CML::swakRegistryProxySurface::sample
(
    const volVectorField& vField
) const
{
    return realSurface().sample(vField);
}


CML::tmp<CML::sphericalTensorField>
CML::swakRegistryProxySurface::sample
(
    const volSphericalTensorField& vField
) const
{
    return realSurface().sample(vField);
}


CML::tmp<CML::symmTensorField>
CML::swakRegistryProxySurface::sample
(
    const volSymmTensorField& vField
) const
{
    return realSurface().sample(vField);
}


CML::tmp<CML::tensorField>
CML::swakRegistryProxySurface::sample
(
    const volTensorField& vField
) const
{
    return realSurface().sample(vField);
}


CML::tmp<CML::scalarField>
CML::swakRegistryProxySurface::interpolate
(
    const interpolation<scalar>& interpolator
) const
{
    return realSurface().interpolate(interpolator);
}


CML::tmp<CML::vectorField>
CML::swakRegistryProxySurface::interpolate
(
    const interpolation<vector>& interpolator
) const
{
    return realSurface().interpolate(interpolator);
}

CML::tmp<CML::sphericalTensorField>
CML::swakRegistryProxySurface::interpolate
(
    const interpolation<sphericalTensor>& interpolator
) const
{
    return realSurface().interpolate(interpolator);
}


CML::tmp<CML::symmTensorField>
CML::swakRegistryProxySurface::interpolate
(
    const interpolation<symmTensor>& interpolator
) const
{
    return realSurface().interpolate(interpolator);
}


CML::tmp<CML::tensorField>
CML::swakRegistryProxySurface::interpolate
(
    const interpolation<tensor>& interpolator
) const
{
    return realSurface().interpolate(interpolator);
}


void CML::swakRegistryProxySurface::print(Ostream& os) const
{
    os  << "swakRegistryProxySurface: " << name() << " :"
        << "  realSurface:" << surfaceName_;
}

// ************************************************************************* //
