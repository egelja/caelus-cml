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
    CML::fileFormats::STARCDsurfaceFormatCore

Description
    Internal class used by the STARCDsurfaceFormat

SourceFiles
    STARCDsurfaceFormatCore.cpp

\*---------------------------------------------------------------------------*/

#ifndef STARCDsurfaceFormatCore_H
#define STARCDsurfaceFormatCore_H

#include "IFstream.hpp"
#include "Ostream.hpp"
#include "OFstream.hpp"
#include "MeshedSurface.hpp"
#include "STARCDCore.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                   Class STARCDsurfaceFormatCore Declaration
\*---------------------------------------------------------------------------*/

class STARCDsurfaceFormatCore
:
    public STARCDCore
{
protected:

    // Protected Member Functions

    static Map<word> readInpCellTable(IFstream&);

    static void writeCase
    (
        Ostream&,
        const pointField&,
        const label nFaces,
        const UList<surfZone>&
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fileFormats
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
