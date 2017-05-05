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

Class
    CML::fv::multivariateGaussConvectionScheme

Description
    Basic second-order convection using face-gradients and Gauss' theorem.

SourceFiles
    multivariateGaussConvectionScheme.cpp

\*---------------------------------------------------------------------------*/

#ifndef multivariateGaussConvectionScheme_H
#define multivariateGaussConvectionScheme_H

#include "convectionScheme.hpp"
#include "gaussConvectionScheme.hpp"
#include "fvMatrices.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                       Class multivariateGaussConvectionScheme Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class multivariateGaussConvectionScheme
:
    public fv::convectionScheme<Type>
{
    // Private data

        tmp<multivariateSurfaceInterpolationScheme<Type> > tinterpScheme_;


public:

    //- Runtime type information
    TypeName("Gauss");


    // Constructors

        //- Construct from flux and Istream
        multivariateGaussConvectionScheme
        (
            const fvMesh& mesh,
            const typename multivariateSurfaceInterpolationScheme<Type>::
                fieldTable& fields,
            const surfaceScalarField& faceFlux,
            Istream& is
        )
        :
            convectionScheme<Type>(mesh, faceFlux),
            tinterpScheme_
            (
                multivariateSurfaceInterpolationScheme<Type>::New
                (
                    mesh, fields, faceFlux, is
                )
            )
        {}


    // Member Functions

        tmp<multivariateSurfaceInterpolationScheme<Type> >
        interpolationScheme() const
        {
            return tinterpScheme_;
        }

        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > interpolate
        (
            const surfaceScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;

        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > flux
        (
            const surfaceScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;

        tmp<fvMatrix<Type> > fvmDiv
        (
            const surfaceScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;

        tmp<GeometricField<Type, fvPatchField, volMesh> > fvcDiv
        (
            const surfaceScalarField&,
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;
};



template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
multivariateGaussConvectionScheme<Type>::interpolate
(
    const surfaceScalarField& faceFlux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return gaussConvectionScheme<Type>
    (
        this->mesh(),
        faceFlux,
        tinterpScheme_()(vf)
    ).interpolate(faceFlux, vf);
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
multivariateGaussConvectionScheme<Type>::flux
(
    const surfaceScalarField& faceFlux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return gaussConvectionScheme<Type>
    (
        this->mesh(),
        faceFlux,
        tinterpScheme_()(vf)
    ).flux(faceFlux, vf);
}


template<class Type>
tmp<fvMatrix<Type> >
multivariateGaussConvectionScheme<Type>::fvmDiv
(
    const surfaceScalarField& faceFlux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return gaussConvectionScheme<Type>
    (
        this->mesh(),
        faceFlux,
        tinterpScheme_()(vf)
    ).fvmDiv(faceFlux, vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
multivariateGaussConvectionScheme<Type>::fvcDiv
(
    const surfaceScalarField& faceFlux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return gaussConvectionScheme<Type>
    (
        this->mesh(),
        faceFlux,
        tinterpScheme_()(vf)
    ).fvcDiv(faceFlux, vf);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
