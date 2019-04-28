/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014-2016 H. Jasak
Copyright (C) 2014 V. Vukcevic
Copyright (C) 2017-2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Description
    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.


\*---------------------------------------------------------------------------*/

#include "cellLimitedBlockGrad.hpp"
#include "cellLimitedGrad.hpp"
#include "gaussGrad.hpp"
#include "fvMesh.hpp"
#include "volMesh.hpp"
#include "surfaceMesh.hpp"
#include "volFields.hpp"
#include "fixedValueFvPatchFields.hpp"


makeBlockFvGradScheme(cellLimitedBlockGrad)

namespace CML
{
namespace fv
{

template<>
tmp<BlockLduSystem<vector, vector> >
cellLimitedBlockGrad<scalar>::fvmGrad
(
    const volScalarField& vsf
) const
{
    const fvMesh& mesh = vsf.mesh();

    tmp<BlockLduSystem<vector, vector> > tbs =
        basicBlockGradScheme_().fvmGrad(vsf);

    if (k_ < SMALL)
    {
        return tbs;
    }

    BlockLduSystem<vector, vector>& bs = tbs();

    // Calculate current gradient for explicit limiting
    // In blockFvm::grad the current gradient may be cached
    // Using cached gradient?  Check.  HJ, 4/Jun/2016
    tmp<volVectorField> tGrad = basicGradScheme_().grad(vsf);
    const volVectorField& g = tGrad();

    const unallocLabelList& owner = mesh.owner();
    const unallocLabelList& neighbour = mesh.neighbour();

    const volVectorField& C = mesh.C();
    const surfaceVectorField& Cf = mesh.Cf();

    scalarField maxVsf(vsf.internalField());
    scalarField minVsf(vsf.internalField());

    forAll(owner, facei)
    {
        label own = owner[facei];
        label nei = neighbour[facei];

        scalar vsfOwn = vsf[own];
        scalar vsfNei = vsf[nei];

        maxVsf[own] = max(maxVsf[own], vsfNei);
        minVsf[own] = min(minVsf[own], vsfNei);

        maxVsf[nei] = max(maxVsf[nei], vsfOwn);
        minVsf[nei] = min(minVsf[nei], vsfOwn);
    }


    const volScalarField::GeometricBoundaryField& bsf = vsf.boundaryField();

    forAll(bsf, patchi)
    {
        const fvPatchScalarField& psf = bsf[patchi];

        const unallocLabelList& pOwner = mesh.boundary()[patchi].faceCells();

        if (psf.coupled())
        {
            scalarField psfNei(psf.patchNeighbourField());

            forAll(pOwner, pFacei)
            {
                label own = pOwner[pFacei];
                scalar vsfNei = psfNei[pFacei];

                maxVsf[own] = max(maxVsf[own], vsfNei);
                minVsf[own] = min(minVsf[own], vsfNei);
            }
        }
        else
        {
            forAll(pOwner, pFacei)
            {
                label own = pOwner[pFacei];
                scalar vsfNei = psf[pFacei];

                maxVsf[own] = max(maxVsf[own], vsfNei);
                minVsf[own] = min(minVsf[own], vsfNei);
            }
        }
    }

    maxVsf -= vsf;
    minVsf -= vsf;

    if (k_ < 1.0)
    {
        scalarField maxMinVsf((1.0/k_ - 1.0)*(maxVsf - minVsf));
        maxVsf += maxMinVsf;
        minVsf -= maxMinVsf;
    }


    // Create limiter as volScalarField for proper update of coupling coeffs
    volScalarField limitField
    (
        IOobject
        (
            "limitField",
            vsf.instance(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("one", dimless, 1),
        "zeroGradient"
    );
    scalarField& lfIn = limitField.internalField();

    forAll(owner, facei)
    {
        label own = owner[facei];
        label nei = neighbour[facei];

        // owner side
        limitFace
        (
            lfIn[own],
            maxVsf[own],
            minVsf[own],
            (Cf[facei] - C[own]) & g[own]
        );

        // neighbour side
        limitFace
        (
            lfIn[nei],
            maxVsf[nei],
            minVsf[nei],
            (Cf[facei] - C[nei]) & g[nei]
        );
    }

    forAll(bsf, patchi)
    {
        const unallocLabelList& pOwner = mesh.boundary()[patchi].faceCells();
        const vectorField& pCf = Cf.boundaryField()[patchi];

        forAll(pOwner, pFacei)
        {
            label own = pOwner[pFacei];

            limitFace
            (
                lfIn[own],
                maxVsf[own],
                minVsf[own],
                (pCf[pFacei] - C[own]) & g[own]
            );
        }
    }

    limitField.correctBoundaryConditions();

    if (fv::debug)
    {
        Info<< "Implicit scalar gradient limiter for: " << vsf.name()
            << " max = " << gMax(lfIn)
            << " min = " << gMin(lfIn)
            << " average: " << gAverage(lfIn) << endl;
    }

    vectorField& source = bs.source();

    // Grab ldu parts of block matrix as linear always
    CoeffField<vector>::linearTypeField& d = bs.diag().asLinear();
    CoeffField<vector>::linearTypeField& u = bs.upper().asLinear();
    CoeffField<vector>::linearTypeField& l = bs.lower().asLinear();

    // Limit upper and lower coeffs
    forAll(u, faceI)
    {
        label own = owner[faceI];
        label nei = neighbour[faceI];

        u[faceI] *= lfIn[own];
        l[faceI] *= lfIn[nei];
    }

    // Limit diag and source coeffs
    forAll(d, cellI)
    {
        d[cellI] *= lfIn[cellI];
        source[cellI] *= lfIn[cellI];
    }

    // Limit coupling coeffs
    forAll(vsf.boundaryField(), patchI)
    {
        const fvPatchScalarField& pf = vsf.boundaryField()[patchI];
        const fvPatch& patch = pf.patch();

        const labelList& fc = patch.faceCells();

        if (patch.coupled())
        {
            CoeffField<vector>::linearTypeField& pcoupleUpper =
                bs.coupleUpper()[patchI].asLinear();
            CoeffField<vector>::linearTypeField& pcoupleLower =
                bs.coupleLower()[patchI].asLinear();

            const scalarField lfNei(
                limitField.boundaryField()[patchI].patchNeighbourField());

            forAll(pf, faceI)
            {
                pcoupleUpper[faceI] *= lfIn[fc[faceI]];
                pcoupleLower[faceI] *= lfNei[faceI];
            }
        }
    }

    return tbs;
}


template<>
tmp<BlockLduSystem<vector, outerProduct<vector, vector>::type> >
cellLimitedBlockGrad<vector>::fvmGrad
(
    const volVectorField& vsf
) const
{
    FatalErrorInFunction
        << "Implicit bock gradient operators with cell limiters defined"
        << "only for scalar."
        << abort(FatalError);

    typedef outerProduct<vector, vector>::type GradType;

    tmp<BlockLduSystem<vector, GradType> > tbs
    (
        new BlockLduSystem<vector, GradType>(vsf.mesh())
    );

    return tbs;
}


} // End namespace fv
} // End namespace CML
