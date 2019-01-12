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
    CML::surfaceInterpolationScheme

Description
    Abstract base class for surface interpolation schemes.

SourceFiles
    surfaceInterpolationScheme.cpp

\*---------------------------------------------------------------------------*/

#ifndef surfaceInterpolationScheme_H
#define surfaceInterpolationScheme_H

#include "tmp.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;

/*---------------------------------------------------------------------------*\
                 Class surfaceInterpolationScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class surfaceInterpolationScheme
:
    public refCount
{
    // Private data

        //- Hold reference to mesh
        const fvMesh& mesh_;


    // Private Member Functions

        //- Disallow copy construct
        surfaceInterpolationScheme(const surfaceInterpolationScheme&);

        //- Disallow default bitwise assignment
        void operator=(const surfaceInterpolationScheme&);


public:

    //- Runtime type information
    TypeName("surfaceInterpolationScheme");


    // Declare run-time constructor selection tables

        declareRunTimeSelectionTable
        (
            tmp,
            surfaceInterpolationScheme,
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
            surfaceInterpolationScheme,
            MeshFlux,
            (
                const fvMesh& mesh,
                const surfaceScalarField& faceFlux,
                Istream& schemeData
            ),
            (mesh, faceFlux, schemeData)
        );


    // Constructors

        //- Construct from mesh
        surfaceInterpolationScheme(const fvMesh& mesh)
        :
            mesh_(mesh)
        {}


    // Selectors

        //- Return new tmp interpolation scheme
        static tmp<surfaceInterpolationScheme<Type> > New
        (
            const fvMesh& mesh,
            Istream& schemeData
        );

        //- Return new tmp interpolation scheme
        static tmp<surfaceInterpolationScheme<Type> > New
        (
            const fvMesh& mesh,
            const surfaceScalarField& faceFlux,
            Istream& schemeData
        );


    //- Destructor
    virtual ~surfaceInterpolationScheme();


    // Member Functions

        //- Return mesh reference
        const fvMesh& mesh() const
        {
            return mesh_;
        }


        //- Return the face-interpolate of the given cell field
        //  with the given owner and neighbour weigting factors
        static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        interpolate
        (
            const GeometricField<Type, fvPatchField, volMesh>&,
            const tmp<surfaceScalarField>&,
            const tmp<surfaceScalarField>&
        );

        //- Return the face-interpolate of the given cell field
        //  with the given weighting factors
        static tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        interpolate
        (
            const GeometricField<Type, fvPatchField, volMesh>&,
            const tmp<surfaceScalarField>&
        );


        //- Return the interpolation weighting factors for the given field
        virtual tmp<surfaceScalarField> weights
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const = 0;

        //- Return true if this scheme uses an explicit correction
        virtual bool corrected() const
        {
            return false;
        }

        //- Return the explicit correction to the face-interpolate
        //  for the given field
        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        correction(const GeometricField<Type, fvPatchField, volMesh>&) const
        {
            return tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >(nullptr);
        }

        //- Return the face-interpolate of the given cell field
        //  with explicit correction
        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        interpolate(const GeometricField<Type, fvPatchField, volMesh>&) const;

        //- Return the face-interpolate of the given tmp cell field
        //  with explicit correction
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        interpolate
        (
            const tmp<GeometricField<Type, fvPatchField, volMesh> >&
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Add the patch constructor functions to the hash tables

#define makeSurfaceInterpolationTypeScheme(SS, Type)                           \
                                                                               \
defineNamedTemplateTypeNameAndDebug(SS<Type>, 0);                              \
                                                                               \
surfaceInterpolationScheme<Type>::addMeshConstructorToTable<SS<Type> >         \
    add##SS##Type##MeshConstructorToTable_;                                    \
                                                                               \
surfaceInterpolationScheme<Type>::addMeshFluxConstructorToTable<SS<Type> >     \
    add##SS##Type##MeshFluxConstructorToTable_;

#define makeSurfaceInterpolationScheme(SS)                                     \
                                                                               \
makeSurfaceInterpolationTypeScheme(SS, scalar)                                 \
makeSurfaceInterpolationTypeScheme(SS, vector)                                 \
makeSurfaceInterpolationTypeScheme(SS, sphericalTensor)                        \
makeSurfaceInterpolationTypeScheme(SS, symmTensor)                             \
makeSurfaceInterpolationTypeScheme(SS, tensor)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "coupledFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * //

// Return weighting factors for scheme given by name in dictionary
template<class Type>
tmp<surfaceInterpolationScheme<Type> > surfaceInterpolationScheme<Type>::New
(
    const fvMesh& mesh,
    Istream& schemeData
)
{
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

    if (surfaceInterpolation::debug || surfaceInterpolationScheme<Type>::debug)
    {
        Info<< "surfaceInterpolationScheme<Type>::New"
               "(const fvMesh&, Istream&)"
               " : discretisation scheme = "
            << schemeName
            << endl;
    }

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
tmp<surfaceInterpolationScheme<Type> > surfaceInterpolationScheme<Type>::New
(
    const fvMesh& mesh,
    const surfaceScalarField& faceFlux,
    Istream& schemeData
)
{
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

    if (surfaceInterpolation::debug || surfaceInterpolationScheme<Type>::debug)
    {
        Info<< "surfaceInterpolationScheme<Type>::New"
               "(const fvMesh&, const surfaceScalarField&, Istream&)"
               " : discretisation scheme = "
            << schemeName
            << endl;
    }

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
surfaceInterpolationScheme<Type>::~surfaceInterpolationScheme()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Return the face-interpolate of the given cell field
//  with the given owner and neighbour weighting factors
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
surfaceInterpolationScheme<Type>::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const tmp<surfaceScalarField>& tlambdas,
    const tmp<surfaceScalarField>& tys
)
{
    if (surfaceInterpolation::debug)
    {
        Info<< "surfaceInterpolationScheme<Type>::uncorrectedInterpolate"
               "(const GeometricField<Type, fvPatchField, volMesh>&, "
               "const tmp<surfaceScalarField>&, "
               "const tmp<surfaceScalarField>&) : "
               "interpolating volTypeField from cells to faces "
               "without explicit correction"
            << endl;
    }

    const surfaceScalarField& lambdas = tlambdas();
    const surfaceScalarField& ys = tys();

    const Field<Type>& vfi = vf.internalField();
    const scalarField& lambda = lambdas.internalField();
    const scalarField& y = ys.internalField();

    const fvMesh& mesh = vf.mesh();
    const labelUList& P = mesh.owner();
    const labelUList& N = mesh.neighbour();

    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "interpolate("+vf.name()+')',
                vf.instance(),
                vf.db()
            ),
            mesh,
            vf.dimensions()
        )
    );
    GeometricField<Type, fvsPatchField, surfaceMesh>& sf = tsf();

    Field<Type>& sfi = sf.internalField();

    for (register label fi=0; fi<P.size(); fi++)
    {
        sfi[fi] = lambda[fi]*vfi[P[fi]] + y[fi]*vfi[N[fi]];
    }


    // Interpolate across coupled patches using given lambdas and ys

    forAll(lambdas.boundaryField(), pi)
    {
        const fvsPatchScalarField& pLambda = lambdas.boundaryField()[pi];
        const fvsPatchScalarField& pY = ys.boundaryField()[pi];

        if (vf.boundaryField()[pi].coupled())
        {
            sf.boundaryField()[pi] =
                pLambda*vf.boundaryField()[pi].patchInternalField()
              + pY*vf.boundaryField()[pi].patchNeighbourField();
        }
        else
        {
            sf.boundaryField()[pi] = vf.boundaryField()[pi];
        }
    }

    tlambdas.clear();
    tys.clear();

    return tsf;
}


//- Return the face-interpolate of the given cell field
//  with the given weigting factors
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
surfaceInterpolationScheme<Type>::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const tmp<surfaceScalarField>& tlambdas
)
{
    if (surfaceInterpolation::debug)
    {
        Info<< "surfaceInterpolationScheme<Type>::interpolate"
               "(const GeometricField<Type, fvPatchField, volMesh>&, "
               "const tmp<surfaceScalarField>&) : "
               "interpolating volTypeField from cells to faces "
               "without explicit correction"
            << endl;
    }

    const surfaceScalarField& lambdas = tlambdas();

    const Field<Type>& vfi = vf.internalField();
    const scalarField& lambda = lambdas.internalField();

    const fvMesh& mesh = vf.mesh();
    const labelUList& P = mesh.owner();
    const labelUList& N = mesh.neighbour();

    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                "interpolate("+vf.name()+')',
                vf.instance(),
                vf.db()
            ),
            mesh,
            vf.dimensions()
        )
    );
    GeometricField<Type, fvsPatchField, surfaceMesh>& sf = tsf();

    Field<Type>& sfi = sf.internalField();

    for (register label fi=0; fi<P.size(); fi++)
    {
        sfi[fi] = lambda[fi]*(vfi[P[fi]] - vfi[N[fi]]) + vfi[N[fi]];
    }

    // Interpolate across coupled patches using given lambdas

    forAll(lambdas.boundaryField(), pi)
    {
        const fvsPatchScalarField& pLambda = lambdas.boundaryField()[pi];

        if (vf.boundaryField()[pi].coupled())
        {
            tsf().boundaryField()[pi] =
                pLambda*vf.boundaryField()[pi].patchInternalField()
             + (1.0 - pLambda)*vf.boundaryField()[pi].patchNeighbourField();
        }
        else
        {
            sf.boundaryField()[pi] = vf.boundaryField()[pi];
        }
    }

    tlambdas.clear();

    return tsf;
}


//- Return the face-interpolate of the given cell field
//  with explicit correction
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
surfaceInterpolationScheme<Type>::interpolate
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    if (surfaceInterpolation::debug)
    {
        Info<< "surfaceInterpolationScheme<Type>::interpolate"
               "(const GeometricField<Type, fvPatchField, volMesh>&) : "
            << "interpolating volTypeField from cells to faces"
            << endl;
    }

    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsf
        = interpolate(vf, weights(vf));

    if (corrected())
    {
        tsf() += correction(vf);
    }

    return tsf;
}


//- Return the face-interpolate of the given cell field
//  with explicit correction
template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
surfaceInterpolationScheme<Type>::interpolate
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
) const
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tinterpVf
        = interpolate(tvf());
    tvf.clear();
    return tinterpVf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
