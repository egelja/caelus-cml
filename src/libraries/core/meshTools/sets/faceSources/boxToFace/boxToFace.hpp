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
    CML::boxToFace

Description
    A topoSetSource to select faces based on face centres inside box.

SourceFiles
    boxToFace.cpp

\*---------------------------------------------------------------------------*/

#ifndef boxToFace_H
#define boxToFace_H

#include "topoSetSource.hpp"
#include "treeBoundBox.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class boxToFace Declaration
\*---------------------------------------------------------------------------*/

class boxToFace
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
    TypeName("boxToFace");

    // Constructors

        //- Construct from components
        boxToFace
        (
            const polyMesh& mesh,
            const treeBoundBox& bb
        );

        //- Construct from dictionary
        boxToFace
        (
            const polyMesh& mesh,
            const dictionary& dict
        );

        //- Construct from Istream
        boxToFace
        (
            const polyMesh& mesh,
            Istream&
        );


    //- Destructor
    virtual ~boxToFace();


    // Member Functions

        virtual sourceType setType() const
        {
            return FACESETSOURCE;
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
