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
    Calculate the mesh motion flux and convert fluxes from absolute to relative
    and back.

SourceFiles
    fvcMeshPhi.cpp

\*---------------------------------------------------------------------------*/


#ifndef fvcMeshPhi_H
#define fvcMeshPhi_H

#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "dimensionedTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Namespace fvc functions Declaration
\*---------------------------------------------------------------------------*/

namespace fvc
{
    tmp<surfaceScalarField> meshPhi
    (
        const volVectorField& U
    );

    tmp<surfaceScalarField> meshPhi
    (
        const dimensionedScalar& rho,
        const volVectorField& U
    );

    tmp<surfaceScalarField> meshPhi
    (
        const volScalarField& rho,
        const volVectorField& U
    );


    //- Make the given flux relative
    void makeRelative
    (
        surfaceScalarField& phi,
        const volVectorField& U
    );

    //- Make the given flux relative
    void makeRelative
    (
        surfaceScalarField& phi,
        const dimensionedScalar& rho,
        const volVectorField& U
    );

    //- Make the given flux relative
    void makeRelative
    (
        surfaceScalarField& phi,
        const volScalarField& rho,
        const volVectorField& U
    );


    //- Make the given flux absolute
    void makeAbsolute
    (
        surfaceScalarField& phi,
        const volVectorField& U
    );

    //- Make the given flux absolute
    void makeAbsolute
    (
        surfaceScalarField& phi,
        const dimensionedScalar& rho,
        const volVectorField& U
    );

    //- Make the given flux absolute
    void makeAbsolute
    (
        surfaceScalarField& phi,
        const volScalarField& rho,
        const volVectorField& U
    );


    //- Return the given absolute flux in relative form
    tmp<surfaceScalarField> relative
    (
        const tmp<surfaceScalarField>& tphi,
        const volVectorField& U
    );

    //- Return the given absolute flux in relative form
    tmp<surfaceScalarField> relative
    (
        const tmp<surfaceScalarField>& tphi,
        const volScalarField& rho,
        const volVectorField& U
    );


    //- Return the given relative flux in absolute form
    tmp<surfaceScalarField> absolute
    (
        const tmp<surfaceScalarField>& tphi,
        const volVectorField& U
    );

    //- Return the given relative flux in absolute form
    tmp<surfaceScalarField> absolute
    (
        const tmp<surfaceScalarField>& tphi,
        const volScalarField& rho,
        const volVectorField& U
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
