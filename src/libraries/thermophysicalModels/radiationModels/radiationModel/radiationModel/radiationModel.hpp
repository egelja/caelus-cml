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

Namespace
    CML::radiation

Description
    Namespace for radiation modelling

Class
    CML::radiation::radiationModel

Description
    Top level model for radiation modelling

SourceFiles
    radiationModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef radiationModel_H
#define radiationModel_H

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "volFields.hpp"
#include "basicThermo.hpp"
#include "fvMatrices.hpp"
#include "blackBodyEmission.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace radiation
{

// Forward declaration of classes
class absorptionEmissionModel;
class scatterModel;

/*---------------------------------------------------------------------------*\
                       Class radiationModel Declaration
\*---------------------------------------------------------------------------*/

class radiationModel
:
    public IOdictionary
{
protected:

    // Protected data

        //- Reference to the mesh database
        const fvMesh& mesh_;

        //- Reference to the time database
        const Time& time_;

        //- Reference to the temperature field
        const volScalarField& T_;

        //- Model specific dictionary input parameters
        Switch radiation_;

        //- Radiation model dictionary
        dictionary coeffs_;

        //- Radiation solver frequency - number flow solver iterations per
        //  radiation solver iteration
        label solverFreq_;

        //- Flag to enable radiation model to be evaluated on first iteration
        bool firstIter_;


        // References to the radiation sub-models

            //- Absorption/emission model
            autoPtr<absorptionEmissionModel> absorptionEmission_;

            //- Scatter model
            autoPtr<scatterModel> scatter_;


private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        radiationModel(const radiationModel&);

        //- Disallow default bitwise assignment
        void operator=(const radiationModel&);


public:

    //- Runtime type information
    TypeName("radiationModel");


    // Declare runtime constructor selection table

         declareRunTimeSelectionTable
         (
             autoPtr,
             radiationModel,
             dictionary,
             (
                 const volScalarField& T
             ),
             (T)
         );


    // Constructors

        //- Null constructor
        radiationModel(const volScalarField& T);

        //- Construct from components
        radiationModel(const word& type, const volScalarField& T);


    // Selectors

         //- Return a reference to the selected radiation model
         static autoPtr<radiationModel> New(const volScalarField& T);


    //- Destructor
    virtual ~radiationModel();


    // Member Functions

        // Edit

            //- Main update/correction routine
            virtual void correct();

            //- Solve radiation equation(s)
            virtual void calculate() = 0;

            //- Read radiationProperties dictionary
            virtual bool read() = 0;


        // Access

            //- Source term component (for power of T^4)
            virtual tmp<volScalarField> Rp() const = 0;

            //- Source term component (constant)
            virtual tmp<DimensionedField<scalar, volMesh> > Ru() const = 0;

            //- Enthalpy source term
            virtual tmp<fvScalarMatrix> Sh(basicThermo& thermo) const;

            //- Sensible enthalpy source term
            virtual tmp<fvScalarMatrix> Shs(basicThermo& thermo) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace radiation
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
