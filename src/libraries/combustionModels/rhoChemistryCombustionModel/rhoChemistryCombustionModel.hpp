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
    CML::rhoChemistryCombustionModel

Description
    Combustion models for compressibility-based thermodynamics

SourceFiles
    rhoChemistryCombustionModelI.hpp
    rhoChemistryCombustionModel.cpp
    rhoChemistryCombustionModelNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef rhoChemistryCombustionModel_H
#define rhoChemistryCombustionModel_H

#include "combustionModel.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "rhoChemistryModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                     class rhoChemistryCombustionModel Declaration
\*---------------------------------------------------------------------------*/

class rhoChemistryCombustionModel
:
    public combustionModel
{

    // Private Member Functions

        //- Construct as copy (not implemented)
        rhoChemistryCombustionModel(const rhoChemistryCombustionModel&);

        //- Disallow default bitwise assignment
        void operator=(const rhoChemistryCombustionModel&);


protected:

    // Protected data

        //- Auto pointer to psiChemistry
        autoPtr<rhoChemistryModel> pChemistry_;


public:

    //- Runtime type information
    TypeName("rhoChemistryCombustionModel");


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        rhoChemistryCombustionModel,
        dictionary,
        (
            const word& modelType,
            const fvMesh& mesh
        ),
        (modelType, mesh)
    );


    // Constructors


        //- Construct from components and thermo
        rhoChemistryCombustionModel
        (
            const word& modelType,
            const fvMesh& mesh
        );



    //- Selector
    static autoPtr<rhoChemistryCombustionModel> New
    (
        const fvMesh& mesh
    );


    //- Destructor
    virtual ~rhoChemistryCombustionModel();


    // Member Functions


        //- Return access to the thermo package
        inline rhoChemistryModel& pChemistry();

        //- Return const access to the thermo package
        inline const rhoChemistryModel& pChemistry() const;

        //- Return const access to rho
        inline tmp<volScalarField> rho() const;

        //- Return const access to rho
        inline const hsReactionThermo& thermo() const;

        //- Return non const access to rho
        inline hsReactionThermo& thermo();


    // I-O

        //- Update properties from given dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML
} // End namespace combustionModels

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "rhoChemistryCombustionModelI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
