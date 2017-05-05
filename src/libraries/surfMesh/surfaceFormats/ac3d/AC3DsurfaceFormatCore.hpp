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
    CML::fileFormats::AC3DsurfaceFormatCore

Description
    Internal class used by the AC3DsurfaceFormat

SourceFiles
    AC3DsurfaceFormatCore.cpp

\*---------------------------------------------------------------------------*/

#ifndef AC3DsurfaceFormatCore_H
#define AC3DsurfaceFormatCore_H

#include "IFstream.hpp"
#include "Ostream.hpp"
#include "OFstream.hpp"
#include "MeshedSurface.hpp"
#include "IStringStream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                    Class AC3DsurfaceFormatCore Declaration
\*---------------------------------------------------------------------------*/

class AC3DsurfaceFormatCore
{
protected:

    // Protected Member Functions

        //- Read a type via IStringStream
        template<class Type>
        static Type parse(const string&);

        //- Read cmd, args from IFstream
        static bool readCmd(IFstream&, string& cmd, string& args);

        //- Cue up to cmd, reading args
        static bool cueTo(IFstream&, const string& cmd, string& args);

        //- Cue up to cmd, reading args or exit with a FatalError
        //  returns the command args
        static string cueToOrDie
        (
            IFstream&,
            const string& cmd,
            const string& errorMsg=string::null
        );

        //- Write header with materials
        static void writeHeader(Ostream&, const UList<surfZone>&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fileFormats
} // End namespace CML


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
Type CML::fileFormats::AC3DsurfaceFormatCore::parse(const string& s)
{
    IStringStream ss(s);

    Type t;
    ss >> t;
    return t;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
