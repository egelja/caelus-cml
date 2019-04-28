/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::PhiScheme

Description
    Class to create the weighting-factors based on the face-flux.

    The particular differencing scheme class is supplied as a template
    argument, the weight function of which is called by the weight function
    of this class for the internal faces as well as faces of coupled
    patches (e.g. processor-processor patches). The weight function is
    supplied with the central-differencing weighting factor, the face-flux,
    the face neighbour cell values and the face area.

    This code organisation is both neat and efficient, allowing for
    convenient implementation of new schemes to run on parallelised cases.

SourceFiles
    PhiScheme.cpp

\*---------------------------------------------------------------------------*/

#ifndef PhiScheme_H
#define PhiScheme_H

#include "limitedSurfaceInterpolationScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class PhiScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type, class PhiLimiter>
class PhiScheme
:
    public limitedSurfaceInterpolationScheme<Type>,
    public PhiLimiter
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        PhiScheme(const PhiScheme&);

        //- Disallow default bitwise assignment
        void operator=(const PhiScheme&);


public:

    //- Runtime type information
    TypeName("PhiScheme");


    // Constructors

        //- Construct from mesh, faceFlux and blendingFactor
        PhiScheme
        (
            const fvMesh& mesh,
            const surfaceScalarField& faceFlux,
            const PhiLimiter& weight
        )
        :
            limitedSurfaceInterpolationScheme<Type>(mesh, faceFlux),
            PhiLimiter(weight)
        {}

        //- Construct from mesh and Istream.
        //  The name of the flux field is read from the Istream and looked-up
        //  from the mesh objectRegistry
        PhiScheme
        (
            const fvMesh& mesh,
            Istream& is
        )
        :
            limitedSurfaceInterpolationScheme<Type>(mesh, is),
            PhiLimiter(is)
        {}

        //- Construct from mesh, faceFlux and Istream
        PhiScheme
        (
            const fvMesh& mesh,
            const surfaceScalarField& faceFlux,
            Istream& is
        )
        :
            limitedSurfaceInterpolationScheme<Type>(mesh, faceFlux),
            PhiLimiter(is)
        {}


    // Member Functions

        //- Return the interpolation weighting factors
        virtual tmp<surfaceScalarField> limiter
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makePhiSurfaceInterpolationScheme(SS, WEIGHT, TYPE)                    \
                                                                               \
typedef PhiScheme<TYPE, WEIGHT> Phischeme##WEIGHT_;                            \
defineTemplateTypeNameAndDebugWithName(Phischeme##WEIGHT_, #SS, 0);            \
                                                                               \
surfaceInterpolationScheme<TYPE>::addMeshConstructorToTable                    \
<PhiScheme<TYPE, WEIGHT> > add##SS##TYPE##MeshConstructorToTable_;             \
                                                                               \
surfaceInterpolationScheme<TYPE>::addMeshFluxConstructorToTable                \
<PhiScheme<TYPE, WEIGHT> > add##SS##TYPE##MeshFluxConstructorToTable_;         \
                                                                               \
limitedSurfaceInterpolationScheme<TYPE>::addMeshConstructorToTable             \
<PhiScheme<TYPE, WEIGHT> > add##SS##TYPE##MeshConstructorToLimitedTable_;      \
                                                                               \
limitedSurfaceInterpolationScheme<TYPE>::addMeshFluxConstructorToTable         \
<PhiScheme<TYPE, WEIGHT> > add##SS##TYPE##MeshFluxConstructorToLimitedTable_;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "fvcGrad.hpp"
#include "coupledFvPatchFields.hpp"
#include "surfaceInterpolate.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, class PhiLimiter>
CML::tmp<CML::surfaceScalarField>
CML::PhiScheme<Type, PhiLimiter>::limiter
(
    const GeometricField<Type, fvPatchField, volMesh>& phi
) const
{
    const fvMesh& mesh = this->mesh();

    tmp<surfaceScalarField> tLimiter
    (
        new surfaceScalarField
        (
            IOobject
            (
                "PhiLimiter",
                mesh.time().timeName(),
                mesh
            ),
            mesh,
            dimless
        )
    );
    surfaceScalarField& Limiter = tLimiter();

    const surfaceScalarField& CDweights = mesh.surfaceInterpolation::weights();

    const surfaceVectorField& Sf = mesh.Sf();
    const surfaceScalarField& magSf = mesh.magSf();

    const labelUList& owner = mesh.owner();
    const labelUList& neighbour = mesh.neighbour();

    tmp<surfaceScalarField> tUflux = this->faceFlux_;

    if (this->faceFlux_.dimensions() == dimDensity*dimVelocity*dimArea)
    {
        const volScalarField& rho =
            phi.db().objectRegistry::template lookupObject<volScalarField>
            ("rho");

        tUflux = this->faceFlux_/fvc::interpolate(rho);
    }
    else if (this->faceFlux_.dimensions() != dimVelocity*dimArea)
    {
        FatalErrorInFunction
            << "dimensions of faceFlux are not correct"
            << exit(FatalError);
    }

    const surfaceScalarField& Uflux = tUflux();

    scalarField& pLimiter = Limiter.internalField();

    forAll(pLimiter, face)
    {
        pLimiter[face] = PhiLimiter::limiter
        (
            CDweights[face],
            Uflux[face],
            phi[owner[face]],
            phi[neighbour[face]],
            Sf[face],
            magSf[face]
        );
    }


    surfaceScalarField::GeometricBoundaryField& bLimiter =
        Limiter.boundaryField();

    forAll(bLimiter, patchI)
    {
        scalarField& pLimiter = bLimiter[patchI];

        if (bLimiter[patchI].coupled())
        {
            const scalarField& pCDweights = CDweights.boundaryField()[patchI];
            const vectorField& pSf = Sf.boundaryField()[patchI];
            const scalarField& pmagSf = magSf.boundaryField()[patchI];
            const scalarField& pFaceFlux = Uflux.boundaryField()[patchI];

            const Field<Type> pphiP
            (
                phi.boundaryField()[patchI].patchInternalField()
            );
            const Field<Type> pphiN
            (
                phi.boundaryField()[patchI].patchNeighbourField()
            );

            forAll(pLimiter, face)
            {
                pLimiter[face] = PhiLimiter::limiter
                (
                    pCDweights[face],
                    pFaceFlux[face],
                    pphiP[face],
                    pphiN[face],
                    pSf[face],
                    pmagSf[face]
                );
            }
        }
        else
        {
            pLimiter = 1.0;
        }
    }

    return tLimiter;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
