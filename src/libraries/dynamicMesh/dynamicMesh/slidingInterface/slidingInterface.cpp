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

\*---------------------------------------------------------------------------*/

#include "slidingInterface.hpp"
#include "polyTopoChanger.hpp"
#include "polyMesh.hpp"
#include "polyTopoChange.hpp"
#include "addToRunTimeSelectionTable.hpp"
#include "plane.hpp"

// Index of debug signs:
// p - adjusting a projection point
// * - adjusting edge intersection

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(slidingInterface, 0);
    addToRunTimeSelectionTable
    (
        polyMeshModifier,
        slidingInterface,
        dictionary
    );

    template<>
    const char* CML::NamedEnum
    <
        CML::slidingInterface::typeOfMatch,
        2
    >::names[] =
    {
        "integral",
        "partial"
    };
}


const CML::NamedEnum<CML::slidingInterface::typeOfMatch, 2>
CML::slidingInterface::typeOfMatchNames_;


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::slidingInterface::checkDefinition()
{
    const polyMesh& mesh = topoChanger().mesh();

    if
    (
        !masterFaceZoneID_.active()
     || !slaveFaceZoneID_.active()
     || !cutPointZoneID_.active()
     || !cutFaceZoneID_.active()
     || !masterPatchID_.active()
     || !slavePatchID_.active()
    )
    {
        FatalErrorIn
        (
            "void slidingInterface::checkDefinition()"
        )   << "Not all zones and patches needed in the definition "
            << "have been found.  Please check your mesh definition."
            << abort(FatalError);
    }

    // Check the sizes and set up state
    if
    (
        mesh.faceZones()[masterFaceZoneID_.index()].empty()
     || mesh.faceZones()[slaveFaceZoneID_.index()].empty()
    )
    {
        FatalErrorIn("void slidingInterface::checkDefinition()")
            << "Master or slave face zone contain no faces.  "
            << "Please check your mesh definition."
            << abort(FatalError);
    }

    if (debug)
    {
        Pout<< "Sliding interface object " << name() << " :" << nl
            << "    master face zone: " << masterFaceZoneID_.index() << nl
            << "    slave face zone: " << slaveFaceZoneID_.index() << endl;
    }
}


void CML::slidingInterface::clearOut() const
{
    clearPointProjection();
    clearAttachedAddressing();
    clearAddressing();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


// Construct from components
CML::slidingInterface::slidingInterface
(
    const word& name,
    const label index,
    const polyTopoChanger& mme,
    const word& masterFaceZoneName,
    const word& slaveFaceZoneName,
    const word& cutPointZoneName,
    const word& cutFaceZoneName,
    const word& masterPatchName,
    const word& slavePatchName,
    const typeOfMatch tom,
    const bool coupleDecouple,
    const intersection::algorithm algo
)
:
    polyMeshModifier(name, index, mme, true),
    masterFaceZoneID_
    (
        masterFaceZoneName,
        mme.mesh().faceZones()
    ),
    slaveFaceZoneID_
    (
        slaveFaceZoneName,
        mme.mesh().faceZones()
    ),
    cutPointZoneID_
    (
        cutPointZoneName,
        mme.mesh().pointZones()
    ),
    cutFaceZoneID_
    (
        cutFaceZoneName,
        mme.mesh().faceZones()
    ),
    masterPatchID_
    (
        masterPatchName,
        mme.mesh().boundaryMesh()
    ),
    slavePatchID_
    (
        slavePatchName,
        mme.mesh().boundaryMesh()
    ),
    matchType_(tom),
    coupleDecouple_(coupleDecouple),
    attached_(false),
    projectionAlgo_(algo),
    trigger_(false),
    pointMergeTol_(pointMergeTolDefault_),
    edgeMergeTol_(edgeMergeTolDefault_),
    nFacesPerSlaveEdge_(nFacesPerSlaveEdgeDefault_),
    edgeFaceEscapeLimit_(edgeFaceEscapeLimitDefault_),
    integralAdjTol_(integralAdjTolDefault_),
    edgeMasterCatchFraction_(edgeMasterCatchFractionDefault_),
    edgeCoPlanarTol_(edgeCoPlanarTolDefault_),
    edgeEndCutoffTol_(edgeEndCutoffTolDefault_),
    cutFaceMasterPtr_(NULL),
    cutFaceSlavePtr_(NULL),
    masterFaceCellsPtr_(NULL),
    slaveFaceCellsPtr_(NULL),
    masterStickOutFacesPtr_(NULL),
    slaveStickOutFacesPtr_(NULL),
    retiredPointMapPtr_(NULL),
    cutPointEdgePairMapPtr_(NULL),
    slavePointPointHitsPtr_(NULL),
    slavePointEdgeHitsPtr_(NULL),
    slavePointFaceHitsPtr_(NULL),
    masterPointEdgeHitsPtr_(NULL),
    projectedSlavePointsPtr_(NULL)
{
    checkDefinition();

    if (attached_)
    {
        FatalErrorIn
        (
            "CML::slidingInterface::slidingInterface\n"
            "(\n"
            "    const word& name,\n"
            "    const label index,\n"
            "    const polyTopoChanger& mme,\n"
            "    const word& masterFaceZoneName,\n"
            "    const word& slaveFaceZoneName,\n"
            "    const word& cutFaceZoneName,\n"
            "    const word& cutPointZoneName,\n"
            "    const word& masterPatchName,\n"
            "    const word& slavePatchName,\n"
            "    const typeOfMatch tom,\n"
            "    const bool coupleDecouple\n"
            ")"
        )   << "Creation of a sliding interface from components "
            << "in attached state not supported."
            << abort(FatalError);
    }
    else
    {
        calcAttachedAddressing();
    }
}


// Construct from components
CML::slidingInterface::slidingInterface
(
    const word& name,
    const dictionary& dict,
    const label index,
    const polyTopoChanger& mme
)
:
    polyMeshModifier(name, index, mme, Switch(dict.lookup("active"))),
    masterFaceZoneID_
    (
        dict.lookup("masterFaceZoneName"),
        mme.mesh().faceZones()
    ),
    slaveFaceZoneID_
    (
        dict.lookup("slaveFaceZoneName"),
        mme.mesh().faceZones()
    ),
    cutPointZoneID_
    (
        dict.lookup("cutPointZoneName"),
        mme.mesh().pointZones()
    ),
    cutFaceZoneID_
    (
        dict.lookup("cutFaceZoneName"),
        mme.mesh().faceZones()
    ),
    masterPatchID_
    (
        dict.lookup("masterPatchName"),
        mme.mesh().boundaryMesh()
    ),
    slavePatchID_
    (
        dict.lookup("slavePatchName"),
        mme.mesh().boundaryMesh()
    ),
    matchType_(typeOfMatchNames_.read((dict.lookup("typeOfMatch")))),
    coupleDecouple_(dict.lookup("coupleDecouple")),
    attached_(dict.lookup("attached")),
    projectionAlgo_
    (
        intersection::algorithmNames_.read(dict.lookup("projection"))
    ),
    trigger_(false),
    cutFaceMasterPtr_(NULL),
    cutFaceSlavePtr_(NULL),
    masterFaceCellsPtr_(NULL),
    slaveFaceCellsPtr_(NULL),
    masterStickOutFacesPtr_(NULL),
    slaveStickOutFacesPtr_(NULL),
    retiredPointMapPtr_(NULL),
    cutPointEdgePairMapPtr_(NULL),
    slavePointPointHitsPtr_(NULL),
    slavePointEdgeHitsPtr_(NULL),
    slavePointFaceHitsPtr_(NULL),
    masterPointEdgeHitsPtr_(NULL),
    projectedSlavePointsPtr_(NULL)
{
    // Optionally default tolerances from dictionary
    setTolerances(dict);

    checkDefinition();

    // If the interface is attached, the master and slave face zone addressing
    // needs to be read in; otherwise it will be created
    if (attached_)
    {
        if (debug)
        {
            Pout<< "slidingInterface::slidingInterface(...) "
                << " for object " << name << " : "
                << "Interface attached.  Reading master and slave face zones "
                << "and retired point lookup." << endl;
        }

        // The face zone addressing is written out in the definition dictionary
        masterFaceCellsPtr_ = new labelList(dict.lookup("masterFaceCells"));
        slaveFaceCellsPtr_ = new labelList(dict.lookup("slaveFaceCells"));

        masterStickOutFacesPtr_ =
            new labelList(dict.lookup("masterStickOutFaces"));
        slaveStickOutFacesPtr_ =
            new labelList(dict.lookup("slaveStickOutFaces"));

        retiredPointMapPtr_ = new Map<label>(dict.lookup("retiredPointMap"));
        cutPointEdgePairMapPtr_ =
            new Map<Pair<edge> >(dict.lookup("cutPointEdgePairMap"));
    }
    else
    {
        calcAttachedAddressing();
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::slidingInterface::~slidingInterface()
{
    clearOut();
}


void CML::slidingInterface::clearAddressing() const
{
    deleteDemandDrivenData(cutFaceMasterPtr_);
    deleteDemandDrivenData(cutFaceSlavePtr_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::faceZoneID& CML::slidingInterface::masterFaceZoneID() const
{
    return masterFaceZoneID_;
}


const CML::faceZoneID& CML::slidingInterface::slaveFaceZoneID() const
{
    return slaveFaceZoneID_;
}


bool CML::slidingInterface::changeTopology() const
{
    if (coupleDecouple_)
    {
        // Always changes.  If not attached, project points
        if (debug)
        {
            Pout<< "bool slidingInterface::changeTopology() const "
                << "for object " << name() << " : "
                << "Couple-decouple mode." << endl;
        }

        if (!attached_)
        {
            projectPoints();
        }
        else
        {
        }

        return true;
    }

    if
    (
        attached_
     && !topoChanger().mesh().changing()
    )
    {
        // If the mesh is not moving or morphing and the interface is
        // already attached, the topology will not change
        return false;
    }
    else
    {
        // Check if the motion changes point projection
        return projectPoints();
    }
}


void CML::slidingInterface::setRefinement(polyTopoChange& ref) const
{
    if (coupleDecouple_)
    {
        if (attached_)
        {
            // Attached, coupling
            decoupleInterface(ref);
        }
        else
        {
            // Detached, coupling
            coupleInterface(ref);
        }

        return;
    }

    if (trigger_)
    {
        if (attached_)
        {
            // Clear old coupling data
            clearCouple(ref);
        }

        coupleInterface(ref);

        trigger_ = false;
    }
}


void CML::slidingInterface::modifyMotionPoints(pointField& motionPoints) const
{
    if (debug)
    {
        Pout<< "void slidingInterface::modifyMotionPoints("
            << "pointField& motionPoints) const for object " << name() << " : "
            << "Adjusting motion points." << endl;
    }

    const polyMesh& mesh = topoChanger().mesh();

    // Get point from the cut zone
    const labelList& cutPoints = mesh.pointZones()[cutPointZoneID_.index()];

    if (cutPoints.size() && !projectedSlavePointsPtr_)
    {
        return;
    }
    else
    {
        const pointField& projectedSlavePoints = *projectedSlavePointsPtr_;

        const Map<label>& rpm = retiredPointMap();

        const Map<Pair<edge> >& cpepm = cutPointEdgePairMap();

        const Map<label>& slaveZonePointMap =
            mesh.faceZones()[slaveFaceZoneID_.index()]().meshPointMap();

        const primitiveFacePatch& masterPatch =
            mesh.faceZones()[masterFaceZoneID_.index()]();
        const edgeList& masterEdges = masterPatch.edges();
        const pointField& masterLocalPoints = masterPatch.localPoints();

        const primitiveFacePatch& slavePatch =
            mesh.faceZones()[slaveFaceZoneID_.index()]();
        const edgeList& slaveEdges = slavePatch.edges();
        const pointField& slaveLocalPoints = slavePatch.localPoints();
        const vectorField& slavePointNormals = slavePatch.pointNormals();

        forAll(cutPoints, pointI)
        {
            // Try to find the cut point in retired points
            Map<label>::const_iterator rpmIter = rpm.find(cutPoints[pointI]);

            if (rpmIter != rpm.end())
            {
                if (debug)
                {
                    Pout<< "p";
                }

                // Cut point is a retired point
                motionPoints[cutPoints[pointI]] =
                    projectedSlavePoints[slaveZonePointMap.find(rpmIter())()];
            }
            else
            {
                // A cut point is not a projected slave point.  Therefore, it
                // must be an edge-to-edge intersection.

                Map<Pair<edge> >::const_iterator cpepmIter =
                    cpepm.find(cutPoints[pointI]);

                if (cpepmIter != cpepm.end())
                {
                    // Pout<< "Need to re-create hit for point "
                    //     << cutPoints[pointI]
                    //     << " lookup: " << cpepmIter()
                    //     << endl;

                    // Note.
                    // The edge cutting code is repeated in
                    // slidingInterface::coupleInterface.  This is done for
                    // efficiency reasons and avoids multiple creation of
                    // cutting planes.  Please update both simultaneously.
                    //
                    const edge& globalMasterEdge = cpepmIter().first();

                    const label curMasterEdgeIndex =
                        masterPatch.whichEdge
                        (
                            edge
                            (
                                masterPatch.whichPoint
                                (
                                    globalMasterEdge.start()
                                ),
                                masterPatch.whichPoint
                                (
                                    globalMasterEdge.end()
                                )
                            )
                        );

                    const edge& cme = masterEdges[curMasterEdgeIndex];

                    // Pout<< "curMasterEdgeIndex: " << curMasterEdgeIndex
                    //     << " cme: " << cme
                    //     << endl;

                    const edge& globalSlaveEdge = cpepmIter().second();

                    const label curSlaveEdgeIndex =
                        slavePatch.whichEdge
                        (
                            edge
                            (
                                slavePatch.whichPoint
                                (
                                    globalSlaveEdge.start()
                                ),
                                slavePatch.whichPoint
                                (
                                    globalSlaveEdge.end()
                                )
                            )
                        );

                    const edge& curSlaveEdge = slaveEdges[curSlaveEdgeIndex];
                    // Pout<< "curSlaveEdgeIndex: " << curSlaveEdgeIndex
                    //     << " curSlaveEdge: " << curSlaveEdge
                    //     << endl;
                    const point& a = projectedSlavePoints[curSlaveEdge.start()];
                    const point& b = projectedSlavePoints[curSlaveEdge.end()];

                    point c =
                        0.5*
                        (
                            slaveLocalPoints[curSlaveEdge.start()]
                          + slavePointNormals[curSlaveEdge.start()]
                          + slaveLocalPoints[curSlaveEdge.end()]
                          + slavePointNormals[curSlaveEdge.end()]
                        );

                    // Create the plane
                    plane cutPlane(a, b, c);

                    linePointRef curSlaveLine =
                        curSlaveEdge.line(slaveLocalPoints);
                    const scalar curSlaveLineMag = curSlaveLine.mag();

                    scalar cutOnMaster =
                        cutPlane.lineIntersect
                        (
                            cme.line(masterLocalPoints)
                        );

                    if
                    (
                        cutOnMaster > edgeEndCutoffTol_
                     && cutOnMaster < 1.0 - edgeEndCutoffTol_
                    )
                    {
                        // Master is cut, check the slave
                        point masterCutPoint =
                            masterLocalPoints[cme.start()]
                          + cutOnMaster*cme.vec(masterLocalPoints);

                        pointHit slaveCut =
                            curSlaveLine.nearestDist(masterCutPoint);

                        if (slaveCut.hit())
                        {
                            // Strict checking of slave cut to avoid capturing
                            // end points.
                            scalar cutOnSlave =
                                (
                                    (
                                        slaveCut.hitPoint()
                                      - curSlaveLine.start()
                                    ) & curSlaveLine.vec()
                                )/sqr(curSlaveLineMag);

                            // Calculate merge tolerance from the
                            // target edge length
                            scalar mergeTol =
                                edgeCoPlanarTol_*mag(b - a);

                            if
                            (
                                cutOnSlave > edgeEndCutoffTol_
                             && cutOnSlave < 1.0 - edgeEndCutoffTol_
                             && slaveCut.distance() < mergeTol
                            )
                            {
                                // Cut both master and slave.
                                motionPoints[cutPoints[pointI]] =
                                    masterCutPoint;
                            }
                        }
                        else
                        {
                            Pout<< "Missed slave edge!!!  This is an error.  "
                                << "Master edge: "
                                << cme.line(masterLocalPoints)
                                << " slave edge: " << curSlaveLine
                                << " point: " << masterCutPoint
                                << " weight: " <<
                                (
                                    (
                                        slaveCut.missPoint()
                                      - curSlaveLine.start()
                                    ) & curSlaveLine.vec()
                                )/sqr(curSlaveLineMag)
                                << endl;
                        }
                    }
                    else
                    {
                        Pout<< "Missed master edge!!!  This is an error"
                            << endl;
                    }
                }
                else
                {
                    FatalErrorIn
                    (
                        "void slidingInterface::modifyMotionPoints"
                        "(pointField&) const"
                    )   << "Cut point " << cutPoints[pointI]
                        << " not recognised as either the projected "
                        << "or as intersection point.  Error in point "
                        << "projection or data mapping"
                        << abort(FatalError);
                }
            }
        }
        if (debug)
        {
            Pout<< endl;
        }
    }
}


void CML::slidingInterface::updateMesh(const mapPolyMesh& m)
{
    if (debug)
    {
        Pout<< "void slidingInterface::updateMesh(const mapPolyMesh& m)"
            << " const for object " << name() << " : "
            << "Updating topology." << endl;
    }

    // Mesh has changed topologically.  Update local topological data
    const polyMesh& mesh = topoChanger().mesh();

    masterFaceZoneID_.update(mesh.faceZones());
    slaveFaceZoneID_.update(mesh.faceZones());
    cutPointZoneID_.update(mesh.pointZones());
    cutFaceZoneID_.update(mesh.faceZones());

    masterPatchID_.update(mesh.boundaryMesh());
    slavePatchID_.update(mesh.boundaryMesh());

//MJ:Disabled updating
//    if (!attached())
//    {
//        calcAttachedAddressing();
//    }
//    else
//    {
//        renumberAttachedAddressing(m);
//    }
}


const CML::pointField& CML::slidingInterface::pointProjection() const
{
    if (!projectedSlavePointsPtr_)
    {
        projectPoints();
    }

    return *projectedSlavePointsPtr_;
}

void CML::slidingInterface::setTolerances(const dictionary&dict, bool report)
{
    pointMergeTol_ = dict.lookupOrDefault<scalar>
    (
        "pointMergeTol",
        pointMergeTol_
    );
    edgeMergeTol_ = dict.lookupOrDefault<scalar>
    (
        "edgeMergeTol",
        edgeMergeTol_
    );
    nFacesPerSlaveEdge_ = dict.lookupOrDefault<label>
    (
        "nFacesPerSlaveEdge",
        nFacesPerSlaveEdge_
    );
    edgeFaceEscapeLimit_ = dict.lookupOrDefault<label>
    (
        "edgeFaceEscapeLimit",
        edgeFaceEscapeLimit_
    );
    integralAdjTol_ = dict.lookupOrDefault<scalar>
    (
        "integralAdjTol",
        integralAdjTol_
    );
    edgeMasterCatchFraction_ = dict.lookupOrDefault<scalar>
    (
        "edgeMasterCatchFraction",
        edgeMasterCatchFraction_
    );
    edgeCoPlanarTol_ = dict.lookupOrDefault<scalar>
    (
        "edgeCoPlanarTol",
        edgeCoPlanarTol_
    );
    edgeEndCutoffTol_ = dict.lookupOrDefault<scalar>
    (
        "edgeEndCutoffTol",
        edgeEndCutoffTol_
    );

    if (report)
    {
        Info<< "Sliding interface parameters:" << nl
            << "pointMergeTol            : " << pointMergeTol_ << nl
            << "edgeMergeTol             : " << edgeMergeTol_ << nl
            << "nFacesPerSlaveEdge       : " << nFacesPerSlaveEdge_ << nl
            << "edgeFaceEscapeLimit      : " << edgeFaceEscapeLimit_ << nl
            << "integralAdjTol           : " << integralAdjTol_ << nl
            << "edgeMasterCatchFraction  : " << edgeMasterCatchFraction_ << nl
            << "edgeCoPlanarTol          : " << edgeCoPlanarTol_ << nl
            << "edgeEndCutoffTol         : " << edgeEndCutoffTol_ << endl;
    }
}


void CML::slidingInterface::write(Ostream& os) const
{
    os  << nl << type() << nl
        << name()<< nl
        << masterFaceZoneID_.name() << nl
        << slaveFaceZoneID_.name() << nl
        << cutPointZoneID_.name() << nl
        << cutFaceZoneID_.name() << nl
        << masterPatchID_.name() << nl
        << slavePatchID_.name() << nl
        << typeOfMatchNames_[matchType_] << nl
        << coupleDecouple_ << nl
        << attached_ << endl;
}


// To write out all those tolerances
#define WRITE_NON_DEFAULT(name) \
    if ( name ## _ != name ## Default_ )\
    { \
        os << "    " #name " " <<  name ## _ << token::END_STATEMENT << nl; \
    }


void CML::slidingInterface::writeDict(Ostream& os) const
{
    os  << nl << name() << nl << token::BEGIN_BLOCK << nl
        << "    type " << type() << token::END_STATEMENT << nl
        << "    masterFaceZoneName " << masterFaceZoneID_.name()
        << token::END_STATEMENT << nl
        << "    slaveFaceZoneName " << slaveFaceZoneID_.name()
        << token::END_STATEMENT << nl
        << "    cutPointZoneName " << cutPointZoneID_.name()
        << token::END_STATEMENT << nl
        << "    cutFaceZoneName " << cutFaceZoneID_.name()
        << token::END_STATEMENT << nl
        << "    masterPatchName " << masterPatchID_.name()
        << token::END_STATEMENT << nl
        << "    slavePatchName " << slavePatchID_.name()
        << token::END_STATEMENT << nl
        << "    typeOfMatch " << typeOfMatchNames_[matchType_]
        << token::END_STATEMENT << nl
        << "    coupleDecouple " << coupleDecouple_
        << token::END_STATEMENT << nl
        << "    projection " << intersection::algorithmNames_[projectionAlgo_]
        << token::END_STATEMENT << nl
        << "    attached " << attached_
        << token::END_STATEMENT << nl
        << "    active " << active()
        << token::END_STATEMENT << nl;

    if (attached_)
    {
        masterFaceCellsPtr_->writeEntry("masterFaceCells", os);
        slaveFaceCellsPtr_->writeEntry("slaveFaceCells", os);
        masterStickOutFacesPtr_->writeEntry("masterStickOutFaces", os);
        slaveStickOutFacesPtr_->writeEntry("slaveStickOutFaces", os);

         os << "    retiredPointMap " << retiredPointMap()
            << token::END_STATEMENT << nl
            << "    cutPointEdgePairMap " << cutPointEdgePairMap()
            << token::END_STATEMENT << nl;
    }

    WRITE_NON_DEFAULT(pointMergeTol)
    WRITE_NON_DEFAULT(edgeMergeTol)
    WRITE_NON_DEFAULT(nFacesPerSlaveEdge)
    WRITE_NON_DEFAULT(edgeFaceEscapeLimit)
    WRITE_NON_DEFAULT(integralAdjTol)
    WRITE_NON_DEFAULT(edgeMasterCatchFraction)
    WRITE_NON_DEFAULT(edgeCoPlanarTol)
    WRITE_NON_DEFAULT(edgeEndCutoffTol)

    os  << token::END_BLOCK << endl;
}


// ************************************************************************* //
