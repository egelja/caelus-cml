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
    Calulate the matrix for the first temporal derivative.


\*---------------------------------------------------------------------------*/

#ifndef fvmDdt_H
#define fvmDdt_H

#include "volFieldsFwd.hpp"
#include "fvMatrix.hpp"
#include "one.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "ddtScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Namespace fvm functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvm
{
    template<class Type>
    tmp<fvMatrix<Type> > ddt
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<fvMatrix<Type> > ddt
    (
        const one&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<fvMatrix<Type> > ddt
    (
        const dimensionedScalar&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<fvMatrix<Type> > ddt
    (
        const volScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<fvMatrix<Type> >
ddt
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fv::ddtScheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().ddtScheme("ddt(" + vf.name() + ')')
    )().fvmDdt(vf);
}


template<class Type>
tmp<fvMatrix<Type> >
ddt
(
    const one&,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return ddt(vf);
}


template<class Type>
tmp<fvMatrix<Type> >
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
    )().fvmDdt(rho, vf);
}


template<class Type>
tmp<fvMatrix<Type> >
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
    )().fvmDdt(rho, vf);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvm

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
