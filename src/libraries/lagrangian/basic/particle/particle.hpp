/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2019 OpenFOAM Foundation
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
    CML::particle

Description
    Base particle class

\*---------------------------------------------------------------------------*/

#ifndef particle_H
#define particle_H

#include "vector.hpp"
#include "barycentric.hpp"
#include "barycentricTensor.hpp"
#include "Cloud.hpp"
#include "IDLList.hpp"
#include "pointField.hpp"
#include "faceList.hpp"
#include "OFstream.hpp"
#include "tetPointRef.hpp"
#include "FixedList.hpp"
#include "polyMeshTetDecomposition.hpp"
#include "particleMacros.hpp"

#include "polyMesh.hpp"
#include "Time.hpp"
#include "IOPosition.hpp"
#include "cyclicPolyPatch.hpp"
#include "cyclicAMIPolyPatch.hpp"
#include "cyclicACMIPolyPatch.hpp"
#include "processorPolyPatch.hpp"
#include "symmetryPolyPatch.hpp"
#include "wallPolyPatch.hpp"
#include "wedgePolyPatch.hpp"
#include "meshTools.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
class particle;

class polyPatch;

class cyclicPolyPatch;
class cyclicAMIPolyPatch;
class cyclicACMIPolyPatch;
class processorPolyPatch;
class symmetryPolyPatch;
class wallPolyPatch;
class wedgePolyPatch;

// Forward declaration of friend functions and operators

Ostream& operator<<
(
    Ostream&,
    const particle&
);

bool operator==(const particle&, const particle&);

bool operator!=(const particle&, const particle&);

/*---------------------------------------------------------------------------*\
                          Class Particle Declaration
\*---------------------------------------------------------------------------*/

class particle
:
    public IDLList<particle>::link
{
    // Private member data

        //- Size in bytes of the position data
        static const std::size_t sizeofPosition_;

        //- Size in bytes of the fields
        static const std::size_t sizeofFields_;

        //- The value of nBehind_ at which tracking is abandoned. See the
        //  description of nBehind_.
        static const label maxNBehind_;


public:

    class trackingData
    {
    public:

        // Public data

            //- Flag to switch processor
            bool switchProcessor;

            //- Flag to indicate whether to keep particle (false = delete)
            bool keepParticle;


        // Constructor
        template <class TrackCloudType>
        trackingData(const TrackCloudType& cloud)
        {}
    };


private:

    // Private data

        //- Reference to the polyMesh database
        const polyMesh& mesh_;

        //- Coordinates of particle
        barycentric coordinates_;

        //- Index of the cell it is in
        label celli_;

        //- Index of the face that owns the decomposed tet that the
        //  particle is in
        label tetFacei_;

        //- Index of the point on the face that defines the decomposed
        //  tet that the particle is in.  Relative to the face base
        //  point.
        label tetPti_;

        //- Face index if the particle is on a face otherwise -1
        label facei_;

        //- Fraction of time-step completed
        scalar stepFraction_;

        //- The distance behind the maximum distance reached so far
        scalar behind_;

        //- The number of tracks carried out that ended in a distance behind the
        //  maximum distance reached so far. Once this reaches maxNBehind_,
        //  tracking is abandoned for the current step. This is needed because
        //  when tetrahedra are inverted a straight trajectory can form a closed
        //  loop through regions of overlapping positive and negative space.
        //  Without this break clause, such loops can result in a valid track
        //  which never ends.
        label nBehind_;

        //- Originating processor id
        label origProc_;

        //- Local particle id on originating processor
        label origId_;


private:

    // Private Member Functions

        // Tetrahedra functions

            //- Get the vertices of the current tet
            inline void stationaryTetGeometry
            (
                vector& centre,
                vector& base,
                vector& vertex1,
                vector& vertex2
            ) const;

            //- Get the transformation associated with the current tet. This
            //  will convert a barycentric position within the tet to a
            //  cartesian position in the global coordinate system. The
            //  conversion is x = A & y, where x is the cartesian position, y is
            //  the barycentric position and A is the transformation tensor.
            inline barycentricTensor stationaryTetTransform() const;

            //- Get the reverse transform associated with the current tet. The
            //  conversion is detA*y = (x - centre) & T. The variables x, y and
            //  centre have the same meaning as for the forward transform. T is
            //  the transposed inverse of the forward transform tensor, A,
            //  multiplied by its determinant, detA. This separation allows
            //  the barycentric tracking algorithm to function on inverted or
            //  degenerate tetrahedra.
            void stationaryTetReverseTransform
            (
                vector& centre,
                scalar& detA,
                barycentricTensor& T
            ) const;

            //- Get the vertices of the current moving tet. Two values are
            //  returned for each vertex. The first is a constant, and the
            //  second is a linear coefficient of the track fraction.
            inline void movingTetGeometry
            (
                const scalar endStepFraction,
                Pair<vector>& centre,
                Pair<vector>& base,
                Pair<vector>& vertex1,
                Pair<vector>& vertex2
            ) const;

            //- Get the transformation associated with the current, moving, tet.
            //  This is of the same form as for the static case. As with the
            //  moving geometry, a linear function of the tracking fraction is
            //  returned for each component.
            inline Pair<barycentricTensor> movingTetTransform
            (
                const scalar endStepFraction
            ) const;

            //- Get the reverse transformation associated with the current,
            //  moving, tet. This is of the same form as for the static case. As
            //  with the moving geometry, a function of the tracking fraction is
            //  returned for each component. The functions are higher order than
            //  for the forward transform; the determinant is cubic, and the
            //  tensor is quadratic.
            void movingTetReverseTransform
            (
                const scalar endStepFraction,
                Pair<vector>& centre,
                FixedList<scalar, 4>& detA,
                FixedList<barycentricTensor, 3>& T
            ) const;


        // Transformations

            //- Reflection transform. Corrects the coordinates when the particle
            //  moves between two tets which share a base vertex, but for which
            //  the other two non cell-centre vertices are reversed. All hits
            //  which retain the same face behave this way, as do face hits.
            void reflect();

            //- Rotation transform. Corrects the coordinates when the particle
            //  moves between two tets with different base vertices, but are
            //  otherwise similarly oriented. Hits which change the face within
            //  the cell make use of both this and the reflect transform.
            void rotate(const bool direction);


        // Topology changes

            //- Change tet within a cell. Called after a triangle is hit.
            void changeTet(const label tetTriI);

            //- Change tet face within a cell. Called by changeTet.
            void changeFace(const label tetTriI);

            //- Change cell. Called when the particle hits an internal face.
            void changeCell();

            //- Put the particle on the lowest indexed patch for the current set
            //  of coincident faces. In the case of an ACMI-wall pair, this will
            //  move the particle from the wall face to the ACMI face, because
            //  ACMI patches are always listed before their associated non-
            //  overlapping patch.
            void changeToMasterPatch();


        // Geometry changes

            //- Locate the particle at the given position
            void locate
            (
                const vector& position,
                const vector* direction,
                label celli,
                const bool boundaryFail,
                const string boundaryMsg
            );


protected:

    // Patch interactions

        //- Overridable function to handle the particle hitting a patch.
        //  Executed before other patch-hitting functions.
        template<class TrackCloudType>
        bool hitPatch(TrackCloudType&, trackingData&);

        //- Overridable function to handle the particle hitting a wedgePatch
        template<class TrackCloudType>
        void hitWedgePatch(TrackCloudType&, trackingData&);

        //- Overridable function to handle the particle hitting a
        //  symmetryPlanePatch
//        template<class TrackCloudType>
//        void hitSymmetryPlanePatch(TrackCloudType&, trackingData&);

        //- Overridable function to handle the particle hitting a symmetryPatch
        template<class TrackCloudType>
        void hitSymmetryPatch(TrackCloudType&, trackingData&);

        //- Overridable function to handle the particle hitting a cyclicPatch
        template<class TrackCloudType>
        void hitCyclicPatch(TrackCloudType&, trackingData&);

        //- Overridable function to handle the particle hitting a cyclicAMIPatch
        template<class TrackCloudType>
        void hitCyclicAMIPatch(TrackCloudType&, trackingData&, const vector&);

        //- Overridable function to handle the particle hitting a
        //  cyclicACMIPatch
        template<class TrackCloudType>
        void hitCyclicACMIPatch(TrackCloudType&, trackingData&, const vector&);

        //- Overridable function to handle the particle hitting an
        //  cyclicRepeatAMIPolyPatch
//        template<class TrackCloudType>
//        void hitCyclicRepeatAMIPatch
//        (
//            TrackCloudType&,
//            trackingData&,
//            const vector&
//        );

        //- Overridable function to handle the particle hitting a processorPatch
        template<class TrackCloudType>
        void hitProcessorPatch(TrackCloudType&, trackingData&);

        //- Overridable function to handle the particle hitting a wallPatch
        template<class TrackCloudType>
        void hitWallPatch(TrackCloudType&, trackingData&);


public:

    // Static data members

        //- Runtime type information
        TypeName("particle");

        //- String representation of properties
        DefinePropertyList
        (
            "(coordinatesa coordinatesb coordinatesc coordinatesd) "
            "celli tetFacei tetPti facei stepFraction "
            "behind nBehind origProc origId"
        );

        //- Cumulative particle counter - used to provide unique ID
        static label particleCount_;


    // Constructors

        //- Construct from components
        particle
        (
            const polyMesh& mesh,
            const barycentric& coordinates,
            const label celli,
            const label tetFacei,
            const label tetPti
        );

        //- Construct from a position and a cell, searching for the rest of the
        //  required topology
        particle
        (
            const polyMesh& mesh,
            const vector& position,
            const label celli
        );

        //- Construct from Istream
        particle(const polyMesh& mesh, Istream&, bool readFields = true);

        //- Construct as a copy
        particle(const particle& p);

        //- Construct as a copy with references to a new mesh
        particle(const particle& p, const polyMesh& mesh);

        //- Construct a clone
        virtual autoPtr<particle> clone() const
        {
            return autoPtr<particle>(new particle(*this));
        }

        //- Factory class to read-construct particles used for
        //  parallel transfer
        class iNew
        {
            const polyMesh& mesh_;

        public:

            iNew(const polyMesh& mesh)
            :
                mesh_(mesh)
            {}

            autoPtr<particle> operator()(Istream& is) const
            {
                return autoPtr<particle>(new particle(mesh_, is, true));
            }
        };


    //- Destructor
    virtual ~particle()
    {}


    // Member Functions

        // Access

            //- Get unique particle creation id
            inline label getNewParticleID() const;

            //- Return the mesh database
            inline const polyMesh& mesh() const;

            //- Return current particle coordinates
            inline const barycentric& coordinates() const;

            //- Return current cell particle is in
            inline label cell() const;

            //- Return current cell particle is in for manipulation
            inline label& cell();

            //- Return current tet face particle is in
            inline label tetFace() const;

            //- Return current tet face particle is in
            inline label tetPt() const;

            //- Return current face particle is on otherwise -1
            inline label face() const;

            //- Return the fraction of time-step completed
            inline scalar stepFraction() const;

            //- Return the fraction of time-step completed
            inline scalar& stepFraction();

            //- Return the originating processor ID
            inline label origProc() const;

            //- Return the originating processor ID
            inline label& origProc();

            //- Return the particle ID on the originating processor
            inline label origId() const;

            //- Return the particle ID on the originating processor
            inline label& origId();


        // Check

            //- Return the step fraction change within the overall time-step.
            //  Returns the start value and the change as a scalar pair. Always
            //  return Pair<scalar>(0, 1), unless sub-cycling is in effect, in
            //  which case the values will reflect the span of the sub-cycle
            //  within the time-step.
            inline Pair<scalar> stepFractionSpan() const;

            //- Return the current fraction within the timestep. This differs
            //  from the stored step fraction due to sub-cycling.
            inline scalar currentTimeFraction() const;

            //- Return the indices of the current tet that the
            //  particle occupies.
            inline tetIndices currentTetIndices() const;

            //- Return the current tet transformation tensor
            inline barycentricTensor currentTetTransform() const;

            //- Return the normal of the tri on tetFacei_ for the
            //  current tet.
            inline vector normal() const;

            //- Is the particle on a face?
            inline bool onFace() const;

            //- Is the particle on an internal face?
            inline bool onInternalFace() const;

            //- Is the particle on a boundary face?
            inline bool onBoundaryFace() const;

            //- Return the index of patch that the particle is on
            inline label patch() const;

            //- Return current particle position
            inline vector position() const;


    // Track

        //- Set step fraction and behind data to zero in preparation for a new
        //  track
        inline void reset();

        //- Track along the displacement for a given fraction of the overall
        //  step. End when the track is complete, or when a boundary is hit.
        //  On exit, stepFraction_ will have been incremented to the current
        //  position, and facei_ will be set to the index of the boundary
        //  face that was hit, or -1 if the track completed within a cell.
        //  The proportion of the displacement still to be completed is
        //  returned.
        scalar track
        (
            const vector& displacement,
            const scalar fraction
        );

        //- As particle::track, but stops when a new cell is reached.
        scalar trackToCell
        (
            const vector& displacement,
            const scalar fraction
        );

        //- As particle::track, but stops when a face is hit.
        scalar trackToFace
        (
            const vector& displacement,
            const scalar fraction
        );

        //- As particle::trackToFace, but stops when a tet triangle is hit. On
        //  exit, tetTriI is set to the index of the tet triangle that was hit,
        //  or -1 if the end position was reached.
        scalar trackToTri
        (
            const vector& displacement,
            const scalar fraction,
            label& tetTriI
        );

        //- As particle::trackToTri, but for stationary meshes
        scalar trackToStationaryTri
        (
            const vector& displacement,
            const scalar fraction,
            label& tetTriI
        );

        //- As particle::trackToTri, but for moving meshes
        scalar trackToMovingTri
        (
            const vector& displacement,
            const scalar fraction,
            label& tetTriI
        );

        //- Hit the current face. If the current face is internal than this
        //  crosses into the next cell. If it is a boundary face then this will
        //  interact the particle with the relevant patch.
        template<class TrackCloudType>
        void hitFace
        (
            const vector& direction,
            TrackCloudType& cloud,
            trackingData& td
        );

        //- Convenience function. Cobines trackToFace and hitFace
        template<class TrackCloudType>
        scalar trackToAndHitFace
        (
            const vector& direction,
            const scalar fraction,
            TrackCloudType& cloud,
            trackingData& td
        );

        //- Get the displacement from the mesh centre. Used to correct the
        //  particle position in cases with reduced dimensionality. Returns a
        //  zero vector for three-dimensional cases.
        vector deviationFromMeshCentre() const;


    // Patch data

        //- Get the normal and velocity of the current patch location
        inline void patchData(vector& n, vector& U) const;


    // Transformations

        //- Transform the physical properties of the particle
        //  according to the given transformation tensor
        virtual void transformProperties(const tensor& T);

        //- Transform the physical properties of the particle
        //  according to the given separation vector
        virtual void transformProperties(const vector& separation);


    // Parallel transfer

        //- Convert global addressing to the processor patch local equivalents
        void prepareForParallelTransfer();

        //- Convert processor patch addressing to the global equivalents
        //  and set the celli to the face-neighbour
        void correctAfterParallelTransfer(const label patchi, trackingData& td);


    // Interaction list referral

        //- Break the topology and store the particle position so that the
        //  particle can be referred.
        void prepareForInteractionListReferral
        (
            const vectorTensorTransform& transform
        );

        //- Correct the topology after referral. The particle may still be
        //  outside the stored tet and therefore not track-able.
        void correctAfterInteractionListReferral(const label celli);


    // Decompose and reconstruct

        //- Return the tet point appropriate for decomposition or reconstruction
        //  to or from the given mesh.
        label procTetPt
        (
            const polyMesh& procMesh,
            const label procCell,
            const label procTetFace
        ) const;


    // Mapping

        //- Map after a topology change
        void autoMap(const vector& position, const mapPolyMesh& mapper);


    // I-O

        //- Read the fields associated with the owner cloud
        template<class TrackCloudType>
        static void readFields(TrackCloudType& c);

        //- Write the fields associated with the owner cloud
        template<class TrackCloudType>
        static void writeFields(const TrackCloudType& c);

        //- Write the particle position and cell
        void writePosition(Ostream&) const;


    // Friend Operators

        friend Ostream& operator<<(Ostream&, const particle&);

        friend bool operator==(const particle& pA, const particle& pB);

        friend bool operator!=(const particle& pA, const particle& pB);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::particle::stationaryTetGeometry
(
    vector& centre,
    vector& base,
    vector& vertex1,
    vector& vertex2
) const
{
    const triFace triIs(currentTetIndices().faceTriIs(mesh_));
    const vectorField& ccs = mesh_.cellCentres();
    const pointField& pts = mesh_.points();

    centre = ccs[celli_];
    base = pts[triIs[0]];
    vertex1 = pts[triIs[1]];
    vertex2 = pts[triIs[2]];
}


inline CML::barycentricTensor CML::particle::stationaryTetTransform() const
{
    vector centre, base, vertex1, vertex2;
    stationaryTetGeometry(centre, base, vertex1, vertex2);

    return barycentricTensor(centre, base, vertex1, vertex2);
}


inline void CML::particle::movingTetGeometry
(
    const scalar fraction,
    Pair<vector>& centre,
    Pair<vector>& base,
    Pair<vector>& vertex1,
    Pair<vector>& vertex2
) const
{
    const triFace triIs(currentTetIndices().faceTriIs(mesh_));
    const pointField& ptsOld = mesh_.oldPoints();
    const pointField& ptsNew = mesh_.points();
    const vector ccOld = mesh_.oldCellCentres()[celli_];
    const vector ccNew = mesh_.cellCentres()[celli_];

    // Old and new points and cell centres are not sub-cycled. If we are sub-
    // cycling, then we have to account for the timestep change here by
    // modifying the fractions that we take of the old and new geometry.
    const Pair<scalar> s = stepFractionSpan();
    const scalar f0 = s[0] + stepFraction_*s[1], f1 = fraction*s[1];

    centre[0] = ccOld + f0*(ccNew - ccOld);
    base[0] = ptsOld[triIs[0]] + f0*(ptsNew[triIs[0]] - ptsOld[triIs[0]]);
    vertex1[0] = ptsOld[triIs[1]] + f0*(ptsNew[triIs[1]] - ptsOld[triIs[1]]);
    vertex2[0] = ptsOld[triIs[2]] + f0*(ptsNew[triIs[2]] - ptsOld[triIs[2]]);

    centre[1] = f1*(ccNew - ccOld);
    base[1] = f1*(ptsNew[triIs[0]] - ptsOld[triIs[0]]);
    vertex1[1] = f1*(ptsNew[triIs[1]] - ptsOld[triIs[1]]);
    vertex2[1] = f1*(ptsNew[triIs[2]] - ptsOld[triIs[2]]);
}


inline CML::Pair<CML::barycentricTensor> CML::particle::movingTetTransform
(
    const scalar fraction
) const
{
    Pair<vector> centre, base, vertex1, vertex2;
    movingTetGeometry(fraction, centre, base, vertex1, vertex2);

    return
        Pair<barycentricTensor>
        (
            barycentricTensor(centre[0], base[0], vertex1[0], vertex2[0]),
            barycentricTensor(centre[1], base[1], vertex1[1], vertex2[1])
        );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::particle::getNewParticleID() const
{
    label id = particleCount_++;

    if (id == labelMax)
    {
        WarningInFunction
            << "Particle counter has overflowed. This might cause problems"
            << " when reconstructing particle tracks." << endl;
    }
    return id;
}


inline const CML::polyMesh& CML::particle::mesh() const
{
    return mesh_;
}


inline const CML::barycentric& CML::particle::coordinates() const
{
    return coordinates_;
}


inline CML::label CML::particle::cell() const
{
    return celli_;
}

inline CML::label& CML::particle::cell()
{
    return celli_;
}

inline CML::label CML::particle::tetFace() const
{
    return tetFacei_;
}


inline CML::label CML::particle::tetPt() const
{
    return tetPti_;
}


inline CML::label CML::particle::face() const
{
    return facei_;
}


inline CML::scalar CML::particle::stepFraction() const
{
    return stepFraction_;
}


inline CML::scalar& CML::particle::stepFraction()
{
    return stepFraction_;
}


inline CML::label CML::particle::origProc() const
{
    return origProc_;
}


inline CML::label& CML::particle::origProc()
{
    return origProc_;
}


inline CML::label CML::particle::origId() const
{
    return origId_;
}


inline CML::label& CML::particle::origId()
{
    return origId_;
}


inline CML::Pair<CML::scalar> CML::particle::stepFractionSpan() const
{
    if (mesh_.time().subCycling())
    {
        const TimeState& tsNew = mesh_.time();
        const TimeState& tsOld = mesh_.time().prevTimeState();

        const scalar tFrac =
        (
            (tsNew.value() - tsNew.deltaTValue())
          - (tsOld.value() - tsOld.deltaTValue())
        )/tsOld.deltaTValue();

        const scalar dtFrac = tsNew.deltaTValue()/tsOld.deltaTValue();

        return Pair<scalar>(tFrac, dtFrac);
    }
    else
    {
        return Pair<scalar>(0, 1);
    }
}


inline CML::scalar CML::particle::currentTimeFraction() const
{
    const Pair<scalar> s = stepFractionSpan();

    return s[0] + stepFraction_*s[1];
}


inline CML::tetIndices CML::particle::currentTetIndices() const
{
    return tetIndices(celli_, tetFacei_, tetPti_);
}


inline CML::barycentricTensor CML::particle::currentTetTransform() const
{
    if (mesh_.moving())
    {
        return movingTetTransform(0)[0];
    }
    else
    {
        return stationaryTetTransform();
    }
}


inline CML::vector CML::particle::normal() const
{
    return currentTetIndices().faceTri(mesh_).normal();
}


inline bool CML::particle::onFace() const
{
    return facei_ >= 0;
}


inline bool CML::particle::onInternalFace() const
{
    return onFace() && mesh_.isInternalFace(facei_);
}


inline bool CML::particle::onBoundaryFace() const
{
    return onFace() && !mesh_.isInternalFace(facei_);
}


inline CML::label CML::particle::patch() const
{
    return onFace() ? mesh_.boundaryMesh().whichPatch(facei_) : -1;
}


inline CML::vector CML::particle::position() const
{
    return currentTetTransform() & coordinates_;
}


inline void CML::particle::reset()
{
    stepFraction_ = 0;
    nBehind_ = 0;
    behind_ = 0;
}


void CML::particle::patchData(vector& n, vector& U) const
{
    if (!onBoundaryFace())
    {
        FatalErrorInFunction
            << "Patch data was requested for a particle that isn't on a patch"
            << exit(FatalError);
    }

    if (mesh_.moving())
    {
        Pair<vector> centre, base, vertex1, vertex2;
        movingTetGeometry(1, centre, base, vertex1, vertex2);

        n = triPointRef(base[0], vertex1[0], vertex2[0]).normal();

        // Interpolate the motion of the three face vertices to the current
        // coordinates
        U =
            coordinates_.b()*base[1]
          + coordinates_.c()*vertex1[1]
          + coordinates_.d()*vertex2[1];

        // The movingTetGeometry method gives the motion as a displacement
        // across the time-step, so we divide by the time-step to get velocity
        U /= mesh_.time().deltaTValue();
    }
    else
    {
        vector centre, base, vertex1, vertex2;
        stationaryTetGeometry(centre, base, vertex1, vertex2);

        n = triPointRef(base, vertex1, vertex2).normal();

        U = Zero;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class TrackCloudType>
void CML::particle::readFields(TrackCloudType& c)
{
    if (!c.size())
    {
        return;
    }

    IOobject procIO(c.fieldIOobject("origProcId", IOobject::MUST_READ));

    if (procIO.headerOk())
    {
        IOField<label> origProcId(procIO);
        c.checkFieldIOobject(c, origProcId);
        IOField<label> origId(c.fieldIOobject("origId", IOobject::MUST_READ));
        c.checkFieldIOobject(c, origId);

        label i = 0;
        forAllIter(typename TrackCloudType, c, iter)
        {
            particle& p = iter();

            p.origProc_ = origProcId[i];
            p.origId_ = origId[i];
            i++;
        }
    }
}


template<class TrackCloudType>
void CML::particle::writeFields(const TrackCloudType& c)
{
    label np = c.size();

    IOPosition<TrackCloudType> ioP(c);
    ioP.write();

    IOField<label> origProc
    (
        c.fieldIOobject("origProcId", IOobject::NO_READ),
        np
    );
    IOField<label> origId
    (
        c.fieldIOobject("origId", IOobject::NO_READ),
        np
    );

    label i = 0;
    forAllConstIter(typename TrackCloudType, c, iter)
    {
        origProc[i] = iter().origProc_;
        origId[i] = iter().origId_;
        i++;
    }

    origProc.write();
    origId.write();
}


template<class TrackCloudType>
void CML::particle::hitFace
(
    const vector& direction,
    TrackCloudType& cloud,
    trackingData& td
)
{
    if (debug)
    {
        Info << "Particle " << origId() << nl << FUNCTION_NAME << nl << endl;
    }

    typename TrackCloudType::particleType& p =
        static_cast<typename TrackCloudType::particleType&>(*this);
    typename TrackCloudType::particleType::trackingData& ttd =
        static_cast<typename TrackCloudType::particleType::trackingData&>(td);

    if (!onFace())
    {
        return;
    }
    else if (onInternalFace())
    {
        changeCell();
    }
    else if (onBoundaryFace())
    {
        changeToMasterPatch();

        if (!p.hitPatch(cloud, ttd))
        {
            const polyPatch& patch = mesh_.boundaryMesh()[p.patch()];

            if (isA<wedgePolyPatch>(patch))
            {
                p.hitWedgePatch(cloud, ttd);
            }
//            else if (isA<symmetryPlanePolyPatch>(patch))
//            {
//                p.hitSymmetryPlanePatch(cloud, ttd);
//            }
            else if (isA<symmetryPolyPatch>(patch))
            {
                p.hitSymmetryPatch(cloud, ttd);
            }
            else if (isA<cyclicPolyPatch>(patch))
            {
                p.hitCyclicPatch(cloud, ttd);
            }
            else if (isA<cyclicACMIPolyPatch>(patch))
            {
                p.hitCyclicACMIPatch(cloud, ttd, direction);
            }
            else if (isA<cyclicAMIPolyPatch>(patch))
            {
                p.hitCyclicAMIPatch(cloud, ttd, direction);
            }
//            else if (isA<cyclicRepeatAMIPolyPatch>(patch))
//            {
//                p.hitCyclicRepeatAMIPatch(cloud, ttd, direction);
//            }
            else if (isA<processorPolyPatch>(patch))
            {
                p.hitProcessorPatch(cloud, ttd);
            }
            else if (isA<wallPolyPatch>(patch))
            {
                p.hitWallPatch(cloud, ttd);
            }
            else
            {
                td.keepParticle = false;
            }
        }
    }
}


template<class TrackCloudType>
CML::scalar CML::particle::trackToAndHitFace
(
    const vector& direction,
    const scalar fraction,
    TrackCloudType& cloud,
    trackingData& td
)
{
    if (debug)
    {
        Info << "Particle " << origId() << nl << FUNCTION_NAME << nl << endl;
    }

    const scalar f = trackToFace(direction, fraction);

    hitFace(direction, cloud, td);

    return f;
}


template<class TrackCloudType>
bool CML::particle::hitPatch(TrackCloudType&, trackingData&)
{
    return false;
}


template<class TrackCloudType>
void CML::particle::hitWedgePatch(TrackCloudType& cloud, trackingData& td)
{
    FatalErrorInFunction
        << "Hitting a wedge patch should not be possible."
        << abort(FatalError);

    hitSymmetryPatch(cloud, td);
}


//template<class TrackCloudType>
//void CML::particle::hitSymmetryPlanePatch
//(
//    TrackCloudType& cloud,
//    trackingData& td
//)
//{
//    hitSymmetryPatch(cloud, td);
//}


template<class TrackCloudType>
void CML::particle::hitSymmetryPatch(TrackCloudType&, trackingData&)
{
    const vector nf = normal();
    transformProperties(I - 2.0*nf*nf);
}


template<class TrackCloudType>
void CML::particle::hitCyclicPatch(TrackCloudType&, trackingData&)
{
    const cyclicPolyPatch& cpp =
        static_cast<const cyclicPolyPatch&>(mesh_.boundaryMesh()[patch()]);
    const cyclicPolyPatch& receiveCpp = cpp.neighbPatch();
    const label receiveFacei = receiveCpp.whichFace(facei_);

    // Set the topology
    facei_ = tetFacei_ = cpp.transformGlobalFace(facei_);
    celli_ = mesh_.faceOwner()[facei_];
    // See note in correctAfterParallelTransfer for tetPti addressing ...
    tetPti_ = mesh_.faces()[tetFacei_].size() - 1 - tetPti_;

    // Reflect to account for the change of triangle orientation in the new cell
    reflect();

    // Transform the properties
    if (!receiveCpp.parallel())
    {
        const tensor& T =
        (
            receiveCpp.forwardT().size() == 1
          ? receiveCpp.forwardT()[0]
          : receiveCpp.forwardT()[receiveFacei]
        );
        transformProperties(T);
    }
    else if (receiveCpp.separated())
    {
        const vector& s =
        (
            (receiveCpp.separation().size() == 1)
          ? receiveCpp.separation()[0]
          : receiveCpp.separation()[receiveFacei]
        );
        transformProperties(-s);
    }
}


template<class TrackCloudType>
void CML::particle::hitCyclicAMIPatch
(
    TrackCloudType&,
    trackingData& td,
    const vector& direction
)
{
    vector pos = position();

    const cyclicAMIPolyPatch& cpp =
        static_cast<const cyclicAMIPolyPatch&>(mesh_.boundaryMesh()[patch()]);
    const cyclicAMIPolyPatch& receiveCpp = cpp.neighbPatch();
    const label sendFacei = cpp.whichFace(facei_);
    const label receiveFacei = cpp.pointFace(sendFacei, direction, pos);

    if (receiveFacei < 0)
    {
        // If the patch face of the particle is not known assume that the
        // particle is lost and mark it to be deleted.
        td.keepParticle = false;
        WarningInFunction
            << "Particle lost across " << cyclicAMIPolyPatch::typeName
            << " patches " << cpp.name() << " and " << receiveCpp.name()
            << " at position " << pos << endl;
    }

    // Set the topology
    facei_ = tetFacei_ = receiveFacei + receiveCpp.start();

    // Locate the particle on the receiving side
    vector directionT = direction;
    cpp.reverseTransformDirection(directionT, sendFacei);
    locate
    (
        pos,
        &directionT,
        mesh_.faceOwner()[facei_],
        false,
        "Particle crossed between " + cyclicAMIPolyPatch::typeName +
        " patches " + cpp.name() + " and " + receiveCpp.name() +
        " to a location outside of the mesh."
    );

    // The particle must remain associated with a face for the tracking to
    // register as incomplete
    facei_ = tetFacei_;

    // Transform the properties
    if (!receiveCpp.parallel())
    {
        const tensor& T =
        (
            receiveCpp.forwardT().size() == 1
          ? receiveCpp.forwardT()[0]
          : receiveCpp.forwardT()[receiveFacei]
        );
        transformProperties(T);
    }
    else if (receiveCpp.separated())
    {
        const vector& s =
        (
            (receiveCpp.separation().size() == 1)
          ? receiveCpp.separation()[0]
          : receiveCpp.separation()[receiveFacei]
        );
        transformProperties(-s);
    }
}


template<class TrackCloudType>
void CML::particle::hitCyclicACMIPatch
(
    TrackCloudType& cloud,
    trackingData& td,
    const vector& direction
)
{
    const cyclicACMIPolyPatch& cpp =
        static_cast<const cyclicACMIPolyPatch&>(mesh_.boundaryMesh()[patch()]);

    const label localFacei = cpp.whichFace(facei_);

    // If the mask is within the patch tolerance at either end, then we can
    // assume an interaction with the appropriate part of the ACMI pair.
    const scalar mask = cpp.mask()[localFacei];
    bool couple = mask >= 1 - cpp.tolerance();
    bool nonOverlap = mask <= cpp.tolerance();

    // If the mask is an intermediate value, then we search for a location on
    // the other side of the AMI. If we can't find a location, then we assume
    // that we have hit the non-overlap patch.
    if (!couple && !nonOverlap)
    {
        vector pos = position();
        couple = cpp.pointFace(localFacei, direction, pos) >= 0;
        nonOverlap = !couple;
    }

    if (couple)
    {
        hitCyclicAMIPatch(cloud, td, direction);
    }
    else
    {
        // Move to the face associated with the non-overlap patch and redo the
        // face interaction.
        tetFacei_ = facei_ = cpp.nonOverlapPatch().start() + localFacei;
        hitFace(direction, cloud, td);
    }
}


//template<class TrackCloudType>
//void CML::particle::hitCyclicRepeatAMIPatch
//(
//    TrackCloudType& cloud,
//    trackingData& td,
//    const vector& direction
//)
//{
//
//    hitCyclicAMIPatch(cloud, td, direction);
//}


template<class TrackCloudType>
void CML::particle::hitProcessorPatch(TrackCloudType&, trackingData&)
{}


template<class TrackCloudType>
void CML::particle::hitWallPatch(TrackCloudType&, trackingData&)
{}


#endif

// ************************************************************************* //
