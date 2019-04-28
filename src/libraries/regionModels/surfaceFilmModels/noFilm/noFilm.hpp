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
    CML::regionModels::surfaceFilmModels::noFilm

Description
    Dummy surfaceFilmModel to allow solvers supporting film simulations to be
    run without a film region.

SourceFiles
    noFilm.C

\*---------------------------------------------------------------------------*/

#ifndef noFilm_H
#define noFilm_H

#include "surfaceFilmModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                          Class noFilm Declaration
\*---------------------------------------------------------------------------*/

class noFilm
:
    public surfaceFilmModel
{
    // Private member data

        //- Reference to the mesh
        const fvMesh& mesh_;


    // Private member functions

        //- Disallow default bitwise copy construct
        noFilm(const noFilm&);

        //- Disallow default bitwise assignment
        void operator=(const noFilm&);


public:

    //- Runtime type information
    TypeName("none");


    // Constructors

        //- Construct from components
        noFilm
        (
            const word& modelType,
            const fvMesh& mesh,
            const dimensionedVector& g,
            const word& regionType
        );


    //- Destructor
    virtual ~noFilm();


    // Member Functions

        // Solution parameters

            //- Courant number evaluation
            virtual scalar CourantNumber() const;


        // Primary region source fields

            //- Return total mass source - Eulerian phase only
            virtual tmp<DimensionedField<scalar, volMesh> > Srho() const;

            //- Return mass source for specie i - Eulerian phase only
            virtual tmp<DimensionedField<scalar, volMesh> > Srho
            (
                const label i
            ) const;

            //- Return enthalpy source - Eulerian phase only
            virtual tmp<DimensionedField<scalar, volMesh> > Sh() const;


        // Evolution

            //- Main driver routing to evolve the region - calls other evolves
            virtual void evolve();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
