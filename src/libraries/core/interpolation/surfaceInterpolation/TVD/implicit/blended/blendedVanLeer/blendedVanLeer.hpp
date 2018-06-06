/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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
    CML::blendedVanLeer

Description
    blendedVanLeer scheme class.

SourceFiles
    blendedVanLeer.cpp

References

    [1] "Towards the ultimate conservative difference scheme II. Monotonicity 
         and Conservation Combined in Second Order Scheme", B. Van Leer,
         J. Comp. Phys. 14 (4), pp 361-370, 1974

    [2] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef blendedVanLeer_HPP
#define blendedVanLeer_HPP

#include "upwind.hpp"
#include "gaussGrad.hpp"

namespace CML
{

template<class Type>
class blendedVanLeer : public upwind<Type>
{

    // Private Data

    word gradSchemeName_;
    tmp<fv::gradScheme<Type> > gradScheme_;
    scalar beta_;

    // Private Member Functions

    //- Disallow default bitwise copy construct
    blendedVanLeer(blendedVanLeer const&);

    //- Disallow default bitwise assignment
    void operator=(blendedVanLeer const&);

public:

    //- Runtime type information
    TypeName("blendedVanLeer");

    // Constructors

    //- Construct from faceFlux
    blendedVanLeer
    (
        fvMesh const& mesh,
        surfaceScalarField const& faceFlux
    ) :
        upwind<Type>(mesh, faceFlux),
        gradSchemeName_("grad"),
        gradScheme_(new fv::gaussGrad<Type>(mesh)),
        beta_(1.0)
    {}

    //- Construct from Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    blendedVanLeer
    (
        fvMesh const& mesh,
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
	  beta_ = scalar(0.75);
        }
        else
        {
            beta_ = readScalar(schemeData);
            if (beta_ < 0 || beta_ > 1)
            {
                FatalIOErrorIn("blendedVanLeer(fvMesh const&, Istream& is)", schemeData)
                    << "coefficient = " << beta_
                    << " should be >= 0 and <= 1"
                    << exit(FatalIOError);
            }
	}
    }

    //- Construct from faceFlux and Istream
    blendedVanLeer
    (
        fvMesh const& mesh,
        surfaceScalarField const& faceFlux,
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
            beta_ = scalar(0.75);
        }
        else
        {
            beta_ = readScalar(schemeData);
            if (beta_ < 0 || beta_ > 1)
            {
	        FatalIOErrorIn("blendedVanLeer(fvMesh&, surfaceScalarField const&, Istream& is)", schemeData)
                    << "coefficient = " << beta_
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
        GeometricField<Type, fvPatchField, volMesh> const& vf
    ) const;

    scalar slopeLimiter(scalar const) const;      
  
};

}

#endif
