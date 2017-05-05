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
    Calculate the gradient of the given field.


\*---------------------------------------------------------------------------*/


#ifndef fvcGrad_H
#define fvcGrad_H

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
    > grad
    (
        const GeometricField<Type, fvsPatchField, surfaceMesh>&
    );

    template<class Type>
    tmp
    <
        GeometricField
        <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > grad
    (
        const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >&
    );

    template<class Type>
    tmp
    <
        GeometricField
        <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > grad
    (
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word& name
    );

    template<class Type>
    tmp
    <
        GeometricField
        <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > grad
    (
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word& name
    );

    template<class Type>
    tmp
    <
        GeometricField
        <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > grad
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp
    <
        GeometricField
        <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > grad
    (
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvcSurfaceIntegrate.hpp"
#include "fvMesh.hpp"
#include "gaussGrad.hpp"

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
        typename outerProduct<vector, Type>::type, fvPatchField, volMesh
    >
>
grad
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& ssf
)
{
    return fv::gaussGrad<Type>::gradf(ssf, "grad(" + ssf.name() + ')');
}


template<class Type>
tmp
<
    GeometricField
    <
        typename outerProduct<vector,Type>::type, fvPatchField, volMesh
    >
>
grad
(
    const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >& tssf
)
{
    typedef typename outerProduct<vector, Type>::type GradType;
    tmp<GeometricField<GradType, fvPatchField, volMesh> > Grad
    (
        fvc::grad(tssf())
    );
    tssf.clear();
    return Grad;
}


template<class Type>
tmp
<
    GeometricField
    <
        typename outerProduct<vector,Type>::type, fvPatchField, volMesh
    >
>
grad
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    return fv::gradScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().gradScheme(name)
    )().grad(vf, name);
}


template<class Type>
tmp
<
    GeometricField
    <
        typename outerProduct<vector,Type>::type, fvPatchField, volMesh
    >
>
grad
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp
    <
        GeometricField
        <
            typename outerProduct<vector, Type>::type, fvPatchField, volMesh
        >
    > tGrad
    (
        fvc::grad(tvf(), name)
    );
    tvf.clear();
    return tGrad;
}


template<class Type>
tmp
<
    GeometricField
    <
        typename outerProduct<vector,Type>::type, fvPatchField, volMesh
    >
>
grad
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvc::grad(vf, "grad(" + vf.name() + ')');
}


template<class Type>
tmp
<
    GeometricField
    <
        typename outerProduct<vector,Type>::type, fvPatchField, volMesh
    >
>
grad
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    typedef typename outerProduct<vector, Type>::type GradType;
    tmp<GeometricField<GradType, fvPatchField, volMesh> > Grad
    (
        fvc::grad(tvf())
    );
    tvf.clear();
    return Grad;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
