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
    CML::nearestToCell

Description
    A topoSetSource to select cells nearest to points.

SourceFiles
    nearestToCell.cpp

\*---------------------------------------------------------------------------*/

#ifndef nearestToCell_H
#define nearestToCell_H

#include "topoSetSource.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class nearestToCell Declaration
\*---------------------------------------------------------------------------*/

class nearestToCell
:
    public topoSetSource
{

    // Private data

        //- Add usage string
        static addToUsageTable usage_;

        //- points to select nearest to
        pointField points_;


    // Private Member Functions

        void combine(topoSet& set, const bool add) const;


public:

    //- Runtime type information
    TypeName("nearestToCell");

    // Constructors

        //- Construct from components
        nearestToCell
        (
            const polyMesh& mesh,
            const pointField& points
        );

        //- Construct from dictionary
        nearestToCell
        (
            const polyMesh& mesh,
            const dictionary& dict
        );

        //- Construct from Istream
        nearestToCell
        (
            const polyMesh& mesh,
            Istream&
        );


    //- Destructor
    virtual ~nearestToCell();


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
