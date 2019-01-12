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
    CML::regionModels::surfaceFilmModels::transferModelList

Description
    List container for film transfer models

SourceFiles
    transferModelList.cpp

\*---------------------------------------------------------------------------*/

#ifndef transferModelList_HPP
#define transferModelList_HPP

#include "PtrList.hpp"
#include "transferModel.hpp"
#include "filmSubModelBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace regionModels
{
namespace surfaceFilmModels
{

/*---------------------------------------------------------------------------*\
                    Class transferModelList Declaration
\*---------------------------------------------------------------------------*/

class transferModelList
:
    public PtrList<transferModel>,
    public filmSubModelBase
{
    // Private data

        //- List of mass transferred per patch
        scalarField massTransferred_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        transferModelList(const transferModelList&);

        //- Disallow default bitwise assignment
        void operator=(const transferModelList&);


public:

    // Constructors

        //- Construct null
        transferModelList(surfaceFilmRegionModel& film);

        //- Construct from type name, dictionary and surface film model
        transferModelList
        (
            surfaceFilmRegionModel& film,
            const dictionary& dict
        );


    //- Destructor
    virtual ~transferModelList();


    // Member Functions

        //- Correct kinematic transfers
        virtual void correct
        (
            scalarField& availableMass,
            volScalarField& massToTransfer
        );

        //- Correct kinematic and thermodynamic transfers
        virtual void correct
        (
            scalarField& availableMass,
            volScalarField& massToTransfer,
            volScalarField& energyToTransfer
        );

        //- Provide some info
        virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace surfaceFilmModels
} // End namespace regionModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
