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
    CML::polyAddPoint

Description
    Class containing data for point addition.

\*---------------------------------------------------------------------------*/

#ifndef polyAddPoint_H
#define polyAddPoint_H

#include "label.hpp"
#include "point.hpp"
#include "topoAction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class polyAddPoint Declaration
\*---------------------------------------------------------------------------*/

class polyAddPoint
:
    public topoAction
{
    // Private data

        //- Point to add
        point p_;

        //- Master point
        label masterPointID_;

        //- Point zone ID
        label zoneID_;

        //- Does the point support a cell
        bool inCell_;


public:

    // Static data members

        //- Runtime type information
        TypeName("addPoint");


    // Constructors

        //- Construct null.  Used only for list construction
        polyAddPoint()
        :
            p_(vector::zero),
            masterPointID_(-1),
            zoneID_(-1),
            inCell_(false)
        {}

        //- Construct from components
        polyAddPoint
        (
            const point& p,
            const label masterPointID,
            const label zoneID,
            const bool inCell
        )
        :
            p_(p),
            masterPointID_(masterPointID),
            zoneID_(zoneID),
            inCell_(inCell)
        {
            if (zoneID_ < 0 && !inCell)
            {
                FatalErrorIn
                (
                    "polyAddPoint\n"
                    "(\n"
                    "    const point& p,\n"
                    "    const label masterPointID,\n"
                    "    const label zoneID,\n"
                    "    const bool inCell\n"
                    ")"
                )   << "Point is not in a cell and not in a zone.  "
                    << "This is not allowed.\n"
                    << "point: " << p
                    << " master: " << masterPointID_
                    << " zone: " << zoneID_
                    << abort(FatalError);
            }

        }

        //- Construct and return a clone
        virtual autoPtr<topoAction> clone() const
        {
            return autoPtr<topoAction>(new polyAddPoint(*this));
        }


    // Default Destructor


    // Member Functions

        //- Point location
        const point& newPoint() const
        {
            return p_;
        }

        //- Master point label
        label masterPointID() const
        {
            return masterPointID_;
        }

        //- Is the point appended with no master
        bool appended() const
        {
            return masterPointID_ < 0;
        }

        //- Does the point belong to a zone?
        bool isInZone() const
        {
            return zoneID_ >= 0;
        }

        //- Point zone ID
        label zoneID() const
        {
            return zoneID_;
        }

        //- Does the point support a cell
        bool inCell() const
        {
            return inCell_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //