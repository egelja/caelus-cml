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
    CML::linearUpwindMDV

Description

    A TVD property staisfying limited central scheme based on the 
    multidimensional Venkatakrishnan limiter to prevent unwanted oscillations. 
    Implementation suitable for scalar fields.

    The scheme inherits from upwind class but it does not use 
    upwind inteprolation since that function is virtual it is 
    overwritten here.

SourceFiles
    linearUpwindMDV.cpp

References

    [1] “On the Accuracy of Limiters and Convergence to Steady-State Solutions,”
         Venkatakrishnan, V., AIAA paper 93-0880, Jan. 1993.

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef linearUpwindMDV_HPP
#define linearUpwindMDV_HPP

#include "upwind.hpp"
#include "gaussGrad.hpp"

namespace CML
{

template<class Type>
class linearUpwindMDV
:
    public upwind<Type>
{

    // Private Data

        word gradSchemeName_;
        tmp<fv::gradScheme<Type> > gradScheme_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        linearUpwindMDV(const linearUpwindMDV&);

        //- Disallow default bitwise assignment
        void operator=(const linearUpwindMDV&);


public:

    //- Runtime type information
    TypeName("linearUpwindMDV");


    // Constructors

    //- Construct from faceFlux
    linearUpwindMDV
    (
        const fvMesh& mesh,
        const surfaceScalarField& faceFlux
    )
    :
        upwind<Type>(mesh, faceFlux),
        gradSchemeName_("grad"),
        gradScheme_
        (
            new fv::gaussGrad<Type>(mesh)
        )
        {}

    //- Construct from Istream.
    //  The name of the flux field is read from the Istream and looked-up
    //  from the mesh objectRegistry
    linearUpwindMDV
    (
        const fvMesh& mesh,
        Istream& schemeData
    )
    :
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
            IOWarningIn("linearUpwindMDV(const fvMesh&, Istream&)", schemeData)
                << "unexpected additional entries in stream." << nl
                << "    Only the name of the gradient scheme in the"
                   " 'gradSchemes' dictionary should be specified."
                << endl;
        }
    }

    //- Construct from faceFlux and Istream
    linearUpwindMDV
    (
        const fvMesh& mesh,
        const surfaceScalarField& faceFlux,
        Istream& schemeData
    )
    :
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
            IOWarningIn
            (
                "linearUpwindMDV(const fvMesh&, "
                "const surfaceScalarField& faceFlux, Istream&)",
                schemeData
            )   << "unexpected additional entries in stream." << nl
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
        const GeometricField<Type, fvPatchField, volMesh>& vf
    ) const;
};

}

#endif

