/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::compressible::LESModels::vanDriestDelta

Description
    Simple cube-root of cell volume delta used in compressible LES models.

SourceFiles
    compressiblevanDriestDelta.cpp

\*---------------------------------------------------------------------------*/

#ifndef compressiblevanDriestDelta_HPP
#define compressiblevanDriestDelta_HPP

#include "compressibleLESdelta.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace compressible
{
namespace LESModels
{

/*---------------------------------------------------------------------------*\
                           Class vanDriestDelta Declaration
\*---------------------------------------------------------------------------*/

class vanDriestDelta
:
    public LESdelta
{
    // Private data

        autoPtr<CML::LESdelta> geometricDelta_;
        scalar kappa_;
        scalar Aplus_;
        scalar Cdelta_;
        label calcInterval_;


    // Private Member Functions

        //- Disallow default bitwise copy construct and assignment
        vanDriestDelta(const vanDriestDelta&);
        void operator=(const vanDriestDelta&);

        // Calculate the delta values
        void calcDelta();


public:

    //- Runtime type information
    TypeName("vanDriest");


    // Constructors

        //- Construct from name, mesh and IOdictionary
        vanDriestDelta(const word& name, const fvMesh& mesh, const dictionary&);


    //- Destructor
    virtual ~vanDriestDelta()
    {}


    // Member Functions

        //- Read the LESdelta dictionary
        virtual void read(const dictionary&);

        // Correct values
        virtual void correct();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace LESModels
} // End namespace compressible
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
