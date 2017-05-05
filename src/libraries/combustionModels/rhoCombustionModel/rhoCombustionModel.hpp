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
    CML::rhoCombustionModel

Description
    Combustion models for rho-based thermodynamics

SourceFiles
    rhoCombustionModelI.hpp
    rhoCombustionModel.cpp
    rhoCombustionModelNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef rhoCombustionModel_H
#define rhoCombustionModel_H

#include "combustionModel.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "hsReactionThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                     class rhoCombustionModel Declaration
\*---------------------------------------------------------------------------*/

class rhoCombustionModel
:
    public combustionModel
{

    // Private Member Functions

        //- Construct as copy (not implemented)
        rhoCombustionModel(const rhoCombustionModel&);

        //- Disallow default bitwise assignment
        void operator=(const rhoCombustionModel&);


protected:

    // Protected data

        //- Thermo package
        autoPtr<hsReactionThermo> thermo_;


public:

    //- Runtime type information
    TypeName("rhoCombustionModel");


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        rhoCombustionModel,
        dictionary,
        (
            const word& modelType,
            const fvMesh& mesh
        ),
        (modelType, mesh)
    );


    // Constructors


        //- Construct from components and thermo
        rhoCombustionModel
        (
            const word& modelType,
            const fvMesh& mesh
        );



    //- Selector
    static autoPtr<rhoCombustionModel> New
    (
        const fvMesh& mesh
    );


    //- Destructor
    virtual ~rhoCombustionModel();


    // Member Functions

        // Access functions

            //- Access combustion dict
             inline const dictionary& coeff() const;


        //- Return access to the thermo package
        inline hsReactionThermo& thermo();

        //- Return const access to the thermo package
        inline const hsReactionThermo& thermo() const;

        //- Return tmp of rho
        virtual tmp<volScalarField> rho() const;

     // I-O

        //- Update properties from given dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML
} // End namespace combustionModels

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "rhoCombustionModelI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
