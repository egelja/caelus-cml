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
    CML::filmSubModelBase

Description
    Base class for surface film sub-models

SourceFiles
    filmSubModelBaseI.H
    filmSubModelBase.C

\*---------------------------------------------------------------------------*/

#ifndef filmSubModelBase_H
#define filmSubModelBase_H

#include "surfaceFilmModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                      Class subModelBase Declaration
\*---------------------------------------------------------------------------*/

class filmSubModelBase
{
private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        filmSubModelBase(const filmSubModelBase&);

        //- Disallow default bitwise assignment
        void operator=(const filmSubModelBase&);


protected:

    // Protected data

        //- Reference to the owner surface film model
        const surfaceFilmModel& owner_;

        //- Model coefficients dictionary
        dictionary coeffs_;


public:

    // Constructors

        //- Construct null
        filmSubModelBase(const surfaceFilmModel& owner);

        //- Construct from type name, dictionary and surface film model
        filmSubModelBase
        (
            const word& type,
            const surfaceFilmModel& owner,
            const dictionary& dict
        );


    //- Destructor
    virtual ~filmSubModelBase();


    // Member Functions

        // Access

            //- Return the reference to the owner surface film model
            inline const surfaceFilmModel& owner() const;

            //- Return the model coefficients dictionary
            inline const dictionary& coeffs() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "filmSubModelBaseI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
