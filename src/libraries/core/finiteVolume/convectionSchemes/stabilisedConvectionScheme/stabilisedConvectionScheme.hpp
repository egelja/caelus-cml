/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2012 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute itand/or modify it
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
    CML::fv::stabilisedConvectionScheme

Description
    Stabilised form of the selected convection scheme.

    Stabilisation is achieved by subtracting div(phi)*vf or Sp(div(phi), vf)
    which is non-conservative if div(phi) != 0 but conservative otherwise.

    Can be used for convection of stabilised scalar properties in steady-state
    solvers to improve stability if insufficient convergence of the pressure
    equation causes temporary divergence of the flux field.

SourceFiles
    stabilisedConvectionScheme.C

\*---------------------------------------------------------------------------*/

#ifndef stabilisedConvectionScheme_HPP
#define stabilisedConvectionScheme_HPP

#include "convectionScheme.hpp"

namespace CML
{

namespace fv
{

template<class Type>
class stabilisedConvectionScheme
:
    public fv::convectionScheme<Type>
{
    tmp<fv::convectionScheme<Type> > scheme_;

    //- Disallow default bitwise copy construct
    stabilisedConvectionScheme(const stabilisedConvectionScheme&);

    //- Disallow default bitwise assignment
    void operator=(const stabilisedConvectionScheme&);


public:

    //- Runtime type information
    TypeName("stabilised");


    //- Construct from flux and Istream
    stabilisedConvectionScheme
    (
        const fvMesh& mesh,
        const surfaceScalarField& faceFlux,
        Istream& is
    ):
    convectionScheme<Type>(mesh, faceFlux),
    scheme_
    (
        fv::convectionScheme<Type>::New(mesh, faceFlux, is)
    )
    {}

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


} // End namespace fv

} // End namespace CML

#include "fvcSurfaceIntegrate.hpp"
#include "fvMatrices.hpp"
#include "fvmSup.hpp"

namespace CML
{

namespace fv
{

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
stabilisedConvectionScheme<Type>::interpolate
(
    const surfaceScalarField& phi,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return scheme_().interpolate(phi, vf);
}


template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
stabilisedConvectionScheme<Type>::flux
(
    const surfaceScalarField& faceFlux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return scheme_().flux(faceFlux, vf);
}


template<class Type>
tmp<fvMatrix<Type> >
stabilisedConvectionScheme<Type>::fvmDiv
(
    const surfaceScalarField& faceFlux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return
        scheme_().fvmDiv(faceFlux, vf)
      - fvm::Sp(fvc::surfaceIntegrate(faceFlux), vf);
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
stabilisedConvectionScheme<Type>::fvcDiv
(
    const surfaceScalarField& faceFlux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return
        scheme_().fvcDiv(faceFlux, vf)
      - fvc::surfaceIntegrate(faceFlux)*vf;
}


} // End namespace fv

} // End namespace CML


#endif


