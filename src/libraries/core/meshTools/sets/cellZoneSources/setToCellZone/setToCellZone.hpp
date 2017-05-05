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
    CML::setToCellZone

Description
    A topoSetSource to select cells based on usage in a cellSet.

SourceFiles
    setToCellZone.cpp

\*---------------------------------------------------------------------------*/

#ifndef setToCellZone_H
#define setToCellZone_H

#include "topoSetSource.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class setToCellZone Declaration
\*---------------------------------------------------------------------------*/

class setToCellZone
:
    public topoSetSource
{
    // Private data

        //- Add usage string
        static addToUsageTable usage_;

        //- Name of set to use
        word setName_;

public:

    //- Runtime type information
    TypeName("setToCellZone");

    // Constructors

        //- Construct from components
        setToCellZone
        (
            const polyMesh& mesh,
            const word& setName
        );

        //- Construct from dictionary
        setToCellZone
        (
            const polyMesh& mesh,
            const dictionary& dict
        );

        //- Construct from Istream
        setToCellZone
        (
            const polyMesh& mesh,
            Istream&
        );


    //- Destructor
    virtual ~setToCellZone();


    // Member Functions

        virtual sourceType setType() const
        {
            return CELLZONESOURCE;
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
