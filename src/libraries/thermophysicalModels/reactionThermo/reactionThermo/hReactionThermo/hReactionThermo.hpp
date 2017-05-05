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
    CML::hReactionThermo

Description
    CML::hReactionThermo

SourceFiles
    hReactionThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef hReactionThermo_H
#define hReactionThermo_H

#include "basicRhoThermo.hpp"
#include "basicMultiComponentMixture.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class hReactionThermo Declaration
\*---------------------------------------------------------------------------*/

class hReactionThermo
:
    public basicRhoThermo
{

protected:

    // Protected data

        //- Enthalpy field
        volScalarField h_;


public:

    //- Runtime type information
    TypeName("hReactionThermo");


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        hReactionThermo,
        fvMesh,
        (const fvMesh& mesh),
        (mesh)
    );


    // Constructors

        //- Construct from dictionary and mesh
        hReactionThermo(const fvMesh&);


    // Selectors

        //- Standard selection based on fvMesh
        static autoPtr<hReactionThermo> New(const fvMesh&);

        //- Select and check that package contains 'thermoType'
        static autoPtr<hReactionThermo> NewType
        (
            const fvMesh&,
            const word& thermoType
        );


    //- Destructor
    virtual ~hReactionThermo();


    // Member functions

        //- Return the composition of the multi-component mixture
        virtual basicMultiComponentMixture& composition() = 0;

        //- Return the composition of the multi-component mixture
        virtual const basicMultiComponentMixture& composition() const = 0;


        // Access to thermodynamic state variables

            //- Enthalpy [J/kg]
            //  Non-const access allowed for transport equations
            virtual volScalarField& h()
            {
                return h_;
            }

            //- Enthalpy [J/kg]
            virtual const volScalarField& h() const
            {
                return h_;
            }


        //- Update properties
        virtual void correct() = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
