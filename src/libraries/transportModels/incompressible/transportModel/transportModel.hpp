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
    CML::transportModel

Description
    Base-class for all transport models used by the incompressible turbulence
    models.

SourceFiles
    transportModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef transportModel_H
#define transportModel_H

#include "IOdictionary.hpp"
#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                Class transportModel Declaration
\*---------------------------------------------------------------------------*/

class transportModel
:
    public IOdictionary
{
    // Private Member Functions

        //- Disallow copy construct
        transportModel(const transportModel&);

        //- Disallow default bitwise assignment
        void operator=(const transportModel&);


public:

    // Constructors

        //- Construct from components
        transportModel
        (
            const volVectorField& U,
            const surfaceScalarField& phi
        );


    //- Destructor
    virtual ~transportModel();


    // Member Functions

        //- Return the laminar viscosity
        virtual tmp<volScalarField> nu() const = 0;

        //- Correct the laminar viscosity
        virtual void correct() = 0;

        //- Read transportProperties dictionary
        virtual bool read() = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
