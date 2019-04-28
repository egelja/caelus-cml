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
    CML::radiation::scatterModel

Description
    Base class for radiation scattering

\*---------------------------------------------------------------------------*/

#ifndef scatterModel_HPP
#define scatterModel_HPP

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
                        Class scatterModel Declaration
\*---------------------------------------------------------------------------*/

class scatterModel
{

protected:

    //- Reference to the fvMesh
    const fvMesh& mesh_;

public:

    //- Runtime type information
    TypeName("scatterModel");

    // Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        scatterModel,
        dictionary,
        (
            const dictionary& dict,
            const fvMesh& mesh
        ),
        (dict, mesh)
    );


    //- Construct from components
    scatterModel(const dictionary& dict, const fvMesh& mesh);


    // Selector
    static autoPtr<scatterModel> New
    (
        const dictionary& dict,
        const fvMesh& mesh
    );


    //- Destructor
    virtual ~scatterModel()
    {}


    // Member Functions

    //- Return scatter coefficient
    virtual tmp<volScalarField> sigmaEff() const = 0;

};


} // End namespace radiation
} // End namespace CML


#endif
