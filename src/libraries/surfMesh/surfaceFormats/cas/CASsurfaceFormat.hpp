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
    CML::fileFormats::CASsurfaceFormat

Description
    Provide a means of reading/writing the single-file CAELUS surface format.

Note
    This class provides more methods than the regular surface format interface.


\*---------------------------------------------------------------------------*/

#ifndef CASsurfaceFormat_H
#define CASsurfaceFormat_H

#include "Ostream.hpp"
#include "OFstream.hpp"
#include "MeshedSurface.hpp"
#include "MeshedSurfaceProxy.hpp"
#include "UnsortedMeshedSurface.hpp"
#include "CASsurfaceFormatCore.hpp"
#include "IFstream.hpp"
#include "IStringStream.hpp"
#include "ListOps.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fileFormats
{

/*---------------------------------------------------------------------------*\
                      Class CASsurfaceFormat Declaration
\*---------------------------------------------------------------------------*/

template<class Face>
class CASsurfaceFormat
:
    public MeshedSurface<Face>,
    public CASsurfaceFormatCore
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        CASsurfaceFormat(const CASsurfaceFormat<Face>&);

        //- Disallow default bitwise assignment
        void operator=(const CASsurfaceFormat<Face>&);


public:

    // Constructors

        //- Construct from file name
        CASsurfaceFormat(const fileName&);


    // Selectors

        //- Read file and return surface
        static autoPtr<MeshedSurface<Face> > New(const fileName& name)
        {
            return autoPtr<MeshedSurface<Face> >
            (
                new CASsurfaceFormat<Face>(name)
            );
        }


    //- Destructor
    virtual ~CASsurfaceFormat()
    {}



    // Member Functions

        //- Read surface mesh components
        static bool read
        (
            Istream&,
            pointField&,
            List<Face>&,
            List<surfZone>&
        );

        //- Read MeshedSurface
        static bool read
        (
            Istream&,
            MeshedSurface<Face>&
        );

        //- Read UnsortedMeshedSurface
        //  The output is sorted by zones
        static bool read
        (
            Istream&,
            UnsortedMeshedSurface<Face>&
        );

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
CML::fileFormats::CASsurfaceFormat<Face>::CASsurfaceFormat
(
    const fileName& filename
)
{
    read(filename);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
bool CML::fileFormats::CASsurfaceFormat<Face>::read
(
    const fileName& filename
)
{
    this->clear();

    IFstream is(filename);
    if (!is.good())
    {
        FatalErrorInFunction
            << "Cannot read file " << filename
            << exit(FatalError);
    }

    // read surfZones:
    is >> this->storedZones();

    // read points:
    is >> this->storedPoints();

    // must triangulate?
    if (MeshedSurface<Face>::isTri())
    {
        // read faces as 'face' and transcribe to 'triFace'
        List<face> faceLst(is);

        MeshedSurface<face> surf
        (
            xferMove(this->storedPoints()),
            xferMove(faceLst),
            xferMove(this->storedZones())
        );

        this->transcribe(surf);
    }
    else
    {
        // read faces directly
        is >> this->storedFaces();
    }

    return true;
}


template<class Face>
bool CML::fileFormats::CASsurfaceFormat<Face>::read
(
    Istream& is,
    pointField& pointLst,
    List<Face>& faceLst,
    List<surfZone>& zoneLst
)
{
    if (!is.good())
    {
        FatalErrorInFunction
            << "read error "
            << exit(FatalError);
    }

    // read surfZones:
    is >> zoneLst;

    // read points:
    is >> pointLst;

    // must triangulate?
    if (MeshedSurface<Face>::isTri())
    {
        // read faces as 'face' and transcribe to 'triFace'
        List<face> origFaces(is);

        MeshedSurface<face> origSurf
        (
            xferMove(pointLst),
            xferMove(origFaces),
            xferMove(zoneLst)
        );

        MeshedSurface<Face> surf;
        surf.transcribe(origSurf);
    }
    else
    {
        // read faces directly
        is >> faceLst;
    }

    return true;
}


template<class Face>
bool CML::fileFormats::CASsurfaceFormat<Face>::read
(
    Istream& is,
    MeshedSurface<Face>& surf
)
{
    surf.clear();

    if (!is.good())
    {
        FatalErrorInFunction
            << "read error "
            << exit(FatalError);
    }

    pointField pointLst;
    List<Face> faceLst;
    List<surfZone> zoneLst;

    read(is, pointLst, faceLst, zoneLst);

    surf.reset
    (
        xferMove(pointLst),
        xferMove(faceLst),
        xferMove(zoneLst)
    );

    return true;
}


template<class Face>
bool CML::fileFormats::CASsurfaceFormat<Face>::read
(
    Istream& is,
    UnsortedMeshedSurface<Face>& surf
)
{
    surf.clear();
    MeshedSurface<Face> origSurf(is);
    surf.transfer(origSurf);

    return true;
}



template<class Face>
void CML::fileFormats::CASsurfaceFormat<Face>::write
(
    const fileName& filename,
    const MeshedSurfaceProxy<Face>& surf
)
{
    const List<Face>&  faceLst = surf.faces();
    const List<label>& faceMap = surf.faceMap();

    OFstream os(filename);
    if (!os.good())
    {
        FatalErrorInFunction
            << "Cannot open file for writing " << filename
            << exit(FatalError);
    }


    CASsurfaceFormatCore::writeHeader(os, surf.points(), surf.surfZones());

    const List<surfZone>& zones = surf.surfZones();
    const bool useFaceMap = (surf.useFaceMap() && zones.size() > 1);

    if (useFaceMap)
    {
        os  << "\n// faces:"  << nl
            << faceLst.size() << token::BEGIN_LIST << nl;

        label faceI = 0;
        forAll(zones, zoneI)
        {
            // Print all faces belonging to this zone
            const surfZone& zone = zones[zoneI];

            forAll(zone, localFaceI)
            {
                os << faceLst[faceMap[faceI++]] << nl;
            }
        }
        os << token::END_LIST << nl;
    }
    else
    {
        os  << "\n// faces:"  << nl << faceLst << nl;
    }

    IOobject::writeDivider(os);

    // Check state of Ostream
    os.check("CASsurfaceFormat<Face>::write(Ostream&)");
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
