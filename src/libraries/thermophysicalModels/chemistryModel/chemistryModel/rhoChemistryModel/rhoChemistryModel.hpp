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
    CML::rhoChemistryModel

Description
    Chemistry model for density-based thermodynamics

SourceFiles
    rhoChemistryModelI.hpp
    rhoChemistryModel.cpp
    newChemistryModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef rhoChemistryModel_H
#define rhoChemistryModel_H

#include "basicChemistryModel.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "hsReactionThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class fvMesh;

/*---------------------------------------------------------------------------*\
                     class rhoChemistryModel Declaration
\*---------------------------------------------------------------------------*/

class rhoChemistryModel
:
    public basicChemistryModel
{
    // Private Member Functions

        //- Construct as copy (not implemented)
        rhoChemistryModel(const rhoChemistryModel&);

        //- Disallow default bitwise assignment
        void operator=(const rhoChemistryModel&);


protected:

    // Protected data

        //- Thermo package
        autoPtr<hsReactionThermo> thermo_;


public:

    //- Runtime type information
    TypeName("rhoChemistryModel");


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        rhoChemistryModel,
        fvMesh,
        (
            const fvMesh& mesh,
            const word& compTypeName,
            const word& thermoTypeName
        ),
        (mesh, compTypeName, thermoTypeName)
    );


    // Constructors

        //- Construct from mesh
        rhoChemistryModel(const fvMesh& mesh, const word& thermoTypeName);


    //- Selector
    static autoPtr<rhoChemistryModel> New(const fvMesh& mesh);


    //- Destructor
    virtual ~rhoChemistryModel();


    // Member Functions

        //- Return access to the thermo package
        inline hsReactionThermo& thermo();

        //- Return const access to the thermo package
        inline const hsReactionThermo& thermo() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "rhoChemistryModelI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
