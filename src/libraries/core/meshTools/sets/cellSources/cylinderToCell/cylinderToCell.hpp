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
    CML::cylinderToCell

Description
    A topoSetSource to select cells based on cell centres inside a cylinder.

SourceFiles
    cylinderToCell.cpp

\*---------------------------------------------------------------------------*/

#ifndef cylinderToCell_H
#define cylinderToCell_H

#include "topoSetSource.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class cylinderToCell Declaration
\*---------------------------------------------------------------------------*/

class cylinderToCell
:
    public topoSetSource
{

    // Private data

        //- Add usage string
        static addToUsageTable usage_;

        //- First point on cylinder axis
        vector p1_;

        //- Second point on cylinder axis
        vector p2_;

        //- Radius
        scalar radius_;


    // Private Member Functions

        void combine(topoSet& set, const bool add) const;


public:

    //- Runtime type information
    TypeName("cylinderToCell");


    // Constructors

        //- Construct from components
        cylinderToCell
        (
            const polyMesh& mesh,
            const vector& p1,
            const vector& p2,
            const scalar radius
        );

        //- Construct from dictionary
        cylinderToCell
        (
            const polyMesh& mesh,
            const dictionary& dict
        );

        //- Construct from Istream
        cylinderToCell
        (
            const polyMesh& mesh,
            Istream&
        );


    //- Destructor
    virtual ~cylinderToCell();


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
