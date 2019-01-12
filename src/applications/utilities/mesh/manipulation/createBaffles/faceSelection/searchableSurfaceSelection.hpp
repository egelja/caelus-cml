/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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
    CML::faceSelections::searchableSurfaceSelection

Description
    Selects all (internal or coupled) faces intersecting the searchableSurface.

SourceFiles
    searchableSurfaceSelection.cpp

\*---------------------------------------------------------------------------*/

#ifndef searchableSurfaceSelection_H
#define searchableSurfaceSelection_H

#include "faceSelection.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class searchableSurface;

namespace faceSelections
{

/*---------------------------------------------------------------------------*\
                     Class searchableSurfaceSelection Declaration
\*---------------------------------------------------------------------------*/

class searchableSurfaceSelection
:
    public faceSelection
{
    // Private data

        autoPtr<searchableSurface> surfacePtr_;

public:

    //- Runtime type information
    TypeName("searchableSurface");


    // Constructors

        //- Construct from dictionary
        searchableSurfaceSelection
        (
            const word& name,
            const fvMesh& mesh,
            const dictionary& dict
        );

        //- Clone
        autoPtr<faceSelection> clone() const
        {
            NotImplemented;
            return autoPtr<faceSelection>(nullptr);
        }


    //- Destructor
    virtual ~searchableSurfaceSelection();


    // Member Functions

        virtual void select(const label zoneID, labelList&, boolList&) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace faceSelections
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
