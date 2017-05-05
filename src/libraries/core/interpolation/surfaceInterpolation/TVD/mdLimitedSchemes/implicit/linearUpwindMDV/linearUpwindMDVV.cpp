/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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

Description
    LinearUpwindMDVV scheme class.

\*---------------------------------------------------------------------------*/

#include "mdLimiter.hpp"
#include "fvMesh.hpp"
#include "linearUpwindMDVV.hpp"


template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvsPatchField, CML::surfaceMesh> >
CML::linearUpwindMDVV<Type>::correction
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
                "linearUpwindMDVV::correction(" + vf.name() + ')',
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh,
            dimensioned<Type>(vf.name(), vf.dimensions(), pTraits<Type>::zero)
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

    MDLimiter<VLimiter,volVectorField> vectorLimiter
    (
        mesh,
        vf
    );

    vectorLimiter.limitField(gradVf);

    volVectorField const& limiter = vectorLimiter.getPhiLimiter();

    forAll(faceFlux, facei)
    {
        label cellU;
        if (faceFlux[facei] > 0)
        {
            cellU = own[facei];
        }
        else
        {
            cellU = nei[facei];
        }

        sfCorr[facei] += cmptMultiply
        (
            limiter[cellU],
            (Cf[facei] - C[cellU]) & gradVf[cellU]
        );

    }

    typename GeometricField<Type, fvsPatchField, surfaceMesh>::
        GeometricBoundaryField& bSfCorr = sfCorr.boundaryField();

    forAll(bSfCorr, patchi)
    {
        fvsPatchField<Type>& pSfCorr = bSfCorr[patchi];

        if (pSfCorr.coupled())
        {
            labelUList const& pOwner =
                mesh.boundary()[patchi].faceCells();

            vectorField const& pCf = Cf.boundaryField()[patchi];

            const scalarField& pFaceFlux = faceFlux.boundaryField()[patchi];

            vectorField const vfNei
            (
                vf.boundaryField()[patchi].patchNeighbourField()
            );

            vectorField const limiterNei
            (
                limiter.boundaryField()[patchi].patchNeighbourField()
            );

            const Field<typename outerProduct<vector, Type>::type> pGradVfNei
            (
                gradVf.boundaryField()[patchi].patchNeighbourField()
            );

            // Build the d-vectors
            vectorField const pd(Cf.boundaryField()[patchi].patch().delta());

            forAll(pOwner, facei)
            {
                label const pown = pOwner[facei];

                if (pFaceFlux[facei] > 0)
                {
                    pSfCorr[facei] += cmptMultiply
                    (
                        limiter[pown],
                        (pCf[facei] - C[pown]) & gradVf[pown]
                    );
                }
                else
                {
                    pSfCorr[facei] += cmptMultiply
                    ( 
                        limiterNei[facei],
                        (pCf[facei] - pd[facei] - C[pown]) & pGradVfNei[facei]
                    );
                 }
            }

        }
    }

    return tsfCorr;
}

namespace CML
{
    makelimitedSurfaceInterpolationTypeScheme(linearUpwindMDVV, vector)
}


