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
    CML::psiChemistryCombustionModel

Description
    Combustion models for compressibility-based thermodynamics

SourceFiles
    psiChemistryCombustionModelI.hpp
    psiChemistryCombustionModel.cpp
    psiChemistryCombustionModelNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef psiChemistryCombustionModel_H
#define psiChemistryCombustionModel_H

#include "combustionModel.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "psiChemistryModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                     class psiChemistryCombustionModel Declaration
\*---------------------------------------------------------------------------*/

class psiChemistryCombustionModel
:
    public combustionModel
{

    // Private Member Functions

        //- Construct as copy (not implemented)
        psiChemistryCombustionModel(const psiChemistryCombustionModel&);

        //- Disallow default bitwise assignment
        void operator=(const psiChemistryCombustionModel&);


protected:

    // Protected data

        //- Auto pointer to psiChemistry
        autoPtr<psiChemistryModel> pChemistry_;


public:

    //- Runtime type information
    TypeName("psiChemistryCombustionModel");


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        psiChemistryCombustionModel,
        dictionary,
        (
            const word& modelType,
            const fvMesh& mesh
        ),
        (modelType, mesh)
    );


    // Constructors


        //- Construct from components and thermo
        psiChemistryCombustionModel
        (
            const word& modelType,
            const fvMesh& mesh
        );



    //- Selector
    static autoPtr<psiChemistryCombustionModel> New
    (
        const fvMesh& mesh
    );


    //- Destructor
    virtual ~psiChemistryCombustionModel();


    // Member Functions


        //- Return access to the thermo package
        inline psiChemistryModel& pChemistry();

        //- Return const access to the thermo package
        inline const psiChemistryModel& pChemistry() const;

        //- Return const access to rho
        inline tmp<volScalarField> rho() const;

        //- Return const access to rho
        inline const hsCombustionThermo& thermo() const;

        //- Return non const access to rho
        inline hsCombustionThermo& thermo();


    // I-O

        //- Update properties from given dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML
} // End namespace combustionModels

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "psiChemistryCombustionModelI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
