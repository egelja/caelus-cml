/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    CML::compressible::LESdelta

Description
    Abstract base class for compressible LES deltas

SourceFiles
    compressibleLESdelta.cpp

\*---------------------------------------------------------------------------*/

#ifndef compressibleLESdelta_H
#define compressibleLESdelta_H

#include "LESdelta.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// To avoid macro problems typedef scoped class
typedef CML::LESdelta cmlLESdelta;

namespace compressible
{

/*---------------------------------------------------------------------------*\
                          Class LESdelta Declaration
\*---------------------------------------------------------------------------*/

class LESdelta
:
    public cmlLESdelta
{

public:

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            cmlLESdelta,
            dictionary,
            (
                const word& name,
                const fvMesh& mesh,
                const dictionary& LESdeltaDict
            ),
            (name, mesh, LESdeltaDict)
        );

    // Constructors

        //- Construct from name and mesh
        LESdelta(const word& name, const fvMesh& mesh);


    // Selectors

        //- Return a reference to the selected LES delta
        static autoPtr<cmlLESdelta> New
        (
            const word& name,
            const fvMesh& mesh,
            const dictionary& dict
        );


    //- Destructor
    virtual ~LESdelta()
    {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace compressible
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
