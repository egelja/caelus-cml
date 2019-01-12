/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2015 OpenFOAM Foundation
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
    CML::AMIMethod

Description
    Base class for Arbitrary Mesh Interface (AMI) methods


\*---------------------------------------------------------------------------*/

#ifndef AMIMethod_H
#define AMIMethod_H

#include "className.hpp"
#include "DynamicList.hpp"
#include "faceAreaIntersect.hpp"
#include "indexedOctree.hpp"
#include "treeDataPrimitivePatch.hpp"
#include "treeBoundBoxList.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                          Class AMIMethod Declaration
\*---------------------------------------------------------------------------*/

template<class SourcePatch, class TargetPatch>
class AMIMethod
{

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        AMIMethod(const AMIMethod&);

        //- Disallow default bitwise assignment
        void operator=(const AMIMethod&);


protected:

    //- Local typedef to octree tree-type
    typedef treeDataPrimitivePatch<TargetPatch> treeType;


    // Protected data

        //- Reference to source patch
        const SourcePatch& srcPatch_;

        //- Reference to target patch
        const TargetPatch& tgtPatch_;

        //- Flag to indicate that the two patches are co-directional and
        //  that the orientation of the target patch should be reversed
        const bool reverseTarget_;

        //- Flag to indicate that the two patches must be matched/an overlap
        //  exists between them
        const bool requireMatch_;

        //- Source face areas
        const scalarField& srcMagSf_;

        //- Target face areas
        const scalarField& tgtMagSf_;

        //- Labels of faces that are not overlapped by any target faces
        //  (should be empty for correct functioning)
        labelList srcNonOverlap_;

        //- Octree used to find face seeds
        autoPtr<indexedOctree<treeType> > treePtr_;

        //- Face triangulation mode
        const faceAreaIntersect::triangulationMode triMode_;


    // Protected Member Functions

        // Helper functions

            //- Check AMI patch coupling
            void checkPatches() const;

            //- Initialise and return true if all ok
            bool initialise
            (
                labelListList& srcAddress,
                scalarListList& srcWeights,
                labelListList& tgtAddress,
                scalarListList& tgtWeights,
                label& srcFaceI,
                label& tgtFaceI
            );

            //- Write triangle intersection to OBJ file
            void writeIntersectionOBJ
            (
                const scalar area,
                const face& f1,
                const face& f2,
                const pointField& f1Points,
                const pointField& f2Points
            ) const;


        // Common AMI method functions

            //- Reset the octree for the target patch face search
            void resetTree();

            //- Find face on target patch that overlaps source face
            label findTargetFace(const label srcFaceI) const;

            //- Add faces neighbouring faceI to the ID list
            void appendNbrFaces
            (
                const label faceI,
                const TargetPatch& patch,
                const DynamicList<label>& visitedFaces,
                DynamicList<label>& faceIDs
            ) const;


public:

    //- Runtime type information
    TypeName("AMIMethod");

    //- Declare runtime constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        AMIMethod,
        components,
        (
            const SourcePatch& srcPatch,
            const TargetPatch& tgtPatch,
            const scalarField& srcMagSf,
            const scalarField& tgtMagSf,
            const faceAreaIntersect::triangulationMode& triMode,
            const bool reverseTarget,
            const bool requireMatch
        ),
        (
            srcPatch,
            tgtPatch,
            srcMagSf,
            tgtMagSf,
            triMode,
            reverseTarget,
            requireMatch
        )
    );

    //- Construct from components
    AMIMethod
    (
        const SourcePatch& srcPatch,
        const TargetPatch& tgtPatch,
        const scalarField& srcMagSf,
        const scalarField& tgtMagSf,
        const faceAreaIntersect::triangulationMode& triMode,
        const bool reverseTarget,
        const bool requireMatch
    );

    //- Selector
    static autoPtr<AMIMethod> New
    (
        const word& methodName,
        const SourcePatch& srcPatch,
        const TargetPatch& tgtPatch,
        const scalarField& srcMagSf,
        const scalarField& tgtMagSf,
        const faceAreaIntersect::triangulationMode& triMode,
        const bool reverseTarget,
        const bool requireMatch
    );


    //- Destructor
    virtual ~AMIMethod();


    // Member Functions

        // Access

            //- Labels of faces that are not overlapped by any target faces
            //  Note: this should be empty for correct functioning
            inline const labelList& srcNonOverlap() const;

            //- Flag to indicate that interpolation patches are conformal
            virtual bool conformal() const;


        // Manipulation

            //- Update addressing and weights
            virtual void calculate
            (
                labelListList& srcAddress,
                scalarListList& srcWeights,
                labelListList& tgtAddress,
                scalarListList& tgtWeights,
                label srcFaceI = -1,
                label tgtFaceI = -1
            ) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeAMIMethod(AMIType)                                                \
                                                                              \
    typedef AMIMethod<AMIType::sourcePatchType,AMIType::targetPatchType>      \
        AMIMethod##AMIType;                                                   \
                                                                              \
    defineNamedTemplateTypeNameAndDebug(AMIMethod##AMIType, 0);               \
    defineTemplateRunTimeSelectionTable(AMIMethod##AMIType, components);


#define makeAMIMethodType(AMIType, Method)                                    \
                                                                              \
    typedef Method<AMIType::sourcePatchType,AMIType::targetPatchType>         \
        Method##AMIType;                                                      \
                                                                              \
    defineNamedTemplateTypeNameAndDebug(Method##AMIType, 0);                  \
                                                                              \
    AMIMethod<AMIType::sourcePatchType,AMIType::targetPatchType>::            \
        addcomponentsConstructorToTable<Method##AMIType>                      \
        add##Method##AMIType##ConstructorToTable_;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


template<class SourcePatch, class TargetPatch>
inline const CML::labelList&
CML::AMIMethod<SourcePatch, TargetPatch>::srcNonOverlap() const
{
    return srcNonOverlap_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "meshTools.hpp"
#include "mapDistribute.hpp"
#include "unitConversion.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class SourcePatch, class TargetPatch>
void CML::AMIMethod<SourcePatch, TargetPatch>::checkPatches() const
{
    if (debug && (!srcPatch_.size() || !tgtPatch_.size()))
    {
        Pout<< "AMI: Patches not on processor: Source faces = "
            << srcPatch_.size() << ", target faces = " << tgtPatch_.size()
            << endl;
    }


    if (conformal())
    {
        const scalar maxBoundsError = 0.05;

        // check bounds of source and target
        boundBox bbSrc(srcPatch_.points(), srcPatch_.meshPoints(), true);
        boundBox bbTgt(tgtPatch_.points(), tgtPatch_.meshPoints(), true);

        boundBox bbTgtInf(bbTgt);
        bbTgtInf.inflate(maxBoundsError);

        if (!bbTgtInf.contains(bbSrc))
        {
            WarningInFunction
                << "Source and target patch bounding boxes are not similar"
                << nl
                << "    source box span     : " << bbSrc.span() << nl
                << "    target box span     : " << bbTgt.span() << nl
                << "    source box          : " << bbSrc << nl
                << "    target box          : " << bbTgt << nl
                << "    inflated target box : " << bbTgtInf << endl;
        }
    }
}


template<class SourcePatch, class TargetPatch>
bool CML::AMIMethod<SourcePatch, TargetPatch>::initialise
(
    labelListList& srcAddress,
    scalarListList& srcWeights,
    labelListList& tgtAddress,
    scalarListList& tgtWeights,
    label& srcFaceI,
    label& tgtFaceI
)
{
    checkPatches();

    // set initial sizes for weights and addressing - must be done even if
    // returns false below
    srcAddress.setSize(srcPatch_.size());
    srcWeights.setSize(srcPatch_.size());
    tgtAddress.setSize(tgtPatch_.size());
    tgtWeights.setSize(tgtPatch_.size());

    // check that patch sizes are valid
    if (!srcPatch_.size())
    {
        return false;
    }
    else if (!tgtPatch_.size())
    {
        WarningInFunction
            << srcPatch_.size() << " source faces but no target faces" << endl;

        return false;
    }

    // reset the octree
    resetTree();

    // find initial face match using brute force/octree search
    if ((srcFaceI == -1) || (tgtFaceI == -1))
    {
        srcFaceI = 0;
        tgtFaceI = 0;
        bool foundFace = false;
        forAll(srcPatch_, faceI)
        {
            tgtFaceI = findTargetFace(faceI);
            if (tgtFaceI >= 0)
            {
                srcFaceI = faceI;
                foundFace = true;
                break;
            }
        }

        if (!foundFace)
        {
            if (requireMatch_)
            {
                FatalErrorInFunction
                    << "Unable to find initial target face"
                    << abort(FatalError);
            }

            return false;
        }
    }

    if (debug)
    {
        Pout<< "AMI: initial target face = " << tgtFaceI << endl;
    }

    return true;
}


template<class SourcePatch, class TargetPatch>
void CML::AMIMethod<SourcePatch, TargetPatch>::writeIntersectionOBJ
(
    const scalar area,
    const face& f1,
    const face& f2,
    const pointField& f1Points,
    const pointField& f2Points
) const
{
    static label count = 1;

    const pointField f1pts = f1.points(f1Points);
    const pointField f2pts = f2.points(f2Points);

    Pout<< "Face intersection area (" << count <<  "):" << nl
        << "    f1 face = " << f1 << nl
        << "    f1 pts  = " << f1pts << nl
        << "    f2 face = " << f2 << nl
        << "    f2 pts  = " << f2pts << nl
        << "    area    = " << area
        << endl;

    OFstream os("areas" + name(count) + ".obj");

    forAll(f1pts, i)
    {
        meshTools::writeOBJ(os, f1pts[i]);
    }
    os<< "l";
    forAll(f1pts, i)
    {
        os<< " " << i + 1;
    }
    os<< " 1" << endl;


    forAll(f2pts, i)
    {
        meshTools::writeOBJ(os, f2pts[i]);
    }
    os<< "l";
    forAll(f2pts, i)
    {
        os<< " " << f1pts.size() + i + 1;
    }
    os<< " " << f1pts.size() + 1 << endl;

    count++;
}


template<class SourcePatch, class TargetPatch>
void CML::AMIMethod<SourcePatch, TargetPatch>::resetTree()
{
    // Clear the old octree
    treePtr_.clear();

    treeBoundBox bb(tgtPatch_.points());
    bb.inflate(0.01);

    if (!treePtr_.valid())
    {
        treePtr_.reset
        (
            new indexedOctree<treeType>
            (
                treeType
                (
                    false,
                    tgtPatch_,
                    indexedOctree<treeType>::perturbTol()
                ),
                bb,                         // overall search domain
                8,                          // maxLevel
                10,                         // leaf size
                3.0                         // duplicity
            )
        );
    }
}


template<class SourcePatch, class TargetPatch>
CML::label CML::AMIMethod<SourcePatch, TargetPatch>::findTargetFace
(
    const label srcFaceI
) const
{
    label targetFaceI = -1;

    const pointField& srcPts = srcPatch_.points();
    const face& srcFace = srcPatch_[srcFaceI];
    const point srcPt = srcFace.centre(srcPts);
    const scalar srcFaceArea = srcMagSf_[srcFaceI];

    pointIndexHit sample = treePtr_->findNearest(srcPt, 10.0*srcFaceArea);

    if (sample.hit())
    {
        targetFaceI = sample.index();

        if (debug)
        {
            Pout<< "Source point = " << srcPt << ", Sample point = "
                << sample.hitPoint() << ", Sample index = " << sample.index()
                << endl;
        }
    }

    return targetFaceI;
}


template<class SourcePatch, class TargetPatch>
void CML::AMIMethod<SourcePatch, TargetPatch>::appendNbrFaces
(
    const label faceI,
    const TargetPatch& patch,
    const DynamicList<label>& visitedFaces,
    DynamicList<label>& faceIDs
) const
{
    const labelList& nbrFaces = patch.faceFaces()[faceI];

    // filter out faces already visited from face neighbours
    forAll(nbrFaces, i)
    {
        label nbrFaceI = nbrFaces[i];
        bool valid = true;
        forAll(visitedFaces, j)
        {
            if (nbrFaceI == visitedFaces[j])
            {
                valid = false;
                break;
            }
        }

        if (valid)
        {
            forAll(faceIDs, j)
            {
                if (nbrFaceI == faceIDs[j])
                {
                    valid = false;
                    break;
                }
            }
        }

        // prevent addition of face if it is not on the same plane-ish
        if (valid)
        {
            const vector& n1 = patch.faceNormals()[faceI];
            const vector& n2 = patch.faceNormals()[nbrFaceI];

            scalar cosI = n1 & n2;

            if (cosI > CML::cos(degToRad(89.0)))
            {
                faceIDs.append(nbrFaceI);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class SourcePatch, class TargetPatch>
CML::AMIMethod<SourcePatch, TargetPatch>::AMIMethod
(
    const SourcePatch& srcPatch,
    const TargetPatch& tgtPatch,
    const scalarField& srcMagSf,
    const scalarField& tgtMagSf,
    const faceAreaIntersect::triangulationMode& triMode,
    const bool reverseTarget,
    const bool requireMatch
)
:
    srcPatch_(srcPatch),
    tgtPatch_(tgtPatch),
    reverseTarget_(reverseTarget),
    requireMatch_(requireMatch),
    srcMagSf_(srcMagSf),
    tgtMagSf_(tgtMagSf),
    srcNonOverlap_(),
    triMode_(triMode)
{}


// * * * * * * * * * * * * * * * * Destructor * * * * * * * * * * * * * * * //

template<class SourcePatch, class TargetPatch>
CML::AMIMethod<SourcePatch, TargetPatch>::~AMIMethod()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class SourcePatch, class TargetPatch>
bool CML::AMIMethod<SourcePatch, TargetPatch>::conformal() const
{
    return true;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class SourcePatch, class TargetPatch>
CML::autoPtr<CML::AMIMethod<SourcePatch, TargetPatch> >
CML::AMIMethod<SourcePatch, TargetPatch>::New
(
    const word& methodName,
    const SourcePatch& srcPatch,
    const TargetPatch& tgtPatch,
    const scalarField& srcMagSf,
    const scalarField& tgtMagSf,
    const faceAreaIntersect::triangulationMode& triMode,
    const bool reverseTarget,
    const bool requireMatch
)
{
    if (debug)
    {
        Info<< "Selecting AMIMethod " << methodName << endl;
    }

    typename componentsConstructorTable::iterator cstrIter =
        componentsConstructorTablePtr_->find(methodName);

    if (cstrIter == componentsConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown AMIMethod type "
            << methodName << nl << nl
            << "Valid AMIMethod types are:" << nl
            << componentsConstructorTablePtr_->sortedToc() << exit(FatalError);
    }

    return autoPtr<AMIMethod<SourcePatch, TargetPatch> >
    (
        cstrIter()
        (
            srcPatch,
            tgtPatch,
            srcMagSf,
            tgtMagSf,
            triMode,
            reverseTarget,
            requireMatch
        )
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
