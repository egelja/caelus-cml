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
    CML::dcLinearUpwindBJ

Description
    A TVD property staisfying dcLinear upwind scheme based on the 
    Barth-Jespersen limiter to prevent unwanted oscillations. 
    Implementation suitable for scalar fields.

SourceFiles
    dcLinearUpwindBJ.cpp

References

    [1] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef dcLinearUpwindBJ_H
#define dcLinearUpwindBJ_H

#include "upwind.hpp"
#include "gaussGrad.hpp"

namespace CML
{

template<class Type>
class dcLinearUpwindBJ : public upwind<Type>
{
    word gradSchemeName_;
    
    tmp<fv::gradScheme<Type> > gradScheme_;

    dcLinearUpwindBJ(const dcLinearUpwindBJ&);

    void operator=(const dcLinearUpwindBJ&);

public:

    TypeName("dcLinearUpwindBJ");

    dcLinearUpwindBJ
    (
        const fvMesh& mesh,
        const surfaceScalarField& faceFlux
    ) : upwind<Type>(mesh, faceFlux),
        gradSchemeName_("grad"),
        gradScheme_
        (
            new fv::gaussGrad<Type>(mesh)
        )
    {}

    dcLinearUpwindBJ
    (
        const fvMesh& mesh,
        Istream& schemeData
    ) : upwind<Type>(mesh, schemeData),
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

    dcLinearUpwindBJ
    (
        const fvMesh& mesh,
        const surfaceScalarField& faceFlux,
        Istream& schemeData
    ) : upwind<Type>(mesh, faceFlux, schemeData),
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

    // Return false for deferred correction
    virtual bool corrected() const
    {
        return false;
    }

    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};

}

#endif

