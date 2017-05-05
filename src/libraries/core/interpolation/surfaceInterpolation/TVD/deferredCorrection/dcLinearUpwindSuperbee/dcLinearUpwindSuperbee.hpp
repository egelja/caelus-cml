/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::dcLinearUpwindSuperbee

Description
    A TVD property staisfying dcLinear upwind scheme based on the Superbee 
    limiter to prevent unwanted oscillations. Implementation suitable for
    scalar fields.

SourceFiles
    dcLinearUpwindSuperbee.cpp

References

    [1] "Cahracteristic-based schemes for the Euler Equations", P.L., Roe,
        Ann. Rev. Fluid Mech. 18: pp 337-365, 1986

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef dcLinearUpwindSuperbee_H
#define dcLinearUpwindSuperbee_H

#include "upwind.hpp"
#include "gaussGrad.hpp"

namespace CML
{

template<class Type>
class dcLinearUpwindSuperbee : public upwind<Type>
{
    word gradSchemeName_;
    tmp<fv::gradScheme<Type> > gradScheme_;

    //- Disallow default bitwise copy construct
    dcLinearUpwindSuperbee(dcLinearUpwindSuperbee const&);

    //- Disallow default bitwise assignment
    void operator=(dcLinearUpwindSuperbee const&);

public:

    //- Runtime type information
    TypeName("dcLinearUpwindSuperbee");

    //- Construct from faceFlux
    dcLinearUpwindSuperbee
    (
        fvMesh const& mesh,
        surfaceScalarField const& faceFlux
    )   :
        upwind<Type>(mesh, faceFlux),
        gradSchemeName_("grad"),
        gradScheme_(new fv::gaussGrad<Type>(mesh))
    {}

    //- Construct from Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    dcLinearUpwindSuperbee
    (
        fvMesh const& mesh,
        Istream& schemeData
    )   :
        upwind<Type>(mesh, schemeData),
        gradSchemeName_(schemeData),
        gradScheme_
        (
            fv::gradScheme<Type>::New
            (
                mesh,
                mesh.gradScheme(gradSchemeName_)
            )
        )
    {}

    //- Construct from faceFlux and Istream
    dcLinearUpwindSuperbee
    (
        fvMesh const& mesh,
        surfaceScalarField const& faceFlux,
        Istream& schemeData
    )   :
        upwind<Type>(mesh, faceFlux, schemeData),
        gradSchemeName_(schemeData),
        gradScheme_
        (
            fv::gradScheme<Type>::New
            (
                mesh,
                mesh.gradScheme(gradSchemeName_)
            )
        )
    {}

    // Member Functions

    //- Return false for deferred correction
    virtual bool corrected() const
    {
        return false;
    }

    //- Return the explicit correction to the face-interpolate
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction
    (
        GeometricField<Type, fvPatchField, volMesh> const&
    ) const;

    scalar slopeLimiter(scalar const) const;

};

}

#endif

