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
    CML::fileFormats::TRIsurfaceFormatCore

Description
    Internal class used by the TRIsurfaceFormat

SourceFiles
    TRIsurfaceFormatCore.cpp

\*---------------------------------------------------------------------------*/

#ifndef TRIsurfaceFormatCore_H
#define TRIsurfaceFormatCore_H

#include "surfaceFormatsCore.hpp"
#include "triFace.hpp"

#include "IFstream.hpp"
#include "Ostream.hpp"
#include "OFstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                    Class TRIsurfaceFormatCore Declaration
\*---------------------------------------------------------------------------*/

class TRIsurfaceFormatCore
:
    public surfaceFormatsCore
{
    // Private Data

        bool sorted_;

        //- The points supporting the facets
        pointField points_;

        //- The zones associated with the faces
        List<label> zoneIds_;

        //- The solid count, in the order of their first appearance
        List<label> sizes_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        TRIsurfaceFormatCore(const TRIsurfaceFormatCore&);

        //- Disallow default bitwise assignment
        void operator=(const TRIsurfaceFormatCore&);

        bool read(const fileName&);


public:

    // Constructors

        //- Read from file, filling in the information
        TRIsurfaceFormatCore(const fileName&);


    //- Destructor
    ~TRIsurfaceFormatCore();


    // Member Functions

        //- File read was already sorted
        bool sorted() const
        {
            return sorted_;
        }

        //- Flush all values
        void clear()
        {
            sorted_ = true;
            points_.clear();
            zoneIds_.clear();
            sizes_.clear();
        }

        //- Return full access to the points
        pointField& points()
        {
            return points_;
        }

        //- Return full access to the zones
        List<label>& zoneIds()
        {
            return zoneIds_;
        }

        //- The list of zone sizes in the order of their first appearance
        List<label>& sizes()
        {
            return sizes_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fileFormats
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
