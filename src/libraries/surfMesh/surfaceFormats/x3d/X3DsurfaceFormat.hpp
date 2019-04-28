/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::fileFormats::X3DsurfaceFormat

Description
    Provide a means of writing x3d format.


\*---------------------------------------------------------------------------*/

#ifndef X3DsurfaceFormat_H
#define X3DsurfaceFormat_H

#include "MeshedSurface.hpp"
#include "MeshedSurfaceProxy.hpp"
#include "UnsortedMeshedSurface.hpp"
#include "X3DsurfaceFormatCore.hpp"
#include "clock.hpp"
#include "IFstream.hpp"
#include "IStringStream.hpp"
#include "Ostream.hpp"
#include "OFstream.hpp"
#include "ListOps.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                      Class X3DsurfaceFormat Declaration
\*---------------------------------------------------------------------------*/

template<class Face>
class X3DsurfaceFormat
:
    public MeshedSurface<Face>,
    public X3DsurfaceFormatCore
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        X3DsurfaceFormat(const X3DsurfaceFormat<Face>&);

        //- Disallow default bitwise assignment
        void operator=(const X3DsurfaceFormat<Face>&);

public:

    // Constructors

        //- Construct null
        X3DsurfaceFormat();


    //- Destructor
    virtual ~X3DsurfaceFormat()
    {}


    // Member Functions

        //- Write surface mesh components by proxy
        static void write(const fileName&, const MeshedSurfaceProxy<Face>&);

        //- Write object file
        virtual void write(const fileName& name) const
        {
            write(name, MeshedSurfaceProxy<Face>(*this));
        }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fileFormats
} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Face>
CML::fileFormats::X3DsurfaceFormat<Face>::X3DsurfaceFormat()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
void CML::fileFormats::X3DsurfaceFormat<Face>::write
(
    const fileName& filename,
    const MeshedSurfaceProxy<Face>& surf
)
{
    const pointField& pointLst = surf.points();
    const List<Face>&  faceLst = surf.faces();
    const List<label>& faceMap = surf.faceMap();

    // for no zones, suppress the group name
    const List<surfZone>& zones =
    (
        surf.surfZones().empty()
      ? surfaceFormatsCore::oneZone(faceLst, "")
      : surf.surfZones()
    );

    const bool useFaceMap = (surf.useFaceMap() && zones.size() > 1);

    OFstream os(filename);
    if (!os.good())
    {
        FatalErrorInFunction
            << "Cannot open file for writing " << filename
            << exit(FatalError);
    }

    writeHeader(os);

    os  << "\n"
        "<Group>\n"
        " <Shape>\n";

    writeAppearance(os);


    // NOTE: we could provide an optimized IndexedTriangleSet output for
    // triangulated surfaces too

    os  <<
        "  <IndexedFaceSet coordIndex='\n";

    label faceIndex = 0;
    forAll(zones, zoneI)
    {
        const surfZone& zone = zones[zoneI];

        if (useFaceMap)
        {
            forAll(zone, localFaceI)
            {
                const Face& f = faceLst[faceMap[faceIndex++]];

                forAll(f, fp)
                {
                    os << f[fp] << ' ';
                }
                os << "-1\n";
            }
        }
        else
        {
            forAll(zone, localFaceI)
            {
                const Face& f = faceLst[faceIndex++];

                forAll(f, fp)
                {
                    os << f[fp] << ' ';
                }
                os << "-1\n";
            }
        }
    }

    os <<
        "' >\n"
        "    <Coordinate point='\n";

    // Write vertex coords
    forAll(pointLst, ptI)
    {
        const point& pt = pointLst[ptI];

        os  << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
    }

    os  <<
        "' />\n"                       // end Coordinate
        "   </IndexedFaceSet>\n"
        "  </Shape>\n"
        " </Group>\n"
        "</X3D>\n";

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
