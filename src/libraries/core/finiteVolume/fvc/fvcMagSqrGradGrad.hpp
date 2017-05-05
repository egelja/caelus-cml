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
    Calculate the magnitiude of the square of the gradient of the gradient of
    the given volField.


\*---------------------------------------------------------------------------*/


#ifndef fvcMagSqrGradGrad_H
#define fvcMagSqrGradGrad_H

#include "volFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Namespace fvc functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvc
{
    template<class Type>
    tmp<volScalarField> magSqrGradGrad
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<volScalarField> magSqrGradGrad
    (
        const tmp<GeometricField<Type, fvPatchField, volMesh> >&
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvcGrad.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<volScalarField> magSqrGradGrad
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<volScalarField> tMagSqrGradGrad
    (
        magSqr(fvc::grad(fvc::grad(vf.component(0))))
    );

    // Loop over other vector field components
    for (direction cmpt = 1; cmpt < pTraits<Type>::nComponents; cmpt++)
    {
        tMagSqrGradGrad() += magSqr(fvc::grad(fvc::grad(vf.component(cmpt))))();
    }

    return tMagSqrGradGrad;
}


template<class Type>
tmp<volScalarField>
magSqrGradGrad
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<volScalarField> tMagSqrGradGrad(fvc::magSqrGradGrad(tvf()));
    tvf.clear();
    return tMagSqrGradGrad;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
