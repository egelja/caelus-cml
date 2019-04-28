/*---------------------------------------------------------------------------*\
Copyright (C) 2015-2018 Applied CCM
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
    CML::blendedMinmod

Description
    blendedMinmod scheme class.

    The scheme inherits from upwind class but it does not use 
    upwind inteprolation since that function is virtual it is 
    overwritten here.

SourceFiles
    blendedMinmod.cpp

References

    [1] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef blendedMinmod_HPP
#define blendedMinmod_HPP

#include "upwind.hpp"
#include "gaussGrad.hpp"

namespace CML
{

template<class Type>
class blendedMinmod : public upwind<Type>
{

    // Private Data

    word gradSchemeName_;
    tmp<fv::gradScheme<Type> > gradScheme_;
    scalar beta_;

    // Private Member Functions

    //- Disallow default bitwise copy construct
    blendedMinmod(blendedMinmod const&);

    //- Disallow default bitwise assignment
    void operator=(blendedMinmod const&);

public:

    //- Runtime type information
    TypeName("blendedMinmod");

    // Constructors

    //- Construct from faceFlux
    blendedMinmod
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
    blendedMinmod
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
                FatalIOErrorInFunction(schemeData)
                    << "coefficient = " << beta_
                    << " should be >= 0 and <= 1"
                    << exit(FatalIOError);
            }
	}
    }

    //- Construct from faceFlux and Istream
    blendedMinmod
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
	        FatalIOErrorInFunction(schemeData)
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

