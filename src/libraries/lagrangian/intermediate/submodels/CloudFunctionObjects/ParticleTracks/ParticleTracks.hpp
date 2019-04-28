/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2017 OpenFOAM Foundation
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
    CML::ParticleTracks

Description
    Records particle state (all variables) on each call to postFace

SourceFiles
    ParticleTracks.cpp

\*---------------------------------------------------------------------------*/

#ifndef ParticleTracks_H
#define ParticleTracks_H

#include "CloudFunctionObject.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class ParticleTracks Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class ParticleTracks
:
    public CloudFunctionObject<CloudType>
{
    // Private data

        // Typedefs

            //- Convenience typedef for parcel type
            typedef typename CloudType::parcelType parcelType;

            //- Convenience typedef for hash hit-table
            typedef HashTable<label, labelPair, typename labelPair::Hash<> >
                hitTableType;

        //- Number of face-hit intervals between storing parcel data
        label trackInterval_;

        //- Maximum number of particles to store per track
        label maxSamples_;

        //- Flag to indicate whether data should be reset/cleared on writing
        Switch resetOnWrite_;

        //- Table of number of times a particle has hit a face
        hitTableType faceHitCounter_;

        //- Pointer to the cloud storage
        autoPtr<Cloud<parcelType> > cloudPtr_;


protected:

    // Protected member functions

        //- Write post-processing info
        void write();


public:

    //- Runtime type information
    TypeName("particleTracks");


    // Constructors

        //- Construct from dictionary
        ParticleTracks
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        ParticleTracks(const ParticleTracks<CloudType>& ppm);

        //- Construct and return a clone
        virtual autoPtr<CloudFunctionObject<CloudType> > clone() const
        {
            return autoPtr<CloudFunctionObject<CloudType> >
            (
                new ParticleTracks<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~ParticleTracks();


    // Member Functions

        // Access

            //- Return const access to the track interval
            inline label trackInterval() const;

            //- Return const access to the max samples
            inline label maxSamples() const;

            //- Return const access to the reset on write flag
            inline const Switch& resetOnWrite() const;

            //- Return the table of number of times a particle has hit a face
            inline const hitTableType& faceHitCounter() const;

            //- Return const access to the cloud
            inline const Cloud<parcelType>& cloud() const;


        // Evaluation

            //- Pre-evolve hook
            virtual void preEvolve();

            //- Post-face hook
            virtual void postFace(const parcelType& p, bool& keepParticle);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


template<class CloudType>
inline CML::label CML::ParticleTracks<CloudType>::trackInterval() const
{
    return trackInterval_;
}


template<class CloudType>
inline CML::label CML::ParticleTracks<CloudType>::maxSamples() const
{
    return maxSamples_;
}


template<class CloudType>
inline const CML::Switch& CML::ParticleTracks<CloudType>::resetOnWrite() const
{
    return resetOnWrite_;
}


template<class CloudType>
inline const typename CML::ParticleTracks<CloudType>::hitTableType&
CML::ParticleTracks<CloudType>::faceHitCounter() const
{
    return faceHitCounter_;
}


template<class CloudType>
inline const CML::Cloud<typename CloudType::parcelType>&
CML::ParticleTracks<CloudType>::cloud() const
{
    return cloudPtr_();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Pstream.hpp"
#include "ListListOps.hpp"
#include "IOPtrList.hpp"

// * * * * * * * * * * * * * protected Member Functions  * * * * * * * * * * //

template<class CloudType>
void CML::ParticleTracks<CloudType>::write()
{
    if (cloudPtr_.valid())
    {
        cloudPtr_->write();

        if (resetOnWrite_)
        {
            cloudPtr_->clear();
        }
    }
    else
    {
        if (debug)
        {
            InfoInFunction << "cloupPtr invalid" << endl;
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParticleTracks<CloudType>::ParticleTracks
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    CloudFunctionObject<CloudType>(dict, owner, modelName, typeName),
    trackInterval_(readLabel(this->coeffDict().lookup("trackInterval"))),
    maxSamples_(readLabel(this->coeffDict().lookup("maxSamples"))),
    resetOnWrite_(this->coeffDict().lookup("resetOnWrite")),
    faceHitCounter_(),
    cloudPtr_(nullptr)
{}


template<class CloudType>
CML::ParticleTracks<CloudType>::ParticleTracks
(
    const ParticleTracks<CloudType>& ppm
)
:
    CloudFunctionObject<CloudType>(ppm),
    trackInterval_(ppm.trackInterval_),
    maxSamples_(ppm.maxSamples_),
    resetOnWrite_(ppm.resetOnWrite_),
    faceHitCounter_(ppm.faceHitCounter_),
    cloudPtr_(ppm.cloudPtr_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::ParticleTracks<CloudType>::~ParticleTracks()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::ParticleTracks<CloudType>::preEvolve()
{
    if (!cloudPtr_.valid())
    {
        cloudPtr_.reset
        (
            this->owner().cloneBare(this->owner().name() + "Tracks").ptr()
        );
    }
}


template<class CloudType>
void CML::ParticleTracks<CloudType>::postFace(const parcelType& p, bool&)
{
    if
    (
        this->owner().solution().output()
     || this->owner().solution().transient()
    )
    {
        if (!cloudPtr_.valid())
        {
            FatalErrorInFunction
             << "Cloud storage not allocated" << abort(FatalError);
        }

        hitTableType::iterator iter =
            faceHitCounter_.find(labelPair(p.origProc(), p.origId()));

        label localI = -1;
        if (iter != faceHitCounter_.end())
        {
            iter()++;
            localI = iter();
        }
        else
        {
            localI = 1;
            faceHitCounter_.insert(labelPair(p.origProc(), p.origId()), localI);
        }

        label nSamples = floor(static_cast<scalar>(localI/trackInterval_));
        if ((localI % trackInterval_ == 0) && (nSamples < maxSamples_))
        {
            cloudPtr_->append
            (
                static_cast<parcelType*>(p.clone(this->owner().mesh()).ptr())
            );
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
