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
    Calculate the field for explicit evaluation of implicit and explicit
    sources.


\*---------------------------------------------------------------------------*/

#ifndef fvcSup_H
#define fvcSup_H

#include "volFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Namespace fvc functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvc
{
   // Explicit source

        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> > Su
        (
            const GeometricField<Type, fvPatchField, volMesh>&,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> > Su
        (
            const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
            GeometricField<Type, fvPatchField, volMesh>&
        );


    // Implicit source

        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> > Sp
        (
            const volScalarField&,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> > Sp
        (
            const tmp<volScalarField>&,
            GeometricField<Type, fvPatchField, volMesh>&
        );


        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> > Sp
        (
            const dimensionedScalar&,
            GeometricField<Type, fvPatchField, volMesh>&
        );


    // Implicit/Explicit source depending on sign of coefficient

        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> > SuSp
        (
            const volScalarField&,
            GeometricField<Type, fvPatchField, volMesh>&
        );

        template<class Type>
        tmp<GeometricField<Type, fvPatchField, volMesh> > SuSp
        (
            const tmp<volScalarField>&,
            GeometricField<Type, fvPatchField, volMesh>&
        );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fvc
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
Su
(
    const GeometricField<Type, fvPatchField, volMesh>& su,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return su;
}

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
Su
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return tsu;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
Sp
(
    const volScalarField& sp,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return sp*vf;
}

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
Sp
(
    const tmp<volScalarField>& tsp,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return tsp*vf;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
Sp
(
    const dimensionedScalar& sp,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return sp*vf;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
SuSp
(
    const volScalarField& sp,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return sp*vf;
}

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
SuSp
(
    const tmp<volScalarField>& tsp,
    GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return tsp*vf;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
