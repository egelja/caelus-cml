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
    CML::fileFormats::SMESHsurfaceFormat

Description
    Provide a means of writing tetgen SMESH format.

    Tetgen http://tetgen.berlios.de

See Also
    File format information:
    http://tetgen.berlios.de/fformats.smesh.html


\*---------------------------------------------------------------------------*/

#ifndef SMESHsurfaceFormat_H
#define SMESHsurfaceFormat_H

#include "MeshedSurface.hpp"
#include "MeshedSurfaceProxy.hpp"
#include "UnsortedMeshedSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                     Class SMESHsurfaceFormat Declaration
\*---------------------------------------------------------------------------*/

template<class Face>
class SMESHsurfaceFormat
:
    public MeshedSurface<Face>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        SMESHsurfaceFormat(const SMESHsurfaceFormat<Face>&);

        //- Disallow default bitwise assignment
        void operator=(const SMESHsurfaceFormat<Face>&);


public:

    // Constructors

        //- Construct null
        SMESHsurfaceFormat();


    //- Destructor
    virtual ~SMESHsurfaceFormat()
    {}


    // Member Functions

        //- Write surface mesh components by proxy
        static void write(const fileName&, const MeshedSurfaceProxy<Face>&);

        //- Write object
        virtual void write(const fileName& name) const
        {
            write(name, MeshedSurfaceProxy<Face>(*this));
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fileFormats
} // End namespace CML

#include "clock.hpp"
#include "IFstream.hpp"
#include "OFstream.hpp"
#include "Ostream.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Face>
CML::fileFormats::SMESHsurfaceFormat<Face>::SMESHsurfaceFormat()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
void CML::fileFormats::SMESHsurfaceFormat<Face>::write
(
    const fileName& filename,
    const MeshedSurfaceProxy<Face>& surf
)
{
    const pointField& pointLst = surf.points();
    const List<Face>&  faceLst = surf.faces();
    const List<label>& faceMap = surf.faceMap();

    const List<surfZone>& zones =
    (
        surf.surfZones().empty()
      ? surfaceFormatsCore::oneZone(faceLst)
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


    // Write header
    os  << "# tetgen .smesh file written " << clock::dateTime().c_str() << nl
        << "# <points count=\"" << pointLst.size() << "\">" << nl
        << pointLst.size() << " 3" << nl;    // 3: dimensions

    // Write vertex coords
    forAll(pointLst, ptI)
    {
        const point& pt = pointLst[ptI];

        os  << ptI << ' ' << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
    }
    os  << "# </points>" << nl
        << nl
        << "# <faces count=\"" << faceLst.size() << "\">" << endl;

    os  << faceLst.size() << " 1" << endl;   // one attribute: zone number


    label faceIndex = 0;
    forAll(zones, zoneI)
    {
        const surfZone& zone = zones[zoneI];

        if (useFaceMap)
        {
            forAll(zone, localFaceI)
            {
                const Face& f = faceLst[faceMap[faceIndex++]];

                os << f.size();
                forAll(f, fp)
                {
                    os << ' ' << f[fp];
                }
                os << ' ' << zoneI << endl;
            }
        }
        else
        {
            forAll(zones[zoneI], localFaceI)
            {
                const Face& f = faceLst[faceIndex++];

                os << f.size();
                forAll(f, fp)
                {
                    os << ' ' << f[fp];
                }
                os << ' ' << zoneI << endl;
            }
        }
    }

    // write tail

    os  << "# </faces>" << nl
        << nl
        << "# no holes or regions:" << nl
        << '0' << nl        // holes
        << '0' << endl;     // regions
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
