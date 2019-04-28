/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::mappedPatchBase

Description
    Determines a mapping between patch face centres and mesh cell or face
    centres and processors they're on.

    If constructed from dictionary:
    \verbatim
        // Region to sample (default is region0)
        sampleRegion region0;

        // What to sample:
        // - nearestCell         : sample cell containing point
        // - nearestOnlyCell     : nearest sample cell (even if not containing
        //                         point)
        // - nearestPatchFace    : nearest face on selected patch
        // - nearestPatchFaceAMI : nearest face on selected patch
                                   - patches need not conform
                                   - uses AMI interpolation
        // - nearestFace         : nearest boundary face on any patch
        // - nearestPatchPoint   : nearest patch point (for coupled points
        //                         this might be any of the points so you have
        //                         to guarantee the point data is synchronised
        //                         beforehand)
        sampleMode nearestCell;

        // If sampleMode is nearestPatchFace : patch to find faces of
        samplePatch movingWall;

        // If sampleMode is nearestPatchFace : specify patchgroup to find
        // samplePatch and sampleRegion (if not provided)
        coupleGroup baffleGroup;

        // How to supply offset (w.r.t. my patch face centres):
        // - uniform : single offset vector
        // - nonuniform : per-face offset vector
        // - normal : using supplied distance and face normal
        offsetMode uniform;

        // According to offsetMode (see above) supply one of
        // offset, offsets or distance
        offset  (1 0 0);
    \endverbatim

    Note: if offsetMode is \c normal it uses outwards pointing normals. So
    supply a negative distance if sampling inside the domain.


Note
    Storage is not optimal. It temporary collects all (patch)face centres
    on all processors to keep the addressing calculation simple.

SourceFiles
    mappedPatchBase.cpp

\*---------------------------------------------------------------------------*/

#ifndef mappedPatchBase_H
#define mappedPatchBase_H

#include "pointField.hpp"
#include "Tuple2.hpp"
#include "pointIndexHit.hpp"
#include "AMIPatchToPatchInterpolation.hpp"
#include "coupleGroupIdentifier.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class polyPatch;
class polyMesh;
class mapDistribute;

/*---------------------------------------------------------------------------*\
                       Class mappedPatchBase Declaration
\*---------------------------------------------------------------------------*/

class mappedPatchBase
{

public:

    // Type enumerations

        //- Mesh items to sample
        enum sampleMode
        {
            NEARESTCELL,         // nearest cell containing sample
            NEARESTPATCHFACE,    // nearest face on selected patch
            NEARESTPATCHFACEAMI, // nearest patch face + AMI interpolation
            NEARESTPATCHPOINT,   // nearest point on selected patch
            NEARESTFACE,         // nearest face
            NEARESTONLYCELL      // nearest cell (even if not containing cell)
        };

        //- How to project face centres
        enum offsetMode
        {
            UNIFORM,            // single offset vector
            NONUNIFORM,         // per-face offset vector
            NORMAL              // use face normal + distance
        };

        static const NamedEnum<sampleMode, 6> sampleModeNames_;

        static const NamedEnum<offsetMode, 3> offsetModeNames_;


    //- Helper class for finding nearest
    //  Nearest:
    //  - point+local index
    //  - sqr(distance)
    //  - processor
    typedef Tuple2<pointIndexHit, Tuple2<scalar, label> > nearInfo;

    class nearestEqOp
    {

    public:

        void operator()(nearInfo& x, const nearInfo& y) const
        {
            if (y.first().hit())
            {
                if (!x.first().hit())
                {
                    x = y;
                }
                else if (y.second().first() < x.second().first())
                {
                    x = y;
                }
            }
        }
    };

    class maxProcEqOp
    {

    public:

        void operator()(nearInfo& x, const nearInfo& y) const
        {
            if (y.first().hit())
            {
                if (!x.first().hit())
                {
                    x = y;
                }
                else if (y.second().second() > x.second().second())
                {
                    x = y;
                }
            }
        }
    };


protected:

    // Protected data

        //- Patch to sample
        const polyPatch& patch_;

        //- Region to sample
        mutable word sampleRegion_;

        //- What to sample
        const sampleMode mode_;

        //- Patch (if in sampleMode NEARESTPATCH*)
        mutable word samplePatch_;

        //- PatchGroup (if in sampleMode NEARESTPATCH*)
        const coupleGroupIdentifier coupleGroup_;

        //- How to obtain samples
        offsetMode offsetMode_;

        //- Offset vector (uniform)
        vector offset_;

        //- Offset vector (nonuniform)
        vectorField offsets_;

        //- Offset distance (normal)
        scalar distance_;

        //- Same region
        mutable bool sameRegion_;


        // Derived information

            //- Communication schedule:
            //
            //    - Cells/faces to sample per processor
            //    - Patch faces to receive per processor
            //    - schedule
            mutable autoPtr<mapDistribute> mapPtr_;


        // AMI interpolator (only for NEARESTPATCHFACEAMI)

            //- Pointer to AMI interpolator
            mutable autoPtr<AMIPatchToPatchInterpolation> AMIPtr_;

            //- Flag to indicate that slave patch should be reversed for AMI
            const bool AMIReverse_;

            //- Pointer to projection surface employed by AMI interpolator
            mutable autoPtr<searchableSurface> surfPtr_;

            //- Dictionary storing projection surface description
            dictionary surfDict_;


    // Protected Member Functions

        //- Get the points from face-centre-decomposition face centres
        //  and project them onto the face-diagonal-decomposition triangles.
        tmp<pointField> facePoints(const polyPatch&) const;

        //- Collect single list of samples and originating processor+face.
        void collectSamples
        (
            const pointField& facePoints,
            pointField&,
            labelList& patchFaceProcs,
            labelList& patchFaces,
            pointField& patchFc
        ) const;

        //- Find cells/faces containing samples
        void findSamples
        (
            const pointField&,
            labelList& sampleProcs,     // processor containing sample
            labelList& sampleIndices,   // local index of cell/face
            pointField& sampleLocations // actual representative location
        ) const;

        //- Get the sample points given the face points
        tmp<pointField> samplePoints(const pointField&) const;

        //- Calculate mapping
        void calcMapping() const;

        //- Calculate AMI interpolator
        void calcAMI() const;

        //- Helper to read field or non-uniform list from dictionary
        static tmp<pointField> readListOrField
        (
            const word& keyword,
            const dictionary& dict,
            const label size
        );


public:

    //- Runtime type information
    TypeName("mappedPatchBase");


    // Constructors

        //- Construct from patch
        mappedPatchBase(const polyPatch&);

        //- Construct with offsetMode=non-uniform
        mappedPatchBase
        (
            const polyPatch& pp,
            const word& sampleRegion,
            const sampleMode sampleMode,
            const word& samplePatch,
            const vectorField& offsets
        );

        //- Construct from offsetMode=uniform
        mappedPatchBase
        (
            const polyPatch& pp,
            const word& sampleRegion,
            const sampleMode sampleMode,
            const word& samplePatch,
            const vector& offset
        );

        //- Construct from offsetMode=normal and distance
        mappedPatchBase
        (
            const polyPatch& pp,
            const word& sampleRegion,
            const sampleMode sampleMode,
            const word& samplePatch,
            const scalar distance
        );

        //- Construct from dictionary
        mappedPatchBase(const polyPatch&, const dictionary&);

        //- Construct from dictionary and (collocated) sample mode
        //  (only for nearestPatchFace, nearestPatchFaceAMI, nearestPatchPoint)
        //  Assumes zero offset.
        mappedPatchBase(const polyPatch&, const sampleMode, const dictionary&);

        //- Construct as copy, resetting patch
        mappedPatchBase(const polyPatch&, const mappedPatchBase&);

        //- Construct as copy, resetting patch, map original data
        mappedPatchBase
        (
            const polyPatch&,
            const mappedPatchBase&,
            const labelUList& mapAddressing
        );


    //- Destructor
    virtual ~mappedPatchBase();


    // Member functions

        void clearOut();

        // Access

            //- What to sample
            inline const sampleMode& mode() const;

            //- Region to sample
            inline const word& sampleRegion() const;

            //- Patch (only if NEARESTPATCHFACE)
            inline const word& samplePatch() const;

            //- PatchGroup (only if NEARESTPATCHFACE)
            inline const word& coupleGroup() const;

            //- Return size of mapped mesh/patch/boundary
            inline label sampleSize() const;

            //- Offset vector (from patch faces to destination mesh objects)
            inline const vector& offset() const;

            //- Offset vector (from patch faces to destination mesh objects)
            inline const vectorField& offsets() const;

            //- Cached sampleRegion != mesh.name()
            inline bool sameRegion() const;

            //- Return reference to the parallel distribution map
            inline const mapDistribute& map() const;

            //- Return reference to the AMI interpolator
            inline const AMIPatchToPatchInterpolation& AMI
            (
                const bool forceUpdate = false
            ) const;

            //- Return a pointer to the AMI projection surface
            const autoPtr<CML::searchableSurface>& surfPtr() const;

            //- Get the region mesh
            const polyMesh& sampleMesh() const;

            //- Get the patch on the region
            const polyPatch& samplePolyPatch() const;


        // Helpers

            //- Get the sample points
            tmp<pointField> samplePoints() const;

            //- Get a point on the face given a face decomposition method:
            //  face-centre-tet : face centre. Returns index of face.
            //  face-planes     : face centre. Returns index of face.
            //  face-diagonal   : intersection of ray from cellcentre to
            //                    facecentre with any of the triangles.
            //                    Returns index (0..size-2) of triangle.
            static pointIndexHit facePoint
            (
                const polyMesh&,
                const label facei,
                const polyMesh::cellRepresentation
            );


        // Distribute

            //- Wrapper around map/interpolate data distribution
            template<class Type>
            void distribute(List<Type>& lst) const;

            //- Wrapper around map/interpolate data distribution with operation
            template<class Type, class CombineOp>
            void distribute(List<Type>& lst, const CombineOp& cop) const;

            //- Wrapper around map/interpolate data distribution
            template<class Type>
            void reverseDistribute(List<Type>& lst) const;

            //- Wrapper around map/interpolate data distribution with operation
            template<class Type, class CombineOp>
            void reverseDistribute(List<Type>& lst, const CombineOp& cop) const;


        // I/O

            //- Write as a dictionary
            virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline const CML::mappedPatchBase::sampleMode&
CML::mappedPatchBase::mode() const
{
    return mode_;
}


inline const CML::word& CML::mappedPatchBase::sampleRegion() const
{
    if (sampleRegion_.empty())
    {
        if (!coupleGroup_.valid())
        {
            FatalErrorInFunction
                << "Supply either a regionName or a coupleGroup"
                << " for patch " << patch_.name()
                << " in region " << patch_.boundaryMesh().mesh().name()
                << exit(FatalError);
        }

        // Try and use patchGroup to find samplePatch and sampleRegion
        label samplePatchID = coupleGroup_.findOtherPatchID
        (
            patch_,
            sampleRegion_
        );

        samplePatch_ = sampleMesh().boundaryMesh()[samplePatchID].name();
    }
    return sampleRegion_;
}


inline const CML::word& CML::mappedPatchBase::samplePatch() const
{
    if (samplePatch_.empty())
    {
        if (!coupleGroup_.valid())
        {
            FatalErrorInFunction
                << "Supply either a patchName or a coupleGroup"
                << " for patch " << patch_.name()
                << " in region " << patch_.boundaryMesh().mesh().name()
                << exit(FatalError);
        }

        // Try and use patchGroup to find samplePatch and sampleRegion
        label samplePatchID = coupleGroup_.findOtherPatchID
        (
            patch_,
            sampleRegion_
        );

        samplePatch_ = sampleMesh().boundaryMesh()[samplePatchID].name();
    }
    return samplePatch_;
}


inline const CML::word& CML::mappedPatchBase::coupleGroup() const
{
    return coupleGroup_.name();
}


inline CML::label CML::mappedPatchBase::sampleSize() const
{
    switch (mode_)
    {
        case NEARESTPATCHFACEAMI:
        {
            return samplePolyPatch().size();
        }
        case NEARESTCELL:
        {
            return sampleMesh().nCells();
        }
        case NEARESTPATCHFACE:
        {
            return samplePolyPatch().size();
        }
        case NEARESTFACE:
        {
            const polyMesh& mesh = sampleMesh();
            return mesh.nFaces() - mesh.nInternalFaces();
        }
        default:
        {
            FatalErrorInFunction
                << "problem." << abort(FatalError);
            return -1;
        }
    }
}


inline const CML::vector& CML::mappedPatchBase::offset() const
{
    return offset_;
}


inline const CML::vectorField& CML::mappedPatchBase::offsets() const
{
    return offsets_;
}


inline bool CML::mappedPatchBase::sameRegion() const
{
    return sameRegion_;
}


inline const CML::mapDistribute& CML::mappedPatchBase::map() const
{
    if (mapPtr_.empty())
    {
        calcMapping();
    }

    return mapPtr_();
}


inline const CML::AMIPatchToPatchInterpolation& CML::mappedPatchBase::AMI
(
    bool forceUpdate
) const
{
    if (forceUpdate || AMIPtr_.empty())
    {
        calcAMI();
    }

    return AMIPtr_();
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void CML::mappedPatchBase::distribute(List<Type>& lst) const
{
    switch (mode_)
    {
        case NEARESTPATCHFACEAMI:
        {
            lst = AMI().interpolateToSource(Field<Type>(lst.xfer()));
            break;
        }
        default:
        {
            map().distribute(lst);
        }
    }
}


template<class Type, class CombineOp>
void CML::mappedPatchBase::distribute
(
    List<Type>& lst,
    const CombineOp& cop
) const
{
    switch (mode_)
    {
        case NEARESTPATCHFACEAMI:
        {
            lst = AMI().interpolateToSource
                (
                    Field<Type>(lst.xfer()),
                    cop
                );
            break;
        }
        default:
        {
            mapDistributeBase::distribute
            (
                Pstream::defaultCommsType,
                map().schedule(),
                map().constructSize(),
                map().subMap(),
                false,
                map().constructMap(),
                false,
                lst,
                cop,
                flipOp(),
                Type(Zero)
            );
        }
    }
}


template<class Type>
void CML::mappedPatchBase::reverseDistribute(List<Type>& lst) const
{
    switch (mode_)
    {
        case NEARESTPATCHFACEAMI:
        {
            lst = AMI().interpolateToTarget(Field<Type>(lst.xfer()));
            break;
        }
        default:
        {
            map().reverseDistribute(sampleSize(), lst);
            break;
        }
    }
}


template<class Type, class CombineOp>
void CML::mappedPatchBase::reverseDistribute
(
    List<Type>& lst,
    const CombineOp& cop
) const
{
    switch (mode_)
    {
        case NEARESTPATCHFACEAMI:
        {
            lst = AMI().interpolateToTarget
                (
                    Field<Type>(lst.xfer()),
                    cop
                );
            break;
        }
        default:
        {
            label cSize = sampleSize();
            mapDistributeBase::distribute
            (
                Pstream::defaultCommsType,
                map().schedule(),
                cSize,
                map().constructMap(),
                false,
                map().subMap(),
                false,
                lst,
                cop,
                flipOp(),
                Type(Zero)
            );
            break;
        }
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
