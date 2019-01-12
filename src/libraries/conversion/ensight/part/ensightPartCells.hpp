/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2017 OpenFOAM Foundation
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
    CML::ensightPartCells

Description
    An implementation of ensightPart to hold volume mesh cells.

SourceFiles
    ensightPartCells.cpp

\*---------------------------------------------------------------------------*/

#ifndef ensightPartCells_H
#define ensightPartCells_H

#include "ensightPart.hpp"
#include "typeInfo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class ensightPartCells Declaration
\*---------------------------------------------------------------------------*/

class ensightPartCells
:
    public ensightPart
{
    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const ensightPartCells&);

        //- Classify the cell types, set elemLists.
        void classify
        (
            const polyMesh&,
            const labelUList& idLabels = labelUList::null()
        );

        //- track points used
        virtual localPoints calcLocalPoints() const;

        //- track the points used
        // virtual void makeLocalPointMap();

        //- element connectivity
        virtual void writeConnectivity
        (
            ensightGeoFile&,
            const word& key,
            const labelUList& idList,
            const labelUList& pointMap
        ) const;


protected:

        //- addressable ensight element types
        enum elemType
        {
            tetra4Elements,
            pyramid5Elements,
            penta6Elements,
            hexa8Elements,
            nfacedElements
        };


    // Static data members
    
    static const polyMesh* const polyMeshNullPtr_;
    
    static const List<word> elemTypes_;


    // Protected data

        //- mesh referenced
        const polyMesh& mesh_;


public:

    //- Runtime type information
    TypeName("ensightCells");

    // Constructors

        //- Construct empty part with number and description
        ensightPartCells(label partNumber, const string& partDescription);

        //- Construct from polyMesh without zones
        ensightPartCells(label partNumber, const polyMesh&);

        //- Construct from polyMesh and list of (non-zoned) cells
        ensightPartCells
        (
            label partNumber,
            const polyMesh&,
            const labelUList&
        );

        //- Construct from polyMesh and cellZone
        ensightPartCells
        (
            label partNumber,
            const polyMesh&,
            const cellZone&
        );

        //- Construct as copy
        ensightPartCells(const ensightPartCells&);

        //- Reconstruct part characteristics (eg, element types) from Istream
        //  A part reconstructed in this manner can be used when writing fields,
        //  but cannot be used to write a new geometry
        //  \sa CML::ensightPart::reconstruct
        ensightPartCells(Istream&);

        //- Reconstruct part characteristics on freestore from Istream
        static autoPtr<ensightPartCells> New(Istream& is)
        {
            return autoPtr<ensightPartCells>(new ensightPartCells(is));
        }


    //- Destructor
    virtual ~ensightPartCells();


    // Member Functions

        //- write geometry
        virtual void writeGeometry(ensightGeoFile&) const;

        //- static listing of the element types
        virtual const List<word>& elementTypes() const
        {
            return elemTypes_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
