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
    Calculate the substantive (total) derivative.


\*---------------------------------------------------------------------------*/


#ifndef fvcDDt_subst_H
#define fvcDDt_subst_H

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
    tmp<GeometricField<Type, fvPatchField, volMesh> > DDt
    (
        const surfaceScalarField&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > DDt
    (
        const tmp<surfaceScalarField>&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvcDiv.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
DDt
(
    const surfaceScalarField& phi,
    const GeometricField<Type, fvPatchField, volMesh>& psi
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > ddtDivPhiPsi
        = fvc::ddt(psi) + fvc::div(phi, psi);

    if (phi.mesh().moving())
    {
        return ddtDivPhiPsi - fvc::div(phi + phi.mesh().phi())*psi;
    }
    else
    {
        return ddtDivPhiPsi - fvc::div(phi)*psi;
    }
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
DDt
(
    const tmp<surfaceScalarField>& tphi,
    const GeometricField<Type, fvPatchField, volMesh>& psi
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > DDtPsi
    (
        fvc::DDt(tphi(), psi)
    );
    tphi.clear();
    return DDtPsi;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
