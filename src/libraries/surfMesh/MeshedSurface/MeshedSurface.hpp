/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
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
    CML::MeshedSurface

Description
    A surface geometry mesh with zone information, not to be confused with
    the similarly named surfaceMesh, which actually refers to the cell faces
    of a volume mesh.

    A MeshedSurface can have zero or more surface zones (roughly equivalent
    to faceZones for a polyMesh). If surface zones are defined, they must
    be contiguous and cover all of the faces.

    The MeshedSurface is intended for surfaces from a variety of sources.
    - A set of points and faces without any surface zone information.
    - A set of points and faces with randomly ordered zone information.
      This could arise, for example, from reading external file formats
      such as STL, etc.


\*---------------------------------------------------------------------------*/

#ifndef MeshedSurface_H
#define MeshedSurface_H

#include "PrimitivePatch_.hpp"
#include "PatchTools.hpp"
#include "pointField.hpp"
#include "face.hpp"
#include "triFace.hpp"

#include "surfZoneList.hpp"
#include "surfaceFormatsCore.hpp"
#include "runTimeSelectionTables.hpp"
#include "memberFunctionSelectionTables.hpp"
#include "HashSet.hpp"
#include "boundBox.hpp"
#include "Ostream.hpp"
#include "UnsortedMeshedSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class Time;
class surfMesh;
class polyBoundaryMesh;

template<class Face> class MeshedSurface;
template<class Face> class MeshedSurfaceProxy;
template<class Face> class UnsortedMeshedSurface;

/*---------------------------------------------------------------------------*\
                      Class MeshedSurface Declaration
\*---------------------------------------------------------------------------*/

template<class Face>
class MeshedSurface
:
    public PrimitivePatch<Face, ::CML::List, pointField, point>,
    public fileFormats::surfaceFormatsCore
{
    // friends - despite different face representationsx
    template<class Face2> friend class MeshedSurface;
    template<class Face2> friend class UnsortedMeshedSurface;
    friend class surfMesh;


private:

    // Private typedefs for convenience

        typedef PrimitivePatch
        <
            Face,
            ::CML::List,
            pointField,
            point
        >
        ParentType;

        typedef UnsortedMeshedSurface<Face>  FriendType;
        typedef MeshedSurfaceProxy<Face>     ProxyType;


    // Private Member Data

        //- Zone information
        // (face ordering nFaces/startFace only used during reading/writing)
        List<surfZone> zones_;


protected:

    // Protected Member functions

        //- Transfer points/zones and transcribe face -> triFace
        void transcribe(MeshedSurface<face>&);

        //- basic sanity check on zones
        void checkZones();

        //- Non-const access to global points
        pointField& storedPoints()
        {
            return const_cast<pointField&>(ParentType::points());
        }

        //- Non-const access to the faces
        List<Face>& storedFaces()
        {
            return static_cast<List<Face> &>(*this);
        }

        //- Non-const access to the zones
        surfZoneList& storedZones()
        {
            return zones_;
        }

        //- sort faces by zones and store sorted faces
        void sortFacesAndStore
        (
            const Xfer<List<Face> >& unsortedFaces,
            const Xfer<List<label> >& zoneIds,
            const bool sorted
        );

        //- Set new zones from faceMap
        virtual void remapFaces(const labelUList& faceMap);


public:

    // Public typedefs

        //- Face type used
        typedef Face FaceType;

        //- Runtime type information
        ClassName("MeshedSurface");


    // Static

        //- Face storage only handles triangulated faces
        inline static bool isTri();

        //- Can we read this file format?
        static bool canRead(const fileName&, const bool verbose=false);

        //- Can we read this file format?
        static bool canReadType(const word& ext, const bool verbose=false);

        //- Can we write this file format?
        static bool canWriteType(const word& ext, const bool verbose=false);

        static wordHashSet readTypes();
        static wordHashSet writeTypes();


    // Constructors

        //- Construct null
        MeshedSurface();

        //- Construct by transferring components (points, faces, zones).
        MeshedSurface
        (
            const Xfer<pointField>&,
            const Xfer<List<Face> >&,
            const Xfer<surfZoneList>&
        );

        //- Construct by transferring components (points, faces).
        //  Use zone information if available
        MeshedSurface
        (
            const Xfer<pointField>&,
            const Xfer<List<Face> >&,
            const labelUList& zoneSizes = labelUList(),
            const UList<word>& zoneNames = UList<word>()
        );

        //- Construct as copy
        MeshedSurface(const MeshedSurface&);

        //- Construct from a UnsortedMeshedSurface
        MeshedSurface(const UnsortedMeshedSurface<Face>&);

        //- Construct from a boundary mesh with local points/faces
        MeshedSurface
        (
            const polyBoundaryMesh&,
            const bool globalPoints=false
        );

        //- Construct from a surfMesh
        MeshedSurface(const surfMesh&);

        //- Construct by transferring the contents from a UnsortedMeshedSurface
        MeshedSurface(const Xfer<UnsortedMeshedSurface<Face> >&);

        //- Construct by transferring the contents from a MeshedSurface
        MeshedSurface(const Xfer<MeshedSurface<Face> >&);

        //- Construct from file name (uses extension to determine type)
        MeshedSurface(const fileName&);

        //- Construct from file name (uses extension to determine type)
        MeshedSurface(const fileName&, const word& ext);

        //- Construct from database
        MeshedSurface(const Time&, const word& surfName="");


    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            MeshedSurface,
            fileExtension,
            (
                const fileName& name
            ),
            (name)
        );


    // Selectors

        //- Select constructed from filename (explicit extension)
        static autoPtr<MeshedSurface> New
        (
            const fileName&,
            const word& ext
        );

        //- Select constructed from filename (implicit extension)
        static autoPtr<MeshedSurface> New(const fileName&);


    //- Destructor
    virtual ~MeshedSurface();


    // Member Function Selectors

        declareMemberFunctionSelectionTable
        (
            void,
            UnsortedMeshedSurface,
            write,
            fileExtension,
            (
                const fileName& name,
                const MeshedSurface<Face>& surf
            ),
            (name, surf)
        );

        //- Write to file
        static void write(const fileName&, const MeshedSurface<Face>&);


    // Member Functions

        // Access

            //- The surface size is the number of faces
            label size() const
            {
                return ParentType::size();
            }

            //- Return const access to the faces
            inline const List<Face>& faces() const
            {
                return static_cast<const List<Face> &>(*this);
            }

            //- Const access to the surface zones.
            //  If zones are defined, they must be contiguous and cover the
            //  entire surface
            const List<surfZone>& surfZones() const
            {
                return zones_;
            }

            //- Add surface zones
            virtual void addZones
            (
                const UList<surfZone>&,
                const bool cullEmpty=false
            );

            //- Add surface zones
            virtual void addZones
            (
                const labelUList& sizes,
                const UList<word>& names,
                const bool cullEmpty=false
            );

            //- Add surface zones
            virtual void addZones
            (
                const labelUList& sizes,
                const bool cullEmpty=false
            );

            //- Remove surface zones
            virtual void removeZones();


        // Edit

            //- Clear all storage
            virtual void clear();

            //- Move points
            virtual void movePoints(const pointField&);

            //- Scale points. A non-positive factor is ignored
            virtual void scalePoints(const scalar);

            //- Reset primitive data (points, faces and zones)
            //  Note, optimized to avoid overwriting data (with Xfer::null)
            virtual void reset
            (
                const Xfer<pointField >& points,
                const Xfer<List<Face> >& faces,
                const Xfer<surfZoneList>& zones
            );

            //- Reset primitive data (points, faces and zones)
            //  Note, optimized to avoid overwriting data (with Xfer::null)
            virtual void reset
            (
                const Xfer<List<point> >& points,
                const Xfer<List<Face> >& faces,
                const Xfer<surfZoneList >& zones
            );

            //- Remove invalid faces
            virtual void cleanup(const bool verbose);

            virtual bool stitchFaces
            (
                const scalar tol=SMALL,
                const bool verbose=false
            );

            virtual bool checkFaces
            (
                const bool verbose=false
            );

            //- Triangulate in-place, returning the number of triangles added
            virtual label triangulate();

            //- Triangulate in-place, returning the number of triangles added
            //  and setting a map of original face Ids.
            //  The faceMap is zero-sized when no triangulation was done.
            virtual label triangulate(List<label>& faceMap);


            //- Return new surface.
            //  Returns return pointMap, faceMap from subsetMeshMap
            MeshedSurface subsetMesh
            (
                const labelHashSet& include,
                labelList& pointMap,
                labelList& faceMap
            ) const;

            //- Return new surface.
            MeshedSurface subsetMesh
            (
                const labelHashSet& include
            ) const;

            //- Transfer the contents of the argument and annul the argument
            void transfer(MeshedSurface<Face>&);

            //- Transfer the contents of the argument and annul the argument
            void transfer(UnsortedMeshedSurface<Face>&);

            //- Transfer contents to the Xfer container
            Xfer<MeshedSurface<Face> > xfer();


        // Read

            //- Read from file. Chooses reader based on explicit extension
            bool read(const fileName&, const word& ext);

            //- Read from file. Chooses reader based on detected extension
            virtual bool read(const fileName&);


        // Write

            void writeStats(Ostream& os) const;

            //- Generic write routine. Chooses writer based on extension.
            virtual void write(const fileName& name) const
            {
                write(name, *this);
            }

            //- Write to database
            void write(const Time&, const word& surfName="") const;


    // Member operators

        void operator=(const MeshedSurface<Face>&);

        //- Conversion operator to MeshedSurfaceProxy
        operator MeshedSurfaceProxy<Face>() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Specialization for holding triangulated information
template<>
inline bool MeshedSurface<triFace>::isTri()
{
    return true;
}


//- Specialization for holding triangulated information
template<>
inline label MeshedSurface<triFace>::triangulate()
{
    return 0;
}


//- Specialization for holding triangulated information
template<>
inline label MeshedSurface<triFace>::triangulate(List<label>& faceMap)
{
    if (notNull(faceMap))
    {
        faceMap.clear();
    }

    return 0;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "UnsortedMeshedSurface.hpp"
#include "MeshedSurfaceProxy.hpp"
#include "mergePoints.hpp"
#include "Time.hpp"
#include "ListOps.hpp"
#include "polyBoundaryMesh.hpp"
#include "polyMesh.hpp"
#include "surfMesh.hpp"
#include "primitivePatch.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class Face>
inline bool CML::MeshedSurface<Face>::isTri()
{
    return false;
}


template<class Face>
CML::wordHashSet CML::MeshedSurface<Face>::readTypes()
{
    return wordHashSet(*fileExtensionConstructorTablePtr_);
}


template<class Face>
CML::wordHashSet CML::MeshedSurface<Face>::writeTypes()
{
    return wordHashSet(*writefileExtensionMemberFunctionTablePtr_);
}


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

template<class Face>
bool CML::MeshedSurface<Face>::canReadType
(
    const word& ext,
    const bool verbose
)
{
    return fileFormats::surfaceFormatsCore::checkSupport
    (
        readTypes() | FriendType::readTypes(),
        ext,
        verbose,
        "reading"
   );
}


template<class Face>
bool CML::MeshedSurface<Face>::canWriteType
(
    const word& ext,
    const bool verbose
)
{
    return fileFormats::surfaceFormatsCore::checkSupport
    (
        writeTypes() | ProxyType::writeTypes(),
        ext,
        verbose,
        "writing"
    );
}


template<class Face>
bool CML::MeshedSurface<Face>::canRead
(
    const fileName& name,
    const bool verbose
)
{
    word ext = name.ext();
    if (ext == "gz")
    {
        ext = name.lessExt().ext();
    }
    return canReadType(ext, verbose);
}


template<class Face>
void CML::MeshedSurface<Face>::write
(
    const fileName& name,
    const MeshedSurface<Face>& surf
)
{
    if (debug)
    {
        Info<< "MeshedSurface::write"
            "(const fileName&, const MeshedSurface&) : "
            "writing to " << name
            << endl;
    }

    const word ext = name.ext();

    typename writefileExtensionMemberFunctionTable::iterator mfIter =
        writefileExtensionMemberFunctionTablePtr_->find(ext);

    if (mfIter == writefileExtensionMemberFunctionTablePtr_->end())
    {
        // no direct writer, delegate to proxy if possible
        wordHashSet supported = ProxyType::writeTypes();

        if (supported.found(ext))
        {
            MeshedSurfaceProxy<Face>(surf).write(name);
        }
        else
        {
            FatalErrorInFunction
                << "Unknown file extension " << ext << nl << nl
                << "Valid types are :" << endl
                << (supported | writeTypes())
                << exit(FatalError);
        }
    }
    else
    {
        mfIter()(name, surf);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Face>
CML::MeshedSurface<Face>::MeshedSurface()
:
    ParentType(List<Face>(), pointField())
{}


template<class Face>
CML::MeshedSurface<Face>::MeshedSurface
(
    const Xfer<pointField>& pointLst,
    const Xfer<List<Face> >& faceLst,
    const Xfer<surfZoneList>& zoneLst
)
:
    ParentType(List<Face>(), pointField()),
    zones_()
{
    reset(pointLst, faceLst, zoneLst);
}


template<class Face>
CML::MeshedSurface<Face>::MeshedSurface
(
    const Xfer<pointField>& pointLst,
    const Xfer<List<Face> >& faceLst,
    const labelUList& zoneSizes,
    const UList<word>& zoneNames
)
:
    ParentType(List<Face>(), pointField())
{
    reset(pointLst, faceLst, Xfer<surfZoneList>());

    if (zoneSizes.size())
    {
        if (zoneNames.size())
        {
            addZones(zoneSizes, zoneNames);
        }
        else
        {
            addZones(zoneSizes);
        }
    }
}


template<class Face>
CML::MeshedSurface<Face>::MeshedSurface
(
    const MeshedSurface<Face>& surf
)
:
    ParentType(surf.faces(), surf.points()),
    zones_(surf.surfZones())
{}


template<class Face>
CML::MeshedSurface<Face>::MeshedSurface
(
    const UnsortedMeshedSurface<Face>& surf
)
:
    ParentType(List<Face>(), surf.points())
{
    labelList faceMap;
    this->storedZones().transfer(surf.sortedZones(faceMap));

    const List<Face>& origFaces = surf.faces();
    List<Face> newFaces(origFaces.size());

    // this is somewhat like ListOps reorder and/or IndirectList
    forAll(newFaces, faceI)
    {
        newFaces[faceI] = origFaces[faceMap[faceI]];
    }

    this->storedFaces().transfer(newFaces);
}


template<class Face>
CML::MeshedSurface<Face>::MeshedSurface(const surfMesh& mesh)
:
    ParentType(List<Face>(), pointField())
{
    // same face type as surfMesh
    MeshedSurface<face> surf
    (
        xferCopy(mesh.points()),
        xferCopy(mesh.faces()),
        xferCopy(mesh.surfZones())
    );

    this->transcribe(surf);
}


template<class Face>
CML::MeshedSurface<Face>::MeshedSurface
(
    const polyBoundaryMesh& bMesh,
    const bool useGlobalPoints
)
:
    ParentType(List<Face>(), pointField())
{
    const polyMesh& mesh = bMesh.mesh();
    const polyPatchList& bPatches = bMesh;

    // Get a single patch for all boundaries
    primitivePatch allBoundary
    (
        SubList<face>
        (
            mesh.faces(),
            mesh.nFaces() - mesh.nInternalFaces(),
            mesh.nInternalFaces()
        ),
        mesh.points()
    );

    // use global/local points:
    const pointField& bPoints =
    (
        useGlobalPoints ? mesh.points() : allBoundary.localPoints()
    );

    // global/local face addressing:
    const List<Face>& bFaces =
    (
        useGlobalPoints ? allBoundary : allBoundary.localFaces()
    );


    // create zone list
    surfZoneList newZones(bPatches.size());

    label startFaceI = 0;
    label nZone = 0;
    forAll(bPatches, patchI)
    {
        const polyPatch& p = bPatches[patchI];

        if (p.size())
        {
            newZones[nZone] = surfZone
            (
                p.name(),
                p.size(),
                startFaceI,
                nZone
            );

            nZone++;
            startFaceI += p.size();
        }
    }

    newZones.setSize(nZone);

    // same face type as the polyBoundaryMesh
    MeshedSurface<face> surf
    (
        xferCopy(bPoints),
        xferCopy(bFaces),
        xferMove(newZones)
    );

    this->transcribe(surf);
}


template<class Face>
CML::MeshedSurface<Face>::MeshedSurface
(
    const fileName& name,
    const word& ext
)
:
    ParentType(List<Face>(), pointField())
{
    read(name, ext);
}


template<class Face>
CML::MeshedSurface<Face>::MeshedSurface(const fileName& name)
:
    ParentType(List<Face>(), pointField())
{
    read(name);
}


template<class Face>
CML::MeshedSurface<Face>::MeshedSurface
(
    const Time& t,
    const word& surfName
)
:
    ParentType(List<Face>(), pointField())
{
    surfMesh mesh
    (
        IOobject
        (
            "dummyName",
            t.timeName(),
            t,
            IOobject::MUST_READ_IF_MODIFIED,
            IOobject::NO_WRITE,
            false
        ),
        surfName
    );

    // same face type as surfMesh
    MeshedSurface<face> surf
    (
        xferMove(mesh.storedPoints()),
        xferMove(mesh.storedFaces()),
        xferMove(mesh.storedZones())
    );

    this->transcribe(surf);
}


template<class Face>
CML::MeshedSurface<Face>::MeshedSurface
(
    const Xfer<UnsortedMeshedSurface<Face> >& surf
)
:
    ParentType(List<Face>(), pointField())
{
    transfer(surf());
}


template<class Face>
CML::MeshedSurface<Face>::MeshedSurface
(
    const Xfer<MeshedSurface<Face> >& surf
)
:
    ParentType(List<Face>(), pointField())
{
    transfer(surf());
}



// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Face>
CML::MeshedSurface<Face>::~MeshedSurface()
{}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class Face>
void CML::MeshedSurface<Face>::remapFaces
(
    const labelUList& faceMap
)
{
    // recalculate the zone start/size
    if (notNull(faceMap) && faceMap.size())
    {
        surfZoneList& zones = storedZones();

        if (zones.size() == 1)
        {
            // optimized for single zone case
            zones[0].size() = faceMap.size();
        }
        else if (zones.size())
        {
            label newFaceI = 0;
            label origEndI = 0;
            forAll(zones, zoneI)
            {
                surfZone& zone = zones[zoneI];

                // adjust zone start
                zone.start() = newFaceI;
                origEndI += zone.size();

                for (label faceI = newFaceI; faceI < faceMap.size(); ++faceI)
                {
                    if (faceMap[faceI] < origEndI)
                    {
                        ++newFaceI;
                    }
                    else
                    {
                        break;
                    }
                }

                // adjust zone size
                zone.size() = newFaceI - zone.start();
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
void CML::MeshedSurface<Face>::clear()
{
    ParentType::clearOut();

    storedPoints().clear();
    storedFaces().clear();
    storedZones().clear();
}


template<class Face>
void CML::MeshedSurface<Face>::movePoints(const pointField& newPoints)
{
    // Adapt for new point position
    ParentType::movePoints(newPoints);

    // Copy new points
    storedPoints() = newPoints;
}


template<class Face>
void CML::MeshedSurface<Face>::scalePoints(const scalar scaleFactor)
{
    // avoid bad scaling
    if (scaleFactor > 0 && scaleFactor != 1.0)
    {
        pointField newPoints(scaleFactor*this->points());

        // Adapt for new point position
        ParentType::movePoints(newPoints);

        storedPoints() = newPoints;
    }
}


template<class Face>
void CML::MeshedSurface<Face>::reset
(
    const Xfer<pointField>& pointLst,
    const Xfer<List<Face> >& faceLst,
    const Xfer<surfZoneList>& zoneLst
)
{
    ParentType::clearOut();

    // Take over new primitive data.
    // Optimized to avoid overwriting data at all
    if (notNull(pointLst))
    {
        storedPoints().transfer(pointLst());
    }

    if (notNull(faceLst))
    {
        storedFaces().transfer(faceLst());
    }

    if (notNull(zoneLst))
    {
        storedZones().transfer(zoneLst());
    }
}


template<class Face>
void CML::MeshedSurface<Face>::reset
(
    const Xfer<List<point> >& pointLst,
    const Xfer<List<Face> >& faceLst,
    const Xfer<surfZoneList>& zoneLst
)
{
    ParentType::clearOut();

    // Take over new primitive data.
    // Optimized to avoid overwriting data at all
    if (notNull(pointLst))
    {
        storedPoints().transfer(pointLst());
    }

    if (notNull(faceLst))
    {
        storedFaces().transfer(faceLst());
    }

    if (notNull(zoneLst))
    {
        storedZones().transfer(zoneLst());
    }
}


// Remove badly degenerate faces, double faces.
template<class Face>
void CML::MeshedSurface<Face>::cleanup(const bool verbose)
{
    // merge points (already done for STL, TRI)
    stitchFaces(SMALL, verbose);

    checkFaces(verbose);
    this->checkTopology(verbose);
}


template<class Face>
bool CML::MeshedSurface<Face>::stitchFaces
(
    const scalar tol,
    const bool verbose
)
{
    pointField& pointLst = this->storedPoints();

    // Merge points
    labelList  pointMap(pointLst.size());
    pointField newPoints(pointLst.size());

    bool hasMerged = mergePoints(pointLst, tol, verbose, pointMap, newPoints);

    if (!hasMerged)
    {
        return false;
    }

    if (verbose)
    {
        Info<< "MeshedSurface::stitchFaces : Renumbering all faces"
            << endl;
    }

    // Set the coordinates to the merged ones
    pointLst.transfer(newPoints);

    List<Face>& faceLst = this->storedFaces();

    List<label> faceMap(faceLst.size());

    // Reset the point labels to the unique points array
    label newFaceI = 0;
    forAll(faceLst, faceI)
    {
        Face& f = faceLst[faceI];
        forAll(f, fp)
        {
            f[fp] = pointMap[f[fp]];
        }

        // for extra safety: collapse face as well
        if (f.collapse() >= 3)
        {
            if (newFaceI != faceI)
            {
                faceLst[newFaceI] = f;
            }
            faceMap[newFaceI] = faceI;
            newFaceI++;
        }
        else if (verbose)
        {
            Pout<< "MeshedSurface::stitchFaces : "
                << "Removing collapsed face " << faceI << endl
                << "    vertices   :" << f << endl;
        }
    }
    pointMap.clear();

    if (newFaceI != faceLst.size())
    {
        if (verbose)
        {
            Pout<< "MeshedSurface::stitchFaces : "
                << "Removed " << faceLst.size() - newFaceI
                << " faces" << endl;
        }
        faceLst.setSize(newFaceI);
        faceMap.setSize(newFaceI);
        remapFaces(faceMap);
    }
    faceMap.clear();

    // Merging points might have changed geometric factors
    ParentType::clearOut();
    return true;
}


// Remove badly degenerate faces and double faces.
template<class Face>
bool CML::MeshedSurface<Face>::checkFaces
(
    const bool verbose
)
{
    bool changed = false;
    List<Face>& faceLst = this->storedFaces();

    List<label> faceMap(faceLst.size());

    label newFaceI = 0;
    // Detect badly labelled faces and mark degenerate faces
    const label maxPointI = this->points().size() - 1;
    forAll(faceLst, faceI)
    {
        Face& f = faceLst[faceI];

        // avoid degenerate faces
        if (f.collapse() >= 3)
        {
            forAll(f, fp)
            {
                if (f[fp] < 0 || f[fp] > maxPointI)
                {
                    FatalErrorInFunction
                        << "face " << f
                        << " uses point indices outside point range 0.."
                    << maxPointI
                        << exit(FatalError);
                }
            }

            faceMap[faceI] = faceI;
            newFaceI++;
        }
        else
        {
            // mark as bad face
            faceMap[faceI] = -1;

            changed = true;
            if (verbose)
            {
                WarningInFunction
                    << "face[" << faceI << "] = " << f
                    << " does not have three unique vertices" << endl;
            }
        }
    }

    // Detect doubled faces
    // do not touch the faces
    const labelListList& fFaces = this->faceFaces();
    newFaceI = 0;
    forAll(faceLst, faceI)
    {
        // skip already collapsed faces:
        if (faceMap[faceI] < 0)
        {
            continue;
        }

        const Face& f = faceLst[faceI];

        // duplicate face check
        bool okay = true;
        const labelList& neighbours = fFaces[faceI];

        // Check if faceNeighbours use same points as this face.
        // Note: discards normal information - sides of baffle are merged.
        forAll(neighbours, neighI)
        {
            const label neiFaceI = neighbours[neighI];

            if (neiFaceI <= faceI || faceMap[neiFaceI] < 0)
            {
                // lower numbered faces already checked
                // skip neighbours that are themselves collapsed
                continue;
            }

            const Face& nei = faceLst[neiFaceI];

            if (f == nei)
            {
                okay = false;

                if (verbose)
                {
                    WarningInFunction
                        << "faces share the same vertices:" << nl
                        << "    face[" << faceI << "] : " << f << nl
                        << "    face[" << neiFaceI << "] : " << nei << endl;
                    // printFace(Warning, "    ", f, points());
                    // printFace(Warning, "    ", nei, points());
                }

                break;
            }
        }

        if (okay)
        {
            faceMap[faceI] = faceI;
            newFaceI++;
        }
        else
        {
            faceMap[faceI] = -1;
        }
    }

    // Phase 1: pack
    // Done to keep numbering constant in phase 1

    if (changed || newFaceI < faceLst.size())
    {
        changed = true;

        if (verbose)
        {
            WarningInFunction
                << "Removed " << faceLst.size() - newFaceI
                << " illegal faces." << endl;
        }

        // compress the face list
        newFaceI = 0;
        forAll(faceLst, faceI)
        {
            if (faceMap[faceI] >= 0)
            {
                if (newFaceI != faceI)
                {
                    faceLst[newFaceI] = faceLst[faceI];
                }
                faceMap[newFaceI] = faceI;
                newFaceI++;
            }
        }

        faceLst.setSize(newFaceI);
        remapFaces(faceMap);
    }
    faceMap.clear();

    // Topology can change because of renumbering
    ParentType::clearOut();
    return changed;
}


template<class Face>
CML::label CML::MeshedSurface<Face>::triangulate()
{
    return triangulate
    (
        const_cast<List<label>&>(List<label>::null())
    );
}


template<class Face>
CML::label CML::MeshedSurface<Face>::triangulate
(
    List<label>& faceMapOut
)
{
    label nTri = 0;
    label maxTri = 0;  // the maximum number of triangles for any single face
    List<Face>& faceLst = this->storedFaces();

    // determine how many triangles will be needed
    forAll(faceLst, faceI)
    {
        const label n = faceLst[faceI].nTriangles();
        if (maxTri < n)
        {
            maxTri = n;
        }
        nTri += n;
    }

    // nothing to do
    if (nTri <= faceLst.size())
    {
        if (notNull(faceMapOut))
        {
            faceMapOut.clear();
        }
        return 0;
    }

    List<Face>  newFaces(nTri);
    List<label> faceMap;

    // reuse storage from optional faceMap
    if (notNull(faceMapOut))
    {
        faceMap.transfer(faceMapOut);
    }
    faceMap.setSize(nTri);

    // remember the number of *additional* faces
    nTri -= faceLst.size();

    if (this->points().empty())
    {
        // triangulate without points
        // simple face triangulation around f[0]
        label newFaceI = 0;
        forAll(faceLst, faceI)
        {
            const Face& f = faceLst[faceI];

            for (label fp = 1; fp < f.size() - 1; ++fp)
            {
                label fp1 = f.fcIndex(fp);

                newFaces[newFaceI] = triFace(f[0], f[fp], f[fp1]);
                faceMap[newFaceI] = faceI;
                newFaceI++;
            }
        }
    }
    else
    {
        // triangulate with points
        List<face> tmpTri(maxTri);

        label newFaceI = 0;
        forAll(faceLst, faceI)
        {
            // 'face' not '<Face>'
            const face& f = faceLst[faceI];

            label nTmp = 0;
            f.triangles(this->points(), nTmp, tmpTri);
            for (label triI = 0; triI < nTmp; triI++)
            {
                newFaces[newFaceI] = Face
                (
                    static_cast<labelUList&>(tmpTri[triI])
                );
                faceMap[newFaceI] = faceI;
                newFaceI++;
            }
        }
    }

    faceLst.transfer(newFaces);
    remapFaces(faceMap);

    // optionally return the faceMap
    if (notNull(faceMapOut))
    {
        faceMapOut.transfer(faceMap);
    }
    faceMap.clear();

    // Topology can change because of renumbering
    ParentType::clearOut();
    return nTri;
}




template<class Face>
CML::MeshedSurface<Face> CML::MeshedSurface<Face>::subsetMesh
(
    const labelHashSet& include,
    labelList& pointMap,
    labelList& faceMap
) const
{
    const pointField& locPoints = this->localPoints();
    const List<Face>& locFaces  = this->localFaces();


    // Fill pointMap, faceMap
    PatchTools::subsetMap(*this, include, pointMap, faceMap);

    // Create compact coordinate list and forward mapping array
    pointField newPoints(pointMap.size());
    labelList oldToNew(locPoints.size());
    forAll(pointMap, pointI)
    {
        newPoints[pointI] = locPoints[pointMap[pointI]];
        oldToNew[pointMap[pointI]] = pointI;
    }

    // create/copy a new zones list, each zone with zero size
    surfZoneList newZones(this->surfZones());
    forAll(newZones, zoneI)
    {
        newZones[zoneI].size() = 0;
    }

    // Renumber face node labels
    List<Face> newFaces(faceMap.size());
    forAll(faceMap, faceI)
    {
        const label origFaceI = faceMap[faceI];
        newFaces[faceI] = Face(locFaces[origFaceI]);

        // Renumber labels for face
        Face& f = newFaces[faceI];
        forAll(f, fp)
        {
            f[fp] = oldToNew[f[fp]];
        }
    }
    oldToNew.clear();

    // recalculate the zones start/size
    label newFaceI = 0;
    label origEndI = 0;

    // adjust zone sizes
    forAll(newZones, zoneI)
    {
        surfZone& zone = newZones[zoneI];

        // adjust zone start
        zone.start() = newFaceI;
        origEndI += zone.size();

        for (label faceI = newFaceI; faceI < faceMap.size(); ++faceI)
        {
            if (faceMap[faceI] < origEndI)
            {
                ++newFaceI;
            }
            else
            {
                break;
            }
        }

        // adjust zone size
        zone.size() = newFaceI - zone.start();
    }


    // construct a sub-surface
    return MeshedSurface
    (
        xferMove(newPoints),
        xferMove(newFaces),
        xferMove(newZones)
    );
}


template<class Face>
CML::MeshedSurface<Face> CML::MeshedSurface<Face>::subsetMesh
(
    const labelHashSet& include
) const
{
    labelList pointMap, faceMap;
    return subsetMesh(include, pointMap, faceMap);
}



template<class Face>
void CML::MeshedSurface<Face>::transfer
(
    MeshedSurface<Face>& surf
)
{
    reset
    (
        xferMove(surf.storedPoints()),
        xferMove(surf.storedFaces()),
        xferMove(surf.storedZones())
    );
}


template<class Face>
void CML::MeshedSurface<Face>::transfer
(
    UnsortedMeshedSurface<Face>& surf
)
{
    clear();

    labelList faceMap;
    surfZoneList zoneLst = surf.sortedZones(faceMap);

    if (zoneLst.size() <= 1)
    {
        reset
        (
            xferMove(surf.storedPoints()),
            xferMove(surf.storedFaces()),
            Xfer<surfZoneList>()
        );
    }
    else
    {
        List<Face>& oldFaces = surf.storedFaces();
        List<Face> newFaces(faceMap.size());

        forAll(faceMap, faceI)
        {
            newFaces[faceI].transfer(oldFaces[faceMap[faceI]]);
        }

        reset
        (
            xferMove(surf.storedPoints()),
            xferMove(newFaces),
            xferMove(zoneLst)
        );
    }

    faceMap.clear();
    surf.clear();
}


template<class Face>
CML::Xfer<CML::MeshedSurface<Face> > CML::MeshedSurface<Face>::xfer()
{
    return xferMove(*this);
}


// Read from file, determine format from extension
template<class Face>
bool CML::MeshedSurface<Face>::read(const fileName& name)
{
    word ext = name.ext();
    if (ext == "gz")
    {
        fileName unzipName = name.lessExt();
        return read(unzipName, unzipName.ext());
    }
    else
    {
        return read(name, ext);
    }
}


// Read from file in given format
template<class Face>
bool CML::MeshedSurface<Face>::read
(
    const fileName& name,
    const word& ext
)
{
    clear();

    // read via selector mechanism
    transfer(New(name, ext)());
    return true;
}


template<class Face>
void CML::MeshedSurface<Face>::write
(
    const Time& t,
    const word& surfName
) const
{
    MeshedSurfaceProxy<Face>(*this).write(t, surfName);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Face>
void CML::MeshedSurface<Face>::operator=(const MeshedSurface& surf)
{
    clear();

    this->storedPoints() = surf.points();
    this->storedFaces()  = surf.faces();
    this->storedZones()  = surf.surfZones();
}


template<class Face>
CML::MeshedSurface<Face>::operator CML::MeshedSurfaceProxy<Face>() const
{
    return MeshedSurfaceProxy<Face>
    (
        this->points(),
        this->faces(),
        this->surfZones()
    );
}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class Face>
void CML::MeshedSurface<Face>::checkZones()
{
    // extra safety, ensure we have at some zones
    // and they cover all the faces - fix start silently
    surfZoneList& zones = this->storedZones();
    if (zones.size())
    {
        label count = 0;
        forAll(zones, zoneI)
        {
            zones[zoneI].start() = count;
            count += zones[zoneI].size();
        }

        if (count < this->size())
        {
            WarningInFunction
                << "more faces " << this->size() << " than zones " << count
                << " ... extending final zone"
                << endl;

            zones.last().size() += count - this->size();
        }
        else if (count > this->size())
        {
            FatalErrorInFunction
                << "more zones " << count << " than faces " << this->size()
                << exit(FatalError);
        }
    }
}


template<class Face>
void CML::MeshedSurface<Face>::sortFacesAndStore
(
    const Xfer<List<Face> >& unsortedFaces,
    const Xfer<List<label> >& zoneIds,
    const bool sorted
)
{
    List<Face>  oldFaces(unsortedFaces);
    List<label> zones(zoneIds);

    if (sorted)
    {
        // already sorted - simply transfer faces
        this->storedFaces().transfer(oldFaces);
    }
    else
    {
        // unsorted - determine the sorted order:
        // avoid SortableList since we discard the main list anyhow
        List<label> faceMap;
        sortedOrder(zones, faceMap);
        zones.clear();

        // sorted faces
        List<Face> newFaces(faceMap.size());
        forAll(faceMap, faceI)
        {
            // use transfer to recover memory where possible
            newFaces[faceI].transfer(oldFaces[faceMap[faceI]]);
        }
        this->storedFaces().transfer(newFaces);
    }
    zones.clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
void CML::MeshedSurface<Face>::addZones
(
    const UList<surfZone>& srfZones,
    const bool cullEmpty
)
{
    label nZone = 0;

    surfZoneList& zones = this->storedZones();
    zones.setSize(zones.size());
    forAll(zones, zoneI)
    {
        if (srfZones[zoneI].size() || !cullEmpty)
        {
            zones[nZone] = surfZone(srfZones[zoneI], nZone);
            nZone++;
        }
    }
    zones.setSize(nZone);
}


template<class Face>
void CML::MeshedSurface<Face>::addZones
(
    const labelUList& sizes,
    const UList<word>& names,
    const bool cullEmpty
)
{
    label start   = 0;
    label nZone = 0;

    surfZoneList& zones = this->storedZones();
    zones.setSize(sizes.size());
    forAll(zones, zoneI)
    {
        if (sizes[zoneI] || !cullEmpty)
        {
            zones[nZone] = surfZone
            (
                names[zoneI],
                sizes[zoneI],
                start,
                nZone
            );
            start += sizes[zoneI];
            nZone++;
        }
    }
    zones.setSize(nZone);
}


template<class Face>
void CML::MeshedSurface<Face>::addZones
(
    const labelUList& sizes,
    const bool cullEmpty
)
{
    label start   = 0;
    label nZone = 0;

    surfZoneList& zones = this->storedZones();
    zones.setSize(sizes.size());
    forAll(zones, zoneI)
    {
        if (sizes[zoneI] || !cullEmpty)
        {
            zones[nZone] = surfZone
            (
                word("zone") + ::CML::name(nZone),
                sizes[zoneI],
                start,
                nZone
            );
            start += sizes[zoneI];
            nZone++;
        }
    }
    zones.setSize(nZone);
}


template<class Face>
void CML::MeshedSurface<Face>::removeZones()
{
    this->storedZones().clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
void CML::MeshedSurface<Face>::writeStats(Ostream& os) const
{
    os  << "points      : " << this->points().size() << nl;
    if (MeshedSurface<Face>::isTri())
    {
        os << "triangles   : " << this->size() << nl;
    }
    else
    {
        label nTri = 0;
        label nQuad = 0;
        forAll(*this, i)
        {
            const label n = this->operator[](i).size();

            if (n == 3)
            {
                nTri++;
            }
            else if (n == 4)
            {
                nQuad++;
            }
        }

        os  << "faces       : " << this->size()
            << "  (tri:" << nTri << " quad:" << nQuad
            << " poly:" << (this->size() - nTri - nQuad ) << ")" << nl;
    }

    os  << "boundingBox : " << boundBox(this->points()) << endl;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


template<class Face>
CML::autoPtr< CML::MeshedSurface<Face> >
CML::MeshedSurface<Face>::New(const fileName& name, const word& ext)
{
    if (debug)
    {
        Info<< "MeshedSurface::New(const fileName&, const word&) : "
            "constructing MeshedSurface"
            << endl;
    }

    typename fileExtensionConstructorTable::iterator cstrIter =
        fileExtensionConstructorTablePtr_->find(ext);

    if (cstrIter == fileExtensionConstructorTablePtr_->end())
    {
        // no direct reader, delegate if possible
        wordHashSet supported = FriendType::readTypes();
        if (supported.found(ext))
        {
            // create indirectly
            autoPtr< MeshedSurface<Face> > surf(new MeshedSurface<Face>);
            surf().transfer(FriendType::New(name, ext)());

            return surf;
        }

        // nothing left to try, issue error
        supported += readTypes();

        FatalErrorInFunction
            << "Unknown file extension " << ext << nl << nl
            << "Valid types are :" << nl
            << supported
            << exit(FatalError);
    }

    return autoPtr< MeshedSurface<Face> >(cstrIter()(name));
}


template<class Face>
CML::autoPtr< CML::MeshedSurface<Face> >
CML::MeshedSurface<Face>::New(const fileName& name)
{
    word ext = name.ext();
    if (ext == "gz")
    {
        ext = name.lessExt().ext();
    }
    return New(name, ext);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
