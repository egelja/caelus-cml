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

\*---------------------------------------------------------------------------*/

#include "sampledSet.hpp"
#include "polyMesh.hpp"
#include "primitiveMesh.hpp"
#include "meshSearch.hpp"
#include "writer.hpp"
#include "particle.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    const scalar sampledSet::tol = 1e-6;

    defineTypeNameAndDebug(sampledSet, 0);
    defineRunTimeSelectionTable(sampledSet, word);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::label CML::sampledSet::getBoundaryCell(const label facei) const
{
    return mesh().faceOwner()[facei];
}


CML::label CML::sampledSet::getCell
(
    const label facei,
    const point& sample
) const
{
    if (facei == -1)
    {
        FatalErrorInFunction
            << "Illegal face label " << facei
            << abort(FatalError);
    }

    if (facei >= mesh().nInternalFaces())
    {
        label celli = getBoundaryCell(facei);

        if (!mesh().pointInCell(sample, celli, searchEngine_.decompMode()))
        {
            FatalErrorInFunction
                << "Found cell " << celli << " using face " << facei
                << ". But cell does not contain point " << sample
                << abort(FatalError);
        }
        return celli;
    }
    else
    {
        // Try owner and neighbour to see which one contains sample

        label celli = mesh().faceOwner()[facei];

        if (mesh().pointInCell(sample, celli, searchEngine_.decompMode()))
        {
            return celli;
        }
        else
        {
            celli = mesh().faceNeighbour()[facei];

            if (mesh().pointInCell(sample, celli, searchEngine_.decompMode()))
            {
                return celli;
            }
            else
            {
                FatalErrorInFunction
                    << "None of the neighbours of face "
                    << facei << " contains point " << sample
                    << abort(FatalError);

                return -1;
            }
        }
    }
}


CML::scalar CML::sampledSet::calcSign
(
    const label facei,
    const point& sample
) const
{
    vector vec = sample - mesh().faceCentres()[facei];

    scalar magVec = mag(vec);

    if (magVec < VSMALL)
    {
        // Sample on face centre. Regard as inside
        return -1;
    }

    vec /= magVec;

    vector n = mesh().faceAreas()[facei];

    n /= mag(n) + VSMALL;

    return n & vec;
}


// Return face (or -1) of face which is within smallDist of sample
CML::label CML::sampledSet::findNearFace
(
    const label celli,
    const point& sample,
    const scalar smallDist
) const
{
    const cell& myFaces = mesh().cells()[celli];

    forAll(myFaces, myFacei)
    {
        const face& f = mesh().faces()[myFaces[myFacei]];

        pointHit inter = f.nearestPoint(sample, mesh().points());

        scalar dist;

        if (inter.hit())
        {
            dist = mag(inter.hitPoint() - sample);
        }
        else
        {
            dist = mag(inter.missPoint() - sample);
        }

        if (dist < smallDist)
        {
            return myFaces[myFacei];
        }
    }
    return -1;
}


// 'Pushes' point facePt (which is almost on face) in direction of cell centre
// so it is clearly inside.
CML::point CML::sampledSet::pushIn
(
    const point& facePt,
    const label facei
) const
{
    label celli = mesh().faceOwner()[facei];
    const point& cC = mesh().cellCentres()[celli];

    point newPosition = facePt;

    // Taken from particle::initCellFacePt()
    label tetFacei;
    label tetPtI;
    mesh().findTetFacePt(celli, facePt, tetFacei, tetPtI);

    // This is the tolerance that was defined as a static constant of the
    // particle class. It is no longer used by particle, following the switch to
    // barycentric tracking. The only place in which the tolerance is now used
    // is here. I'm not sure what the purpose of this code is, but it probably
    // wants removing. It is doing tet-searches for a particle position. This
    // should almost certainly be left to the particle class.
    const scalar trackingCorrectionTol = 1e-5;

    if (tetFacei == -1 || tetPtI == -1)
    {
        newPosition = facePt;

        label trap(1.0/trackingCorrectionTol + 1);

        label iterNo = 0;

        do
        {
            newPosition += trackingCorrectionTol*(cC - facePt);

            mesh().findTetFacePt
            (
                celli,
                newPosition,
                tetFacei,
                tetPtI
            );

            iterNo++;

        } while (tetFacei < 0  && iterNo <= trap);
    }

    if (tetFacei == -1)
    {
        FatalErrorInFunction
            << "After pushing " << facePt << " to " << newPosition
            << " it is still outside face " << facei
            << " at " << mesh().faceCentres()[facei]
            << " of cell " << celli
            << " at " << cC << endl
            << "Please change your starting point"
            << abort(FatalError);
    }

    return newPosition;
}


// Calculates start of tracking given samplePt and first boundary intersection
// (bPoint, bFaceI). bFaceI == -1 if no boundary intersection.
// Returns true if trackPt is sampling point
bool CML::sampledSet::getTrackingPoint
(
    const vector& offset,
    const point& samplePt,
    const point& bPoint,
    const label bFacei,

    point& trackPt,
    label& trackCelli,
    label& trackFacei
) const
{
    const scalar smallDist = mag(tol*offset);

    bool isGoodSample = false;

    if (bFacei == -1)
    {
        // No boundary intersection. Try and find cell samplePt is in
        trackCelli = mesh().findCell(samplePt, searchEngine_.decompMode());

        if
        (
            (trackCelli == -1)
        || !mesh().pointInCell
            (
                samplePt,
                trackCelli,
                searchEngine_.decompMode()
            )
        )
        {
            // Line samplePt - end_ does not intersect domain at all.
            // (or is along edge)

            trackCelli = -1;
            trackFacei = -1;

            isGoodSample = false;
        }
        else
        {
            // Start is inside. Use it as tracking point

            trackPt = samplePt;
            trackFacei = -1;

            isGoodSample = true;
        }
    }
    else if (mag(samplePt - bPoint) < smallDist)
    {
        // samplePt close to bPoint. Snap to it
        trackPt = pushIn(bPoint, bFacei);
        trackFacei = bFacei;
        trackCelli = getBoundaryCell(trackFacei);

        isGoodSample = true;
    }
    else
    {
        scalar sign = calcSign(bFacei, samplePt);

        if (sign < 0)
        {
            // samplePt inside or marginally outside.
            trackPt = samplePt;
            trackFacei = -1;
            trackCelli = mesh().findCell(trackPt, searchEngine_.decompMode());

            isGoodSample = true;
        }
        else
        {
            // samplePt outside. use bPoint
            trackPt = pushIn(bPoint, bFacei);
            trackFacei = bFacei;
            trackCelli = getBoundaryCell(trackFacei);

            isGoodSample = false;
        }
    }

    if (debug)
    {
        InfoInFunction
            << " samplePt:" << samplePt
            << " bPoint:" << bPoint
            << " bFacei:" << bFacei
            << endl << "   Calculated first tracking point :"
            << " trackPt:" << trackPt
            << " trackCelli:" << trackCelli
            << " trackFacei:" << trackFacei
            << " isGoodSample:" << isGoodSample
            << endl;
    }

    return isGoodSample;
}


void CML::sampledSet::setSamples
(
    const List<point>& samplingPts,
    const labelList& samplingCells,
    const labelList& samplingFaces,
    const labelList& samplingSegments,
    const scalarList& samplingCurveDist
)
{
    setSize(samplingPts.size());
    cells_.setSize(samplingCells.size());
    faces_.setSize(samplingFaces.size());
    segments_.setSize(samplingSegments.size());
    curveDist_.setSize(samplingCurveDist.size());

    if
    (
        (cells_.size() != size())
     || (faces_.size() != size())
     || (segments_.size() != size())
     || (curveDist_.size() != size())
    )
    {
        FatalErrorInFunction
            << "sizes not equal : "
            << "  points:" << size()
            << "  cells:" << cells_.size()
            << "  faces:" << faces_.size()
            << "  segments:" << segments_.size()
            << "  curveDist:" << curveDist_.size()
            << abort(FatalError);
    }

    forAll(samplingPts, sampleI)
    {
        operator[](sampleI) = samplingPts[sampleI];
    }
    curveDist_ = samplingCurveDist;

    cells_ = samplingCells;
    faces_ = samplingFaces;
    segments_ = samplingSegments;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::sampledSet::sampledSet
(
    const word& name,
    const polyMesh& mesh,
    const meshSearch& searchEngine,
    const word& axis
)
:
    coordSet(name, axis),
    mesh_(mesh),
    searchEngine_(searchEngine),
    segments_(0),
    cells_(0),
    faces_(0)
{}


CML::sampledSet::sampledSet
(
    const word& name,
    const polyMesh& mesh,
    const meshSearch& searchEngine,
    const dictionary& dict
)
:
    coordSet(name, dict.lookup("axis")),
    mesh_(mesh),
    searchEngine_(searchEngine),
    segments_(0),
    cells_(0),
    faces_(0)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::sampledSet::~sampledSet()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::autoPtr<CML::sampledSet> CML::sampledSet::New
(
    const word& name,
    const polyMesh& mesh,
    const meshSearch& searchEngine,
    const dictionary& dict
)
{
    const word sampleType(dict.lookup("type"));

    wordConstructorTable::iterator cstrIter =
        wordConstructorTablePtr_->find(sampleType);

    if (cstrIter == wordConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown sample type "
            << sampleType << nl << nl
            << "Valid sample types : " << endl
            << wordConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<sampledSet>
    (
        cstrIter()
        (
            name,
            mesh,
            searchEngine,
            dict
        )
    );
}


CML::Ostream& CML::sampledSet::write(Ostream& os) const
{
    coordSet::write(os);

    os  << endl << "\t(celli)\t(facei)" << endl;

    forAll(*this, sampleI)
    {
        os  << '\t' << cells_[sampleI]
            << '\t' << faces_[sampleI]
            << endl;
    }

    return os;
}


// ************************************************************************* //
