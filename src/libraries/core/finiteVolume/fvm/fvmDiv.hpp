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
    CML::fvm

Description
    Calculate the matrix for the divergence of the given field and flux.


\*---------------------------------------------------------------------------*/

#ifndef fvmDiv_H
#define fvmDiv_H

#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "surfaceInterpolationScheme.hpp"
#include "fvMatrix.hpp"
#include "fvMesh.hpp"
#include "fvMatrix.hpp"
#include "convectionScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Namespace fvm functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvm
{
    template<class Type>
    tmp<fvMatrix<Type> > div
    (
        const surfaceScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word& name
    );

    template<class Type>
    tmp<fvMatrix<Type> > div
    (
        const tmp<surfaceScalarField>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word& name
    );


    template<class Type>
    tmp<fvMatrix<Type> > div
    (
        const surfaceScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<fvMatrix<Type> > div
    (
        const tmp<surfaceScalarField>&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


template<class Type>
tmp<fvMatrix<Type> >
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
    )().fvmDiv(flux, vf);
}

template<class Type>
tmp<fvMatrix<Type> >
div
(
    const tmp<surfaceScalarField>& tflux,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    tmp<fvMatrix<Type> > Div(fvm::div(tflux(), vf, name));
    tflux.clear();
    return Div;
}


template<class Type>
tmp<fvMatrix<Type> >
div
(
    const surfaceScalarField& flux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvm::div(flux, vf, "div("+flux.name()+','+vf.name()+')');
}

template<class Type>
tmp<fvMatrix<Type> >
div
(
    const tmp<surfaceScalarField>& tflux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > Div(fvm::div(tflux(), vf));
    tflux.clear();
    return Div;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvm

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
