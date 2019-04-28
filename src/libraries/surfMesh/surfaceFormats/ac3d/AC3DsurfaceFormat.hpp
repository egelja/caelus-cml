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
    CML::fileFormats::AC3DsurfaceFormat

Description
    Provide a means of reading/writing AC3D format.

    http://www.inivis.com/ac3d/man/ac3dfileformat.html

Note
    On input, the faces are already organized as zones.
    The output is always sorted by zones.
    In the absence of zones, a single zone will be assigned.


\*---------------------------------------------------------------------------*/

#ifndef AC3DsurfaceFormat_H
#define AC3DsurfaceFormat_H

#include "MeshedSurface.hpp"
#include "MeshedSurfaceProxy.hpp"
#include "UnsortedMeshedSurface.hpp"
#include "AC3DsurfaceFormatCore.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                      Class AC3DsurfaceFormat Declaration
\*---------------------------------------------------------------------------*/

template<class Face>
class AC3DsurfaceFormat
:
    public MeshedSurface<Face>,
    public AC3DsurfaceFormatCore
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        AC3DsurfaceFormat(const AC3DsurfaceFormat<Face>&);

        //- Disallow default bitwise assignment
        void operator=(const AC3DsurfaceFormat<Face>&);


public:

    // Constructors

        //- Construct from file name
        AC3DsurfaceFormat(const fileName&);


    // Selectors

        //- Read file and return surface
        static autoPtr<MeshedSurface<Face> > New(const fileName& name)
        {
            return autoPtr<MeshedSurface<Face> >
            (
                new AC3DsurfaceFormat<Face>(name)
            );
        }


    //- Destructor
    virtual ~AC3DsurfaceFormat()
    {}


    // Member Functions

        //- Write surface mesh components by proxy
        static void write(const fileName&, const MeshedSurfaceProxy<Face>&);

        //- Write UnsortedMeshedSurface, the output is always sorted by zones.
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

#include "clock.hpp"
#include "IStringStream.hpp"
#include "tensor.hpp"
#include "primitivePatch.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Face>
CML::fileFormats::AC3DsurfaceFormat<Face>::AC3DsurfaceFormat
(
    const fileName& filename
)
{
    read(filename);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
bool CML::fileFormats::AC3DsurfaceFormat<Face>::read
(
    const fileName& filename
)
{
    const bool mustTriangulate = this->isTri();
    this->clear();

    IFstream is(filename);
    if (!is.good())
    {
        FatalErrorInFunction
            << "Cannot read file " << filename
            << exit(FatalError);
    }

    string line, cmd, args;

    is.getLine(line);

    string version = line.substr(4);

    if (version != "b")
    {
        WarningInFunction
            << "When reading AC3D file " << filename
            << " read header " << line << " with version "
            << version << endl
            << "Only tested reading with version 'b'."
            << " This might give problems" << endl;
    }


    if (!cueTo(is, "OBJECT", args) || (args != "world"))
    {
        FatalErrorInFunction
            << "Cannot find \"OBJECT world\" in file " << filename
            << exit(FatalError);
    }

    // # of kids is the # of zones
    args = cueToOrDie(is, "kids");
    label nZones = parse<int>(args);

    // Start of vertices for object/zones
    label vertexOffset = 0;

    DynamicList<point> dynPoints;
    DynamicList<Face>  dynFaces;
    List<word>         names(nZones);
    List<label>        sizes(nZones, 0);

    for (label zoneI = 0; zoneI < nZones; ++zoneI)
    {
        names[zoneI] = word("zone") + CML::name(zoneI);

        args = cueToOrDie(is, "OBJECT", "while reading " + names[zoneI]);

        // number of vertices for this zone
        label  nZonePoints = 0;
        vector location(pTraits<vector>::zero);
        // tensor rotation(I);

        // Read all info for current zone
        while (is.good())
        {
            // Read line and get first word. If end of file break since
            // zone should always end with 'kids' command ?not sure.
            if (!readCmd(is, cmd, args))
            {
                FatalErrorInFunction
                    << "Did not read up to \"kids 0\" while reading zone "
                    << zoneI << " from file " << filename
                    << exit(FatalError);
            }

            if (cmd == "name")
            {
                // name %s
                string str = parse<string>(args);
                string::stripInvalid<word>(str);

                names[zoneI] = str;
            }
            else if (cmd == "rot")
            {
                // rot  %f %f %f  %f %f %f  %f %f %f

                // IStringStream lineStream(args);
                //
                // lineStream
                //     >> rotation.xx() >> rotation.xy() >> rotation.xz()
                //     >> rotation.yx() >> rotation.yy() >> rotation.yz()
                //     >> rotation.zx() >> rotation.zy() >> rotation.zz();

                WarningInFunction
                    << "rot (rotation tensor) command not implemented"
                    << "Line:" << cmd << ' ' << args << endl
                    << "while reading zone " << zoneI << endl;
            }
            else if (cmd == "loc")
            {
                // loc  %f %f %f
                IStringStream lineStream(args);

                lineStream
                    >> location.x()
                    >> location.y()
                    >> location.z();
            }
            else if (cmd == "numvert")
            {
                // numvert  %d
                nZonePoints = parse<int>(args);

                for (label vertI = 0; vertI < nZonePoints; ++vertI)
                {
                    is.getLine(line);
                    IStringStream lineStream(line);

                    point pt;
                    lineStream
                        >> pt.x() >> pt.y() >> pt.z();

                    // Offset with current translation vector
                    dynPoints.append(location + pt);
                }
            }
            else if (cmd == "numsurf")
            {
                label nFaces = parse<int>(args);

                for (label faceI = 0; faceI < nFaces; ++faceI)
                {
                    static string errorMsg =
                        string(" while reading face ")
                            + CML::name(faceI) + " on zone "
                            + CML::name(zoneI)
                            + " from file " + filename;

                    cueToOrDie(is, "SURF", errorMsg);
                    cueToOrDie(is, "mat", errorMsg);
                    args = cueToOrDie(is, "refs", errorMsg);

                    label nVert = parse<int>(args);

                    List<label> verts(nVert);
                    forAll(verts, vertI)
                    {
                        is.getLine(line);
                        verts[vertI] = parse<int>(line) + vertexOffset;
                    }

                    labelUList& f = static_cast<labelUList&>(verts);

                    if (mustTriangulate && f.size() > 3)
                    {
                        // simple face triangulation about f[0]
                        // points may be incomplete
                        for (label fp1 = 1; fp1 < f.size() - 1; ++fp1)
                        {
                            label fp2 = f.fcIndex(fp1);

                            dynFaces.append(triFace(f[0], f[fp1], f[fp2]));
                            sizes[zoneI]++;
                        }
                    }
                    else
                    {
                        dynFaces.append(Face(f));
                        sizes[zoneI]++;
                    }
                }

                // Done the current zone.
                // Increment the offset vertices are stored at
                vertexOffset += nZonePoints;
            }
            else if (cmd == "kids")
            {
                // 'kids' denotes the end of the current zone.
                label nKids = parse<int>(args);

                if (nKids != 0)
                {
                    FatalErrorInFunction
                        << "Can only read objects without kids."
                        << " Encountered " << nKids << " kids when"
                        << " reading zone " << zoneI
                        << exit(FatalError);
                }

                // Done reading current zone
                break;
            }
        }
    }

    // transfer to normal lists
    this->storedPoints().transfer(dynPoints);
    this->storedFaces().transfer(dynFaces);

    // add zones, culling empty ones
    this->addZones(sizes, names, true);
    this->stitchFaces(SMALL);
    return true;
}


template<class Face>
void CML::fileFormats::AC3DsurfaceFormat<Face>::write
(
    const fileName& filename,
    const MeshedSurfaceProxy<Face>& surf
)
{
    const pointField& pointLst = surf.points();
    const List<Face>&  faceLst = surf.faces();

    const List<surfZone>& zones =
    (
        surf.surfZones().size()
      ? surf.surfZones()
      : surfaceFormatsCore::oneZone(faceLst)
    );

    const bool useFaceMap = (surf.useFaceMap() && zones.size() > 1);

    if (useFaceMap)
    {
        FatalErrorInFunction
            << "output with faceMap is not supported " << filename
            << exit(FatalError);
    }


    OFstream os(filename);
    if (!os.good())
    {
        FatalErrorInFunction
            << "Cannot open file for writing " << filename
            << exit(FatalError);
    }

    writeHeader(os, zones);

    forAll(zones, zoneI)
    {
        const surfZone& zone = zones[zoneI];

        os  << "OBJECT poly" << nl
            << "name \"" << zone.name() << "\"\n";

        // Temporary PrimitivePatch to calculate compact points & faces
        // use 'UList' to avoid allocations!
        PrimitivePatch<Face, CML::UList, const pointField&> patch
        (
            SubList<Face>
            (
                faceLst,
                zone.size(),
                zone.start()
            ),
            pointLst
        );

        os << "numvert " << patch.nPoints() << endl;

        forAll(patch.localPoints(), ptI)
        {
            const point& pt = patch.localPoints()[ptI];

            os << pt.x() << ' ' << pt.y() << ' ' << pt.z() << nl;
        }

        os << "numsurf " << patch.localFaces().size() << endl;

        forAll(patch.localFaces(), localFaceI)
        {
            const Face& f = patch.localFaces()[localFaceI];

            os  << "SURF 0x20" << nl          // polygon
                << "mat " << zoneI << nl
                << "refs " << f.size() << nl;

            forAll(f, fp)
            {
                os << f[fp] << " 0 0" << nl;
            }
        }

        os << "kids 0" << endl;
    }
}


template<class Face>
void CML::fileFormats::AC3DsurfaceFormat<Face>::write
(
    const fileName& filename,
    const UnsortedMeshedSurface<Face>& surf
)
{
    labelList faceMap;
    List<surfZone> zoneLst = surf.sortedZones(faceMap);

    if (zoneLst.size() <= 1)
    {
        write
        (
            filename,
            MeshedSurfaceProxy<Face>
            (
                surf.points(),
                surf.faces(),
                zoneLst
            )
        );
    }
    else
    {
        OFstream os(filename);
        if (!os.good())
        {
            FatalErrorInFunction
                << "Cannot open file for writing " << filename
                << exit(FatalError);
        }

        writeHeader(os, zoneLst);

        label faceIndex = 0;
        forAll(zoneLst, zoneI)
        {
            const surfZone& zone = zoneLst[zoneI];

            os  << "OBJECT poly" << nl
                << "name \"" << zone.name() << "\"\n";

            // Create zone with only zone faces included for ease of addressing
            labelHashSet include(surf.size());

            forAll(zone, localFaceI)
            {
                const label faceI = faceMap[faceIndex++];
                include.insert(faceI);
            }

            UnsortedMeshedSurface<Face> subm = surf.subsetMesh(include);

            // Now we have isolated surface for this patch alone. Write it.
            os << "numvert " << subm.nPoints() << endl;

            forAll(subm.localPoints(), ptI)
            {
                const point& pt = subm.localPoints()[ptI];

                os << pt.x() << ' ' << pt.y() << ' ' << pt.z() << endl;
            }

            os << "numsurf " << subm.localFaces().size() << endl;

            forAll(subm.localFaces(), localFaceI)
            {
                const Face& f = subm.localFaces()[localFaceI];

                os  << "SURF 0x20" << nl          // polygon
                    << "mat " << zoneI << nl
                    << "refs " << f.size() << nl;

                forAll(f, fp)
                {
                    os << f[fp] << " 0 0" << nl;
                }
            }

            os << "kids 0" << endl;
        }
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
