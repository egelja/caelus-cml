/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2016 Applied CCM
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
    CML::limitedSurfaceInterpolationScheme

Description
    Abstract base class for limited surface interpolation schemes.


\*---------------------------------------------------------------------------*/

#ifndef limitedSurfaceInterpolationScheme_H
#define limitedSurfaceInterpolationScheme_H

#include "surfaceInterpolationScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class limitedSurfaceInterpolationScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class limitedSurfaceInterpolationScheme
:
    public surfaceInterpolationScheme<Type>
{
    // Private Member Functions

        //- Disallow copy construct
        limitedSurfaceInterpolationScheme
        (
            const limitedSurfaceInterpolationScheme&
        );

        //- Disallow default bitwise assignment
        void operator=(const limitedSurfaceInterpolationScheme&);


protected:

    // Protected data

        const surfaceScalarField& faceFlux_;


public:

    //- Runtime type information
    TypeName("limitedSurfaceInterpolationScheme");


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            limitedSurfaceInterpolationScheme,
            Mesh,
            (
                const fvMesh& mesh,
                Istream& schemeData
            ),
            (mesh, schemeData)
        );

        declareRunTimeSelectionTable
        (
            tmp,
            limitedSurfaceInterpolationScheme,
            MeshFlux,
            (
                const fvMesh& mesh,
                const surfaceScalarField& faceFlux,
                Istream& schemeData
            ),
            (mesh, faceFlux, schemeData)
        );


    // Constructors

        //- Construct from mesh and faceFlux
        limitedSurfaceInterpolationScheme
        (
            const fvMesh& mesh,
            const surfaceScalarField& faceFlux
        )
        :
            surfaceInterpolationScheme<Type>(mesh),
            faceFlux_(faceFlux)
        {}


        //- Construct from mesh and Istream.
        //  The name of the flux field is read from the Istream and looked-up
        //  from the mesh objectRegistry
        limitedSurfaceInterpolationScheme
        (
            const fvMesh& mesh,
            Istream& is
        )
        :
            surfaceInterpolationScheme<Type>(mesh),
            faceFlux_
            (
                mesh.lookupObject<surfaceScalarField>
                (
                    word(is)
                )
            )
        {}


    // Selectors

        //- Return new tmp interpolation scheme
        static tmp<limitedSurfaceInterpolationScheme<Type> > New
        (
            const fvMesh& mesh,
            Istream& schemeData
        );

        //- Return new tmp interpolation scheme
        static tmp<limitedSurfaceInterpolationScheme<Type> > New
        (
            const fvMesh& mesh,
            const surfaceScalarField& faceFlux,
            Istream& schemeData
        );


    //- Destructor
    virtual ~limitedSurfaceInterpolationScheme();


    // Member Functions

        //- Return the interpolation weighting factors
        virtual tmp<surfaceScalarField> limiter
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const = 0;

        //- Return the interpolation weighting factors for the given field,
        //  by limiting the given weights with the given limiter
        tmp<surfaceScalarField> weights
        (
            const GeometricField<Type, fvPatchField, volMesh>&,
            const surfaceScalarField& CDweights,
            tmp<surfaceScalarField> tLimiter
        ) const;

        //- Return the interpolation weighting factors for the given field
        virtual tmp<surfaceScalarField> weights
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;

        //- Return the interpolation weighting factors
        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        flux
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makelimitedSurfaceInterpolationTypeScheme(SS, Type)                    \
                                                                               \
defineNamedTemplateTypeNameAndDebug(SS<Type>, 0);                              \
                                                                               \
surfaceInterpolationScheme<Type>::addMeshConstructorToTable<SS<Type> >         \
    add##SS##Type##MeshConstructorToTable_;                                    \
                                                                               \
surfaceInterpolationScheme<Type>::addMeshFluxConstructorToTable<SS<Type> >     \
    add##SS##Type##MeshFluxConstructorToTable_;                                \
                                                                               \
limitedSurfaceInterpolationScheme<Type>::addMeshConstructorToTable<SS<Type> >  \
    add##SS##Type##MeshConstructorToLimitedTable_;                             \
                                                                               \
limitedSurfaceInterpolationScheme<Type>::                                      \
    addMeshFluxConstructorToTable<SS<Type> >                                   \
    add##SS##Type##MeshFluxConstructorToLimitedTable_;

#define makelimitedSurfaceInterpolationScheme(SS)                              \
                                                                               \
makelimitedSurfaceInterpolationTypeScheme(SS, scalar)                          \
makelimitedSurfaceInterpolationTypeScheme(SS, vector)                          \
makelimitedSurfaceInterpolationTypeScheme(SS, sphericalTensor)                 \
makelimitedSurfaceInterpolationTypeScheme(SS, symmTensor)                      \
makelimitedSurfaceInterpolationTypeScheme(SS, tensor)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "coupledFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

template<class Type>
tmp<limitedSurfaceInterpolationScheme<Type> >
limitedSurfaceInterpolationScheme<Type>::New
(
    const fvMesh& mesh,
    Istream& schemeData
)
{
    if (surfaceInterpolation::debug)
    {
        Info<< "limitedSurfaceInterpolationScheme<Type>::"
               "New(const fvMesh&, Istream&)"
               " : constructing limitedSurfaceInterpolationScheme<Type>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorInFunction(schemeData)
            << "Discretisation scheme not specified"
            << endl << endl
            << "Valid schemes are :" << endl
            << MeshConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename MeshConstructorTable::iterator constructorIter =
        MeshConstructorTablePtr_->find(schemeName);

    if (constructorIter == MeshConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(schemeData)
            << "Unknown discretisation scheme "
            << schemeName << nl << nl
            << "Valid schemes are :" << endl
            << MeshConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return constructorIter()(mesh, schemeData);
}


// Return weighting factors for scheme given by name in dictionary
template<class Type>
tmp<limitedSurfaceInterpolationScheme<Type> >
limitedSurfaceInterpolationScheme<Type>::New
(
    const fvMesh& mesh,
    const surfaceScalarField& faceFlux,
    Istream& schemeData
)
{
    if (surfaceInterpolation::debug)
    {
        Info<< "limitedSurfaceInterpolationScheme<Type>::New"
               "(const fvMesh&, const surfaceScalarField&, Istream&) : "
               "constructing limitedSurfaceInterpolationScheme<Type>"
            << endl;
    }

    if (schemeData.eof())
    {
        FatalIOErrorInFunction(schemeData)
            << "Discretisation scheme not specified"
            << endl << endl
            << "Valid schemes are :" << endl
            << MeshConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    const word schemeName(schemeData);

    typename MeshFluxConstructorTable::iterator constructorIter =
        MeshFluxConstructorTablePtr_->find(schemeName);

    if (constructorIter == MeshFluxConstructorTablePtr_->end())
    {
        FatalIOErrorInFunction(schemeData)
            << "Unknown discretisation scheme "
            << schemeName << nl << nl
            << "Valid schemes are :" << endl
            << MeshFluxConstructorTablePtr_->sortedToc()
            << exit(FatalIOError);
    }

    return constructorIter()(mesh, faceFlux, schemeData);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
limitedSurfaceInterpolationScheme<Type>::~limitedSurfaceInterpolationScheme()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
tmp<surfaceScalarField> limitedSurfaceInterpolationScheme<Type>::weights
(
    const GeometricField<Type, fvPatchField, volMesh>& phi,
    const surfaceScalarField& CDweights,
    tmp<surfaceScalarField> tLimiter
) const
{
    // Note that here the weights field is initialised as the limiter
    // from which the weight is calculated using the limiter value
    surfaceScalarField& Weights = tLimiter();

    scalarField& pWeights = Weights.internalField();

    forAll(pWeights, face)
    {
        pWeights[face] = pos(faceFlux_[face])
            + pWeights[face]*(CDweights[face]-pos(faceFlux_[face]));
    }

    surfaceScalarField::GeometricBoundaryField& bWeights =
        Weights.boundaryField();

    forAll(bWeights, patchI)
    {
        scalarField& pWeights = bWeights[patchI];

        const scalarField& pCDweights = CDweights.boundaryField()[patchI];
        const scalarField& pFaceFlux = faceFlux_.boundaryField()[patchI];

        forAll(pWeights, face)
        {
            pWeights[face] = pos(pFaceFlux[face])
                + pWeights[face]*(pCDweights[face]-pos(pFaceFlux[face]));
        }
    }

    return tLimiter;
}

template<class Type>
tmp<surfaceScalarField> limitedSurfaceInterpolationScheme<Type>::weights
(
    const GeometricField<Type, fvPatchField, volMesh>& phi
) const
{
    return this->weights
    (
        phi,
        this->mesh().surfaceInterpolation::weights(),
        this->limiter(phi)
    );
}

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
limitedSurfaceInterpolationScheme<Type>::flux
(
    const GeometricField<Type, fvPatchField, volMesh>& phi
) const
{
    return faceFlux_*this->interpolate(phi);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
