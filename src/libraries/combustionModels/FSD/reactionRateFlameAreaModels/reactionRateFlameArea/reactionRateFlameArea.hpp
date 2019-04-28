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
    CML::reactionRateFlameArea

Description
    Abstract class for reaction rate per flame area unit

SourceFiles
    reactionRateFlameArea.cpp
    reactionRateFlameAreaNew.cpp

\*---------------------------------------------------------------------------*/

#ifndef reactionRateFlameArea_HPP
#define reactionRateFlameArea_HPP

#include "runTimeSelectionTables.hpp"
#include "dictionary.hpp"
#include "autoPtr.hpp"
#include "volFields.hpp"
#include "combustionModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
class fvMesh;

/*---------------------------------------------------------------------------*\
                           Class reactionRateFlameArea Declaration
\*---------------------------------------------------------------------------*/

class reactionRateFlameArea
{

protected:

    //- Dictionary
    dictionary coeffDict_;

    //- Mesh reference
    const fvMesh& mesh_;

    //- Combstion model owner
    const combustionModel& combModel_;

    //- Fuel name
    word fuel_;

    //- Fuel consumption rate per unit of flame area
    volScalarField omega_;


private:

    // Private member functions

    //- Disallow copy construct
    reactionRateFlameArea(const reactionRateFlameArea&);

    //- Disallow default bitwise assignment
    void operator=(const reactionRateFlameArea&);


public:

    //- Runtime type information
    TypeName("reactionRateFlameArea");


    // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        reactionRateFlameArea,
        dictionary,
        (
            const word modelType,
            const dictionary& dict,
            const fvMesh& mesh,
            const combustionModel& combModel
        ),
        (modelType, dict, mesh, combModel)
    );


    //- Construct from components
    reactionRateFlameArea
    (
        const word& modelType,
        const dictionary& dict,
        const fvMesh& mesh,
        const combustionModel& combModel
    );


    // Selector
    static autoPtr<reactionRateFlameArea> New
    (
        const dictionary& dict,
        const fvMesh& mesh,
        const combustionModel& combModel
    );


    // Destructor
    virtual ~reactionRateFlameArea()
    {}


    // Member functions
    //- Access functions

    //- Return omega
    const volScalarField& omega() const
    {
        return omega_;
    }

    //- Correct omega
    virtual void correct(const volScalarField& sigma) = 0;

    //- Update from dictionary
    virtual bool read(const dictionary& dictProperties);

};


} // End namespace CML


#endif
