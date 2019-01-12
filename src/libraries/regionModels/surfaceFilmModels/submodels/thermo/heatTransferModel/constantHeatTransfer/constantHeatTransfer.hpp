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
    CML::constantHeatTransfer

Description
    Constant heat transfer model

SourceFiles
    constantHeatTransfer.C

\*---------------------------------------------------------------------------*/

#ifndef constantHeatTransfer_H
#define constantHeatTransfer_H

#include "heatTransferModel.hpp"
#include "volFieldsFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                        Class constantHeatTransfer Declaration
\*---------------------------------------------------------------------------*/

class constantHeatTransfer
:
    public heatTransferModel
{
private:

    // Private data

        //- Constant heat transfer coefficient [W/m2/K]
        scalar c0_;


    // Private member functions

        //- Disallow default bitwise copy construct
        constantHeatTransfer(const constantHeatTransfer&);

        //- Disallow default bitwise assignment
        void operator=(const constantHeatTransfer&);


public:

    //- Runtime type information
    TypeName("constant");


    // Constructors

        //- Construct from surface film model and dictionary
        constantHeatTransfer
        (
            surfaceFilmRegionModel& film,
            const dictionary& dict
        );


    //- Destructor
    virtual ~constantHeatTransfer();


    // Member Functions

        // Evolution

            //- Correct
            virtual void correct();

            //- Return the heat transfer coefficient [W/m2/K]
            virtual tmp<volScalarField> h() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
