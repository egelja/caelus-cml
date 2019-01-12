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
    CML::fileFormats::OFFsurfaceFormat

Description
    Provide a means of reading/writing Geomview OFF polyList format.


See Also
    The <a href="http://www.geoview.org">Geoview</a>
    file format information:
    http://www.geomview.org/docs/html/OFF.html#OFF

Note
    When reading, the optional \a colorspec is ignored.
    When writing, it is set to the zone number (integer).


\*---------------------------------------------------------------------------*/

#ifndef OFFsurfaceFormat_H
#define OFFsurfaceFormat_H

#include "MeshedSurface.hpp"
#include "MeshedSurfaceProxy.hpp"
#include "UnsortedMeshedSurface.hpp"
#include "clock.hpp"
#include "IFstream.hpp"
#include "IStringStream.hpp"
#include "Ostream.hpp"
#include "OFstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                      Class OFFsurfaceFormat Declaration
\*---------------------------------------------------------------------------*/

template<class Face>
class OFFsurfaceFormat
:
    public MeshedSurface<Face>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        OFFsurfaceFormat(const OFFsurfaceFormat&);

        //- Disallow default bitwise assignment
        void operator=(const OFFsurfaceFormat&);


public:

    // Constructors

        //- Construct from file name
        OFFsurfaceFormat(const fileName&);


    // Selectors

        //- Read file and return surface
        static autoPtr<MeshedSurface<Face> > New(const fileName& name)
        {
            return autoPtr<MeshedSurface<Face> >
            (
                new OFFsurfaceFormat(name)
            );
        }


    //- Destructor
    virtual ~OFFsurfaceFormat()
    {}


    // Member Functions

        //- Write surface mesh components by proxy
        static void write(const fileName&, const MeshedSurfaceProxy<Face>&);

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


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Face>
CML::fileFormats::OFFsurfaceFormat<Face>::OFFsurfaceFormat
(
    const fileName& filename
)
{
    read(filename);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
bool CML::fileFormats::OFFsurfaceFormat<Face>::read
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

    // Read header
    string hdr = this->getLineNoComment(is);
    if (hdr != "OFF")
    {
        FatalErrorInFunction
            << "OFF file " << filename << " does not start with 'OFF'"
            << exit(FatalError);
    }


    // get dimensions
    label nPoints, nElems, nEdges;

    string line = this->getLineNoComment(is);
    {
        IStringStream lineStream(line);
        lineStream >> nPoints >> nElems >> nEdges;
    }

    // Read points
    pointField pointLst(nPoints);
    forAll(pointLst, pointI)
    {
        scalar x, y, z;
        line = this->getLineNoComment(is);
        {
            IStringStream lineStream(line);
            lineStream >> x >> y >> z;
        }
        pointLst[pointI] = point(x, y, z);
    }

    // Read faces - ignore optional zone information
    // use a DynamicList for possible on-the-fly triangulation
    DynamicList<Face>  dynFaces(nElems);

    for (label faceI = 0; faceI < nElems; ++faceI)
    {
        line = this->getLineNoComment(is);

        {
            IStringStream lineStream(line);

            label nVerts;
            lineStream >> nVerts;

            List<label> verts(nVerts);

            forAll(verts, vertI)
            {
                lineStream >> verts[vertI];
            }

            labelUList& f = static_cast<labelUList&>(verts);

            if (mustTriangulate && f.size() > 3)
            {
                // simple face triangulation about f[0]
                // cannot use face::triangulation (points may be incomplete)
                for (label fp1 = 1; fp1 < f.size() - 1; fp1++)
                {
                    label fp2 = f.fcIndex(fp1);

                    dynFaces.append(triFace(f[0], f[fp1], f[fp2]));
                }
            }
            else
            {
                dynFaces.append(Face(f));
            }
        }
    }

    // transfer to normal lists, no zone information
    this->reset(pointLst.xfer(), dynFaces.xfer(), Xfer<surfZoneList>());

    return true;
}


template<class Face>
void CML::fileFormats::OFFsurfaceFormat<Face>::write
(
    const fileName& filename,
    const MeshedSurfaceProxy<Face>& surf
)
{
    const pointField& pointLst = surf.points();
    const List<Face>&  faceLst = surf.faces();
    const List<label>& faceMap = surf.faceMap();
    const List<surfZone>& zoneLst = surf.surfZones();

    OFstream os(filename);
    if (!os.good())
    {
        FatalErrorInFunction
            << "Cannot open file for writing " << filename
            << exit(FatalError);
    }

    // Write header
    os  << "OFF" << endl
        << "# Geomview OFF file written " << clock::dateTime().c_str() << nl
        << nl
        << "# points : " << pointLst.size() << nl
        << "# faces  : " << faceLst.size() << nl
        << "# zones  : " << zoneLst.size() << nl;

    // Print zone names as comment
    forAll(zoneLst, zoneI)
    {
        os  << "#   " << zoneI << "  " << zoneLst[zoneI].name()
            << "  (nFaces: " << zoneLst[zoneI].size() << ")" << nl;
    }

    os  << nl
        << "# nPoints  nFaces  nEdges" << nl
        << pointLst.size() << ' ' << faceLst.size() << ' ' << 0 << nl
        << nl
        << "# <points count=\"" << pointLst.size() << "\">" << endl;

    // Write vertex coords
    forAll(pointLst, ptI)
    {
        os  << pointLst[ptI].x() << ' '
            << pointLst[ptI].y() << ' '
            << pointLst[ptI].z() << " #" << ptI << endl;
    }

    os  << "# </points>" << nl
        << nl
        << "# <faces count=\"" << faceLst.size() << "\">" << endl;

    label faceIndex = 0;
    forAll(zoneLst, zoneI)
    {
        os << "# <zone name=\"" << zoneLst[zoneI].name() << "\">" << endl;

        if (surf.useFaceMap())
        {
            forAll(zoneLst[zoneI], localFaceI)
            {
                const Face& f = faceLst[faceMap[faceIndex++]];

                os << f.size();
                forAll(f, fp)
                {
                    os << ' ' << f[fp];
                }

                // add optional zone information
                os << ' ' << zoneI << endl;
            }
        }
        else
        {
            forAll(zoneLst[zoneI], localFaceI)
            {
                const Face& f = faceLst[faceIndex++];

                os << f.size();
                forAll(f, fp)
                {
                    os << ' ' << f[fp];
                }

                // add optional zone information
                os << ' ' << zoneI << endl;
            }
        }
        os << "# </zone>" << endl;
    }
    os << "# </faces>" << endl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
