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

Class
    CML::singlePhaseTransportModel

Description
    A simple single-phase transport model based on viscosityModel.

    Used by the incompressible single-phase solvers like simpleCML,
    turbCML etc.

SourceFiles
    singlePhaseTransportModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef singlePhaseTransportModel_H
#define singlePhaseTransportModel_H

#include "incompressible/transportModel/transportModel.hpp"
#include "autoPtr.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class viscosityModel;

/*---------------------------------------------------------------------------*\
                Class singlePhaseTransportModel Declaration
\*---------------------------------------------------------------------------*/

class singlePhaseTransportModel
:
    public transportModel
{
    // Private Data

        autoPtr<viscosityModel> viscosityModelPtr_;


    // Private Member Functions

        //- Disallow copy construct
        singlePhaseTransportModel(const singlePhaseTransportModel&);

        //- Disallow default bitwise assignment
        void operator=(const singlePhaseTransportModel&);


public:

    // Constructors

        //- Construct from components
        singlePhaseTransportModel
        (
            const volVectorField& U,
            const surfaceScalarField& phi
        );


    //- Destructor
    virtual ~singlePhaseTransportModel();


    // Member Functions

        //- Return the laminar viscosity
        tmp<volScalarField> nu() const;

        //- Correct the laminar viscosity
        void correct();

        //- Read transportProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
