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
    CML::fileFormats::VTKsurfaceFormat

Description
    Provide a means of writing VTK legacy format.
    The output is never sorted by zone.


\*---------------------------------------------------------------------------*/

#ifndef VTKsurfaceFormat_H
#define VTKsurfaceFormat_H

#include "MeshedSurface.hpp"
#include "MeshedSurfaceProxy.hpp"
#include "UnsortedMeshedSurface.hpp"
#include "VTKsurfaceFormatCore.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                     Class VTKsurfaceFormat Declaration
\*---------------------------------------------------------------------------*/

template<class Face>
class VTKsurfaceFormat
:
    public MeshedSurface<Face>,
    public VTKsurfaceFormatCore
{
    // Private Member Functions

        //- Write header information about number of polygon points
        static void writeHeaderPolygons(Ostream&, const UList<Face>&);

        //- Disallow default bitwise copy construct
        VTKsurfaceFormat(const VTKsurfaceFormat<Face>&);

        //- Disallow default bitwise assignment
        void operator=(const VTKsurfaceFormat<Face>&);


public:

    // Constructors

        //- Construct null
        VTKsurfaceFormat();


    //- Destructor
    virtual ~VTKsurfaceFormat()
    {}


    // Member Functions

        // Write

            //- Write surface mesh components by proxy
            static void write
            (
                const fileName&, const MeshedSurfaceProxy<Face>&
            );

            //- Write UnsortedMeshedSurface, the output remains unsorted
            static void write
            (
                const fileName&, const UnsortedMeshedSurface<Face>&
            );

//CAELUSFIX - not used and doesn't work
            // //- Write object
            // virtual void write(Ostream& os) const
            // {
                // write(os, MeshedSurfaceProxy<Face>(*this));
            // }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fileFormats
} // End namespace CML

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Face>
void CML::fileFormats::VTKsurfaceFormat<Face>::writeHeaderPolygons
(
    Ostream& os,
    const UList<Face>& faceLst
)
{
    label nNodes = 0;

    forAll(faceLst, faceI)
    {
        nNodes += faceLst[faceI].size();
    }

    os  << nl
        << "POLYGONS " << faceLst.size() << ' '
        << faceLst.size() + nNodes << nl;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Face>
CML::fileFormats::VTKsurfaceFormat<Face>::VTKsurfaceFormat()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
void CML::fileFormats::VTKsurfaceFormat<Face>::write
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


    writeHeader(os, pointLst);
    writeHeaderPolygons(os, faceLst);

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
                os << ' ' << nl;
            }
        }
        else
        {
            forAll(zone, localFaceI)
            {
                const Face& f = faceLst[faceIndex++];

                os << f.size();
                forAll(f, fp)
                {
                    os << ' ' << f[fp];
                }
                os << ' ' << nl;
            }
        }
    }

    writeTail(os, zones);
}


template<class Face>
void CML::fileFormats::VTKsurfaceFormat<Face>::write
(
    const fileName& filename,
    const UnsortedMeshedSurface<Face>& surf
)
{
    OFstream os(filename);
    if (!os.good())
    {
        FatalErrorInFunction
            << "Cannot open file for writing " << filename
            << exit(FatalError);
    }


    const List<Face>& faceLst = surf.faces();

    writeHeader(os, surf.points());
    writeHeaderPolygons(os, faceLst);

    forAll(faceLst, faceI)
    {
        const Face& f = faceLst[faceI];

        os << f.size();
        forAll(f, fp)
        {
            os << ' ' << f[fp];
        }
        os << ' ' << nl;
    }

    writeTail(os, surf.zoneIds());
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
