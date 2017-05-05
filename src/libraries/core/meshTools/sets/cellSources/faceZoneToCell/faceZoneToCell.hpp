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
    CML::faceZoneToCell

Description
    A topoSetSource to select cells based on side of faceZone.

SourceFiles
    faceZoneToCell.cpp

\*---------------------------------------------------------------------------*/

#ifndef faceZoneToCell_H
#define faceZoneToCell_H

#include "topoSetSource.hpp"
#include "wordRe.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class faceZoneToCell Declaration
\*---------------------------------------------------------------------------*/

class faceZoneToCell
:
    public topoSetSource
{
public:
        //- Enumeration defining the valid options
        enum faceAction
        {
            MASTER,
            SLAVE
        };

private:

    // Private data

        static const NamedEnum<faceAction, 2> faceActionNames_;

        //- Add usage string
        static addToUsageTable usage_;

        //- Name/regular expression of faceZone
        wordRe zoneName_;

        //- Option
        faceAction option_;


    // Private Member Functions

        void combine(topoSet& set, const bool add) const;


public:

    //- Runtime type information
    TypeName("faceZoneToCell");

    // Constructors

        //- Construct from components
        faceZoneToCell
        (
            const polyMesh& mesh,
            const word& zoneName,
            const faceAction option
        );

        //- Construct from dictionary
        faceZoneToCell
        (
            const polyMesh& mesh,
            const dictionary& dict
        );

        //- Construct from Istream
        faceZoneToCell
        (
            const polyMesh& mesh,
            Istream&
        );


    //- Destructor
    virtual ~faceZoneToCell();


    // Member Functions

        virtual sourceType setType() const
        {
            return CELLSETSOURCE;
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
