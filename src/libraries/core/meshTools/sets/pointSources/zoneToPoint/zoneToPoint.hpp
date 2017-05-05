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
    CML::zoneToPoint

Description
    A topoSetSource to select points based on pointZone.

SourceFiles
    zoneToPoint.cpp

\*---------------------------------------------------------------------------*/

#ifndef zoneToPoint_H
#define zoneToPoint_H

#include "topoSetSource.hpp"
#include "wordRe.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class zoneToPoint Declaration
\*---------------------------------------------------------------------------*/

class zoneToPoint
:
    public topoSetSource
{

    // Private data

        //- Add usage string
        static addToUsageTable usage_;

        //- Name/regular expression of zone
        wordRe zoneName_;


    // Private Member Functions

        void combine(topoSet& set, const bool add) const;


public:

    //- Runtime type information
    TypeName("zoneToPoint");

    // Constructors

        //- Construct from components
        zoneToPoint
        (
            const polyMesh& mesh,
            const word& zoneName
        );

        //- Construct from dictionary
        zoneToPoint
        (
            const polyMesh& mesh,
            const dictionary& dict
        );

        //- Construct from Istream
        zoneToPoint
        (
            const polyMesh& mesh,
            Istream&
        );


    //- Destructor
    virtual ~zoneToPoint();


    // Member Functions

        virtual sourceType setType() const
        {
            return POINTSETSOURCE;
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
