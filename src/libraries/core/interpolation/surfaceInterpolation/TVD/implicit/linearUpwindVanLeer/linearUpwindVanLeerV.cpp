/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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

#include "linearUpwindVanLeerV.hpp"
#include "fvMesh.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvsPatchField, CML::surfaceMesh> >
CML::linearUpwindVanLeerV<Type>::correction
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    const fvMesh& mesh = this->mesh();

    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsfCorr
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "linearUpwindVanLeerV::correction(" + vf.name() + ')',
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh,
            dimensioned<Type>
            (
                vf.name(),
                vf.dimensions(),
                pTraits<Type>::zero
            )
        )
    );

    GeometricField<Type, fvsPatchField, surfaceMesh>& sfCorr = tsfCorr();

    const surfaceScalarField& faceFlux = this->faceFlux_;

    const labelList& own = mesh.owner();
    const labelList& nei = mesh.neighbour();

    const volVectorField& C = mesh.C();
    const surfaceVectorField& Cf = mesh.Cf();

    tmp
    <
        GeometricField
        <
            typename outerProduct<vector, Type>::type,
            fvPatchField,
            volMesh
        >
    > tgradVf = gradScheme_().grad(vf, gradSchemeName_);

    const GeometricField
    <
        typename outerProduct<vector, Type>::type,
        fvPatchField,
        volMesh
    >& gradVf = tgradVf();

    forAll(faceFlux, facei)
    {
        label cellD, cellU;

        if (faceFlux[facei] > 0.0)
        {
            cellD = nei[facei];
            cellU = own[facei];
        }
        else
        {
            cellD = own[facei];
            cellU = nei[facei];
        }

        vector R = (scalar(2.0)*(C[cellD]-C[cellU])&gradVf[cellU]);
        for (int i = 0; i < 3; ++i)
        {
            R[i] /= (vf[cellD][i]-vf[cellU][i] + SMALL);
            R[i] += scalar(1.0);
        }

        scalar const r(max(0,(min(min(R[0],R[1]),R[2]))));

        scalar const limiter = (r + mag(r))/(1 + mag(r) + VSMALL);

        sfCorr[facei] = limiter*(Cf[facei] - C[cellU]) & gradVf[cellU];
    }


    typename GeometricField<Type, fvsPatchField, surfaceMesh>::
        GeometricBoundaryField& bSfCorr = sfCorr.boundaryField();

    forAll(bSfCorr, patchi)
    {
        fvsPatchField<Type>& pSfCorr = bSfCorr[patchi];

        if (pSfCorr.coupled())
        {
            const labelUList& pOwner =
                mesh.boundary()[patchi].faceCells();

            const vectorField& pCf = Cf.boundaryField()[patchi];

            const scalarField& pFaceFlux = faceFlux.boundaryField()[patchi];

            const vectorField vfNei
            (
                vf.boundaryField()[patchi].patchNeighbourField()
            );

            const Field<typename outerProduct<vector, Type>::type> pGradVfNei
            (
                gradVf.boundaryField()[patchi].patchNeighbourField()
            );

            // Build the d-vectors
            vectorField pd(Cf.boundaryField()[patchi].patch().delta());

            forAll(pOwner, facei)
            {
                label pown = pOwner[facei];

                scalar r = 0;
                scalar limiter = 0;
                vector R;
                
                if (pFaceFlux[facei] > 0)
                {
                    R = (scalar(2.0)*(pCf[facei]-C[pown])&gradVf[pown]);

                    for (label i = 0; i < 3; ++i)
                    {
                        R[i] /= (vfNei[facei][i]-vf[pown][i] + SMALL);
                        R[i] += scalar(1.0);
                    }

                    r = (max(0,(min(min(R[0],R[1]),R[2]))));

                    limiter = (r + mag(r))/(1 + mag(r) + VSMALL);

                    pSfCorr[facei] = limiter*(pCf[facei] - C[pown]) 
                                   & gradVf[pown];
                }
                else
                {
                    R = (scalar(2.0)*(pCf[facei]-pd[facei]-C[pown])
                        &pGradVfNei[facei]);

                    for (label i = 0; i < 3; ++i)
                    {
                        R[i] /= (vf[pown][i]-vfNei[facei][i] + SMALL);
                        R[i] += scalar(1.0);
                    }

                    r = (max(0,(min(min(R[0],R[1]),R[2]))));

                    limiter = (r + mag(r))/(1 + mag(r) + VSMALL);

                    pSfCorr[facei] = limiter*(pCf[facei] - pd[facei] - C[pown]) 
                                   & pGradVfNei[facei];
                }
            }
        }
    }

    return tsfCorr;
}

namespace CML
{
    makelimitedSurfaceInterpolationTypeScheme(linearUpwindVanLeerV, vector)
}

