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
    Calculate the matrix for the second-order temporal derivative.


\*---------------------------------------------------------------------------*/

#ifndef fvmD2dt2_H
#define fvmD2dt2_H

#include "volFieldsFwd.hpp"
#include "fvMatrix.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "d2dt2Scheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Namespace fvm functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvm
{
    template<class Type>
    tmp<fvMatrix<Type> > d2dt2
    (
        const dimensionedScalar&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<fvMatrix<Type> > d2dt2
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<fvMatrix<Type> > d2dt2
    (
        const volScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<fvMatrix<Type> >
d2dt2
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fv::d2dt2Scheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().d2dt2Scheme("d2dt2(" + vf.name() + ')')
    )().fvmD2dt2(vf);
}


template<class Type>
tmp<fvMatrix<Type> >
d2dt2
(
    const dimensionedScalar& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fv::d2dt2Scheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().d2dt2Scheme("d2dt2(" + rho.name() + ',' + vf.name() + ')')
    )().fvmD2dt2(rho, vf);
}


template<class Type>
tmp<fvMatrix<Type> >
d2dt2
(
    const volScalarField& rho,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fv::d2dt2Scheme<Type>::New
    (
        vf.mesh(),
        vf.mesh().d2dt2Scheme("d2dt2(" + rho.name() + ',' + vf.name() + ')')
    )().fvmD2dt2(rho, vf);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvm

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
