/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
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
    CML::regionModels::surfaceFilmModels::filmThermoModel

Description
    Base class for film thermo models

SourceFiles
    filmThermoModel.cpp
    filmThermoModelNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef filmThermoModel_HPP
#define filmThermoModel_HPP

#include "filmSubModelBase.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                       Class filmThermoModel Declaration
\*---------------------------------------------------------------------------*/

class filmThermoModel
:
    public filmSubModelBase
{
private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        filmThermoModel(const filmThermoModel&);

        //- Disallow default bitwise assignment
        void operator=(const filmThermoModel&);


public:

    //- Runtime type information
    TypeName("filmThermoModel");


    // Declare runtime constructor selection table

         declareRunTimeSelectionTable
         (
             autoPtr,
             filmThermoModel,
             dictionary,
             (
                 surfaceFilmRegionModel& film,
                 const dictionary& dict
             ),
             (film, dict)
         );

    // Constructors

        //- Construct null
        filmThermoModel(surfaceFilmRegionModel& film);

        //- Construct from type name, dictionary and surface film model
        filmThermoModel
        (
            const word& modelType,
            surfaceFilmRegionModel& film,
            const dictionary& dict
        );


    // Selectors

        //- Return a reference to the selected phase change model
        static autoPtr<filmThermoModel> New
        (
            surfaceFilmRegionModel& film,
            const dictionary& dict
        );


    //- Destructor
    virtual ~filmThermoModel();


    // Member Functions

        //- Return the specie name
        virtual const word& name() const = 0;


        // Elemental access

            //- Return density [kg/m3]
            virtual scalar rho(const scalar p, const scalar T) const = 0;

            //- Return dynamic viscosity [Pa.s]
            virtual scalar mu(const scalar p, const scalar T) const = 0;

            //- Return surface tension [kg/s2]
            virtual scalar sigma(const scalar p, const scalar T) const = 0;

            //- Return specific heat capacity [J/kg/K]
            virtual scalar Cp(const scalar p, const scalar T) const = 0;

            //- Return thermal conductivity [W/m/K]
            virtual scalar kappa(const scalar p, const scalar T) const = 0;

            //- Return diffusivity [m2/s]
            virtual scalar D(const scalar p, const scalar T) const = 0;

            //- Return latent heat [J/kg]
            virtual scalar hl(const scalar p, const scalar T) const = 0;

            //- Return vapour pressure [Pa]
            virtual scalar pv(const scalar p, const scalar T) const = 0;

            //- Return molecular weight [kg/kmol]
            virtual scalar W() const = 0;

            //- Return boiling temperature [K]
            virtual scalar Tb(const scalar p) const = 0;


        // Field access

            //- Return density [kg/m3]
            virtual tmp<volScalarField> rho() const = 0;

            //- Return dynamic viscosity [Pa.s]
            virtual tmp<volScalarField> mu() const = 0;

            //- Return surface tension [kg/s2]
            virtual tmp<volScalarField> sigma() const = 0;

            //- Return specific heat capacity [J/kg/K]
            virtual tmp<volScalarField> Cp() const = 0;

            //- Return thermal conductivity [W/m/K]
            virtual tmp<volScalarField> kappa() const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
