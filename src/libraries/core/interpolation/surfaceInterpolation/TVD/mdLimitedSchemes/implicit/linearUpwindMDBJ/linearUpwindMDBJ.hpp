/*---------------------------------------------------------------------------*\
Copyright (C) 2015 - 2018 Applied CCM
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
    CML::linearUpwindMDBJ

Description

    A TVD property staisfying limited central scheme based on the 
    Barth-Jespersen limiter to prevent unwanted oscillations. 

SourceFiles

    linearUpwindMDBJ.cpp

References

    [1] “The Design and Application of Upwind Schemes on Unstructured Meshes,” 
         Barth, T. J. and Jespersen, D. C., AIAA paper 89-0366, Jan. 1989.

Author

    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef linearUpwindMDBJ_HPP
#define linearUpwindMDBJ_HPP

#include "upwind.hpp"
#include "gaussGrad.hpp"

namespace CML
{

template<class Type>
class linearUpwindMDBJ : public upwind<Type>
{

    word gradSchemeName_;
    tmp<fv::gradScheme<Type> > gradScheme_;
    scalar order_;

    linearUpwindMDBJ(const linearUpwindMDBJ&);

    void operator=(const linearUpwindMDBJ&);

public:

    TypeName("linearUpwindMDBJ");

    linearUpwindMDBJ
    (
        const fvMesh& mesh,
        const surfaceScalarField& faceFlux
    ) :
        upwind<Type>(mesh, faceFlux),
        gradSchemeName_("grad"),
        gradScheme_(new fv::gaussGrad<Type>(mesh)),
        order_(1)
    {}

    linearUpwindMDBJ
    (
        const fvMesh& mesh,
        Istream& schemeData
    ) :
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
    {
        if (schemeData.eof())
        {
            order_ = scalar(1);
        }
        else
        {
            order_ = readScalar(schemeData);
            if (order_ < 0 || order_ > 1)
            {
                FatalIOErrorInFunction(schemeData)
                        << "coefficient = " << order_
                        << " should be >= 0 and <= 1"
                        << exit(FatalIOError);
            }
        }

    }

    linearUpwindMDBJ
    (
        const fvMesh& mesh,
        const surfaceScalarField& faceFlux,
        Istream& schemeData
    ) :
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
    {

        if (schemeData.eof())
        {
            order_ = scalar(1);
        }
        else
        {
            order_ = readScalar(schemeData);
            if (order_ < 0 || order_ > 1)
            {
                FatalIOErrorInFunction(schemeData)
                        << "coefficient = " << order_
                        << " should be >= 0 and <= 1"
                        << exit(FatalIOError);
            }
        }
    }

    //- Return true if this scheme uses an explicit correction
    virtual bool corrected() const
    {
        return true;
    }

    //- Return the explicit correction to the face-interpolate
    virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
    correction
    (
        const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const;
};

}

#endif

