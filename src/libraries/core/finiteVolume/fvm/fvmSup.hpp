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
    Calculate the matrix for implicit and explicit sources.

SourceFiles
    fvmSup.cpp

\*---------------------------------------------------------------------------*/

#ifndef fvmSup_H
#define fvmSup_H

#include "volFieldsFwd.hpp"
#include "fvMatrix.hpp"
#include "zeroField.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "fvMatrix.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Namespace fvm functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvm
{
   // Explicit source

        template<class Type>
        tmp<fvMatrix<Type> > Su
        (
            const DimensionedField<Type, volMesh>&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        template<class Type>
        tmp<fvMatrix<Type> > Su
        (
            const tmp<DimensionedField<Type, volMesh> >&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        template<class Type>
        tmp<fvMatrix<Type> > Su
        (
            const tmp<GeometricField<Type, fvPatchField, volMesh> >&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        template<class Type>
        zeroField Su
        (
            const zero&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );


    // Implicit source

        template<class Type>
        tmp<fvMatrix<Type> > Sp
        (
            const DimensionedField<scalar, volMesh>&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        template<class Type>
        tmp<fvMatrix<Type> > Sp
        (
            const tmp<DimensionedField<scalar, volMesh> >&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        template<class Type>
        tmp<fvMatrix<Type> > Sp
        (
            const tmp<volScalarField>&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );


        template<class Type>
        tmp<fvMatrix<Type> > Sp
        (
            const dimensionedScalar&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );


        template<class Type>
        zeroField Sp
        (
            const zero&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );


    // Implicit/Explicit source depending on sign of coefficient

        template<class Type>
        tmp<fvMatrix<Type> > SuSp
        (
            const DimensionedField<scalar, volMesh>&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        template<class Type>
        tmp<fvMatrix<Type> > SuSp
        (
            const tmp<DimensionedField<scalar, volMesh> >&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        template<class Type>
        tmp<fvMatrix<Type> > SuSp
        (
            const tmp<volScalarField>&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );

        template<class Type>
        zeroField SuSp
        (
            const zero&,
            const GeometricField<Type, fvPatchField, volMesh>&
        );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::fvm::Su
(
    const DimensionedField<Type, volMesh>& su,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = vf.mesh();

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            dimVol*su.dimensions()
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    fvm.source() -= mesh.V()*su.field();

    return tfvm;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::fvm::Su
(
    const tmp<DimensionedField<Type, volMesh> >& tsu,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm = fvm::Su(tsu(), vf);
    tsu.clear();
    return tfvm;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::fvm::Su
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tsu,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm = fvm::Su(tsu(), vf);
    tsu.clear();
    return tfvm;
}


template<class Type>
CML::zeroField
CML::fvm::Su
(
    const zero&,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return zeroField();
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::fvm::Sp
(
    const DimensionedField<scalar, volMesh>& sp,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = vf.mesh();

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            dimVol*sp.dimensions()*vf.dimensions()
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    fvm.diag() += mesh.V()*sp.field();

    return tfvm;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::fvm::Sp
(
    const tmp<DimensionedField<scalar, volMesh> >& tsp,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm = fvm::Sp(tsp(), vf);
    tsp.clear();
    return tfvm;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::fvm::Sp
(
    const tmp<volScalarField>& tsp,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm = fvm::Sp(tsp(), vf);
    tsp.clear();
    return tfvm;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::fvm::Sp
(
    const dimensionedScalar& sp,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = vf.mesh();

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            dimVol*sp.dimensions()*vf.dimensions()
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    fvm.diag() += mesh.V()*sp.value();

    return tfvm;
}


template<class Type>
CML::zeroField
CML::fvm::Sp
(
    const zero&,
    const GeometricField<Type, fvPatchField, volMesh>&
)
{
    return zeroField();
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::fvm::SuSp
(
    const DimensionedField<scalar, volMesh>& susp,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    const fvMesh& mesh = vf.mesh();

    tmp<fvMatrix<Type> > tfvm
    (
        new fvMatrix<Type>
        (
            vf,
            dimVol*susp.dimensions()*vf.dimensions()
        )
    );
    fvMatrix<Type>& fvm = tfvm();

    fvm.diag() += mesh.V()*max(susp.field(), scalar(0));

    fvm.source() -= mesh.V()*min(susp.field(), scalar(0))
        *vf.internalField();

    return tfvm;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::fvm::SuSp
(
    const tmp<DimensionedField<scalar, volMesh> >& tsusp,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm = fvm::SuSp(tsusp(), vf);
    tsusp.clear();
    return tfvm;
}


template<class Type>
CML::tmp<CML::fvMatrix<Type> >
CML::fvm::SuSp
(
    const tmp<volScalarField>& tsusp,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    tmp<fvMatrix<Type> > tfvm = fvm::SuSp(tsusp(), vf);
    tsusp.clear();
    return tfvm;
}


template<class Type>
CML::zeroField
CML::fvm::SuSp
(
    const zero&,
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    return zeroField();
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
