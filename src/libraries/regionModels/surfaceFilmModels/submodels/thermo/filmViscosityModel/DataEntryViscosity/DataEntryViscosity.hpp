/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
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
    CML::regionModels::surfaceFilmModels::DataEntryViscosity

Description
    The CML::DataEntry temperature-dependent viscosity model multiplies the
    viscosity of a base model by a DataEntry temperature function.

SourceFiles
    DataEntryViscosity.cpp

\*---------------------------------------------------------------------------*/

#ifndef DataEntryViscosity_HPP
#define DataEntryViscosity_HPP

#include "filmViscosityModel.hpp"
#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                    Class DataEntryViscosity Declaration
\*---------------------------------------------------------------------------*/

class DataEntryViscosity
:
    public filmViscosityModel
{
    // Private member functions

        //- Disallow default bitwise copy construct
        DataEntryViscosity(const DataEntryViscosity&);

        //- Disallow default bitwise assignment
        void operator=(const DataEntryViscosity&);


protected:

    // Protected data

        //- Base viscosity model
        autoPtr<filmViscosityModel> viscosity_;

        //- Viscosity factor as a function of temperature
        autoPtr<DataEntry<scalar> > function_;


public:

    //- Runtime type information
    TypeName("DataEntry");


    // Constructors

        //- Construct from surface film model
        DataEntryViscosity
        (
            surfaceFilmRegionModel& film,
            const dictionary& dict,
            volScalarField& mu
        );


    //- Destructor
    virtual ~DataEntryViscosity();


    // Member Functions

        //- Correct
        virtual void correct
        (
            const volScalarField& p,
            const volScalarField& T
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
