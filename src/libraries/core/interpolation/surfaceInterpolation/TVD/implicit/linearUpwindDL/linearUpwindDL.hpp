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
    CML::linearUpwindDL

Description
    A TVD property staisfying linear upwind scheme. The limiter is 
    based on Venkatakrishana limiter. Largely eliminates unsteady 
    switching of the limiter on unstructured meshes and promotes 
    convergence.

    DL is short for Differentiable Limiter.

SourceFiles
    linearUpwindDL.cpp

References

    [1] “On the Accuracy of Limiters and Convergence to Steady-State Solutions,”
         Venkatakrishnan, V., AIAA paper 93-0880, Jan. 1993.

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef linearUpwindDL_H
#define linearUpwindDL_H

#include "upwind.hpp"
#include "gaussGrad.hpp"

namespace CML
{

template<class Type>
class linearUpwindDL
:
    public upwind<Type>
{
    // Private Data

        word gradSchemeName_;
        tmp<fv::gradScheme<Type> > gradScheme_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        linearUpwindDL(const linearUpwindDL&);

        //- Disallow default bitwise assignment
        void operator=(const linearUpwindDL&);


public:

    //- Runtime type information
    TypeName("linearUpwindDL");


    // Constructors

        //- Construct from faceFlux
        linearUpwindDL
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
        linearUpwindDL
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
                IOWarningIn("linearUpwindDL(const fvMesh&, Istream&)", schemeData)
                    << "unexpected additional entries in stream." << nl
                    << "    Only the name of the gradient scheme in the"
                       " 'gradSchemes' dictionary should be specified."
                    << endl;
            }
        }

        //- Construct from faceFlux and Istream
        linearUpwindDL
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
                    "linearUpwindDL(const fvMesh&, "
                    "const surfaceScalarField& faceFlux, Istream&)",
                    schemeData
                )   << "unexpected additional entries in stream." << nl
                    << "    Only the name of the gradient scheme in the"
                       " 'gradSchemes' dictionary should be specified."
                    << endl;
            }
        }


    // Member Functions

        //- Return true if this scheme uses an explicit correction
        virtual bool corrected() const
        {
            return true;
        }

        //- Return the explicit correction to the face-interpolate
        virtual tmp<GeometricField<Type, fvsPatchField, surfaceMesh> >
        correction
        (
            const GeometricField<Type, fvPatchField, volMesh>&
        ) const;

};

}

#endif

