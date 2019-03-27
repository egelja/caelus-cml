/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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

Class
    CML::fv::cellLimitedGrad

Description
    cellLimitedGrad gradient scheme applied to a runTime selected base gradient
    scheme.

    The scalar limiter based on limiting the extrapolated face values
    between the maximum and minimum cell and cell neighbour values and is
    applied to all components of the gradient.

SourceFiles
    cellLimitedGrad.cpp

\*---------------------------------------------------------------------------*/

#ifndef cellLimitedGrad_HPP
#define cellLimitedGrad_HPP

#include "gradScheme.hpp"
#include "Field.hpp"
#include "gaussGrad.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fv
{

/*---------------------------------------------------------------------------*\
                       Class cellLimitedGrad Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class Limiter>
class cellLimitedGrad
:
    public fv::gradScheme<Type>,
    public Limiter
{
    // Private Data
    tmp<fv::gradScheme<Type> > basicGradScheme_;

    //- Limiter coefficient
    const scalar k_;


    // Private Member Functions
    void limitGradient
    (
        const Field<scalar>& limiter,
        Field<vector>& gIf
    ) const;

    void limitGradient
    (
        const Field<vector>& limiter,
        Field<tensor>& gIf
    ) const;

    //- Disallow default bitwise copy construct
    cellLimitedGrad(const cellLimitedGrad&);

    //- Disallow default bitwise assignment
    void operator=(const cellLimitedGrad&);


public:

    //- RunTime type information
    TypeName("cellLimited");


    //- Construct from mesh and schemeData
    cellLimitedGrad(const fvMesh& mesh, Istream& schemeData)
    :
        gradScheme<Type>(mesh),
        Limiter(schemeData),
        basicGradScheme_(fv::gradScheme<Type>::New(mesh, schemeData)),
        k_(readScalar(schemeData))
    {
        if (k_ < 0 || k_ > 1)
        {
            FatalIOErrorInFunction(schemeData)
                << "coefficient = " << k_
                << " should be >= 0 and <= 1"
                << exit(FatalIOError);
        }
    }


    // Member Functions
    inline void limitFaceCmpt
    (
        scalar& limiter,
        const scalar maxDelta,
        const scalar minDelta,
        const scalar extrapolate
    ) const;

    inline void limitFace
    (
        Type& limiter,
        const Type& maxDelta,
        const Type& minDelta,
        const Type& extrapolate
    ) const;

    //- Return the gradient of the given field to the gradScheme::grad
    //  for optional caching
    virtual tmp
    <
        GeometricField
        <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > calcGrad
    (
        const GeometricField<Type, fvPatchField, volMesh>& vsf,
        const word& name
    ) const;
};


// * * * * * * * * * * * * Inline Member Function  * * * * * * * * * * * * * //

template<class Type, class Limiter>
inline void cellLimitedGrad<Type, Limiter>::limitFaceCmpt
(
    scalar& limiter,
    const scalar maxDelta,
    const scalar minDelta,
    const scalar extrapolate
) const
{
    scalar r = 1;

    if (extrapolate > SMALL)
    {
        r = maxDelta/extrapolate;
    }
    else if (extrapolate < -SMALL)
    {
        r = minDelta/extrapolate;
    }
    else
    {
        return;
    }

// Foundation version limits the limtier to be < 1 by clipping but is then not
// differentiable and so no longer conforms to the basic principles of this kind
// of limiter function.
    limiter = min(limiter, Limiter::limiter(r));
}


template<class Type, class Limiter>
inline void cellLimitedGrad<Type, Limiter>::limitFace
(
    Type& limiter,
    const Type& maxDelta,
    const Type& minDelta,
    const Type& extrapolate
) const
{
    for (direction cmpt=0; cmpt<pTraits<Type>::nComponents; cmpt++)
    {
        limitFaceCmpt
        (
            setComponent(limiter, cmpt),
            component(maxDelta, cmpt),
            component(minDelta, cmpt),
            component(extrapolate, cmpt)
        );
    }
}


} // End namespace fv
} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


template<class Type, class Limiter>
void CML::fv::cellLimitedGrad<Type, Limiter>::limitGradient
(
    const Field<scalar>& limiter,
    Field<vector>& gIf
) const
{
    gIf *= limiter;
}


template<class Type, class Limiter>
void CML::fv::cellLimitedGrad<Type, Limiter>::limitGradient
(
    const Field<vector>& limiter,
    Field<tensor>& gIf
) const
{
    forAll(gIf, celli)
    {
        gIf[celli] = tensor
        (
            cmptMultiply(limiter[celli], gIf[celli].x()),
            cmptMultiply(limiter[celli], gIf[celli].y()),
            cmptMultiply(limiter[celli], gIf[celli].z())
        );
    }
}


template<class Type, class Limiter>
CML::tmp
<
    CML::GeometricField
    <
        typename CML::outerProduct<CML::vector, Type>::type,
        CML::fvPatchField,
        CML::volMesh
    >
>
CML::fv::cellLimitedGrad<Type, Limiter>::calcGrad
(
    const GeometricField<Type, fvPatchField, volMesh>& vsf,
    const word& name
) const
{
    const fvMesh& mesh = vsf.mesh();

    tmp
    <
        GeometricField
        <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > tGrad = basicGradScheme_().calcGrad(vsf, name);

    if (k_ < SMALL)
    {
        return tGrad;
    }

    GeometricField
    <
        typename outerProduct<vector, Type>::type,
        fvPatchField,
        volMesh
    >& g = tGrad();

    const labelUList& owner = mesh.owner();
    const labelUList& neighbour = mesh.neighbour();

    const volVectorField& C = mesh.C();
    const surfaceVectorField& Cf = mesh.Cf();

    Field<Type> maxVsf(vsf.internalField());
    Field<Type> minVsf(vsf.internalField());

    forAll(owner, facei)
    {
        label own = owner[facei];
        label nei = neighbour[facei];

        const Type& vsfOwn = vsf[own];
        const Type& vsfNei = vsf[nei];

        maxVsf[own] = max(maxVsf[own], vsfNei);
        minVsf[own] = min(minVsf[own], vsfNei);

        maxVsf[nei] = max(maxVsf[nei], vsfOwn);
        minVsf[nei] = min(minVsf[nei], vsfOwn);
    }


    const typename GeometricField<Type, fvPatchField, volMesh>::GeometricBoundaryField& bsf =
        vsf.boundaryField();

    forAll(bsf, patchi)
    {
        const fvPatchField<Type>& psf = bsf[patchi];
        const labelUList& pOwner = mesh.boundary()[patchi].faceCells();

        if (psf.coupled())
        {
            const Field<Type> psfNei(psf.patchNeighbourField());

            forAll(pOwner, pFacei)
            {
                label own = pOwner[pFacei];
                const Type& vsfNei = psfNei[pFacei];

                maxVsf[own] = max(maxVsf[own], vsfNei);
                minVsf[own] = min(minVsf[own], vsfNei);
            }
        }
        else
        {
            forAll(pOwner, pFacei)
            {
                label own = pOwner[pFacei];
                const Type& vsfNei = psf[pFacei];

                maxVsf[own] = max(maxVsf[own], vsfNei);
                minVsf[own] = min(minVsf[own], vsfNei);
            }
        }
    }

    maxVsf -= vsf;
    minVsf -= vsf;

    if (k_ < 1.0)
    {
        const Field<Type> maxMinVsf((1.0/k_ - 1.0)*(maxVsf - minVsf));
        maxVsf += maxMinVsf;
        minVsf -= maxMinVsf;
    }


    // Create limiter initialized to 1
    // Note: the limiter is not permitted to be > 1
    Field<Type> limiter(vsf.internalField().size(), pTraits<Type>::one);

    forAll(owner, facei)
    {
        label own = owner[facei];
        label nei = neighbour[facei];

        // owner side
        limitFace
        (
            limiter[own],
            maxVsf[own],
            minVsf[own],
            (Cf[facei] - C[own]) & g[own]
        );

        // neighbour side
        limitFace
        (
            limiter[nei],
            maxVsf[nei],
            minVsf[nei],
            (Cf[facei] - C[nei]) & g[nei]
        );
    }

    forAll(bsf, patchi)
    {
        const labelUList& pOwner = mesh.boundary()[patchi].faceCells();
        const vectorField& pCf = Cf.boundaryField()[patchi];

        forAll(pOwner, pFacei)
        {
            label own = pOwner[pFacei];

            limitFace
            (
                limiter[own],
                maxVsf[own],
                minVsf[own],
                ((pCf[pFacei] - C[own]) & g[own])
            );
        }
    }

    if (fv::debug)
    {
        Info<< "gradient limiter for: " << vsf.name()
            << " max = " << gMax(limiter)
            << " min = " << gMin(limiter)
            << " average: " << gAverage(limiter) << endl;
    }

    limitGradient(limiter, g);
    g.correctBoundaryConditions();
    gaussGrad<Type>::correctBoundaryConditions(vsf, g);

    return tGrad;
}


#endif
