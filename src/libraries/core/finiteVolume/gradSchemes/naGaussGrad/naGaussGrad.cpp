/*---------------------------------------------------------------------------*\
Copyright (C) 2016 Applied CCM Pty Ltd
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

#include "naGaussGrad.hpp"
#include "gaussGrad.hpp"
#include "fvMesh.hpp"
#include "volMesh.hpp"
#include "surfaceMesh.hpp"
#include "GeometricField.hpp"
#include "zeroGradientFvPatchField.hpp"
#include "volPointInterpolation.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
CML::tmp
<
    CML::GeometricField
    <
        typename CML::outerProduct<CML::vector, Type>::type,
        CML::fvPatchField,
        CML::volMesh
    >
>
CML::fv::naGaussGrad<Type>::calcGrad
(
    const GeometricField<Type, fvPatchField, volMesh>& vsf,
    const word& name
) const
{
    const fvMesh& mesh = vsf.mesh();

    // Create volPoint interpolation
    const volPointInterpolation& pInterp = volPointInterpolation::New(mesh);

    // Do interpolation from volume to points
    tmp<GeometricField<Type, pointPatchField, pointMesh> > tpf
    (
      pInterp.interpolate(vsf)
    );

    GeometricField<Type, pointPatchField, pointMesh>& pf = tpf();

    // Construct tmp<surfaceField> to be used in Gauss grad
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tssf
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "nag-" + vsf.name(),
                vsf.instance(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            vsf.dimensions()
        )
    );

    GeometricField<Type, fvsPatchField, surfaceMesh>& ssf = tssf();

    const labelUList& owner = mesh.owner();

    const faceList& fcs = mesh.faces();

    forAll(owner, faceI)
    {
        const labelList& f = fcs[faceI];
        label nPoints = f.size();

        // Face centre value calculated as a simple nodal average
        Type fCentre = pTraits<Type>::zero;
        for (label pi = 0; pi < nPoints; pi++)
        {
            fCentre += pf[f[pi]];
        }

        fCentre /= nPoints;

        ssf[faceI] = fCentre;
    }

    forAll(mesh.boundary(), patchi)
    {
        fvsPatchField<Type>& pssf = ssf.boundaryField()[patchi];

        forAll(mesh.boundary()[patchi], faceI)
        {
            label bFaceI = mesh.boundary()[patchi].patch().start() + faceI;
            const labelList& f = fcs[bFaceI];
            label nPoints = f.size();

            // Face centre value calculated as a simple nodal average
            Type fCentre = pTraits<Type>::zero;
            for (label pi = 0; pi < nPoints; pi++)
            {
                fCentre += pf[f[pi]];
            }

            fCentre /= nPoints;

            pssf[faceI] = fCentre;
        }
    }

    // Use Gauss gradient with nodal averaged surfaceScalarField
    typedef typename outerProduct<vector, Type>::type GradType;

    tmp<GeometricField<GradType, fvPatchField, volMesh> > tnagGrad
    (
        gaussGrad<Type>::gradf(ssf, name)
    );

    GeometricField<GradType, fvPatchField, volMesh>& nagGrad = tnagGrad();

    gaussGrad<Type>::correctBoundaryConditions(vsf, nagGrad);

    return tnagGrad;
}


// ************************************************************************* //
