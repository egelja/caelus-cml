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
    CML::limitedCentralUMIST

Description
    LimitedCentralUMIST scheme class.

SourceFiles
    limitedCentralUMIST.cpp

References

    [1] "A Pressure-Velocity Solution Strategy for Compressible Flow and its 
         Application to Shock-Boundary Layer interaction using 2nd-Moment 
         Turbulence Closure", F.S, Lien, M.A. Leshziner, J. of Fluids 
         Eng.-Tans. of ASME, 115, pp. 717-725, 1993

    [2] "Analysis of Slope Limiter on Irregular Grids", M. Berger, 
         M.J. Aftosmis and S.M. Murman, 43rd AIAA Aerospace Sciences Meeting, 
         Jan. 10-13, 2005, Reno, NV

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef limitedCentralUMIST_HPP
#define limitedCentralUMIST_HPP

#include "upwind.hpp"
#include "gaussGrad.hpp"

namespace CML
{

template<class Type>
class limitedCentralUMIST : public upwind<Type>
{

    // Private Data

    word gradSchemeName_;
    tmp<fv::gradScheme<Type> > gradScheme_;

    // Private Member Functions

    //- Disallow default bitwise copy construct
    limitedCentralUMIST(limitedCentralUMIST const&);

    //- Disallow default bitwise assignment
    void operator=(limitedCentralUMIST const&);

public:

    //- Runtime type information
    TypeName("limitedCentralUMIST");

    // Constructors

    //- Construct from faceFlux
    limitedCentralUMIST
    (
        fvMesh const& mesh,
        surfaceScalarField const& faceFlux
    ) :
        upwind<Type>(mesh, faceFlux),
        gradSchemeName_("grad"),
        gradScheme_(new fv::gaussGrad<Type>(mesh))
    {}

    //- Construct from Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    limitedCentralUMIST
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
        if (!schemeData.eof())
        {
            IOWarningInFunction(schemeData)
                << "unexpected additional entries in stream." << nl
                << "    Only the name of the gradient scheme in the"
                   " 'gradSchemes' dictionary should be specified."
                << endl;
        }
    }

    //- Construct from faceFlux and Istream
    limitedCentralUMIST
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

        if (!schemeData.eof())
        {
            IOWarningInFunction(schemeData)
                << "unexpected additional entries in stream." << nl
                << "    Only the name of the gradient scheme in the"
                   " 'gradSchemes' dictionary should be specified."
                << endl;
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

    scalar fluxLimiter(scalar const) const;
};

}

#endif

