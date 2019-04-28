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
    CML::fileFormats::TRIsurfaceFormat

Description
    Provide a means of reading/writing .tri format.

Note
    For efficiency, the zones are sorted before creating the faces.
    The class is thus derived from MeshedSurface.


\*---------------------------------------------------------------------------*/

#ifndef TRIsurfaceFormat_H
#define TRIsurfaceFormat_H

#include "TRIsurfaceFormatCore.hpp"
#include "MeshedSurface.hpp"
#include "MeshedSurfaceProxy.hpp"
#include "UnsortedMeshedSurface.hpp"
#include "ListOps.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                      Class TRIsurfaceFormat Declaration
\*---------------------------------------------------------------------------*/

template<class Face>
class TRIsurfaceFormat
:
    public MeshedSurface<Face>
{
    // Private Member Functions

        static inline void writeShell
        (
            Ostream&,
            const pointField&,
            const Face&,
            const label zoneI
        );

        //- Disallow default bitwise copy construct
        TRIsurfaceFormat(const TRIsurfaceFormat<Face>&);

        //- Disallow default bitwise assignment
        void operator=(const TRIsurfaceFormat<Face>&);


public:

    // Constructors

        //- Construct from file name
        TRIsurfaceFormat(const fileName&);


    // Selectors

        //- Read file and return surface
        static autoPtr<MeshedSurface<Face> > New(const fileName& name)
        {
            return autoPtr<MeshedSurface<Face> >
            (
                new TRIsurfaceFormat<Face>(name)
            );
        }


    //- Destructor
    virtual ~TRIsurfaceFormat()
    {}


    // Member Functions

        //- Write surface mesh components by proxy
        static void write(const fileName&, const MeshedSurfaceProxy<Face>&);

        //- Write UnsortedMeshedSurface,
        //  by default the output is not sorted by zones
        static void write(const fileName&, const UnsortedMeshedSurface<Face>&);

        //- Read from file
        virtual bool read(const fileName&);

        //- Write object
        virtual void write(const fileName& name) const
        {
            write(name, MeshedSurfaceProxy<Face>(*this));
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fileFormats
} // End namespace CML


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Face>
inline void CML::fileFormats::TRIsurfaceFormat<Face>::writeShell
(
    Ostream& os,
    const pointField& pointLst,
    const Face& f,
    const label zoneI
)
{
    // simple triangulation about f[0].
    // better triangulation should have been done before
    const point& p0 = pointLst[f[0]];
    for (label fp1 = 1; fp1 < f.size() - 1; ++fp1)
    {
        label fp2 = f.fcIndex(fp1);

        const point& p1 = pointLst[f[fp1]];
        const point& p2 = pointLst[f[fp2]];

        os  << p0.x() << ' ' << p0.y() << ' ' << p0.z() << ' '
            << p1.x() << ' ' << p1.y() << ' ' << p1.z() << ' '
            << p2.x() << ' ' << p2.y() << ' ' << p2.z() << ' '
            // zone as colour
            << "0x" << hex << zoneI << dec << endl;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Face>
CML::fileFormats::TRIsurfaceFormat<Face>::TRIsurfaceFormat
(
    const fileName& filename
)
{
    read(filename);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
bool CML::fileFormats::TRIsurfaceFormat<Face>::read
(
    const fileName& filename
)
{
    this->clear();

    // read in the values
    TRIsurfaceFormatCore reader(filename);

    // transfer points
    this->storedPoints().transfer(reader.points());

    // retrieve the original zone information
    List<label> sizes(reader.sizes().xfer());
    List<label> zoneIds(reader.zoneIds().xfer());

    // generate the (sorted) faces
    List<Face> faceLst(zoneIds.size());

    if (reader.sorted())
    {
        // already sorted - generate directly
        forAll(faceLst, faceI)
        {
            const label startPt = 3*faceI;
            faceLst[faceI] = triFace(startPt, startPt+1, startPt+2);
        }
    }
    else
    {
        // unsorted - determine the sorted order:
        // avoid SortableList since we discard the main list anyhow
        List<label> faceMap;
        sortedOrder(zoneIds, faceMap);

        // generate sorted faces
        forAll(faceMap, faceI)
        {
            const label startPt = 3*faceMap[faceI];
            faceLst[faceI] = triFace(startPt, startPt+1, startPt+2);
        }
    }
    zoneIds.clear();

    // transfer:
    this->storedFaces().transfer(faceLst);

    this->addZones(sizes);
    this->stitchFaces(SMALL);
    return true;
}


template<class Face>
void CML::fileFormats::TRIsurfaceFormat<Face>::write
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

    label faceIndex = 0;
    forAll(zones, zoneI)
    {
        const surfZone& zone = zones[zoneI];

        if (useFaceMap)
        {
            forAll(zone, localFaceI)
            {
                const Face& f = faceLst[faceMap[faceIndex++]];
                writeShell(os, pointLst, f, zoneI);
            }
        }
        else
        {
            forAll(zone, localFaceI)
            {
                const Face& f = faceLst[faceIndex++];
                writeShell(os, pointLst, f, zoneI);
            }
        }
    }
}


template<class Face>
void CML::fileFormats::TRIsurfaceFormat<Face>::write
(
    const fileName& filename,
    const UnsortedMeshedSurface<Face>& surf
)
{
    const pointField& pointLst = surf.points();
    const List<Face>& faceLst  = surf.faces();

    OFstream os(filename);
    if (!os.good())
    {
        FatalErrorInFunction
            << "Cannot open file for writing " << filename
            << exit(FatalError);
    }


    // a single zone needs no sorting
    if (surf.zoneToc().size() == 1)
    {
        const List<label>& zoneIds  = surf.zoneIds();

        forAll(faceLst, faceI)
        {
            writeShell(os, pointLst, faceLst[faceI], zoneIds[faceI]);
        }
    }
    else
    {
        labelList faceMap;
        List<surfZone> zoneLst = surf.sortedZones(faceMap);

        label faceIndex = 0;
        forAll(zoneLst, zoneI)
        {
            forAll(zoneLst[zoneI], localFaceI)
            {
                const Face& f = faceLst[faceMap[faceIndex++]];
                writeShell(os, pointLst, f, zoneI);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
