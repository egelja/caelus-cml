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
    Calculate the divergence of the given field.


\*---------------------------------------------------------------------------*/


#ifndef fvcDiv_H
#define fvcDiv_H

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
    tmp<GeometricField<Type, fvPatchField, volMesh> > div
    (
        const GeometricField<Type, fvsPatchField, surfaceMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > div
    (
        const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >&
    );


    template<class Type>
    tmp
    <
        GeometricField
        <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
    > div
    (
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word& name
    );

    template<class Type>
    tmp
    <
        GeometricField
        <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
    > div
    (
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word& name
    );


    template<class Type>
    tmp
    <
        GeometricField
        <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
    > div
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp
    <
        GeometricField
        <typename innerProduct<vector, Type>::type, fvPatchField, volMesh>
    > div
    (
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );


    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > div
    (
        const surfaceScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word& name
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > div
    (
        const tmp<surfaceScalarField>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word& name
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > div
    (
        const surfaceScalarField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word& name
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > div
    (
        const tmp<surfaceScalarField>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word& name
    );


    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > div
    (
        const surfaceScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > div
    (
        const tmp<surfaceScalarField>&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > div
    (
        const surfaceScalarField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > div
    (
        const tmp<surfaceScalarField>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvMesh.hpp"
#include "fvcSurfaceIntegrate.hpp"
#include "divScheme.hpp"
#include "convectionScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
div
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& ssf
)
{
    return tmp<GeometricField<Type, fvPatchField, volMesh> >
    (
        new GeometricField<Type, fvPatchField, volMesh>
        (
            "div("+ssf.name()+')',
            fvc::surfaceIntegrate(ssf)
        )
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
div
(
    const tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >& tssf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Div(fvc::div(tssf()));
    tssf.clear();
    return Div;
}


template<class Type>
tmp
<
    GeometricField
    <
        typename innerProduct<vector, Type>::type, fvPatchField, volMesh
    >
>
div
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    return fv::divScheme<Type>::New
    (
        vf.mesh(), vf.mesh().divScheme(name)
    )().fvcDiv(vf);
}


template<class Type>
tmp
<
    GeometricField
    <
        typename innerProduct<vector, Type>::type, fvPatchField, volMesh
    >
>
div
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvvf,
    const word& name
)
{
    typedef typename innerProduct<vector, Type>::type DivType;
    tmp<GeometricField<DivType, fvPatchField, volMesh> > Div
    (
        fvc::div(tvvf(), name)
    );
    tvvf.clear();
    return Div;
}

template<class Type>
tmp
<
    GeometricField
    <
        typename innerProduct<vector, Type>::type, fvPatchField, volMesh
    >
>
div
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvc::div(vf, "div("+vf.name()+')');
}


template<class Type>
tmp
<
    GeometricField
    <
        typename innerProduct<vector, Type>::type, fvPatchField, volMesh
    >
>
div
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvvf
)
{
    typedef typename innerProduct<vector, Type>::type DivType;
    tmp<GeometricField<DivType, fvPatchField, volMesh> > Div(fvc::div(tvvf()));
    tvvf.clear();
    return Div;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
div
(
    const surfaceScalarField& flux,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    return fv::convectionScheme<Type>::New
    (
        vf.mesh(),
        flux,
        vf.mesh().divScheme(name)
    )().fvcDiv(flux, vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
div
(
    const tmp<surfaceScalarField>& tflux,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Div
    (
        fvc::div(tflux(), vf, name)
    );
    tflux.clear();
    return Div;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
div
(
    const surfaceScalarField& flux,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Div
    (
        fvc::div(flux, tvf(), name)
    );
    tvf.clear();
    return Div;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
div
(
    const tmp<surfaceScalarField>& tflux,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Div
    (
        fvc::div(tflux(), tvf(), name)
    );
    tflux.clear();
    tvf.clear();
    return Div;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
div
(
    const surfaceScalarField& flux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvc::div
    (
        flux, vf, "div("+flux.name()+','+vf.name()+')'
    );
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
div
(
    const tmp<surfaceScalarField>& tflux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Div
    (
        fvc::div(tflux(), vf)
    );
    tflux.clear();
    return Div;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
div
(
    const surfaceScalarField& flux,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Div
    (
        fvc::div(flux, tvf())
    );
    tvf.clear();
    return Div;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
div
(
    const tmp<surfaceScalarField>& tflux,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Div
    (
        fvc::div(tflux(), tvf())
    );
    tflux.clear();
    tvf.clear();
    return Div;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
