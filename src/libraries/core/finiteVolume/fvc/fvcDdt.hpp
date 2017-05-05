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
    Calculate the first temporal derivative.


\*---------------------------------------------------------------------------*/


#ifndef fvcDdt_H
#define fvcDdt_H

#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "dimensionedTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Namespace fvc functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvc
{
    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > ddt
    (
        const dimensioned<Type>,
        const fvMesh&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > ddt
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > ddt
    (
        const dimensionedScalar&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > ddt
    (
        const volScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp
    <
        GeometricField
        <
            typename CML::flux<Type>::type,
            fvsPatchField,
            surfaceMesh
        >
    >
    ddtPhiCorr
    (
        const volScalarField& rA,
        const GeometricField<Type, fvPatchField, volMesh>& U,
        const GeometricField
        <
            typename CML::flux<Type>::type,
            fvsPatchField,
            surfaceMesh
        >& phi
    );

    template<class Type>
    tmp
    <
        GeometricField
        <
            typename CML::flux<Type>::type,
            fvsPatchField,
            surfaceMesh
        >
    >
    ddtPhiCorr
    (
        const volScalarField& rA,
        const volScalarField& rho,
        const GeometricField<Type, fvPatchField, volMesh>& U,
        const GeometricField
        <
            typename CML::flux<Type>::type,
            fvsPatchField,
            surfaceMesh
        >& phi
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvMesh.hpp"
#include "ddtScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
ddt
(
    const dimensioned<Type> dt,
    const fvMesh& mesh
)
{
    return fv::ddtScheme<Type>::New
    (
        mesh,
        mesh.ddtScheme("ddt(" + dt.name() + ')')
    )().fvcDdt(dt);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
ddt
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fv::ddtScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().ddtScheme("ddt(" + vf.name() + ')')
    )().fvcDdt(vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
ddt
(
    const dimensionedScalar& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fv::ddtScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().ddtScheme("ddt(" + rho.name() + ',' + vf.name() + ')')
    )().fvcDdt(rho, vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
ddt
(
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fv::ddtScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().ddtScheme("ddt(" + rho.name() + ',' + vf.name() + ')')
    )().fvcDdt(rho, vf);
}


template<class Type>
tmp<GeometricField<typename flux<Type>::type, fvsPatchField, surfaceMesh> >
ddtPhiCorr
(
    const volScalarField& rA,
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const GeometricField
    <
        typename flux<Type>::type,
        fvsPatchField,
        surfaceMesh
    >& phi
)
{
    return fv::ddtScheme<Type>::New
    (
        U.mesh(),
        U.mesh().ddtScheme("ddt(" + U.name() + ')')
    )().fvcDdtPhiCorr(rA, U, phi);
}


template<class Type>
tmp<GeometricField<typename flux<Type>::type, fvsPatchField, surfaceMesh> >
ddtPhiCorr
(
    const volScalarField& rA,
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& U,
    const GeometricField
    <
        typename flux<Type>::type,
        fvsPatchField,
        surfaceMesh
    >& phi
)
{
    return fv::ddtScheme<Type>::New
    (
        U.mesh(),
        U.mesh().ddtScheme("ddt(" + rho.name() + ',' + U.name() + ')')
    )().fvcDdtPhiCorr(rA, rho, U, phi);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
