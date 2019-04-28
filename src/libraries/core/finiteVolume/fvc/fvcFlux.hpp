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
    Calculate the face-flux of the given field.

SourceFiles
    fvcFlux.cpp

\*---------------------------------------------------------------------------*/


#ifndef fvcFlux_H
#define fvcFlux_H

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
    //- Return the face-flux field obtained from the given volVectorField
    tmp<surfaceScalarField> flux(const volVectorField& vvf);

    //- Return the face-flux field obtained from the given tmp volVectorField
    tmp<surfaceScalarField> flux(const tmp<volVectorField>& tvvf);

    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
    (
        const surfaceScalarField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );

    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
    (
        const surfaceScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word& name
    );

    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
    (
        const tmp<surfaceScalarField>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word& name
    );

    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
    (
        const surfaceScalarField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word& name
    );

    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
    (
        const tmp<surfaceScalarField>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
        const word& name
    );


    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
    (
        const surfaceScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
    (
        const tmp<surfaceScalarField>&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
    (
        const surfaceScalarField&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );

    template<class Type>
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
    (
        const tmp<surfaceScalarField>&,
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvMesh.hpp"
#include "convectionScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
flux
(
    const surfaceScalarField& phi,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    return fv::convectionScheme<Type>::New
    (
        vf.mesh(),
        phi,
        vf.mesh().divScheme(name)
    )().flux(phi, vf);
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
flux
(
    const tmp<surfaceScalarField>& tphi,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > Flux
    (
        fvc::flux(tphi(), vf, name)
    );
    tphi.clear();
    return Flux;
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
flux
(
    const surfaceScalarField& phi,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > Flux
    (
        fvc::flux(phi, tvf(), name)
    );
    tvf.clear();
    return Flux;
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
flux
(
    const tmp<surfaceScalarField>& tphi,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf,
    const word& name
)
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > Flux
    (
        fvc::flux(tphi(), tvf(), name)
    );
    tphi.clear();
    tvf.clear();
    return Flux;
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
flux
(
    const surfaceScalarField& phi,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvc::flux
    (
        phi, vf, "flux("+phi.name()+','+vf.name()+')'
    );
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
flux
(
    const tmp<surfaceScalarField>& tphi,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > Flux
    (
        fvc::flux(tphi(), vf)
    );
    tphi.clear();
    return Flux;
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
flux
(
    const surfaceScalarField& phi,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > Flux
    (
        fvc::flux(phi, tvf())
    );
    tvf.clear();
    return Flux;
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
flux
(
    const tmp<surfaceScalarField>& tphi,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > Flux
    (
        fvc::flux(tphi(), tvf())
    );
    tphi.clear();
    tvf.clear();
    return Flux;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
