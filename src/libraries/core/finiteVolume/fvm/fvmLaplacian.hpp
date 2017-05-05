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
    Calculate the matrix for the laplacian of the field.


\*---------------------------------------------------------------------------*/

#ifndef fvmLaplacian_H
#define fvmLaplacian_H

#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "fvMatrix.hpp"
#include "zero.hpp"
#include "one.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "fvMatrix.hpp"
#include "laplacianScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Namespace fvm functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvm
{
    template<class Type>
    tmp<fvMatrix<Type> > laplacian
    (
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<fvMatrix<Type> > laplacian
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    );


    template<class Type>
    tmp<fvMatrix<Type> > laplacian
    (
        const zero&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<fvMatrix<Type> > laplacian
    (
        const zero&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );


    template<class Type>
    tmp<fvMatrix<Type> > laplacian
    (
        const one&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type>
    tmp<fvMatrix<Type> > laplacian
    (
        const one&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );


    template<class Type, class GType>
    tmp<fvMatrix<Type> > laplacian
    (
        const dimensioned<GType>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type, class GType>
    tmp<fvMatrix<Type> > laplacian
    (
        const dimensioned<GType>&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );


    template<class Type, class GType>
    tmp<fvMatrix<Type> > laplacian
    (
        const GeometricField<GType, fvPatchField, volMesh>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type, class GType>
    tmp<fvMatrix<Type> > laplacian
    (
        const GeometricField<GType, fvPatchField, volMesh>&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );


    template<class Type, class GType>
    tmp<fvMatrix<Type> > laplacian
    (
        const tmp<GeometricField<GType, fvPatchField, volMesh> >&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type, class GType>
    tmp<fvMatrix<Type> > laplacian
    (
        const tmp<GeometricField<GType, fvPatchField, volMesh> >&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );


    template<class Type, class GType>
    tmp<fvMatrix<Type> > laplacian
    (
        const GeometricField<GType, fvsPatchField, surfaceMesh>&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type, class GType>
    tmp<fvMatrix<Type> > laplacian
    (
        const tmp<GeometricField<GType, fvsPatchField, surfaceMesh> >&,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const word&
    );

    template<class Type, class GType>
    tmp<fvMatrix<Type> > laplacian
    (
        const GeometricField<GType, fvsPatchField, surfaceMesh>&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type, class GType>
    tmp<fvMatrix<Type> > laplacian
    (
        const tmp<GeometricField<GType, fvsPatchField, surfaceMesh> >&,
        const GeometricField<Type, fvPatchField, volMesh>&
    );


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


template<class Type>
tmp<fvMatrix<Type> >
laplacian
(
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    surfaceScalarField Gamma
    (
        IOobject
        (
            "1",
            vf.time().constant(),
            vf.mesh(),
            IOobject::NO_READ
        ),
        vf.mesh(),
        dimensionedScalar("1", dimless, 1.0)
    );

    return fvm::laplacian(Gamma, vf, name);
}


template<class Type>
tmp<fvMatrix<Type> >
laplacian
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    surfaceScalarField Gamma
    (
        IOobject
        (
            "1",
            vf.time().constant(),
            vf.mesh(),
            IOobject::NO_READ
        ),
        vf.mesh(),
        dimensionedScalar("1", dimless, 1.0)
    );

    return fvm::laplacian
    (
        Gamma,
        vf,
        "laplacian(" + vf.name() + ')'
    );
}


template<class Type>
tmp<fvMatrix<Type> >
laplacian
(
    const zero&,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    return tmp<fvMatrix<Type> >
    (
        new fvMatrix<Type>(vf, dimensionSet(0, 0, -2, 0, 0))
    );
}


template<class Type>
tmp<fvMatrix<Type> >
laplacian
(
    const zero&,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return tmp<fvMatrix<Type> >
    (
        new fvMatrix<Type>(vf, dimensionSet(0, 0, -2, 0, 0))
    );
}


template<class Type>
tmp<fvMatrix<Type> >
laplacian
(
    const one&,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    return fvm::laplacian(vf, name);
}


template<class Type>
tmp<fvMatrix<Type> >
laplacian
(
    const one&,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvm::laplacian(vf);
}


template<class Type, class GType>
tmp<fvMatrix<Type> >
laplacian
(
    const dimensioned<GType>& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    const GeometricField<GType, fvsPatchField, surfaceMesh> Gamma
    (
        IOobject
        (
            gamma.name(),
            vf.instance(),
            vf.mesh(),
            IOobject::NO_READ
        ),
        vf.mesh(),
        gamma
    );

    return fvm::laplacian(Gamma, vf, name);
}


template<class Type, class GType>
tmp<fvMatrix<Type> >
laplacian
(
    const dimensioned<GType>& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const GeometricField<GType, fvsPatchField, surfaceMesh> Gamma
    (
        IOobject
        (
            gamma.name(),
            vf.instance(),
            vf.mesh(),
            IOobject::NO_READ
        ),
        vf.mesh(),
        gamma
    );

    return fvm::laplacian(Gamma, vf);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, class GType>
tmp<fvMatrix<Type> >
laplacian
(
    const GeometricField<GType, fvPatchField, volMesh>& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    return fv::laplacianScheme<Type, GType>::New
    (
        vf.mesh(),
        vf.mesh().laplacianScheme(name)
    )().fvmLaplacian(gamma, vf);
}


template<class Type, class GType>
tmp<fvMatrix<Type> >
laplacian
(
    const tmp<GeometricField<GType, fvPatchField, volMesh> >& tgamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    tmp<fvMatrix<Type> > Laplacian(fvm::laplacian(tgamma(), vf, name));
    tgamma.clear();
    return Laplacian;
}


template<class Type, class GType>
tmp<fvMatrix<Type> >
laplacian
(
    const GeometricField<GType, fvPatchField, volMesh>& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvm::laplacian
    (
        gamma,
        vf,
        "laplacian(" + gamma.name() + ',' + vf.name() + ')'
    );
}


template<class Type, class GType>
tmp<fvMatrix<Type> >
laplacian
(
    const tmp<GeometricField<GType, fvPatchField, volMesh> >& tgamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > Laplacian(fvm::laplacian(tgamma(), vf));
    tgamma.clear();
    return Laplacian;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, class GType>
tmp<fvMatrix<Type> >
laplacian
(
    const GeometricField<GType, fvsPatchField, surfaceMesh>& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    return fv::laplacianScheme<Type, GType>::New
    (
        vf.mesh(),
        vf.mesh().laplacianScheme(name)
    )().fvmLaplacian(gamma, vf);
}


template<class Type, class GType>
tmp<fvMatrix<Type> >
laplacian
(
    const tmp<GeometricField<GType, fvsPatchField, surfaceMesh> >& tgamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf,
    const word& name
)
{
    tmp<fvMatrix<Type> > tLaplacian = fvm::laplacian(tgamma(), vf, name);
    tgamma.clear();
    return tLaplacian;
}


template<class Type, class GType>
tmp<fvMatrix<Type> >
laplacian
(
    const GeometricField<GType, fvsPatchField, surfaceMesh>& gamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return fvm::laplacian
    (
        gamma,
        vf,
        "laplacian(" + gamma.name() + ',' + vf.name() + ')'
    );
}


template<class Type, class GType>
tmp<fvMatrix<Type> >
laplacian
(
    const tmp<GeometricField<GType, fvsPatchField, surfaceMesh> >& tGamma,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm(fvm::laplacian(tGamma(), vf));
    tGamma.clear();
    return tfvm;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvm

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
