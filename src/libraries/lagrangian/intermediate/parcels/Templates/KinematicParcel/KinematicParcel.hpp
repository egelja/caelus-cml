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

Class
    CML::KinematicParcel

Description
    Kinematic parcel class with rotational motion (as spherical
    particles only) and one/two-way coupling with the continuous
    phase.

    Sub-models include:
    - drag
    - turbulent dispersion
    - wall interactions


\*---------------------------------------------------------------------------*/

#ifndef KinematicParcel_H
#define KinematicParcel_H

#include "particle.hpp"
#include "IOstream.hpp"
#include "autoPtr.hpp"
#include "interpolation.hpp"
#include "mathematicalConstants.hpp"
#include "forceSuSp.hpp"
#include "IntegrationScheme.hpp"
#include "meshTools.hpp"
#include "IOstreams.hpp"
#include "IOField.hpp"
#include "Cloud.hpp"

// #include "ParticleForceList.hpp" // TODO

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class ParcelType>
class KinematicParcel;

// Forward declaration of friend functions

template<class ParcelType>
Ostream& operator<<
(
    Ostream&,
    const KinematicParcel<ParcelType>&
);

/*---------------------------------------------------------------------------*\
                         Class KinematicParcel Declaration
\*---------------------------------------------------------------------------*/

template<class ParcelType>
class KinematicParcel
:
    public ParcelType
{
public:

    //- Class to hold kinematic particle constant properties
    class constantProperties
    {
        // Private data

            //- Constant properties dictionary
            const dictionary dict_;

            //- Parcel type id - used for post-processing to flag the type
            //  of parcels issued by this cloud
            label parcelTypeId_;

            //- Minimum density [kg/m3]
            scalar rhoMin_;

            //- Particle density [kg/m3] (constant)
            scalar rho0_;

            //- Minimum particle mass [kg]
            scalar minParticleMass_;

            //- Young's modulus [N/m2]
            scalar youngsModulus_;

            //- Poisson's ratio
            scalar poissonsRatio_;


    public:

        // Constructors

            //- Null constructor
            constantProperties();

            //- Copy constructor
            constantProperties(const constantProperties& cp);

            //- Constructor from dictionary
            constantProperties
            (
                const dictionary& parentDict,
                const bool readFields = true
            );

            //- Construct from components
            constantProperties
            (
                const label parcelTypeId,
                const scalar rhoMin,
                const scalar rho0,
                const scalar minParticleMass,
                const scalar youngsModulus,
                const scalar poissonsRatio
            );


        // Member functions

            //- Return const access to the constant properties dictionary
            inline const dictionary& dict() const;

            //- Return const access to the parcel type id
            inline label parcelTypeId() const;

            //- Return const access to the minimum density
            inline scalar rhoMin() const;

            //- Return const access to the particle density
            inline scalar rho0() const;

            //- Return const access to the minimum particle mass
            inline scalar minParticleMass() const;

            //- Return const access to Young's Modulus
            inline scalar youngsModulus() const;

            //- Return const access to Poisson's ratio
            inline scalar poissonsRatio() const;
    };


    template<class CloudType>
    class TrackingData
    :
        public ParcelType::template TrackingData<CloudType>
    {
    public:

        enum trackPart
        {
            tpVelocityHalfStep,
            tpLinearTrack,
            tpRotationalTrack
        };


    private:

        // Private data

            // Interpolators for continuous phase fields

                //- Density interpolator
                autoPtr<interpolation<scalar> > rhoInterp_;

                //- Velocity interpolator
                autoPtr<interpolation<vector> > UInterp_;

                //- Dynamic viscosity interpolator
                autoPtr<interpolation<scalar> > muInterp_;


            //- Local gravitational or other body-force acceleration
            const vector& g_;

            // label specifying which part of the integration
            // algorithm is taking place
            trackPart part_;


    public:

        // Constructors

            //- Construct from components
            inline TrackingData
            (
                CloudType& cloud,
                trackPart part = tpLinearTrack
            );


        // Member functions

            //- Return conat access to the interpolator for continuous
            //  phase density field
            inline const interpolation<scalar>& rhoInterp() const;

            //- Return conat access to the interpolator for continuous
            //  phase velocity field
            inline const interpolation<vector>& UInterp() const;

            //- Return conat access to the interpolator for continuous
            //  phase dynamic viscosity field
            inline const interpolation<scalar>& muInterp() const;

            // Return const access to the gravitational acceleration vector
            inline const vector& g() const;

            //- Return the part of the tracking operation taking place
            inline trackPart part() const;

            //- Return access to the part of the tracking operation taking place
            inline trackPart& part();
    };


protected:

    // Protected data

        // Parcel properties

            //- Active flag - tracking inactive when active = false
            bool active_;

            //- Parcel type id
            label typeId_;

            //- Number of particles in Parcel
            scalar nParticle_;

            //- Diameter [m]
            scalar d_;

            //- Target diameter [m]
            scalar dTarget_;

            //- Velocity of Parcel [m/s]
            vector U_;

            //- Force on particle due to collisions [N]
            vector f_;

            //- Angular momentum of Parcel in global reference frame
            // [kg m2/s]
            vector angularMomentum_;

            //- Torque on particle due to collisions in global
            //  reference frame [Nm]
            vector torque_;

            //- Density [kg/m3]
            scalar rho_;

            //- Age [s]
            scalar age_;

            //- Time spent in turbulent eddy [s]
            scalar tTurb_;

            //- Turbulent velocity fluctuation [m/s]
            vector UTurb_;


        // Cell-based quantities

            //- Density [kg/m3]
            scalar rhoc_;

            //- Velocity [m/s]
            vector Uc_;

            //- Viscosity [Pa.s]
            scalar muc_;


    // Protected Member Functions

        //- Calculate new particle velocity
        template<class TrackData>
        const vector calcVelocity
        (
            TrackData& td,
            const scalar dt,           // timestep
            const label cellI,         // owner cell
            const scalar Re,           // Reynolds number
            const scalar mu,           // local carrier viscosity
            const scalar mass,         // mass
            const vector& Su,          // explicit particle momentum source
            vector& dUTrans,           // momentum transfer to carrier
            scalar& Spu                // linearised drag coefficient
        ) const;


public:

    // Static data members

        //- String representation of properties
        static string propHeader;

        //- Runtime type information
        TypeName("KinematicParcel");


    // Constructors

        //- Construct from owner, position, and cloud owner
        //  Other properties initialised as null
        inline KinematicParcel
        (
            const polyMesh& mesh,
            const vector& position,
            const label cellI,
            const label tetFaceI,
            const label tetPtI
        );

        //- Construct from components
        inline KinematicParcel
        (
            const polyMesh& mesh,
            const vector& position,
            const label cellI,
            const label tetFaceI,
            const label tetPtI,
            const label typeId,
            const scalar nParticle0,
            const scalar d0,
            const scalar dTarget0,
            const vector& U0,
            const vector& f0,
            const vector& angularMomentum0,
            const vector& torque0,
            const constantProperties& constProps
        );

        //- Construct from Istream
        KinematicParcel
        (
            const polyMesh& mesh,
            Istream& is,
            bool readFields = true
        );

        //- Construct as a copy
        KinematicParcel(const KinematicParcel& p);

        //- Construct as a copy
        KinematicParcel(const KinematicParcel& p, const polyMesh& mesh);

        //- Construct and return a (basic particle) clone
        virtual autoPtr<particle> clone() const
        {
            return autoPtr<particle>(new KinematicParcel(*this));
        }

        //- Construct and return a (basic particle) clone
        virtual autoPtr<particle> clone(const polyMesh& mesh) const
        {
            return autoPtr<particle>(new KinematicParcel(*this, mesh));
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

            autoPtr<KinematicParcel<ParcelType> > operator()(Istream& is) const
            {
                return autoPtr<KinematicParcel<ParcelType> >
                (
                    new KinematicParcel<ParcelType>(mesh_, is, true)
                );
            }
        };


    // Member Functions

        // Access

            //- Return const access to active flag
            inline bool active() const;

            //- Return const access to type id
            inline label typeId() const;

            //- Return const access to number of particles
            inline scalar nParticle() const;

            //- Return const access to diameter
            inline scalar d() const;

            //- Return const access to target diameter
            inline scalar dTarget() const;

            //- Return const access to velocity
            inline const vector& U() const;

            //- Return const access to force
            inline const vector& f() const;

            //- Return const access to angular momentum
            inline const vector& angularMomentum() const;

            //- Return const access to torque
            inline const vector& torque() const;

            //- Return const access to density
            inline scalar rho() const;

            //- Return const access to the age
            inline scalar age() const;

            //- Return const access to time spent in turbulent eddy
            inline scalar tTurb() const;

            //- Return const access to turbulent velocity fluctuation
            inline const vector& UTurb() const;

            //- Return const access to carrier density [kg/m3]
            inline scalar rhoc() const;

            //- Return const access to carrier velocity [m/s]
            inline const vector& Uc() const;

            //- Return const access to carrier viscosity [Pa.s]
            inline scalar muc() const;


        // Edit

            //- Return const access to active flag
            inline bool& active();

            //- Return access to type id
            inline label& typeId();

            //- Return access to number of particles
            inline scalar& nParticle();

            //- Return access to diameter
            inline scalar& d();

            //- Return access to target diameter
            inline scalar& dTarget();

            //- Return access to velocity
            inline vector& U();

            //- Return access to force
            inline vector& f();

            //- Return access to angular momentum
            inline vector& angularMomentum();

            //- Return access to torque
            inline vector& torque();

            //- Return access to density
            inline scalar& rho();

            //- Return access to the age
            inline scalar& age();

            //- Return access to time spent in turbulent eddy
            inline scalar& tTurb();

            //- Return access to turbulent velocity fluctuation
            inline vector& UTurb();


        // Helper functions

            //- Return the index of the face to be used in the interpolation
            //  routine
            inline label faceInterpolation() const;

            //- Cell owner mass
            inline scalar massCell(const label cellI) const;

            //- Particle mass
            inline scalar mass() const;

            //- Particle moment of inertia around diameter axis
            inline scalar momentOfInertia() const;

            //- Particle angular velocity
            inline vector omega() const;

            //- Particle volume
            inline scalar volume() const;

            //- Particle volume for a given diameter
            inline static scalar volume(const scalar d);

            //- Particle projected area
            inline scalar areaP() const;

            //- Projected area for given diameter
            inline static scalar areaP(const scalar d);

            //- Particle surface area
            inline scalar areaS() const;

            //- Surface area for given diameter
            inline static scalar areaS(const scalar d);

            //- Reynolds number
            inline scalar Re
            (
                const vector& U,        // particle velocity
                const scalar d,         // particle diameter
                const scalar rhoc,      // carrier density
                const scalar muc        // carrier dynamic viscosity
            ) const;

            //- Weber number
            inline scalar We
            (
                const vector& U,        // particle velocity
                const scalar d,         // particle diameter
                const scalar rhoc,      // carrier density
                const scalar sigma      // particle surface tension
            ) const;


        // Main calculation loop

            //- Set cell values
            template<class TrackData>
            void setCellValues
            (
                TrackData& td,
                const scalar dt,
                const label cellI
            );

            //- Correct cell values using latest transfer information
            template<class TrackData>
            void cellValueSourceCorrection
            (
                TrackData& td,
                const scalar dt,
                const label cellI
            );

            //- Update parcel properties over the time interval
            template<class TrackData>
            void calc
            (
                TrackData& td,
                const scalar dt,
                const label cellI
            );


        // Tracking

            //- Move the parcel
            template<class TrackData>
            bool move(TrackData& td, const scalar trackTime);


        // Patch interactions

            //- Overridable function to handle the particle hitting a face
            //  without trackData
            void hitFace(int& td);

            //- Overridable function to handle the particle hitting a face
            template<class TrackData>
            void hitFace(TrackData& td);

            //- Overridable function to handle the particle hitting a patch
            //  Executed before other patch-hitting functions
            template<class TrackData>
            bool hitPatch
            (
                const polyPatch& p,
                TrackData& td,
                const label patchI,
                const scalar trackFraction,
                const tetIndices& tetIs
            );

            //- Overridable function to handle the particle hitting a
            //  processorPatch
            template<class TrackData>
            void hitProcessorPatch
            (
                const processorPolyPatch&,
                TrackData& td
            );

            //- Overridable function to handle the particle hitting a wallPatch
            template<class TrackData>
            void hitWallPatch
            (
                const wallPolyPatch&,
                TrackData& td,
                const tetIndices&
            );

            //- Overridable function to handle the particle hitting a polyPatch
            template<class TrackData>
            void hitPatch
            (
                const polyPatch&,
                TrackData& td
            );

            //- Transform the physical properties of the particle
            //  according to the given transformation tensor
            virtual void transformProperties(const tensor& T);

            //- Transform the physical properties of the particle
            //  according to the given separation vector
            virtual void transformProperties(const vector& separation);

            //- The nearest distance to a wall that the particle can be
            //  in the n direction
            virtual scalar wallImpactDistance(const vector& n) const;


        // I-O

            //- Read
            template<class CloudType>
            static void readFields(CloudType& c);

            //- Write
            template<class CloudType>
            static void writeFields(const CloudType& c);


    // Ostream Operator

        friend Ostream& operator<< <ParcelType>
        (
            Ostream&,
            const KinematicParcel<ParcelType>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

using namespace CML::constant::mathematical;

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
inline
CML::KinematicParcel<ParcelType>::constantProperties::constantProperties()
:
    dict_(dictionary::null),
    parcelTypeId_(-1),
    rhoMin_(0.0),
    rho0_(0.0),
    minParticleMass_(0.0),
    youngsModulus_(0.0),
    poissonsRatio_(0.0)
{}


template<class ParcelType>
inline CML::KinematicParcel<ParcelType>::constantProperties::constantProperties
(
    const constantProperties& cp
)
:
    dict_(cp.dict_),
    parcelTypeId_(cp.parcelTypeId_),
    rhoMin_(cp.rhoMin_),
    rho0_(cp.rho0_),
    minParticleMass_(cp.minParticleMass_),
    youngsModulus_(cp.youngsModulus_),
    poissonsRatio_(cp.poissonsRatio_)
{}


template<class ParcelType>
inline CML::KinematicParcel<ParcelType>::constantProperties::constantProperties
(
    const dictionary& parentDict,
    const bool readFields
)
:
    dict_(parentDict.subOrEmptyDict("constantProperties")),
    parcelTypeId_(-1),
    rhoMin_(0.0),
    rho0_(0.0),
    minParticleMass_(0.0),
    youngsModulus_(0.0),
    poissonsRatio_(0.0)
{
    if (readFields)
    {
        dict_.lookup("parcelTypeId") >> parcelTypeId_;
        dict_.lookup("rhoMin") >> rhoMin_;
        dict_.lookup("rho0") >> rho0_;
        dict_.lookup("minParticleMass") >> minParticleMass_;
        dict_.lookup("youngsModulus") >> youngsModulus_;
        dict_.lookup("poissonsRatio") >> poissonsRatio_;
    }
}

template<class ParcelType>
inline CML::KinematicParcel<ParcelType>::constantProperties::constantProperties
(
    const label parcelTypeId,
    const scalar rhoMin,
    const scalar rho0,
    const scalar minParticleMass,
    const scalar youngsModulus,
    const scalar poissonsRatio
)
:
    dict_(dictionary::null),
    parcelTypeId_(parcelTypeId),
    rhoMin_(rhoMin),
    rho0_(rho0),
    minParticleMass_(minParticleMass),
    youngsModulus_(youngsModulus),
    poissonsRatio_(poissonsRatio)
{}


template<class ParcelType>
inline CML::KinematicParcel<ParcelType>::KinematicParcel
(
    const polyMesh& owner,
    const vector& position,
    const label cellI,
    const label tetFaceI,
    const label tetPtI
)
:
    ParcelType(owner, position, cellI, tetFaceI, tetPtI),
    active_(true),
    typeId_(-1),
    nParticle_(0),
    d_(0.0),
    dTarget_(0.0),
    U_(vector::zero),
    f_(vector::zero),
    angularMomentum_(vector::zero),
    torque_(vector::zero),
    rho_(0.0),
    age_(0.0),
    tTurb_(0.0),
    UTurb_(vector::zero),
    rhoc_(0.0),
    Uc_(vector::zero),
    muc_(0.0)
{}


template<class ParcelType>
inline CML::KinematicParcel<ParcelType>::KinematicParcel
(
    const polyMesh& owner,
    const vector& position,
    const label cellI,
    const label tetFaceI,
    const label tetPtI,
    const label typeId,
    const scalar nParticle0,
    const scalar d0,
    const scalar dTarget0,
    const vector& U0,
    const vector& f0,
    const vector& angularMomentum0,
    const vector& torque0,
    const constantProperties& constProps
)
:
    ParcelType(owner, position, cellI, tetFaceI, tetPtI),
    active_(true),
    typeId_(typeId),
    nParticle_(nParticle0),
    d_(d0),
    dTarget_(dTarget0),
    U_(U0),
    f_(f0),
    angularMomentum_(angularMomentum0),
    torque_(torque0),
    rho_(constProps.rho0()),
    age_(0.0),
    tTurb_(0.0),
    UTurb_(vector::zero),
    rhoc_(0.0),
    Uc_(vector::zero),
    muc_(0.0)
{}


// * * * * * * * * * constantProperties Member Functions * * * * * * * * * * //

template<class ParcelType>
inline const CML::dictionary&
CML::KinematicParcel<ParcelType>::constantProperties::dict() const
{
    return dict_;
}


template<class ParcelType>
inline CML::label
CML::KinematicParcel<ParcelType>::constantProperties::parcelTypeId() const
{
    return parcelTypeId_;
}


template<class ParcelType>
inline CML::scalar
CML::KinematicParcel<ParcelType>::constantProperties::rhoMin() const
{
    return rhoMin_;
}


template<class ParcelType>
inline CML::scalar
CML::KinematicParcel<ParcelType>::constantProperties::rho0() const
{
    return rho0_;
}


template<class ParcelType>
inline CML::scalar
CML::KinematicParcel<ParcelType>::constantProperties::minParticleMass() const
{
    return minParticleMass_;
}


template<class ParcelType>
inline CML::scalar
CML::KinematicParcel<ParcelType>::constantProperties::youngsModulus() const
{
    return youngsModulus_;
}


template<class ParcelType>
inline CML::scalar
CML::KinematicParcel<ParcelType>::constantProperties::poissonsRatio() const
{
    return poissonsRatio_;
}


// * * * * * * * KinematicParcel Member Functions  * * * * * * * //

template<class ParcelType>
inline bool CML::KinematicParcel<ParcelType>::active() const
{
    return active_;
}


template<class ParcelType>
inline CML::label CML::KinematicParcel<ParcelType>::typeId() const
{
    return typeId_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::nParticle() const
{
    return nParticle_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::d() const
{
    return d_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::dTarget() const
{
    return dTarget_;
}


template<class ParcelType>
inline const CML::vector& CML::KinematicParcel<ParcelType>::U() const
{
    return U_;
}


template<class ParcelType>
inline const CML::vector&
CML::KinematicParcel<ParcelType>::f() const
{
    return f_;
}


template<class ParcelType>
inline const CML::vector&
CML::KinematicParcel<ParcelType>::angularMomentum() const
{
    return angularMomentum_;
}


template<class ParcelType>
inline const CML::vector&
CML::KinematicParcel<ParcelType>::torque() const
{
    return torque_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::rho() const
{
    return rho_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::age() const
{
    return age_;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::tTurb() const
{
    return tTurb_;
}


template<class ParcelType>
inline const CML::vector& CML::KinematicParcel<ParcelType>::UTurb() const
{
    return UTurb_;
}


template <class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::rhoc() const
{
    return rhoc_;
}


template <class ParcelType>
inline const CML::vector& CML::KinematicParcel<ParcelType>::Uc() const
{
    return Uc_;
}


template <class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::muc() const
{
    return muc_;
}


template<class ParcelType>
inline bool& CML::KinematicParcel<ParcelType>::active()
{
    return active_;
}


template<class ParcelType>
inline CML::label& CML::KinematicParcel<ParcelType>::typeId()
{
    return typeId_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::nParticle()
{
    return nParticle_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::d()
{
    return d_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::dTarget()
{
    return dTarget_;
}


template<class ParcelType>
inline CML::vector& CML::KinematicParcel<ParcelType>::U()
{
    return U_;
}


template<class ParcelType>
inline CML::vector& CML::KinematicParcel<ParcelType>::f()
{
    return f_;
}


template<class ParcelType>
inline CML::vector& CML::KinematicParcel<ParcelType>::angularMomentum()
{
    return angularMomentum_;
}


template<class ParcelType>
inline CML::vector& CML::KinematicParcel<ParcelType>::torque()
{
    return torque_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::rho()
{
    return rho_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::age()
{
    return age_;
}


template<class ParcelType>
inline CML::scalar& CML::KinematicParcel<ParcelType>::tTurb()
{
    return tTurb_;
}


template<class ParcelType>
inline CML::vector& CML::KinematicParcel<ParcelType>::UTurb()
{
    return UTurb_;
}


template<class ParcelType>
inline CML::label CML::KinematicParcel<ParcelType>::faceInterpolation() const
{
    // Use volume-based interpolation if dealing with external faces
    if (this->cloud().internalFace(this->face()))
    {
        return this->face();
    }
    else
    {
        return -1;
    }
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::massCell
(
    const label cellI
) const
{
    return rhoc_*this->mesh().cellVolumes()[cellI];
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::mass() const
{
    return rho_*volume();
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::momentOfInertia() const
{
    return 0.1*mass()*sqr(d_);
}


template<class ParcelType>
inline CML::vector CML::KinematicParcel<ParcelType>::omega() const
{
    return angularMomentum_/momentOfInertia();
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::volume() const
{
    return volume(d_);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::volume(const scalar d)
{
    return pi/6.0*pow3(d);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::areaP() const
{
    return areaP(d_);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::areaP(const scalar d)
{
    return 0.25*areaS(d);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::areaS() const
{
    return areaS(d_);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::areaS(const scalar d)
{
    return pi*d*d;
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::Re
(
    const vector& U,
    const scalar d,
    const scalar rhoc,
    const scalar muc
) const
{
    return rhoc*mag(U - Uc_)*d/(muc + ROOTVSMALL);
}


template<class ParcelType>
inline CML::scalar CML::KinematicParcel<ParcelType>::We
(
    const vector& U,
    const scalar d,
    const scalar rhoc,
    const scalar sigma
) const
{
    return rhoc*magSqr(U - Uc_)*d/(sigma + ROOTVSMALL);
}


// ************************************************************************* //
template<class ParcelType>
template<class CloudType>
inline CML::KinematicParcel<ParcelType>::TrackingData<CloudType>::TrackingData
(
    CloudType& cloud,
    trackPart part
)
:
    ParcelType::template TrackingData<CloudType>(cloud),
    rhoInterp_
    (
        interpolation<scalar>::New
        (
            cloud.solution().interpolationSchemes(),
            cloud.rho()
        )
    ),
    UInterp_
    (
        interpolation<vector>::New
        (
            cloud.solution().interpolationSchemes(),
            cloud.U()
        )
    ),
    muInterp_
    (
        interpolation<scalar>::New
        (
            cloud.solution().interpolationSchemes(),
            cloud.mu()
        )
    ),
    g_(cloud.g().value()),
    part_(part)
{}


template<class ParcelType>
template<class CloudType>
inline const CML::interpolation<CML::scalar>&
CML::KinematicParcel<ParcelType>::TrackingData<CloudType>::rhoInterp() const
{
    return rhoInterp_();
}


template<class ParcelType>
template<class CloudType>
inline const CML::interpolation<CML::vector>&
CML::KinematicParcel<ParcelType>::TrackingData<CloudType>::UInterp() const
{
    return UInterp_();
}


template<class ParcelType>
template<class CloudType>
inline const CML::interpolation<CML::scalar>&
CML::KinematicParcel<ParcelType>::TrackingData<CloudType>::muInterp() const
{
    return muInterp_();
}


template<class ParcelType>
template<class CloudType>
inline const CML::vector&
CML::KinematicParcel<ParcelType>::TrackingData<CloudType>::g() const
{
    return g_;
}


template<class ParcelType>
template<class CloudType>
inline typename CML::KinematicParcel<ParcelType>::template
TrackingData<CloudType>::trackPart
CML::KinematicParcel<ParcelType>::TrackingData<CloudType>::part() const
{
    return part_;
}


template<class ParcelType>
template<class CloudType>
inline typename CML::KinematicParcel<ParcelType>::template
TrackingData<CloudType>::trackPart&
CML::KinematicParcel<ParcelType>::TrackingData<CloudType>::part()
{
    return part_;
}


// * * * * * * * * * * *  Protected Member Functions * * * * * * * * * * * * //

template<class ParcelType>
template<class TrackData>
void CML::KinematicParcel<ParcelType>::setCellValues
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    tetIndices tetIs = this->currentTetIndices();

    rhoc_ = td.rhoInterp().interpolate(this->position(), tetIs);

    if (rhoc_ < td.cloud().constProps().rhoMin())
    {
        if (debug)
        {
            WarningIn
            (
                "void CML::KinematicParcel<ParcelType>::setCellValues"
                "("
                    "TrackData&, "
                    "const scalar, "
                    "const label"
                ")"
            )   << "Limiting observed density in cell " << cellI << " to "
                << td.cloud().constProps().rhoMin() <<  nl << endl;
        }

        rhoc_ = td.cloud().constProps().rhoMin();
    }

    Uc_ = td.UInterp().interpolate(this->position(), tetIs);

    muc_ = td.muInterp().interpolate(this->position(), tetIs);

    // Apply dispersion components to carrier phase velocity
    Uc_ = td.cloud().dispersion().update
    (
        dt,
        cellI,
        U_,
        Uc_,
        UTurb_,
        tTurb_
    );
}


template<class ParcelType>
template<class TrackData>
void CML::KinematicParcel<ParcelType>::cellValueSourceCorrection
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    Uc_ += td.cloud().UTrans()[cellI]/massCell(cellI);
}


template<class ParcelType>
template<class TrackData>
void CML::KinematicParcel<ParcelType>::calc
(
    TrackData& td,
    const scalar dt,
    const label cellI
)
{
    // Define local properties at beginning of time step
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    const scalar np0 = nParticle_;
    const scalar mass0 = mass();

    // Reynolds number
    const scalar Re = this->Re(U_, d_, rhoc_, muc_);


    // Sources
    //~~~~~~~~

    // Explicit momentum source for particle
    vector Su = vector::zero;

    // Linearised momentum source coefficient
    scalar Spu = 0.0;

    // Momentum transfer from the particle to the carrier phase
    vector dUTrans = vector::zero;


    // Motion
    // ~~~~~~

    // Calculate new particle velocity
    this->U_ = calcVelocity(td, dt, cellI, Re, muc_, mass0, Su, dUTrans, Spu);


    // Accumulate carrier phase source terms
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (td.cloud().solution().coupled())
    {
        // Update momentum transfer
        td.cloud().UTrans()[cellI] += np0*dUTrans;

        // Update momentum transfer coefficient
        td.cloud().UCoeff()[cellI] += np0*Spu;
    }
}


template<class ParcelType>
template<class TrackData>
const CML::vector CML::KinematicParcel<ParcelType>::calcVelocity
(
    TrackData& td,
    const scalar dt,
    const label cellI,
    const scalar Re,
    const scalar mu,
    const scalar mass,
    const vector& Su,
    vector& dUTrans,
    scalar& Spu
) const
{
    typedef typename TrackData::cloudType cloudType;
    typedef typename cloudType::parcelType parcelType;
    typedef typename cloudType::forceType forceType;

    const forceType& forces = td.cloud().forces();

    // Momentum source due to particle forces
    const parcelType& p = static_cast<const parcelType&>(*this);
    const forceSuSp Fcp = forces.calcCoupled(p, dt, mass, Re, mu);
    const forceSuSp Fncp = forces.calcNonCoupled(p, dt, mass, Re, mu);
    const forceSuSp Feff = Fcp + Fncp;


    // New particle velocity
    //~~~~~~~~~~~~~~~~~~~~~~

    // Update velocity - treat as 3-D
    const vector abp = (Feff.Sp()*Uc_ + (Feff.Su() + Su))/mass;
    const scalar bp = Feff.Sp()/mass;

    Spu = dt*Feff.Sp();

    IntegrationScheme<vector>::integrationResult Ures =
        td.cloud().UIntegrator().integrate(U_, dt, abp, bp);

    vector Unew = Ures.value();

    // note: Feff.Sp() and Fc.Sp() must be the same
    dUTrans += dt*(Feff.Sp()*(Ures.average() - Uc_) - Fcp.Su());

    // Apply correction to velocity and dUTrans for reduced-D cases
    const polyMesh& mesh = td.cloud().pMesh();
    meshTools::constrainDirection(mesh, mesh.solutionD(), Unew);
    meshTools::constrainDirection(mesh, mesh.solutionD(), dUTrans);

    return Unew;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::KinematicParcel<ParcelType>::KinematicParcel
(
    const KinematicParcel<ParcelType>& p
)
:
    ParcelType(p),
    active_(p.active_),
    typeId_(p.typeId_),
    nParticle_(p.nParticle_),
    d_(p.d_),
    dTarget_(p.dTarget_),
    U_(p.U_),
    f_(p.f_),
    angularMomentum_(p.angularMomentum_),
    torque_(p.torque_),
    rho_(p.rho_),
    age_(p.age_),
    tTurb_(p.tTurb_),
    UTurb_(p.UTurb_),
    rhoc_(p.rhoc_),
    Uc_(p.Uc_),
    muc_(p.muc_)
{}


template<class ParcelType>
CML::KinematicParcel<ParcelType>::KinematicParcel
(
    const KinematicParcel<ParcelType>& p,
    const polyMesh& mesh
)
:
    ParcelType(p, mesh),
    active_(p.active_),
    typeId_(p.typeId_),
    nParticle_(p.nParticle_),
    d_(p.d_),
    dTarget_(p.dTarget_),
    U_(p.U_),
    f_(p.f_),
    angularMomentum_(p.angularMomentum_),
    torque_(p.torque_),
    rho_(p.rho_),
    age_(p.age_),
    tTurb_(p.tTurb_),
    UTurb_(p.UTurb_),
    rhoc_(p.rhoc_),
    Uc_(p.Uc_),
    muc_(p.muc_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ParcelType>
template<class TrackData>
bool CML::KinematicParcel<ParcelType>::move
(
    TrackData& td,
    const scalar trackTime
)
{
    typename TrackData::cloudType::parcelType& p =
        static_cast<typename TrackData::cloudType::parcelType&>(*this);

    td.switchProcessor = false;
    td.keepParticle = true;

    const polyMesh& mesh = td.cloud().pMesh();
    const polyBoundaryMesh& pbMesh = mesh.boundaryMesh();
    const scalarField& V = mesh.cellVolumes();
    const scalar maxCo = td.cloud().solution().maxCo();

    scalar tEnd = (1.0 - p.stepFraction())*trackTime;
    const scalar dtMax = tEnd;

    while (td.keepParticle && !td.switchProcessor && tEnd > ROOTVSMALL)
    {
        // Apply correction to position for reduced-D cases
        meshTools::constrainToMeshCentre(mesh, p.position());

        // Set the Lagrangian time-step
        scalar dt = min(dtMax, tEnd);

        // Remember which cell the parcel is in since this will change if
        // a face is hit
        const label cellI = p.cell();

        const scalar magU = mag(U_);
        if (p.active() && magU > ROOTVSMALL)
        {
            const scalar d = dt*magU;
            const scalar dCorr = min(d, maxCo*cbrt(V[cellI]));
            dt *=
                dCorr/d
               *p.trackToFace(p.position() + dCorr*U_/magU, td);
        }

        tEnd -= dt;
        p.stepFraction() = 1.0 - tEnd/trackTime;

        // Avoid problems with extremely small timesteps
        if (dt > ROOTVSMALL)
        {
            // Update cell based properties
            p.setCellValues(td, dt, cellI);

            if (td.cloud().solution().cellValueSourceCorrection())
            {
                p.cellValueSourceCorrection(td, dt, cellI);
            }

            p.calc(td, dt, cellI);
        }

        if (p.onBoundary() && td.keepParticle)
        {
            if (isA<processorPolyPatch>(pbMesh[p.patch(p.face())]))
            {
                td.switchProcessor = true;
            }
        }

        p.age() += dt;

        td.cloud().functions().postMove(p, cellI, dt);
    }

    return td.keepParticle;
}


template<class ParcelType>
template<class TrackData>
void CML::KinematicParcel<ParcelType>::hitFace(TrackData& td)
{
    typename TrackData::cloudType::parcelType& p =
        static_cast<typename TrackData::cloudType::parcelType&>(*this);

    td.cloud().functions().postFace(p, p.face());
}


template<class ParcelType>
void CML::KinematicParcel<ParcelType>::hitFace(int& td)
{}


template<class ParcelType>
template<class TrackData>
bool CML::KinematicParcel<ParcelType>::hitPatch
(
    const polyPatch& pp,
    TrackData& td,
    const label patchI,
    const scalar trackFraction,
    const tetIndices& tetIs
)
{
    typename TrackData::cloudType::parcelType& p =
        static_cast<typename TrackData::cloudType::parcelType&>(*this);

    // Invoke post-processing model
    td.cloud().functions().postPatch
    (
        p,
        pp,
        trackFraction,
        tetIs
    );

    // Invoke surface film model
    if (td.cloud().surfaceFilm().transferParcel(p, pp, td.keepParticle))
    {
        // All interactions done
        return true;
    }
    else
    {
        // Invoke patch interaction model
        return td.cloud().patchInteraction().correct
        (
            p,
            pp,
            td.keepParticle,
            trackFraction,
            tetIs
        );
    }
}


template<class ParcelType>
template<class TrackData>
void CML::KinematicParcel<ParcelType>::hitProcessorPatch
(
    const processorPolyPatch&,
    TrackData& td
)
{
    td.switchProcessor = true;
}


template<class ParcelType>
template<class TrackData>
void CML::KinematicParcel<ParcelType>::hitWallPatch
(
    const wallPolyPatch& wpp,
    TrackData& td,
    const tetIndices&
)
{
    // Wall interactions handled by generic hitPatch function
}


template<class ParcelType>
template<class TrackData>
void CML::KinematicParcel<ParcelType>::hitPatch
(
    const polyPatch&,
    TrackData& td
)
{
    td.keepParticle = false;
}


template<class ParcelType>
void CML::KinematicParcel<ParcelType>::transformProperties(const tensor& T)
{
    ParcelType::transformProperties(T);

    U_ = transform(T, U_);

    f_ = transform(T, f_);

    angularMomentum_ = transform(T, angularMomentum_);

    torque_ = transform(T, torque_);
}


template<class ParcelType>
void CML::KinematicParcel<ParcelType>::transformProperties
(
    const vector& separation
)
{
    ParcelType::transformProperties(separation);
}


template<class ParcelType>
CML::scalar CML::KinematicParcel<ParcelType>::wallImpactDistance
(
    const vector&
) const
{
    return 0.5*d_;
}


// * * * * * * * * * * * * * * IOStream operators  * * * * * * * * * * * * * //
// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class ParcelType>
CML::string CML::KinematicParcel<ParcelType>::propHeader =
    ParcelType::propHeader
  + " active"
  + " typeId"
  + " nParticle"
  + " d"
  + " dTarget "
  + " (Ux Uy Uz)"
  + " (fx fy fz)"
  + " (angularMomentumx angularMomentumy angularMomentumz)"
  + " (torquex torquey torquez)"
  + " rho"
  + " age"
  + " tTurb"
  + " (UTurbx UTurby UTurbz)";

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParcelType>
CML::KinematicParcel<ParcelType>::KinematicParcel
(
    const polyMesh& mesh,
    Istream& is,
    bool readFields
)
:
    ParcelType(mesh, is, readFields),
    active_(false),
    typeId_(0),
    nParticle_(0.0),
    d_(0.0),
    dTarget_(0.0),
    U_(vector::zero),
    f_(vector::zero),
    angularMomentum_(vector::zero),
    torque_(vector::zero),
    rho_(0.0),
    age_(0.0),
    tTurb_(0.0),
    UTurb_(vector::zero),
    rhoc_(0.0),
    Uc_(vector::zero),
    muc_(0.0)
{
    if (readFields)
    {
        if (is.format() == IOstream::ASCII)
        {
            active_ = readBool(is);
            typeId_ = readLabel(is);
            nParticle_ = readScalar(is);
            d_ = readScalar(is);
            dTarget_ = readScalar(is);
            is >> U_;
            is >> f_;
            is >> angularMomentum_;
            is >> torque_;
            rho_ = readScalar(is);
            age_ = readScalar(is);
            tTurb_ = readScalar(is);
            is >> UTurb_;
        }
        else
        {
            is.read
            (
                reinterpret_cast<char*>(&active_),
                sizeof(active_)
              + sizeof(typeId_)
              + sizeof(nParticle_)
              + sizeof(d_)
              + sizeof(dTarget_)
              + sizeof(U_)
              + sizeof(f_)
              + sizeof(angularMomentum_)
              + sizeof(torque_)
              + sizeof(rho_)
              + sizeof(age_)
              + sizeof(tTurb_)
              + sizeof(UTurb_)
            );
        }
    }

    // Check state of Istream
    is.check
    (
        "KinematicParcel<ParcelType>::KinematicParcel"
        "(const polyMesh&, Istream&, bool)"
    );
}


template<class ParcelType>
template<class CloudType>
void CML::KinematicParcel<ParcelType>::readFields(CloudType& c)
{
    if (!c.size())
    {
        return;
    }

    ParcelType::readFields(c);

    IOField<label> active(c.fieldIOobject("active", IOobject::MUST_READ));
    c.checkFieldIOobject(c, active);

    IOField<label> typeId(c.fieldIOobject("typeId", IOobject::MUST_READ));
    c.checkFieldIOobject(c, typeId);

    IOField<scalar>
        nParticle(c.fieldIOobject("nParticle", IOobject::MUST_READ));
    c.checkFieldIOobject(c, nParticle);

    IOField<scalar> d(c.fieldIOobject("d", IOobject::MUST_READ));
    c.checkFieldIOobject(c, d);

    IOField<scalar> dTarget(c.fieldIOobject("dTarget", IOobject::MUST_READ));
    c.checkFieldIOobject(c, dTarget);

    IOField<vector> U(c.fieldIOobject("U", IOobject::MUST_READ));
    c.checkFieldIOobject(c, U);

    IOField<vector> f(c.fieldIOobject("f", IOobject::MUST_READ));
    c.checkFieldIOobject(c, f);

    IOField<vector> angularMomentum
    (
        c.fieldIOobject("angularMomentum", IOobject::MUST_READ)
    );
    c.checkFieldIOobject(c, angularMomentum);

    IOField<vector> torque(c.fieldIOobject("torque", IOobject::MUST_READ));
    c.checkFieldIOobject(c, torque);

    IOField<scalar> rho(c.fieldIOobject("rho", IOobject::MUST_READ));
    c.checkFieldIOobject(c, rho);

    IOField<scalar> age(c.fieldIOobject("age", IOobject::MUST_READ));
    c.checkFieldIOobject(c, age);

    IOField<scalar> tTurb(c.fieldIOobject("tTurb", IOobject::MUST_READ));
    c.checkFieldIOobject(c, tTurb);

    IOField<vector> UTurb(c.fieldIOobject("UTurb", IOobject::MUST_READ));
    c.checkFieldIOobject(c, UTurb);

    label i = 0;

    forAllIter(typename CloudType, c, iter)
    {
        KinematicParcel<ParcelType>& p = iter();

        p.active_ = active[i];
        p.typeId_ = typeId[i];
        p.nParticle_ = nParticle[i];
        p.d_ = d[i];
        p.dTarget_ = dTarget[i];
        p.U_ = U[i];
        p.f_ = f[i];
        p.angularMomentum_ = angularMomentum[i];
        p.rho_ = rho[i];
        p.age_ = age[i];
        p.tTurb_ = tTurb[i];
        p.UTurb_ = UTurb[i];

        i++;
    }
}


template<class ParcelType>
template<class CloudType>
void CML::KinematicParcel<ParcelType>::writeFields(const CloudType& c)
{
    ParcelType::writeFields(c);

    label np =  c.size();

    IOField<label> active(c.fieldIOobject("active", IOobject::NO_READ), np);
    IOField<label> typeId(c.fieldIOobject("typeId", IOobject::NO_READ), np);
    IOField<scalar> nParticle
    (
        c.fieldIOobject("nParticle", IOobject::NO_READ),
        np
    );
    IOField<scalar> d(c.fieldIOobject("d", IOobject::NO_READ), np);
    IOField<scalar> dTarget(c.fieldIOobject("dTarget", IOobject::NO_READ), np);
    IOField<vector> U(c.fieldIOobject("U", IOobject::NO_READ), np);
    IOField<vector> f(c.fieldIOobject("f", IOobject::NO_READ), np);
    IOField<vector> angularMomentum
    (
        c.fieldIOobject("angularMomentum", IOobject::NO_READ),
        np
    );
    IOField<vector> torque(c.fieldIOobject("torque", IOobject::NO_READ), np);
    IOField<scalar> rho(c.fieldIOobject("rho", IOobject::NO_READ), np);
    IOField<scalar> age(c.fieldIOobject("age", IOobject::NO_READ), np);
    IOField<scalar> tTurb(c.fieldIOobject("tTurb", IOobject::NO_READ), np);
    IOField<vector> UTurb(c.fieldIOobject("UTurb", IOobject::NO_READ), np);

    label i = 0;

    forAllConstIter(typename CloudType, c, iter)
    {
        const KinematicParcel<ParcelType>& p = iter();

        active[i] = p.active();
        typeId[i] = p.typeId();
        nParticle[i] = p.nParticle();
        d[i] = p.d();
        dTarget[i] = p.dTarget();
        U[i] = p.U();
        f[i] = p.f();
        angularMomentum[i] = p.angularMomentum();
        torque[i] = p.torque();
        rho[i] = p.rho();
        age[i] = p.age();
        tTurb[i] = p.tTurb();
        UTurb[i] = p.UTurb();

        i++;
    }

    active.write();
    typeId.write();
    nParticle.write();
    d.write();
    dTarget.write();
    U.write();
    f.write();
    angularMomentum.write();
    torque.write();
    rho.write();
    age.write();
    tTurb.write();
    UTurb.write();
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class ParcelType>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const KinematicParcel<ParcelType>& p
)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << static_cast<const ParcelType&>(p)
            << token::SPACE << p.active()
            << token::SPACE << p.typeId()
            << token::SPACE << p.nParticle()
            << token::SPACE << p.d()
            << token::SPACE << p.dTarget()
            << token::SPACE << p.U()
            << token::SPACE << p.f()
            << token::SPACE << p.angularMomentum()
            << token::SPACE << p.torque()
            << token::SPACE << p.rho()
            << token::SPACE << p.age()
            << token::SPACE << p.tTurb()
            << token::SPACE << p.UTurb();
    }
    else
    {
        os  << static_cast<const ParcelType&>(p);
        os.write
        (
            reinterpret_cast<const char*>(&p.active_),
            sizeof(p.active())
          + sizeof(p.typeId())
          + sizeof(p.nParticle())
          + sizeof(p.d())
          + sizeof(p.dTarget())
          + sizeof(p.U())
          + sizeof(p.f())
          + sizeof(p.angularMomentum())
          + sizeof(p.torque())
          + sizeof(p.rho())
          + sizeof(p.age())
          + sizeof(p.tTurb())
          + sizeof(p.UTurb())
        );
    }

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const KinematicParcel<ParcelType>&)"
    );

    return os;
}


#endif

// ************************************************************************* //
