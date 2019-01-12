/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::regionModels::surfaceFilmModel

Description
    Base class for surface film models

SourceFiles
    surfaceFilmModelI.H
    surfaceFilmModel.C
    surfaceFilmModelNew.C

\*---------------------------------------------------------------------------*/

#ifndef surfaceFilmModel_H
#define surfaceFilmModel_H

#include "runTimeSelectionTables.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{

/*---------------------------------------------------------------------------*\
                      Class surfaceFilmModel Declaration
\*---------------------------------------------------------------------------*/

class surfaceFilmModel
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        surfaceFilmModel(const surfaceFilmModel&);

        //- Disallow default bitwise assignment
        void operator=(const surfaceFilmModel&);


public:

    //- Runtime type information
    TypeName("surfaceFilmModel");

    //- Reference temperature for enthalpy
    static const dimensionedScalar Tref;


    // Declare runtime constructor selection table

         declareRunTimeSelectionTable
         (
             autoPtr,
             surfaceFilmModel,
             mesh,
             (
                const word& modelType,
                const fvMesh& mesh,
                const dimensionedVector& g,
                const word& regionType
             ),
             (modelType, mesh, g, regionType)
         );


    //- Constructor
    surfaceFilmModel();


    // Selectors

        //- Return a reference to the selected surface film model
        static autoPtr<surfaceFilmModel> New
        (
            const fvMesh& mesh,
            const dimensionedVector& g,
            const word& regionType = "surfaceFilm"
        );


    //- Destructor
    virtual ~surfaceFilmModel();


    // Member Functions

        // Solution parameters

            //- Courant number evaluation
            virtual scalar CourantNumber() const = 0;


        // Primary region source fields

            //- Return total mass source - Eulerian phase only
            virtual tmp<DimensionedField<scalar, volMesh> > Srho() const = 0;

            //- Return mass source for specie i - Eulerian phase only
            virtual tmp<DimensionedField<scalar, volMesh> > Srho
            (
                const label i
            ) const = 0;

            //- Return enthalpy source - Eulerian phase only
            virtual tmp<DimensionedField<scalar, volMesh> > Sh() const = 0;


        // Evolution

            //- Main driver routing to evolve the region - calls other evolves
            virtual void evolve() = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
