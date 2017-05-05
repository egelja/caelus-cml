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
    Calculate the curl of the given volField by constructing the Hodge-dual of
    the symmetric part of the gradient.


\*---------------------------------------------------------------------------*/


#ifndef fvcCurl_H
#define fvcCurl_H

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
    tmp<GeometricField<Type, fvPatchField, volMesh> > curl
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    );

    template<class Type>
    tmp<GeometricField<Type, fvPatchField, volMesh> > curl
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
tmp<GeometricField<Type, fvPatchField, volMesh> >
curl
(
    const GeometricField<Type, fvPatchField, volMesh>& vf
)
{
    word nameCurlVf = "curl(" + vf.name() + ')';

    // Gausses theorem curl
    // tmp<GeometricField<Type, fvPatchField, volMesh> > tcurlVf =
    //     fvc::surfaceIntegrate(vf.mesh().Sf() ^ fvc::interpolate(vf));

    // Calculate curl as the Hodge dual of the skew-symmetric part of grad
    tmp<GeometricField<Type, fvPatchField, volMesh> > tcurlVf =
        2.0*(*skew(fvc::grad(vf, nameCurlVf)));

    tcurlVf().rename(nameCurlVf);

    return tcurlVf;
}


template<class Type>
tmp<GeometricField<Type, fvPatchField, volMesh> >
curl
(
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvf
)
{
    tmp<GeometricField<Type, fvPatchField, volMesh> > Curl(fvc::curl(tvf()));
    tvf.clear();
    return Curl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fvc

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
