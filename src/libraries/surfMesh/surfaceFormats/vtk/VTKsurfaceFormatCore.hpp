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
    CML::fileFormats::VTKsurfaceFormatCore

Description
    Internal class used by the VTKsurfaceFormat

SourceFiles
    VTKsurfaceFormatCore.cpp

\*---------------------------------------------------------------------------*/

#ifndef VTKsurfaceFormatCore_H
#define VTKsurfaceFormatCore_H

#include "Ostream.hpp"
#include "OFstream.hpp"
#include "MeshedSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                    Class VTKsurfaceFormatCore Declaration
\*---------------------------------------------------------------------------*/

class VTKsurfaceFormatCore
{
protected:

    // Protected Member Functions

        //- Write header information with points
        static void writeHeader
        (
            Ostream&,
            const pointField&
        );

        //- Write trailing information with zone information
        static void writeTail(Ostream&, const UList<surfZone>&);

        //- Write trailing information with zone Ids
        static void writeTail(Ostream&, const labelUList& zoneIds);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fileFormats
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
