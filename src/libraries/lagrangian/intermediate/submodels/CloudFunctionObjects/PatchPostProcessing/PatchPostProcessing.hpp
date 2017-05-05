/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    CML::PatchPostProcessing

Description
    Standard post-processing

SourceFiles
    PatchPostProcessing.cpp

\*---------------------------------------------------------------------------*/

#ifndef PatchPostProcessing_H
#define PatchPostProcessing_H

#include "CloudFunctionObject.hpp"
#include "Pstream.hpp"
#include "stringListOps.hpp"
#include "ListOps.hpp"
#include "ListListOps.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class PatchPostProcessing Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class PatchPostProcessing
:
    public CloudFunctionObject<CloudType>
{
    // Private data

        typedef typename CloudType::particleType parcelType;

        //- Maximum number of parcels to store - set as a scalar for I/O
        scalar maxStoredParcels_;

        //- List of patch indices to post-process
        labelList patchIDs_;

        //- List of time for each data record
        List<DynamicList<scalar> > times_;

        //- List of output data per patch
        List<DynamicList<string> > patchData_;


    // Private Member Functions

        //- Returns local patchI if patch is in patchIds_ list
        label applyToPatch(const label globalPatchI) const;


protected:

    // Protected Member Functions

        //- Write post-processing info
        void write();


public:

    //- Runtime type information
    TypeName("patchPostProcessing");


    // Constructors

        //- Construct from dictionary
        PatchPostProcessing
        (
            const dictionary& dict,
            CloudType& owner,
            const word& modelName
        );

        //- Construct copy
        PatchPostProcessing(const PatchPostProcessing<CloudType>& ppm);

        //- Construct and return a clone
        virtual autoPtr<CloudFunctionObject<CloudType> > clone() const
        {
            return autoPtr<CloudFunctionObject<CloudType> >
            (
                new PatchPostProcessing<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~PatchPostProcessing();


    // Member Functions

        // Access

            //- Return maximum number of parcels to store per patch
            inline label maxStoredParcels() const;

            //- Return const mapping from local to global patch ids
            inline const labelList& patchIDs() const;


        // Evaluation

            //- Pre-evolve hook
            virtual void preEvolve();

            //- Post-evolve hook
            virtual void postEvolve();

            //- Post-move hook
            virtual void postMove
            (
                typename CloudType::parcelType& p,
                const label cellI,
                const scalar dt,
                const point& position0,
                bool& keepParticle
            );

            //- Post-patch hook
            virtual void postPatch
            (
                const typename CloudType::parcelType& p,
                const polyPatch& pp,
                const scalar trackFraction,
                const tetIndices& tetIs,
                bool& keepParticle
            );

            //- Post-face hook
            virtual void postFace
            (
                const typename CloudType::parcelType& p,
                const label faceI,
                bool& keepParticle
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class CloudType>
CML::label CML::PatchPostProcessing<CloudType>::maxStoredParcels() const
{
    return maxStoredParcels_;
}


template<class CloudType>
const CML::labelList& CML::PatchPostProcessing<CloudType>::patchIDs() const
{
    return patchIDs_;
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class CloudType>
CML::label CML::PatchPostProcessing<CloudType>::applyToPatch
(
    const label globalPatchI
) const
{
    forAll(patchIDs_, i)
    {
        if (patchIDs_[i] == globalPatchI)
        {
            return i;
        }
    }

    return -1;
}


// * * * * * * * * * * * * * protected Member Functions  * * * * * * * * * * //

template<class CloudType>
void CML::PatchPostProcessing<CloudType>::write()
{
    forAll(patchData_, i)
    {
        List<List<scalar> > procTimes(Pstream::nProcs());
        procTimes[Pstream::myProcNo()] = times_[i];
        Pstream::gatherList(procTimes);

        List<List<string> > procData(Pstream::nProcs());
        procData[Pstream::myProcNo()] = patchData_[i];
        Pstream::gatherList(procData);

        if (Pstream::master())
        {
            const fvMesh& mesh = this->owner().mesh();

            // Create directory if it doesn't exist
            mkDir(this->outputTimeDir());

            const word& patchName = mesh.boundaryMesh()[patchIDs_[i]].name();

            OFstream patchOutFile
            (
                this->outputTimeDir()/patchName + ".post",
                IOstream::ASCII,
                IOstream::currentVersion,
                mesh.time().writeCompression()
            );

            List<string> globalData;
            globalData = ListListOps::combine<List<string> >
            (
                procData,
                accessOp<List<string> >()
            );

            List<scalar> globalTimes;
            globalTimes = ListListOps::combine<List<scalar> >
            (
                procTimes,
                accessOp<List<scalar> >()
            );

            labelList indices;
            sortedOrder(globalTimes, indices);

            string header("# Time currentProc " + parcelType::propertyList_);
            patchOutFile<< header.c_str() << nl;

            forAll(globalTimes, i)
            {
                label dataI = indices[i];

                patchOutFile
                    << globalTimes[dataI] << ' '
                    << globalData[dataI].c_str()
                    << nl;
            }
        }

        patchData_[i].clearStorage();
        times_[i].clearStorage();
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CloudType>
CML::PatchPostProcessing<CloudType>::PatchPostProcessing
(
    const dictionary& dict,
    CloudType& owner,
    const word& modelName
)
:
    CloudFunctionObject<CloudType>(dict, owner, modelName, typeName),
    maxStoredParcels_(readScalar(this->coeffDict().lookup("maxStoredParcels"))),
    patchIDs_(),
    times_(),
    patchData_()
{
    const wordList allPatchNames = owner.mesh().boundaryMesh().names();
    wordList patchName(this->coeffDict().lookup("patches"));

    labelHashSet uniquePatchIDs;
    forAllReverse(patchName, i)
    {
        labelList patchIDs = findStrings(patchName[i], allPatchNames);

        if (patchIDs.empty())
        {
            WarningIn
            (
                "CML::PatchPostProcessing<CloudType>::PatchPostProcessing"
                "("
                    "const dictionary&, "
                    "CloudType& "
                ")"
            )   << "Cannot find any patch names matching " << patchName[i]
                << endl;
        }

        uniquePatchIDs.insert(patchIDs);
    }

    patchIDs_ = uniquePatchIDs.toc();

    if (debug)
    {
        forAll(patchIDs_, i)
        {
            const label patchI = patchIDs_[i];
            const word& patchName = owner.mesh().boundaryMesh()[patchI].name();
            Info<< "Post-process patch " << patchName << endl;
        }
    }

    patchData_.setSize(patchIDs_.size());
    times_.setSize(patchIDs_.size());
}


template<class CloudType>
CML::PatchPostProcessing<CloudType>::PatchPostProcessing
(
    const PatchPostProcessing<CloudType>& ppm
)
:
    CloudFunctionObject<CloudType>(ppm),
    maxStoredParcels_(ppm.maxStoredParcels_),
    patchIDs_(ppm.patchIDs_),
    times_(ppm.times_),
    patchData_(ppm.patchData_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
CML::PatchPostProcessing<CloudType>::~PatchPostProcessing()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CloudType>
void CML::PatchPostProcessing<CloudType>::preEvolve()
{
    // Do nothing
}


template<class CloudType>
void CML::PatchPostProcessing<CloudType>::postEvolve()
{
    // Do nothing
}


template<class CloudType>
void CML::PatchPostProcessing<CloudType>::postMove
(
    typename CloudType::parcelType& p,
    const label cellI,
    const scalar dt,
    const point& position0,
    bool& keepParticle
)
{
    // Do nothing
}


template<class CloudType>
void CML::PatchPostProcessing<CloudType>::postPatch
(
    const typename CloudType::parcelType& p,
    const polyPatch& pp,
    const scalar,
    const tetIndices& tetIs,
    bool&
)
{
    const label patchI = pp.index();
    const label localPatchI = applyToPatch(patchI);

    if (localPatchI != -1 && patchData_[localPatchI].size() < maxStoredParcels_)
    {
        times_[localPatchI].append(this->owner().time().value());

        OStringStream data;
        data<< Pstream::myProcNo() << ' ' << p;

        patchData_[localPatchI].append(data.str());
    }
}


template<class CloudType>
void CML::PatchPostProcessing<CloudType>::postFace
(
    const typename CloudType::parcelType& p,
    const label faceI,
    bool& keepParticle
)
{
    // Do nothing
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
