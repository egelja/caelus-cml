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
    Foam::searchableSurfaceToFaceZone

Description
    A topoSetSource to select faces based on intersection (of cell-cell
    vector) with a surface.

SourceFiles
    searchableSurfaceToFaceZone.cpp

\*---------------------------------------------------------------------------*/

#ifndef searchableSurfaceToFaceZone_H
#define searchableSurfaceToFaceZone_H

#include "topoSetSource.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class searchableSurface;

/*---------------------------------------------------------------------------*\
                   Class searchableSurfaceToFaceZone Declaration
\*---------------------------------------------------------------------------*/

class searchableSurfaceToFaceZone
:
    public topoSetSource
{
    // Private data

        //- Add usage string
        static addToUsageTable usage_;

        //- Surface
        autoPtr<searchableSurface> surfacePtr_;

public:

    //- Runtime type information
    TypeName("searchableSurfaceToFaceZone");

    // Constructors

        //- Construct from dictionary
        searchableSurfaceToFaceZone
        (
            const polyMesh& mesh,
            const dictionary& dict
        );


    //- Destructor
    virtual ~searchableSurfaceToFaceZone();


    // Member Functions

        virtual sourceType setType() const
        {
            return FACEZONESOURCE;
        }

        virtual void applyToSet
        (
            const topoSetSource::setAction action,
            topoSet&
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
