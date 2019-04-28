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
    CML::faceSelections::faceZoneSelection

Description
    Deselect cells not reachable from 'inside' points

SourceFiles
    faceZoneSelection.cpp

\*---------------------------------------------------------------------------*/

#ifndef faceZoneSelection_H
#define faceZoneSelection_H

#include "faceSelection.hpp"
#include "pointField.hpp"
#include "boolList.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class regionSplit;

namespace faceSelections
{

/*---------------------------------------------------------------------------*\
                     Class faceZoneSelection Declaration
\*---------------------------------------------------------------------------*/

class faceZoneSelection
:
    public faceSelection
{
    // Private data

        //- Name of faceZone
        const word zoneName_;


   // Private Member Functions

public:

    //- Runtime type information
    TypeName("faceZone");


    // Constructors

        //- Construct from dictionary
        faceZoneSelection
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
    virtual ~faceZoneSelection();


    // Member Functions

        //- Apply this selector
        virtual void select(const label zoneID, labelList&, boolList&) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace faceSelections
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
