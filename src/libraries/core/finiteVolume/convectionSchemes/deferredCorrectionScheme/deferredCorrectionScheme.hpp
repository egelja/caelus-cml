/*---------------------------------------------------------------------------*\
Copyright (C) 2014-2018 Applied CCM
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
    CML::fv::deferredCorrectionScheme

Description
    Deferred correction TVD scheme 

SourceFiles
    deferredCorrectionScheme.C

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef deferredCorrectionScheme_HPP
#define deferredCorrectionScheme_HPP

#include "convectionScheme.hpp"

namespace CML
{

namespace fv
{

template<class Type>
class deferredCorrectionScheme
:
    public fv::convectionScheme<Type>
{

    tmp<fv::convectionScheme<Type> > scheme_;
    scalar r_;

    //- Disallow default bitwise copy construct
    deferredCorrectionScheme(const deferredCorrectionScheme&);

    //- Disallow default bitwise assignment
    void operator=(const deferredCorrectionScheme&);

public:

    //- Runtime type information
    TypeName("deferredCorrection");

    //- Construct from flux and Istream
    deferredCorrectionScheme
    (
        const fvMesh& mesh,
        const surfaceScalarField& faceFlux,
        Istream& is
    ) :
        convectionScheme<Type>(mesh, faceFlux),
        scheme_
        (
            fv::convectionScheme<Type>::New(mesh, faceFlux, is)
        )
    {
        if(is.eof())
        {
            r_ = scalar(1.0);
        }
        else
        {
            r_ = readScalar(is);
            if (r_ < 0 || r_ > 1)
            {
                FatalIOErrorInFunction(is)
                    << "coefficient = " << r_
                    << " should be >= 0 and <= 1"
                    << exit(FatalIOError);
            }
        }
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
deferredCorrectionScheme<Type>::interpolate
(
    const surfaceScalarField& phi,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return scheme_().interpolate(phi, vf);
}

template<class Type>
tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
deferredCorrectionScheme<Type>::flux
(
    const surfaceScalarField& faceFlux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    return scheme_().flux(faceFlux, vf);
}

template<class Type>
tmp<fvMatrix<Type> >
deferredCorrectionScheme<Type>::fvmDiv
(
    const surfaceScalarField& faceFlux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > sd = 
        this->scheme_().interpScheme()().correction(vf);

    return
        scheme_().fvmDiv(faceFlux, vf)
      + this->r_*fvc::surfaceIntegrate(faceFlux*sd());
}

template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
deferredCorrectionScheme<Type>::fvcDiv
(
    const surfaceScalarField& faceFlux,
    const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > sd = 
        this->scheme_().interpScheme()().correction(vf);

    return 
        scheme_().fvcDiv(faceFlux, vf)
      + this->r_*fvc::surfaceIntegrate(faceFlux*sd());
}

} // End namespace fv

} // End namespace CML


#endif


