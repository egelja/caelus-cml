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
    CML::boxToPoint

Description
    A topoSetSource to select points based on whether they are inside box.

SourceFiles
    boxToPoint.cpp

\*---------------------------------------------------------------------------*/

#ifndef boxToPoint_H
#define boxToPoint_H

#include "topoSetSource.hpp"
#include "treeBoundBox.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class boxToPoint Declaration
\*---------------------------------------------------------------------------*/

class boxToPoint
:
    public topoSetSource
{

    // Private data

        //- Add usage string
        static addToUsageTable usage_;

        //- bounding box.
        treeBoundBox bb_;


    // Private Member Functions

        void combine(topoSet& set, const bool add) const;


public:

    //- Runtime type information
    TypeName("boxToPoint");

    // Constructors

        //- Construct from components
        boxToPoint
        (
            const polyMesh& mesh,
            const treeBoundBox& bb
        );

        //- Construct from dictionary
        boxToPoint
        (
            const polyMesh& mesh,
            const dictionary& dict
        );

        //- Construct from Istream
        boxToPoint
        (
            const polyMesh& mesh,
            Istream&
        );


    //- Destructor
    virtual ~boxToPoint();


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
