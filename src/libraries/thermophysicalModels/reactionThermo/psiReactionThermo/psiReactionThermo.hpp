/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::psiReactionThermo

Description
    CML::psiReactionThermo

SourceFiles
    psiReactionThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef psiReactionThermo_HPP
#define psiReactionThermo_HPP

#include "psiThermo.hpp"
#include "basicSpecieMixture.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class psiReactionThermo Declaration
\*---------------------------------------------------------------------------*/

class psiReactionThermo
:
    public psiThermo
{

public:

    //- Runtime type information
    TypeName("psiReactionThermo");


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        psiReactionThermo,
        fvMesh,
        (const fvMesh& mesh, const word& phaseName),
        (mesh, phaseName)
    );


    //- Construct from mesh and phase name
    psiReactionThermo
    (
        const fvMesh&,
        const word& phaseName
    );


    // Selectors

    //- Standard selection based on fvMesh
    static autoPtr<psiReactionThermo> New
    (
        const fvMesh&,
        const word& phaseName=word::null
    );


    //- Destructor
    virtual ~psiReactionThermo()
    {}


    // Member functions

    //- Return the composition of the multi-component mixture
    virtual basicSpecieMixture& composition() = 0;

    //- Return the composition of the multi-component mixture
    virtual const basicSpecieMixture& composition() const = 0;
};


} // End namespace CML


#endif
