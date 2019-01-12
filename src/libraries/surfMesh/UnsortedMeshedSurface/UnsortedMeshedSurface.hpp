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
    CML::UnsortedMeshedSurface

Description
    A surface geometry mesh, in which the surface zone information is
    conveyed by the 'zoneId' associated with each face.

    This form of surface description is particularly useful for reading in
    surface meshes from third-party formats (eg, obj, stl, gts, etc.). It
    can also be particularly useful for situations in which the surface
    many be adjusted in an arbitrary manner without worrying about needed
    to adjust the zone information (eg, surface refinement).

See Also
    The CML::MeshedSurface - which is organized as a surface mesh, but
    with independent zone information.


\*---------------------------------------------------------------------------*/

#ifndef UnsortedMeshedSurface_H
#define UnsortedMeshedSurface_H

#include "MeshedSurface.hpp"
#include "surfZoneIdentifierList.hpp"
#include "surfZoneList.hpp"
#include "surfaceFormatsCore.hpp"
#include "runTimeSelectionTables.hpp"
#include "memberFunctionSelectionTables.hpp"
#include "HashSet.hpp"
#include "MeshedSurface.hpp"
#include "UnsortedMeshedSurface.hpp"
#include "MeshedSurfaceProxy.hpp"
#include "IFstream.hpp"
#include "OFstream.hpp"
#include "Time.hpp"
#include "polyBoundaryMesh.hpp"
#include "polyMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class Time;
class IFstream;

template<class Face> class MeshedSurface;
template<class Face> class MeshedSurfaceProxy;
template<class Face> class UnsortedMeshedSurface;

/*---------------------------------------------------------------------------*\
                   Class UnsortedMeshedSurface Declaration
\*---------------------------------------------------------------------------*/

template<class Face>
class UnsortedMeshedSurface
:
    public MeshedSurface<Face>
{
    // friends - despite different face representationsx
    template<class Face2> friend class MeshedSurface;
    template<class Face2> friend class UnsortedMeshedSurface;
    friend class surfMesh;

private:

    // Private typedefs for convenience

        typedef MeshedSurface<Face>       ParentType;
        typedef MeshedSurface<Face>       FriendType;
        typedef MeshedSurfaceProxy<Face>  ProxyType;


    // Private Member Data

        //- The zone Id associated with each face
        labelList zoneIds_;

        //- Zone information (face ordering nFaces/startFace only used
        //  during reading and writing)
        List<surfZoneIdentifier> zoneToc_;


    // Private Member Functions

        //- Disable resize with value
        void resize(const label, const Face&);

        //- Disable setSize with value
        void setSize(const label, const Face&);


protected:

    // Protected Member functions

        //- Return non-const access to the zone Ids
        List<label>& storedZoneIds()
        {
            return zoneIds_;
        }

        //- Return non-const access to the zone table-of-contents
        List<surfZoneIdentifier>& storedZoneToc()
        {
            return zoneToc_;
        }

        //- Set new zones from faceMap
        virtual void remapFaces(const labelUList& faceMap);


public:

    // Public typedefs

        //- Face type used
        typedef Face FaceType;

        //- Runtime type information
        TypeName("UnsortedMeshedSurface");


    // Static

        //- Can we read this file format?
        static bool canReadType(const word& ext, const bool verbose=false);

        //- Can we read this file format?
        static bool canRead(const fileName&, const bool verbose=false);

        //- Can we write this file format?
        static bool canWriteType(const word& ext, const bool verbose=false);

        static wordHashSet readTypes();
        static wordHashSet writeTypes();


    // Constructors

        //- Construct null
        UnsortedMeshedSurface();

        //- Construct by transferring components
        //  (points, faces, zone ids, zone info).
        UnsortedMeshedSurface
        (
            const Xfer<pointField>&,
            const Xfer<List<Face> >&,
            const Xfer<List<label> >& zoneIds,
            const Xfer<surfZoneIdentifierList>&
        );

        //- Construct by transferring points, faces.
        //  Use zone information, or set single default zone
        UnsortedMeshedSurface
        (
            const Xfer<pointField>&,
            const Xfer<List<Face> >&,
            const labelUList& zoneSizes = labelUList(),
            const UList<word>& zoneNames = UList<word>()
        );

        //- Construct as copy
        UnsortedMeshedSurface(const UnsortedMeshedSurface<Face>&);

        //- Construct from a meshedSurface
        UnsortedMeshedSurface(const MeshedSurface<Face>&);

        //- Construct by transferring the contents from a UnsortedMeshedSurface
        UnsortedMeshedSurface(const Xfer<UnsortedMeshedSurface<Face> >&);

        //- Construct by transferring the contents from a meshedSurface
        UnsortedMeshedSurface(const Xfer<MeshedSurface<Face> >&);

        //- Construct from file name (uses extension to determine type)
        UnsortedMeshedSurface(const fileName&);

        //- Construct from file name (uses extension to determine type)
        UnsortedMeshedSurface(const fileName&, const word&);

        //- Construct from objectRegistry and a named surface
        UnsortedMeshedSurface(const Time&, const word& surfName="");


    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            UnsortedMeshedSurface,
            fileExtension,
            (
                const fileName& name
            ),
            (name)
        );


    // Selectors

        //- Select constructed from filename (explicit extension)
        static autoPtr<UnsortedMeshedSurface> New
        (
            const fileName&,
            const word& ext
        );

        //- Select constructed from filename (implicit extension)
        static autoPtr<UnsortedMeshedSurface> New(const fileName&);


    //- Destructor
    virtual ~UnsortedMeshedSurface();


    // Member Function Selectors

        declareMemberFunctionSelectionTable
        (
            void,
            UnsortedMeshedSurface,
            write,
            fileExtension,
            (
                const fileName& name,
                const UnsortedMeshedSurface<Face>& surf
            ),
            (name, surf)
        );

        //- Write to file
        static void write(const fileName&, const UnsortedMeshedSurface<Face>&);


    // Member Functions

        // Access

            //- The surface size is the number of faces
            label size() const
            {
                return ParentType::size();
            }

            //- Reset size of face and zone list
            void setSize(const label);

            //- Return const access to the zone ids
            const List<label>& zoneIds() const
            {
                return zoneIds_;
            }

            //- Return const access to the zone table-of-contents
            const List<surfZoneIdentifier>& zoneToc() const
            {
                return zoneToc_;
            }

            //- Sort faces according to zoneIds
            //  Returns a surfZoneList and sets faceMap to index within faces()
            surfZoneList sortedZones(labelList& faceMap) const;

            //- Set zones to 0 and set a single zone
            void setOneZone();

            //- Set zone ids and zones
            void setZones(const surfZoneList&);

            //- Set zone ids and zones
            void setZones(const labelUList& sizes, const UList<word>& names);

            //- Set zone ids and zones with default names
            void setZones(const labelUList& sizes);


        // Edit

            //- Clear all storage
            virtual void clear();

            //- Return new surface.
            //  Returns return pointMap, faceMap from subsetMeshMap
            UnsortedMeshedSurface subsetMesh
            (
                const labelHashSet& include,
                labelList& pointMap,
                labelList& faceMap
            ) const;

            //- Return new surface.
            UnsortedMeshedSurface subsetMesh
            (
                const labelHashSet& include
            ) const;
    
            //- Inherit reset from MeshedSurface<Face>
            using MeshedSurface<Face>::reset;
    
            //- Transfer components (points, faces, zone ids).
            virtual void reset
            (
                const Xfer<pointField>&,
                const Xfer<List<Face> >&,
                const Xfer<List<label> >& zoneIds
            );

            //- Transfer components (points, faces, zone ids).
            virtual void reset
            (
                const Xfer<List<point> >&,
                const Xfer<List<Face> >&,
                const Xfer<List<label> >& zoneIds
            );

            //- Transfer the contents of the argument and annul the argument
            void transfer(UnsortedMeshedSurface<Face>&);

            //- Transfer the contents of the argument and annul the argument
            void transfer(MeshedSurface<Face>&);

            //- Transfer contents to the Xfer container
            Xfer<UnsortedMeshedSurface<Face> > xfer();


        // Read

            //- Read from file. Chooses reader based on explicit extension
            bool read(const fileName&, const word& ext);

            //- Read from file. Chooses reader based on detected extension
            virtual bool read(const fileName&);


        // Write

            //- Generic write routine. Chooses writer based on extension.
            virtual void write(const fileName& name) const
            {
                write(name, *this);
            }

            //- Write to database
            void write(const Time&, const word& surfName="") const;


        // Member operators

            void operator=(const UnsortedMeshedSurface<Face>&);

            //- Conversion operator to MeshedSurfaceProxy
            operator MeshedSurfaceProxy<Face>() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

template<class Face>
CML::wordHashSet CML::UnsortedMeshedSurface<Face>::readTypes()
{
    return wordHashSet(*fileExtensionConstructorTablePtr_);
}


template<class Face>
CML::wordHashSet CML::UnsortedMeshedSurface<Face>::writeTypes()
{
    return wordHashSet(*writefileExtensionMemberFunctionTablePtr_);
}


template<class Face>
bool CML::UnsortedMeshedSurface<Face>::canReadType
(
    const word& ext,
    const bool verbose
)
{
   return fileFormats::surfaceFormatsCore::checkSupport
   (
       readTypes() | ParentType::readTypes(),
       ext,
       verbose,
       "reading"
   );
}


template<class Face>
bool CML::UnsortedMeshedSurface<Face>::canWriteType
(
    const word& ext,
    const bool verbose
)
{
    return fileFormats::surfaceFormatsCore::checkSupport
    (
        writeTypes(),
        ext,
        verbose,
        "writing"
    );
}


template<class Face>
bool CML::UnsortedMeshedSurface<Face>::canRead
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
void CML::UnsortedMeshedSurface<Face>::write
(
    const fileName& name,
    const UnsortedMeshedSurface<Face>& surf
)
{
    if (debug)
    {
        Info<< "UnsortedMeshedSurface::write"
            "(const fileName&, const UnsortedMeshedSurface&) : "
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
CML::UnsortedMeshedSurface<Face>::UnsortedMeshedSurface()
:
    ParentType()
{}


template<class Face>
CML::UnsortedMeshedSurface<Face>::UnsortedMeshedSurface
(
    const Xfer<pointField>& pointLst,
    const Xfer<List<Face> >& faceLst,
    const Xfer<List<label> >& zoneIds,
    const Xfer<surfZoneIdentifierList>& zoneTofc
)
:
    ParentType(pointLst, faceLst),
    zoneIds_(zoneIds),
    zoneToc_(zoneTofc)
{}


template<class Face>
CML::UnsortedMeshedSurface<Face>::UnsortedMeshedSurface
(
    const Xfer<pointField>& pointLst,
    const Xfer<List<Face> >& faceLst,
    const labelUList& zoneSizes,
    const UList<word>& zoneNames
)
:
    ParentType(pointLst, faceLst)
{
    if (zoneSizes.size())
    {
        if (zoneNames.size())
        {
            setZones(zoneSizes, zoneNames);
        }
        else
        {
            setZones(zoneSizes);
        }
    }
    else
    {
        setOneZone();
    }
}


template<class Face>
CML::UnsortedMeshedSurface<Face>::UnsortedMeshedSurface
(
    const UnsortedMeshedSurface<Face>& surf
)
:
    ParentType
    (
        xferCopy(surf.points()),
        xferCopy(surf.faces())
    ),
    zoneIds_(surf.zoneIds()),
    zoneToc_(surf.zoneToc())
{}


template<class Face>
CML::UnsortedMeshedSurface<Face>::UnsortedMeshedSurface
(
    const MeshedSurface<Face>& surf
)
:
    ParentType
    (
        xferCopy(surf.points()),
        xferCopy(surf.faces())
    )
{
    setZones(surf.surfZones());
}


template<class Face>
CML::UnsortedMeshedSurface<Face>::UnsortedMeshedSurface
(
    const Xfer<UnsortedMeshedSurface<Face> >& surf
)
:
    ParentType()
{
    transfer(surf());
}


template<class Face>
CML::UnsortedMeshedSurface<Face>::UnsortedMeshedSurface
(
    const Xfer<MeshedSurface<Face> >& surf
)
:
    ParentType()
{
    transfer(surf());
}


template<class Face>
CML::UnsortedMeshedSurface<Face>::UnsortedMeshedSurface
(
    const fileName& name,
    const word& ext
)
:
    ParentType()
{
    read(name, ext);
}


template<class Face>
CML::UnsortedMeshedSurface<Face>::UnsortedMeshedSurface(const fileName& name)
:
    ParentType()
{
    read(name);
}


template<class Face>
CML::UnsortedMeshedSurface<Face>::UnsortedMeshedSurface
(
    const Time& t,
    const word& surfName
)
:
    ParentType()
{
    MeshedSurface<Face> surf(t, surfName);
    transfer(surf);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Face>
CML::UnsortedMeshedSurface<Face>::~UnsortedMeshedSurface()
{}


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class Face>
void CML::UnsortedMeshedSurface<Face>::setOneZone()
{
    zoneIds_.setSize(size());
    zoneIds_ = 0;

    word zoneName;
    if (zoneToc_.size())
    {
        zoneName = zoneToc_[0].name();
    }
    if (zoneName.empty())
    {
        zoneName = "zone0";
    }

    // set single default zone
    zoneToc_.setSize(1);
    zoneToc_[0] = surfZoneIdentifier(zoneName, 0);
}


template<class Face>
void CML::UnsortedMeshedSurface<Face>::setZones
(
    const surfZoneList& zoneLst
)
{
    zoneIds_.setSize(size());
    zoneToc_.setSize(zoneLst.size());

    forAll(zoneToc_, zoneI)
    {
        const surfZone& zone = zoneLst[zoneI];
        zoneToc_[zoneI] = zone;

        // assign sub-zone Ids
        SubList<label> subZone(zoneIds_, zone.size(), zone.start());
        subZone = zoneI;
    }
}


template<class Face>
void CML::UnsortedMeshedSurface<Face>::setZones
(
    const labelUList& sizes,
    const UList<word>& names
)
{
    zoneIds_.setSize(size());
    zoneToc_.setSize(sizes.size());

    label start = 0;
    forAll(zoneToc_, zoneI)
    {
        zoneToc_[zoneI] = surfZoneIdentifier(names[zoneI], zoneI);

        // assign sub-zone Ids
        SubList<label> subZone(zoneIds_, sizes[zoneI], start);
        subZone = zoneI;

        start += sizes[zoneI];
    }
}


template<class Face>
void CML::UnsortedMeshedSurface<Face>::setZones
(
    const labelUList& sizes
)
{
    zoneIds_.setSize(size());
    zoneToc_.setSize(sizes.size());

    label start = 0;
    forAll(zoneToc_, zoneI)
    {
        zoneToc_[zoneI] = surfZoneIdentifier
        (
            word("zone") + ::CML::name(zoneI),
            zoneI
        );

        // assign sub-zone Ids
        SubList<label> subZone(zoneIds_, sizes[zoneI], start);
        subZone = zoneI;

        start += sizes[zoneI];
    }
}


template<class Face>
void CML::UnsortedMeshedSurface<Face>::remapFaces
(
    const labelUList& faceMap
)
{
    // re-assign the zone Ids
    if (notNull(faceMap) && faceMap.size())
    {
        if (zoneToc_.empty())
        {
            setOneZone();
        }
        else if (zoneToc_.size() == 1)
        {
            // optimized for single-zone case
            zoneIds_ = 0;
        }
        else
        {
            List<label> newZones(faceMap.size());

            forAll(faceMap, faceI)
            {
                newZones[faceI] = zoneIds_[faceMap[faceI]];
            }
            zoneIds_.transfer(newZones);
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
void CML::UnsortedMeshedSurface<Face>::setSize(const label s)
{
    this->storedFaces().setSize(s);
    // if zones extend: set with last zoneId
    zoneIds_.setSize(s, zoneToc_.size() - 1);
}


template<class Face>
void CML::UnsortedMeshedSurface<Face>::clear()
{
    ParentType::clear();
    zoneIds_.clear();
    zoneToc_.clear();
}


template<class Face>
CML::surfZoneList CML::UnsortedMeshedSurface<Face>::sortedZones
(
    labelList& faceMap
) const
{
    // supply some zone names
    Map<word> zoneNames;
    forAll(zoneToc_, zoneI)
    {
        zoneNames.insert(zoneI, zoneToc_[zoneI].name());
    }

    // std::sort() really seems to mix up the order.
    // and std::stable_sort() might take too long / too much memory

    // Assuming that we have relatively fewer zones compared to the
    // number of items, just do it ourselves

    // step 1: get zone sizes and store (origId => zoneI)
    Map<label> lookup;
    forAll(zoneIds_, faceI)
    {
        const label origId = zoneIds_[faceI];

        Map<label>::iterator fnd = lookup.find(origId);
        if (fnd != lookup.end())
        {
            fnd()++;
        }
        else
        {
            lookup.insert(origId, 1);
        }
    }

    // step 2: assign start/size (and name) to the newZones
    // re-use the lookup to map (zoneId => zoneI)
    surfZoneList zoneLst(lookup.size());
    label start = 0;
    label zoneI = 0;
    forAllIter(Map<label>, lookup, iter)
    {
        label origId = iter.key();

        word name;
        Map<word>::const_iterator fnd = zoneNames.find(origId);
        if (fnd != zoneNames.end())
        {
            name = fnd();
        }
        else
        {
            name = word("zone") + ::CML::name(zoneI);
        }

        zoneLst[zoneI] = surfZone
        (
            name,
            0,           // initialize with zero size
            start,
            zoneI
        );

        // increment the start for the next zone
        // and save the (zoneId => zoneI) mapping
        start += iter();
        iter() = zoneI++;
    }


    // step 3: build the re-ordering
    faceMap.setSize(zoneIds_.size());

    forAll(zoneIds_, faceI)
    {
        label zoneI = lookup[zoneIds_[faceI]];
        faceMap[faceI] = zoneLst[zoneI].start() + zoneLst[zoneI].size()++;
    }

    // with reordered faces registered in faceMap
    return zoneLst;
}


template<class Face>
CML::UnsortedMeshedSurface<Face>
CML::UnsortedMeshedSurface<Face>::subsetMesh
(
    const labelHashSet& include,
    labelList& pointMap,
    labelList& faceMap
) const
{
    const pointField&  locPoints = this->localPoints();
    const List<Face>&  locFaces  = this->localFaces();

    // Fill pointMap, faceMap
    PatchTools::subsetMap(*this, include, pointMap, faceMap);

    // Create compact coordinate list and forward mapping array
    pointField newPoints(pointMap.size());
    labelList  oldToNew(locPoints.size());
    forAll(pointMap, pointI)
    {
        newPoints[pointI] = locPoints[pointMap[pointI]];
        oldToNew[pointMap[pointI]] = pointI;
    }

    // Renumber face node labels and compact
    List<Face>  newFaces(faceMap.size());
    List<label> newZones(faceMap.size());

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

        newZones[faceI] = zoneIds_[origFaceI];
    }
    oldToNew.clear();

    // construct a sub-surface
    return UnsortedMeshedSurface
    (
        xferMove(newPoints),
        xferMove(newFaces),
        xferMove(newZones),
        xferCopy(zoneToc_)
    );
}


template<class Face>
CML::UnsortedMeshedSurface<Face> CML::UnsortedMeshedSurface<Face>::subsetMesh
(
    const labelHashSet& include
) const
{
    labelList pointMap, faceMap;
    return subsetMesh(include, pointMap, faceMap);
}


template<class Face>
void CML::UnsortedMeshedSurface<Face>::reset
(
    const Xfer<pointField>& pointLst,
    const Xfer<List<Face> >& faceLst,
    const Xfer<List<label> >& zoneIds
)
{
    ParentType::reset
    (
        pointLst,
        faceLst,
        Xfer<surfZoneList>()
    );

    if (notNull(zoneIds))
    {
        zoneIds_.transfer(zoneIds());
    }
}


template<class Face>
void CML::UnsortedMeshedSurface<Face>::reset
(
    const Xfer<List<point> >& pointLst,
    const Xfer<List<Face> >& faceLst,
    const Xfer<List<label> >& zoneIds
)
{
    ParentType::reset
    (
        pointLst,
        faceLst,
        Xfer<surfZoneList>()
    );

    if (notNull(zoneIds))
    {
        zoneIds_.transfer(zoneIds());
    }
}


template<class Face>
void CML::UnsortedMeshedSurface<Face>::transfer
(
    UnsortedMeshedSurface<Face>& surf
)
{
    ParentType::reset
    (
        xferMove(surf.storedPoints()),
        xferMove(surf.storedFaces()),
        Xfer<surfZoneList>()
    );

    zoneIds_.transfer(surf.zoneIds_);
    zoneToc_.transfer(surf.zoneToc_);

    surf.clear();
}


template<class Face>
void CML::UnsortedMeshedSurface<Face>::transfer
(
    MeshedSurface<Face>& surf
)
{
    ParentType::reset
    (
        xferMove(surf.storedPoints()),
        xferMove(surf.storedFaces()),
        Xfer<surfZoneList>()
    );

    setZones(surf.surfZones());
    surf.clear();
}


template<class Face>
CML::Xfer<CML::UnsortedMeshedSurface<Face> >
CML::UnsortedMeshedSurface<Face>::xfer()
{
    return xferMove(*this);
}


// Read from file, determine format from extension
template<class Face>
bool CML::UnsortedMeshedSurface<Face>::read(const fileName& name)
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
bool CML::UnsortedMeshedSurface<Face>::read
(
    const fileName& name,
    const word& ext
)
{
    clear();

    // read via use selector mechanism
    transfer(New(name, ext)());
    return true;
}


template<class Face>
void CML::UnsortedMeshedSurface<Face>::write
(
    const Time& t,
    const word& surfName
) const
{
    MeshedSurfaceProxy<Face>(*this).write(t, surfName);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Face>
void CML::UnsortedMeshedSurface<Face>::operator=
(
    const UnsortedMeshedSurface<Face>& surf
)
{
    clear();

    this->storedPoints() = surf.points();
    this->storedFaces()  = surf.faces();
    zoneIds_ = surf.zoneIds_;
    zoneToc_ = surf.zoneToc_;
}


template<class Face>
CML::UnsortedMeshedSurface<Face>::operator
CML::MeshedSurfaceProxy<Face>() const
{
    labelList faceMap;
    List<surfZone> zoneLst = this->sortedZones(faceMap);

    return MeshedSurfaceProxy<Face>
    (
        this->points(),
        this->faces(),
        zoneLst,
        faceMap
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Face>
CML::autoPtr< CML::UnsortedMeshedSurface<Face> >
CML::UnsortedMeshedSurface<Face>::New(const fileName& name, const word& ext)
{
    if (debug)
    {
        Info<< "UnsortedMeshedSurface::New(const fileName&, const word&) : "
            "constructing UnsortedMeshedSurface"
            << endl;
    }

    typename fileExtensionConstructorTable::iterator cstrIter =
        fileExtensionConstructorTablePtr_->find(ext);

    if (cstrIter == fileExtensionConstructorTablePtr_->end())
    {
        // no direct reader, use the parent if possible
        wordHashSet supported = ParentType::readTypes();
        if (supported.found(ext))
        {
            // create indirectly
            autoPtr<UnsortedMeshedSurface<Face> > surf
            (
                new UnsortedMeshedSurface<Face>
            );
            surf().transfer(ParentType::New(name, ext)());

            return surf;
        }

        // nothing left but to issue an error
        supported += readTypes();

        FatalErrorInFunction
            << "Unknown file extension " << ext << nl << nl
            << "Valid types are:" << nl
            << supported
            << exit(FatalError);
    }

    return autoPtr<UnsortedMeshedSurface<Face> >(cstrIter()(name));
}


template<class Face>
CML::autoPtr< CML::UnsortedMeshedSurface<Face> >
CML::UnsortedMeshedSurface<Face>::New(const fileName& name)
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
