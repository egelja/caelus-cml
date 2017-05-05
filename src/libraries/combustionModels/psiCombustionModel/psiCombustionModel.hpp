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
    CML::psiCombustionModel

Description
    Combustion models for compressibility-based thermodynamics

SourceFiles
    psiCombustionModelI.hpp
    psiCombustionModel.cpp
    psiCombustionModelNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef psiCombustionModel_H
#define psiCombustionModel_H

#include "combustionModel.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "hsCombustionThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                     class psiCombustionModel Declaration
\*---------------------------------------------------------------------------*/

class psiCombustionModel
:
    public combustionModel
{

    // Private Member Functions

        //- Construct as copy (not implemented)
        psiCombustionModel(const psiCombustionModel&);

        //- Disallow default bitwise assignment
        void operator=(const psiCombustionModel&);


protected:

    // Protected data


        //- Thermo package
        autoPtr<hsCombustionThermo> thermo_;




public:

    //- Runtime type information
    TypeName("psiCombustionModel");


    //- Declare run-time constructor selection tables
    declareRunTimeSelectionTable
    (
        autoPtr,
        psiCombustionModel,
        dictionary,
        (
            const word& modelType,
            const fvMesh& mesh
        ),
        (modelType, mesh)
    );


    // Constructors


        //- Construct from components and thermo
        psiCombustionModel
        (
            const word& modelType,
            const fvMesh& mesh
        );



    //- Selector
    static autoPtr<psiCombustionModel> New
    (
        const fvMesh& mesh
    );


    //- Destructor
    virtual ~psiCombustionModel();


    // Member Functions

        //- Return access to the thermo package
        inline hsCombustionThermo& thermo();

        //- Return const access to the thermo package
        inline const hsCombustionThermo& thermo() const;

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

#include "psiCombustionModelI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
