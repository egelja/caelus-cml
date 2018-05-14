/*---------------------------------------------------------------------------*\
Copyright (C) 2010-2015 H. Jasak
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
    Specialisation of blockGaussGrad for scalars. Needed for implicit fvmGrad
    operator for block coupled systems.

    This file has been obtained from FOAM-Extend-4.0.
    https://sourceforge.net/p/foam-extend/foam-extend-4.0/

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

\*---------------------------------------------------------------------------*/

#include "blockGaussGrad.hpp"


namespace CML
{
namespace fv
{

template<>
tmp<BlockLduSystem<vector, vector> > blockGaussGrad<scalar>::fvmGrad
(
    const GeometricField<scalar, fvPatchField, volMesh>& vf
) const
{
    tmp<surfaceScalarField> tweights = this->tinterpScheme_().weights(vf);
    const scalarField& wIn = tweights().internalField();

    const fvMesh& mesh = vf.mesh();

    tmp<BlockLduSystem<vector, vector> > tbs
    (
        new BlockLduSystem<vector, vector>(mesh)
    );
    BlockLduSystem<vector, vector>& bs = tbs();
    vectorField& source = bs.source();

    // Grab ldu parts of block matrix as linear always
    CoeffField<vector>::linearTypeField& d = bs.diag().asLinear();
    CoeffField<vector>::linearTypeField& u = bs.upper().asLinear();
    CoeffField<vector>::linearTypeField& l = bs.lower().asLinear();

    const vectorField& SfIn = mesh.Sf().internalField();

    l = -wIn*SfIn;
    u = l + SfIn;
    bs.negSumDiag();

    // Boundary contributions
    forAll (vf.boundaryField(), patchI)
    {
        const fvPatchScalarField& pf = vf.boundaryField()[patchI];
        const fvPatch& patch = pf.patch();
        const vectorField& pSf = patch.Sf();
        const fvsPatchScalarField& pw = tweights().boundaryField()[patchI];
        const labelList& fc = patch.faceCells();

        const scalarField internalCoeffs(pf.valueInternalCoeffs(pw));

        // Diag contribution
        forAll (pf, faceI)
        {
            d[fc[faceI]] += internalCoeffs[faceI]*pSf[faceI];
        }

        if (patch.coupled())
        {
            CoeffField<vector>::linearTypeField& pcoupleUpper =
                bs.coupleUpper()[patchI].asLinear();
            CoeffField<vector>::linearTypeField& pcoupleLower =
                bs.coupleLower()[patchI].asLinear();

            const vectorField pcl(-pw*pSf);
            const vectorField pcu(pcl + pSf);

            // Coupling  contributions
            pcoupleLower -= pcl;
            pcoupleUpper -= pcu;
        }
        else
        {
            const scalarField boundaryCoeffs(pf.valueBoundaryCoeffs(pw));

            // Boundary contribution
            forAll (pf, faceI)
            {
                source[fc[faceI]] -= boundaryCoeffs[faceI]*pSf[faceI];
            }
        }
    }

    // Interpolation schemes with corrections not accounted for

    return tbs;
}


} // End namespace fv
} // End namespace CML
