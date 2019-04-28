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
    CML::radiation::sootModel

Description
    Base class for soor models

\*---------------------------------------------------------------------------*/

#ifndef radiation_sootModel_HPP
#define radiation_sootModel_HPP

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"
#include "volFields.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace radiation
{


/*---------------------------------------------------------------------------*\
                            Class sootModel Declaration
\*---------------------------------------------------------------------------*/

class sootModel
{

protected:

    //- Radiation model dictionary
    const dictionary dict_;

    //- Reference to the fvMesh
    const fvMesh& mesh_;


public:

    //- Runtime type information
    TypeName("sootModel");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        sootModel,
        dictionary,
        (
            const dictionary& dict,
            const fvMesh& mesh,
            const word& modelType
        ),
        (dict, mesh, modelType)
    );


    //- Construct from components
    sootModel
    (
        const dictionary& dict,
        const fvMesh& mesh,
        const word& modelType
    );


    //- Selector
    static autoPtr<sootModel> New
    (
        const dictionary& dict,
        const fvMesh& mesh
    );


    //- Destructor
    virtual ~sootModel()
    {}


    // Member Functions

    //- Reference to the mesh
    inline const fvMesh& mesh() const
    {
        return mesh_;
    }

    //- Reference to the dictionary
    inline const dictionary& dict() const
    {
        return dict_;
    }

    //- Main update/correction routine
    virtual void correct() = 0;

    //- Return const reference to soot
    virtual const volScalarField& soot() const = 0;

};


} // End namespace CML
} // End namespace radiation


#endif
