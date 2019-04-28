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
    CML::PatchToPatchInterpolation

Description
    Interpolation class dealing with transfer of data between two
    primitivePatches

SourceFiles
    PatchToPatchInterpolation.cpp
    PatchToPatchInterpolate.cpp
    CalcPatchToPatchWeights.cpp

\*---------------------------------------------------------------------------*/

#ifndef PatchToPatchInterpolation__H
#define PatchToPatchInterpolation__H

#include "className.hpp"
#include "labelList.hpp"
#include "scalarField.hpp"
#include "pointField.hpp"
#include "FieldFields.hpp"
#include "faceList.hpp"
#include "intersection.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                Class PatchToPatchInterpolationName Declaration
\*---------------------------------------------------------------------------*/

TemplateName(PatchToPatchInterpolation);


/*---------------------------------------------------------------------------*\
                  Class PatchToPatchInterpolation Declaration
\*---------------------------------------------------------------------------*/

template<class FromPatch, class ToPatch>
class PatchToPatchInterpolation
:
    public PatchToPatchInterpolationName
{
    // Private data

        //- Reference to the source patch
        const FromPatch& fromPatch_;

        //- Reference to the target patch
        const ToPatch& toPatch_;

        //- Type of intersection algorithm to use in projection
        intersection::algorithm alg_;

        //- Direction projection to use in projection
        intersection::direction dir_;


    // Static data

        //- Relative merge tolerance for projected points missing the target
        //  Expressed as the fraction of min involved edge size
        static scalar projectionTol_;


        // Point addressing

            //- Face into which each point of target patch is projected
            mutable labelList* pointAddressingPtr_;

            //- Weighting factors
            mutable FieldField<Field, scalar>* pointWeightsPtr_;

            //- Distance to intersection for patch points
            mutable scalarField* pointDistancePtr_;

        // Face addressing

            //- Face into which each face centre of target patch is projected
            mutable labelList* faceAddressingPtr_;

            //- Weighting factors
            mutable FieldField<Field, scalar>* faceWeightsPtr_;

            //- Distance to intersection for patch face centres
            mutable scalarField* faceDistancePtr_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        PatchToPatchInterpolation(const PatchToPatchInterpolation&);

        //- Disallow default bitwise assignment
        void operator=(const PatchToPatchInterpolation&);

        //- Calculate point weights
        void calcPointAddressing() const;

        //- Calculate face weights
        void calcFaceAddressing() const;

        //- Clear all geometry and addressing
        void clearOut();


        //- Return reference to point addressing
        const labelList& pointAddr() const;

        //- Return reference to point weights
        const FieldField<Field, scalar>& pointWeights() const;

        //- Return reference to face addressing
        const labelList& faceAddr() const;

        //- Return reference to face weights
        const FieldField<Field, scalar>& faceWeights() const;


    // Private static data members

        //- Direct hit tolerance
        static const scalar directHitTol;


public:

    // Constructors

        //- Construct from components
        PatchToPatchInterpolation
        (
            const FromPatch& fromPatch,
            const ToPatch& toPatch,
            const intersection::algorithm alg = intersection::FULL_RAY,
            const intersection::direction dir = intersection::VECTOR
        );


    //- Destructor
    ~PatchToPatchInterpolation();


    // Member Functions

        //- Set the projection tolerance, returning the previous value
        static scalar setProjectionTol(const scalar t)
        {
            if (t < -VSMALL)
            {
                FatalErrorInFunction
                    << "Negative projection tolerance.  This is not allowed."
                    << abort(FatalError);
            }

            scalar oldTol = projectionTol_;
            projectionTol_ = t;

            return oldTol;
        }

        //- Return ype of intersection algorithm to use in projection
        intersection::algorithm projectionAlgo() const
        {
            return alg_;
        }

        //- Return direction projection to use in projection
        intersection::direction projectionDir() const
        {
            return dir_;
        }

        //- Return distance to intersection for patch points
        const scalarField& pointDistanceToIntersection() const;

        //- Return distance to intersection for patch face centres
        const scalarField& faceDistanceToIntersection() const;

        //- Correct weighting factors for moving mesh.
        bool movePoints();


        //- Interpolate point field
        template<class Type>
        tmp<Field<Type> > pointInterpolate(const Field<Type>& pf) const;

        template<class Type>
        tmp<Field<Type> > pointInterpolate(const tmp<Field<Type> >& tpf) const;

        //- Interpolate face field
        template<class Type>
        tmp<Field<Type> > faceInterpolate(const Field<Type>& pf) const;

        template<class Type>
        tmp<Field<Type> > faceInterpolate(const tmp<Field<Type> >& tpf) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "demandDrivenData.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class FromPatch, class ToPatch>
const scalar
PatchToPatchInterpolation<FromPatch, ToPatch>::directHitTol = 1e-5;

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class FromPatch, class ToPatch>
const labelList&
PatchToPatchInterpolation<FromPatch, ToPatch>::pointAddr() const
{
    if (!pointAddressingPtr_)
    {
        calcPointAddressing();
    }

    return *pointAddressingPtr_;
}


template<class FromPatch, class ToPatch>
const FieldField<Field, scalar>&
PatchToPatchInterpolation<FromPatch, ToPatch>::pointWeights() const
{
    if (!pointWeightsPtr_)
    {
        calcPointAddressing();
    }

    return *pointWeightsPtr_;
}


template<class FromPatch, class ToPatch>
const labelList&
PatchToPatchInterpolation<FromPatch, ToPatch>::faceAddr() const
{
    if (!faceAddressingPtr_)
    {
        calcFaceAddressing();
    }

    return *faceAddressingPtr_;
}


template<class FromPatch, class ToPatch>
const FieldField<Field, scalar>&
PatchToPatchInterpolation<FromPatch, ToPatch>::faceWeights() const
{
    if (!faceWeightsPtr_)
    {
        calcFaceAddressing();
    }

    return *faceWeightsPtr_;
}


template<class FromPatch, class ToPatch>
void PatchToPatchInterpolation<FromPatch, ToPatch>::clearOut()
{
    deleteDemandDrivenData(pointAddressingPtr_);
    deleteDemandDrivenData(pointWeightsPtr_);
    deleteDemandDrivenData(pointDistancePtr_);
    deleteDemandDrivenData(faceAddressingPtr_);
    deleteDemandDrivenData(faceWeightsPtr_);
    deleteDemandDrivenData(faceDistancePtr_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
template<class FromPatch, class ToPatch>
PatchToPatchInterpolation<FromPatch, ToPatch>::PatchToPatchInterpolation
(
    const FromPatch& fromPatch,
    const ToPatch& toPatch,
    intersection::algorithm alg,
    const intersection::direction dir
)
:
    fromPatch_(fromPatch),
    toPatch_(toPatch),
    alg_(alg),
    dir_(dir),
    pointAddressingPtr_(nullptr),
    pointWeightsPtr_(nullptr),
    pointDistancePtr_(nullptr),
    faceAddressingPtr_(nullptr),
    faceWeightsPtr_(nullptr),
    faceDistancePtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor * * * * * * * * * * * * * * * //

template<class FromPatch, class ToPatch>
PatchToPatchInterpolation<FromPatch, ToPatch>::~PatchToPatchInterpolation()
{
    clearOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class FromPatch, class ToPatch>
const scalarField&
PatchToPatchInterpolation<FromPatch, ToPatch>
::pointDistanceToIntersection() const
{
    if (!pointDistancePtr_)
    {
        calcPointAddressing();
    }

    return *pointDistancePtr_;
}


template<class FromPatch, class ToPatch>
const scalarField&
PatchToPatchInterpolation<FromPatch, ToPatch>
::faceDistanceToIntersection() const
{
    if (!faceDistancePtr_)
    {
        calcFaceAddressing();
    }

    return *faceDistancePtr_;
}


template<class FromPatch, class ToPatch>
bool PatchToPatchInterpolation<FromPatch, ToPatch>::movePoints()
{
    clearOut();

    return true;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "objectHit.hpp"
#include "pointHit.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class FromPatch, class ToPatch>
scalar PatchToPatchInterpolation<FromPatch, ToPatch>::projectionTol_ = 0.05;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class FromPatch, class ToPatch>
void PatchToPatchInterpolation<FromPatch, ToPatch>::calcPointAddressing() const
{
    // Calculate pointWeights

    pointWeightsPtr_ = new FieldField<Field, scalar>(toPatch_.nPoints());
    FieldField<Field, scalar>& pointWeights = *pointWeightsPtr_;

    pointDistancePtr_ = new scalarField(toPatch_.nPoints(), GREAT);
    scalarField& pointDistance = *pointDistancePtr_;

    const pointField& fromPatchPoints = fromPatch_.localPoints();
    const List<typename FromPatch::FaceType>& fromPatchFaces =
        fromPatch_.localFaces();

    const pointField& toPatchPoints = toPatch_.localPoints();
    const vectorField& projectionDirection = toPatch_.pointNormals();
    const edgeList& toPatchEdges = toPatch_.edges();
    const labelListList& toPatchPointEdges = toPatch_.pointEdges();

    if (debug)
    {
        Info<< "projecting points" << endl;
    }

    List<objectHit> proj =
        toPatch_.projectPoints(fromPatch_, projectionDirection, alg_, dir_);

    pointAddressingPtr_ = new labelList(proj.size(), -1);
    labelList& pointAddressing = *pointAddressingPtr_;

    bool doWeights = false;

    forAll(pointAddressing, pointI)
    {
        doWeights = false;

        const typename FromPatch::FaceType& hitFace =
            fromPatchFaces[proj[pointI].hitObject()];

        point hitPoint = point::zero;

        if (proj[pointI].hit())
        {
            // A hit exists
            doWeights = true;

            pointAddressing[pointI] = proj[pointI].hitObject();

            pointHit curHit =
                hitFace.ray
                (
                    toPatchPoints[pointI],
                    projectionDirection[pointI],
                    fromPatchPoints,
                    alg_,
                    dir_
                );

            // Grab distance to target
            if (dir_ == intersection::CONTACT_SPHERE)
            {
                pointDistance[pointI] =
                    hitFace.contactSphereDiameter
                    (
                        toPatchPoints[pointI],
                        projectionDirection[pointI],
                        fromPatchPoints
                    );
            }
            else
            {
                pointDistance[pointI] = curHit.distance();
            }

            // Grab hit point
            hitPoint = curHit.hitPoint();
        }
        else if (projectionTol_ > SMALL)
        {
            // Check for a near miss
            pointHit ph =
                hitFace.ray
                (
                    toPatchPoints[pointI],
                    projectionDirection[pointI],
                    fromPatchPoints,
                    alg_,
                    dir_
                );

            scalar dist =
                CML::mag
                (
                    toPatchPoints[pointI]
                  + projectionDirection[pointI]*ph.distance()
                  - ph.missPoint()
                );

            // Calculate the local tolerance
            scalar minEdgeLength = GREAT;

            // Do shortest edge of hit object
            edgeList hitFaceEdges =
                fromPatchFaces[proj[pointI].hitObject()].edges();

            forAll(hitFaceEdges, edgeI)
            {
                minEdgeLength =
                    min
                    (
                        minEdgeLength,
                        hitFaceEdges[edgeI].mag(fromPatchPoints)
                    );
            }

            const labelList& curEdges = toPatchPointEdges[pointI];

            forAll(curEdges, edgeI)
            {
                minEdgeLength =
                    min
                    (
                        minEdgeLength,
                        toPatchEdges[curEdges[edgeI]].mag(toPatchPoints)
                    );
            }

            if (dist < minEdgeLength*projectionTol_)
            {
                // This point is being corrected
                doWeights = true;

                pointAddressing[pointI] = proj[pointI].hitObject();

                // Grab nearest point on face as hit point
                hitPoint = ph.missPoint();

                // Grab distance to target
                if (dir_ == intersection::CONTACT_SPHERE)
                {
                    pointDistance[pointI] =
                        hitFace.contactSphereDiameter
                        (
                            toPatchPoints[pointI],
                            projectionDirection[pointI],
                            fromPatchPoints
                        );
                }
                else
                {
                    pointDistance[pointI] =
                        (
                            projectionDirection[pointI]
                            /mag(projectionDirection[pointI])
                        )
                      & (hitPoint - toPatchPoints[pointI]);
                }
            }
        }

        if (doWeights)
        {
            // Set interpolation pointWeights
            pointWeights.set(pointI, new scalarField(hitFace.size()));

            pointField hitFacePoints = hitFace.points(fromPatchPoints);

            forAll(hitFacePoints, masterPointI)
            {
                pointWeights[pointI][masterPointI] =
                    1.0/
                    (
                        mag
                        (
                            hitFacePoints[masterPointI]
                          - hitPoint
                        )
                      + VSMALL
                    );
            }

            pointWeights[pointI] /= sum(pointWeights[pointI]);
        }
        else
        {
            pointWeights.set(pointI, new scalarField(0));
        }
    }
}


template<class FromPatch, class ToPatch>
void PatchToPatchInterpolation<FromPatch, ToPatch>::calcFaceAddressing() const
{
    faceWeightsPtr_ = new FieldField<Field, scalar>(toPatch_.size());
    FieldField<Field, scalar>& faceWeights = *faceWeightsPtr_;

    faceDistancePtr_ = new scalarField(toPatch_.size(), GREAT);
    scalarField& faceDistance = *faceDistancePtr_;

    if (debug)
    {
        Info<< "projecting face centres" << endl;
    }

    const pointField& fromPatchPoints = fromPatch_.points();
    const typename FromPatch::FaceListType& fromPatchFaces = fromPatch_;
    const labelListList& fromPatchFaceFaces = fromPatch_.faceFaces();

    vectorField fromPatchFaceCentres(fromPatchFaces.size());

    forAll(fromPatchFaceCentres, faceI)
    {
        fromPatchFaceCentres[faceI] =
            fromPatchFaces[faceI].centre(fromPatchPoints);
    }

    const pointField& toPatchPoints = toPatch_.points();
    const typename ToPatch::FaceListType& toPatchFaces = toPatch_;

    const vectorField& projectionDirection = toPatch_.faceNormals();

    List<objectHit> proj =
        toPatch_.projectFaceCentres
        (
            fromPatch_,
            projectionDirection,
            alg_,
            dir_
        );

    faceAddressingPtr_ = new labelList(proj.size(), -1);
    labelList& faceAddressing = *faceAddressingPtr_;

    forAll(faceAddressing, faceI)
    {
        if (proj[faceI].hit())
        {
            // A hit exists
            faceAddressing[faceI] = proj[faceI].hitObject();

            const typename FromPatch::FaceType& hitFace =
                fromPatchFaces[faceAddressing[faceI]];

            pointHit curHit =
                hitFace.ray
                (
                    toPatchFaces[faceI].centre(toPatchPoints),
                    projectionDirection[faceI],
                    fromPatchPoints,
                    alg_,
                    dir_
                );

            // grab distance to target
            faceDistance[faceI] = curHit.distance();

            // grab face centre of the hit face
            const point& hitFaceCentre =
                fromPatchFaceCentres[faceAddressing[faceI]];

            // grab neighbours of hit face
            const labelList& neighbours =
                fromPatchFaceFaces[faceAddressing[faceI]];

            scalar m = mag(curHit.hitPoint() - hitFaceCentre);

            if
            (
                m < directHitTol                            // Direct hit
             || neighbours.empty()
            )
            {
                faceWeights.set(faceI, new scalarField(1));
                faceWeights[faceI][0] = 1.0;
            }
            else
            {
                // set interpolation faceWeights

                // The first coefficient corresponds to the centre face.
                // The rest is ordered in the same way as the faceFaces list.
                faceWeights.set(faceI, new scalarField(neighbours.size() + 1));

                faceWeights[faceI][0] = 1.0/m;

                forAll(neighbours, nI)
                {
                    faceWeights[faceI][nI + 1] =
                    1.0/
                    (
                        mag
                        (
                            fromPatchFaceCentres[neighbours[nI]]
                          - curHit.hitPoint()
                        )
                      + VSMALL
                    );
                }
            }

            faceWeights[faceI] /= sum(faceWeights[faceI]);
        }
        else
        {
            faceWeights.set(faceI, new scalarField(0));
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

//- Interpolate point field
template<class FromPatch, class ToPatch>
template<class Type>
tmp<Field<Type> >
PatchToPatchInterpolation<FromPatch, ToPatch>::pointInterpolate
(
    const Field<Type>& pf
) const
{
    if (pf.size() != fromPatch_.nPoints())
    {
        FatalErrorInFunction
            << "given field does not correspond to patch. Patch size: "
            << fromPatch_.nPoints() << " field size: " << pf.size()
            << abort(FatalError);
    }

    tmp<Field<Type> > tresult
    (
        new Field<Type>
        (
            toPatch_.nPoints(),
            pTraits<Type>::zero
        )
    );

    Field<Type>& result = tresult();

    const List<typename FromPatch::FaceType>& fromPatchLocalFaces =
        fromPatch_.localFaces();

    const FieldField<Field, scalar>& weights = pointWeights();

    const labelList& addr = pointAddr();

    forAll(result, pointI)
    {
        const scalarField& curWeights = weights[pointI];

        if (addr[pointI] > -1)
        {
            const labelList& hitFacePoints =
                fromPatchLocalFaces[addr[pointI]];

            forAll(curWeights, wI)
            {
                result[pointI] += curWeights[wI]*pf[hitFacePoints[wI]];
            }
        }
    }

    return tresult;
}


template<class FromPatch, class ToPatch>
template<class Type>
tmp<Field<Type> >
PatchToPatchInterpolation<FromPatch, ToPatch>::pointInterpolate
(
    const tmp<Field<Type> >& tpf
) const
{
    tmp<Field<Type> > tint = pointInterpolate<Type>(tpf());
    tpf.clear();
    return tint;
}


//- Interpolate face field
template<class FromPatch, class ToPatch>
template<class Type>
tmp<Field<Type> >
PatchToPatchInterpolation<FromPatch, ToPatch>::faceInterpolate
(
    const Field<Type>& ff
) const
{
    if (ff.size() != fromPatch_.size())
    {
        FatalErrorInFunction
            << "given field does not correspond to patch. Patch size: "
            << fromPatch_.size() << " field size: " << ff.size()
            << abort(FatalError);
    }

    tmp<Field<Type> > tresult
    (
        new Field<Type>
        (
            toPatch_.size(),
            pTraits<Type>::zero
        )
    );

    Field<Type>& result = tresult();

    const labelListList& fromPatchFaceFaces = fromPatch_.faceFaces();

    const FieldField<Field, scalar>& weights = faceWeights();

    const labelList& addr = faceAddr();

    forAll(result, faceI)
    {
        const scalarField& curWeights = weights[faceI];

        if (addr[faceI] > -1)
        {
            const labelList& hitFaceFaces =
                fromPatchFaceFaces[addr[faceI]];

            // first add the hit face
            result[faceI] += ff[addr[faceI]]*curWeights[0];

            for (label wI = 1; wI < curWeights.size(); wI++)
            {
                result[faceI] += ff[hitFaceFaces[wI - 1]]*curWeights[wI];
            }
        }
    }

    return tresult;
}


template<class FromPatch, class ToPatch>
template<class Type>
tmp<Field<Type> >
PatchToPatchInterpolation<FromPatch, ToPatch>::faceInterpolate
(
    const tmp<Field<Type> >& tff
) const
{
    tmp<Field<Type> > tint = faceInterpolate(tff());
    tff.clear();
    return tint;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
