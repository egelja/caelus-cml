/*---------------------------------------------------------------------------*\
Copyright (C) 2014 - 2016 Applied CCM
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

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef dcLinearUpwindCorrection_HPP_
#define dcLinearUpwindCorrection_HPP_

#define makeDCLinearUpwindCorrection(limiterType)                              \
template<class Type>                                                           \
CML::tmp<CML::GeometricField<Type, CML::fvsPatchField, CML::surfaceMesh> >     \
CML::dcLinearUpwind##limiterType<Type>::correction                             \
(                                                                              \
    GeometricField<Type, fvPatchField, volMesh> const& vf                      \
) const                                                                        \
{                                                                              \
    fvMesh const& mesh = this->mesh();                                         \
                                                                               \
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsfCorr             \
    (                                                                          \
        new GeometricField<Type, fvsPatchField, surfaceMesh>                   \
        (                                                                      \
            IOobject                                                           \
            (                                                                  \
                "dcLinearUpwind##limiterType##::correction(" + vf.name() + ')',\
                mesh.time().timeName(),                                        \
                mesh,                                                          \
                IOobject::NO_READ,                                             \
                IOobject::NO_WRITE,                                            \
                false                                                          \
            ),                                                                 \
            mesh,                                                              \
            dimensioned<Type>(vf.name(), vf.dimensions(), pTraits<Type>::zero) \
        )                                                                      \
    );                                                                         \
                                                                               \
    GeometricField<Type, fvsPatchField, surfaceMesh>& sfCorr = tsfCorr();      \
                                                                               \
    surfaceScalarField const& faceFlux = this->faceFlux_;                      \
                                                                               \
    labelList const& own = mesh.owner();                                       \
    labelList const& nei = mesh.neighbour();                                   \
                                                                               \
    volVectorField const& C = mesh.C();                                        \
    surfaceVectorField const& Cf = mesh.Cf();                                  \
                                                                               \
    tmp                                                                        \
    <                                                                          \
        GeometricField                                                         \
        <                                                                      \
            typename outerProduct<vector, Type>::type,                         \
            fvPatchField,                                                      \
            volMesh                                                            \
        >                                                                      \
    > tgradVf = gradScheme_().grad(vf, gradSchemeName_);                       \
                                                                               \
    GeometricField                                                             \
    <                                                                          \
        typename outerProduct<vector, Type>::type,                             \
        fvPatchField,                                                          \
        volMesh                                                                \
    > const& gradVf = tgradVf();                                               \
                                                                               \
    forAll(faceFlux, facei)                                                    \
    {                                                                          \
        label cellD, cellC;                                                    \
        if (faceFlux[facei] > 0)                                               \
        {                                                                      \
            cellD = nei[facei];                                                \
            cellC = own[facei];                                                \
        }                                                                      \
        else                                                                   \
        {                                                                      \
            cellD = own[facei];                                                \
            cellC = nei[facei];                                                \
        }                                                                      \
                                                                               \
        scalar const r                                                         \
        (                                                                      \
            (scalar(2.0)*mag((C[cellD]-C[cellC])&gradVf[cellC])                \
            /(mag(vf[cellD]-vf[cellC]) + VSMALL))                              \
            -                                                                  \
            scalar(1.0)                                                        \
        );                                                                     \
                                                                               \
        scalar const limiter = slopeLimiter(r);                                \
                                                                               \
        sfCorr[facei] =                                                        \
            limiter*(Cf[facei] - C[cellC]) & gradVf[cellC];                    \
    }                                                                          \
                                                                               \
    typename GeometricField<Type, fvsPatchField, surfaceMesh>::                \
        GeometricBoundaryField& bSfCorr = sfCorr.boundaryField();              \
                                                                               \
    forAll(bSfCorr, patchi)                                                    \
    {                                                                          \
        fvsPatchField<Type>& pSfCorr = bSfCorr[patchi];                        \
                                                                               \
        if (pSfCorr.coupled())                                                 \
        {                                                                      \
            labelUList const& pOwner =                                         \
                mesh.boundary()[patchi].faceCells();                           \
                                                                               \
            vectorField const& pCf = Cf.boundaryField()[patchi];               \
                                                                               \
            scalarField const& pFaceFlux = faceFlux.boundaryField()[patchi];   \
                                                                               \
            Field<Type> const vfNei                                            \
            (                                                                  \
                vf.boundaryField()[patchi].patchNeighbourField()               \
            );                                                                 \
                                                                               \
            Field<typename outerProduct<vector, Type>::type> const pGradVfNei  \
            (                                                                  \
                gradVf.boundaryField()[patchi].patchNeighbourField()           \
            );                                                                 \
                                                                               \
            vectorField const pd(Cf.boundaryField()[patchi].patch().delta());  \
                                                                               \
            forAll(pOwner, facei)                                              \
            {                                                                  \
                label const pown = pOwner[facei];                              \
                                                                               \
                scalar r = 0;                                                  \
                scalar limiter = 0;                                            \
                                                                               \
                if (pFaceFlux[facei] > 0)                                      \
                {                                                              \
                    r = (scalar(2.0)                                           \
                      *mag(pd[facei]&gradVf[pown]))                            \
                      /(mag(vfNei[facei]-vf[pown]) + VSMALL)                   \
                      -                                                        \
                      scalar(1.0);                                             \
                                                                               \
                    limiter = slopeLimiter(r);                                 \
                                                                               \
                    pSfCorr[facei] = limiter*(pCf[facei] - C[pown])            \
                                   & gradVf[pown];                             \
                }                                                              \
                else                                                           \
                {                                                              \
                    r = (scalar(2.0)                                           \
                        *mag(pd[facei]&pGradVfNei[facei]))                     \
                        /(mag(vf[pown]-vfNei[facei]) + VSMALL)                 \
                        -                                                      \
                        scalar(1.0);                                           \
                                                                               \
                    limiter = slopeLimiter(r);                                 \
                                                                               \
                    pSfCorr[facei] = limiter                                   \
                        *(pCf[facei] - pd[facei] - C[pown])                    \
                        & pGradVfNei[facei];                                   \
                }                                                              \
            }                                                                  \
        }                                                                      \
    }                                                                          \
                                                                               \
    return tsfCorr;                                                            \
}


#endif
