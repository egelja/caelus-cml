/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

InNamespace
    CML::fvc

Description
    Reconstruct volField from a face flux field.


\*---------------------------------------------------------------------------*/


#ifndef fvcReconstruct_H
#define fvcReconstruct_H

#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Namespace fvc functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvc
{
    template<class Type>
    tmp
    <
        GeometricField
        <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > reconstruct
    (
        const GeometricField<Type, fvsPatchField, surfaceMesh>&
    );

    template<class Type>
    tmp
    <
        GeometricField
        <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > reconstruct
    (
        const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >&
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvMesh.hpp"
#include "extrapolatedCalculatedFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp
<
    GeometricField
    <
        typename outerProduct<vector,Type>::type, fvPatchField, volMesh
    >
>
reconstruct
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& ssf
)
{
    typedef typename outerProduct<vector, Type>::type GradType;

    const fvMesh& mesh = ssf.mesh();

    surfaceVectorField faceVols
    (
        mesh.Sf()/(mesh.magSf()*mesh.nonOrthDeltaCoeffs())
    );

    faceVols.internalField() *= (1.0 -  mesh.weights().internalField());
    forAll(faceVols.boundaryField(), patchi)
    {
        if (faceVols.boundaryField()[patchi].coupled())
        {
            faceVols.boundaryField()[patchi] *=
                (1.0 -  mesh.weights().boundaryField()[patchi]);
        }
    }

    tmp<GeometricField<GradType, fvPatchField, volMesh> > treconField
    (
        new GeometricField<GradType, fvPatchField, volMesh>
        (
            IOobject
            (
                "volIntegrate("+ssf.name()+')',
                ssf.instance(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            inv(surfaceSum(mesh.Sf()*faceVols))&surfaceSum(faceVols*ssf),
            extrapolatedCalculatedFvPatchField<GradType>::typeName
        )
    );

    treconField().correctBoundaryConditions();

    return treconField;
}


template<class Type>
tmp
<
    GeometricField
    <
        typename outerProduct<vector, Type>::type, fvPatchField, volMesh
    >
>
reconstruct
(
    const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >& tssf
)
{
    typedef typename outerProduct<vector, Type>::type GradType;
    tmp<GeometricField<GradType, fvPatchField, volMesh> > tvf
    (
        fvc::reconstruct(tssf())
    );
    tssf.clear();
    return tvf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
