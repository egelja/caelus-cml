/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
Copyright (C) 2017 OpenCFD Ltd.
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
    CML::Cloud

Description
    Base cloud calls templated on particle type

SourceFiles
    Cloud.cpp
    CloudIO.cpp

\*---------------------------------------------------------------------------*/

#ifndef Cloud_H
#define Cloud_H

#include "cloud_.hpp"
#include "IDLList.hpp"
#include "IOField.hpp"
#include "CompactIOField.hpp"
#include "polyMesh.hpp"
#include "indexedOctree.hpp"
#include "treeDataCell.hpp"
#include "tetPointRef.hpp"
#include "PackedBoolList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of functions
template<class ParticleType>
class Cloud;

template<class ParticleType>
class IOPosition;

template<class ParticleType>
Ostream& operator<<
(
    Ostream&,
    const Cloud<ParticleType>&
);


/*---------------------------------------------------------------------------*\
                           Class Cloud Declaration
\*---------------------------------------------------------------------------*/

template<class ParticleType>
class Cloud
:
    public cloud,
    public IDLList<ParticleType>
{
    // Private data

        //- Reference to the mesh
        const polyMesh& polyMesh_;

        //- Temporary storage for the global particle positions
        mutable autoPtr<vectorField> globalPositionsPtr_;


    // Private Member Functions

        //- Check patches
        void checkPatches() const;

        //- Initialise cloud on IO constructor
        void initCloud(const bool checkClass);

        //- Read cloud properties dictionary
        void readCloudUniformProperties();

        //- Write cloud properties dictionary
        void writeCloudUniformProperties() const;


public:

    friend class particle;
    template<class ParticleT>
    friend class IOPosition;

    typedef ParticleType particleType;

    typedef typename IDLList<ParticleType>::iterator iterator;
    typedef typename IDLList<ParticleType>::const_iterator const_iterator;

    //-Runtime type information
    TypeName("Cloud");


    // Static data

        //- Name of cloud properties dictionary
        static word cloudPropertiesName;


    // Constructors

        //- Construct from mesh and a list of particles
        Cloud
        (
            const polyMesh& mesh,
            const word& cloudName,
            const IDLList<ParticleType>& particles
        );

        //- Construct from mesh by reading from file with given cloud instance
        //  Optionally disable checking of class name for post-processing
        Cloud
        (
            const polyMesh& pMesh,
            const word& cloudName,
            const bool checkClass = true
        );


    // Member Functions

        // Access

            //- Return the polyMesh reference
            const polyMesh& pMesh() const
            {
                return polyMesh_;
            }

            //- Return the number of particles in the cloud
            label size() const
            {
                return IDLList<ParticleType>::size();
            };


            // Iterators

                const const_iterator begin() const
                {
                    return IDLList<ParticleType>::begin();
                };

                const const_iterator cbegin() const
                {
                    return IDLList<ParticleType>::cbegin();
                };

                const const_iterator end() const
                {
                    return IDLList<ParticleType>::end();
                };

                const const_iterator cend() const
                {
                    return IDLList<ParticleType>::cend();
                };

                iterator begin()
                {
                    return IDLList<ParticleType>::begin();
                };

                iterator end()
                {
                    return IDLList<ParticleType>::end();
                };


        // Edit

            void clear()
            {
                IDLList<ParticleType>::clear();
            };

            //- Transfer particle to cloud
            void addParticle(ParticleType* pPtr);

            //- Remove particle from cloud and delete
            void deleteParticle(ParticleType&);

            //- Remove lost particles from cloud and delete
            void deleteLostParticles();

            //- Reset the particles
            void cloudReset(const Cloud<ParticleType>& c);

            //- Move the particles
            template<class TrackCloudType>
            void move
            (
                TrackCloudType& cloud,
                typename ParticleType::trackingData& td,
                const scalar trackTime
            );

            //- Remap the cells of particles corresponding to the
            //  mesh topology change
            void autoMap(const mapPolyMesh&);


        // Read

            //- Helper to construct IOobject for field and current time.
            IOobject fieldIOobject
            (
                const word& fieldName,
                const IOobject::readOption r
            ) const;

            //- Check lagrangian data field
            template<class DataType>
            void checkFieldIOobject
            (
                const Cloud<ParticleType>& c,
                const IOField<DataType>& data
            ) const;

            //- Check lagrangian data fieldfield
            template<class DataType>
            void checkFieldFieldIOobject
            (
                const Cloud<ParticleType>& c,
                const CompactIOField<Field<DataType>, DataType>& data
            ) const;


        // Write

            //- Write the field data for the cloud of particles Dummy at
            //  this level.
            virtual void writeFields() const;

            //- Write using given format, version and compression.
            //  Only writes the cloud file if the Cloud isn't empty
            virtual bool writeObject
            (
                IOstream::streamFormat fmt,
                IOstream::versionNumber ver,
                IOstream::compressionType cmp
            ) const;

            //- Write positions to \<cloudName\>_positions.obj file
            void writePositions() const;

            //- Call this before a topology change. Stores the particles global
            //  positions in the database for use during mapping.
            void storeGlobalPositions() const;


    // Ostream Operator

        friend Ostream& operator<< <ParticleType>
        (
            Ostream&,
            const Cloud<ParticleType>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "processorPolyPatch.hpp"
#include "globalMeshData.hpp"
#include "PstreamCombineReduceOps.hpp"
#include "mapPolyMesh.hpp"
#include "Time.hpp"
#include "OFstream.hpp"
#include "wallPolyPatch.hpp"
#include "cyclicAMIPolyPatch.hpp"

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class ParticleType>
void CML::Cloud<ParticleType>::checkPatches() const
{
    const polyBoundaryMesh& pbm = polyMesh_.boundaryMesh();
    bool ok = true;
    forAll(pbm, patchi)
    {
        if (isA<cyclicAMIPolyPatch>(pbm[patchi]))
        {
            const cyclicAMIPolyPatch& cami =
                refCast<const cyclicAMIPolyPatch>(pbm[patchi]);

            if (cami.owner())
            {
                ok = ok && (cami.AMI().singlePatchProc() != -1);
            }
        }
    }

    if (!ok)
    {
        FatalErrorInFunction
            << "Particle tracking across AMI patches is only currently "
            << "supported for cases where the AMI patches reside on a "
            << "single processor" << abort(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParticleType>
CML::Cloud<ParticleType>::Cloud
(
    const polyMesh& pMesh,
    const word& cloudName,
    const IDLList<ParticleType>& particles
)
:
    cloud(pMesh, cloudName),
    IDLList<ParticleType>(),
    polyMesh_(pMesh),
    globalPositionsPtr_()
{
    checkPatches();

    // Ask for the tetBasePtIs and oldCellCentres to trigger all processors to
    // build them, otherwise, if some processors have no particles then there
    // is a comms mismatch.
    polyMesh_.tetBasePtIs();
    polyMesh_.oldCellCentres();

    if (particles.size())
    {
        IDLList<ParticleType>::operator=(particles);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ParticleType>
void CML::Cloud<ParticleType>::addParticle(ParticleType* pPtr)
{
    this->append(pPtr);
}


template<class ParticleType>
void CML::Cloud<ParticleType>::deleteParticle(ParticleType& p)
{
    delete(this->remove(&p));
}


template<class ParticleType>
void CML::Cloud<ParticleType>::deleteLostParticles()
{
    forAllIter(typename Cloud<ParticleType>, *this, pIter)
    {
        ParticleType& p = pIter();

        if (p.cell() == -1)
        {
            WarningInFunction
                << "deleting lost particle at position " << p.position()
                << endl;

            deleteParticle(p);
        }
    }
}


template<class ParticleType>
void CML::Cloud<ParticleType>::cloudReset(const Cloud<ParticleType>& c)
{
    // Reset particle count and particles only
    // - not changing the cloud object registry or reference to the polyMesh
    ParticleType::particleCount_ = 0;
    IDLList<ParticleType>::operator=(c);
}


template<class ParticleType>
template<class TrackCloudType>
void CML::Cloud<ParticleType>::move
(
    TrackCloudType& cloud,
    typename ParticleType::trackingData& td,
    const scalar trackTime
)
{
    const polyBoundaryMesh& pbm = pMesh().boundaryMesh();
    const globalMeshData& pData = polyMesh_.globalData();

    // Which patches are processor patches
    const labelList& procPatches = pData.processorPatches();

    // Indexing of equivalent patch on neighbour processor into the
    // procPatches list on the neighbour
    const labelList& procPatchNeighbours = pData.processorPatchNeighbours();

    // Which processors this processor is connected to
    const labelList& neighbourProcs = pData[Pstream::myProcNo()];

    // Indexing from the processor number into the neighbourProcs list
    labelList neighbourProcIndices(Pstream::nProcs(), -1);

    forAll(neighbourProcs, i)
    {
        neighbourProcIndices[neighbourProcs[i]] = i;
    }

    // Initialise the stepFraction moved for the particles
    forAllIter(typename Cloud<ParticleType>, *this, pIter)
    {
        pIter().reset();
    }

    // List of lists of particles to be transfered for all of the
    // neighbour processors
    List<IDLList<ParticleType> > particleTransferLists
    (
        neighbourProcs.size()
    );

    // List of destination processorPatches indices for all of the
    // neighbour processors
    List<DynamicList<label> > patchIndexTransferLists
    (
        neighbourProcs.size()
    );

    // Allocate transfer buffers
    PstreamBuffers pBufs(Pstream::nonBlocking);

    // Clear the global positions as there are about to change
    globalPositionsPtr_.clear();

    // While there are particles to transfer
    while (true)
    {
        particleTransferLists = IDLList<ParticleType>();
        forAll(patchIndexTransferLists, i)
        {
            patchIndexTransferLists[i].clear();
        }

        // Loop over all particles
        forAllIter(typename Cloud<ParticleType>, *this, pIter)
        {
            ParticleType& p = pIter();

            // Move the particle
            bool keepParticle = p.move(cloud, td, trackTime);

            // If the particle is to be kept
            // (i.e. it hasn't passed through an inlet or outlet)
            if (keepParticle)
            {
                if (td.switchProcessor)
                {
                    #ifdef FULLDEBUG
                    if
                    (
                        !Pstream::parRun()
                     || !p.onBoundaryFace()
                     || procPatchNeighbours[p.patch()] < 0
                    )
                    {
                        FatalErrorInFunction
                            << "Switch processor flag is true when no parallel "
                            << "transfer is possible. This is a bug."
                            << exit(FatalError);
                    }
                    #endif

                    const label patchi = p.patch();

                    const label n = neighbourProcIndices
                    [
                        refCast<const processorPolyPatch>
                        (
                            pbm[patchi]
                        ).neighbProcNo()
                    ];

                    p.prepareForParallelTransfer();

                    particleTransferLists[n].append(this->remove(&p));

                    patchIndexTransferLists[n].append
                    (
                        procPatchNeighbours[patchi]
                    );
                }
            }
            else
            {
                deleteParticle(p);
            }
        }

        if (!Pstream::parRun())
        {
            break;
        }


        // Clear transfer buffers
        pBufs.clear();

        // Stream into send buffers
        forAll(particleTransferLists, i)
        {
            if (particleTransferLists[i].size())
            {
                UOPstream particleStream
                (
                    neighbourProcs[i],
                    pBufs
                );

                particleStream
                    << patchIndexTransferLists[i]
                    << particleTransferLists[i];
            }
        }


        // Start sending. Sets number of bytes transferred
        labelList allNTrans(Pstream::nProcs());
        pBufs.finishedSends(allNTrans);


        bool transferred = false;

        forAll(allNTrans, i)
        {
            if (allNTrans[i])
            {
                transferred = true;
                break;
            }
        }
        reduce(transferred, orOp<bool>());

        if (!transferred)
        {
            break;
        }

        // Retrieve from receive buffers
        forAll(neighbourProcs, i)
        {
            label neighbProci = neighbourProcs[i];

            label nRec = allNTrans[neighbProci];

            if (nRec)
            {
                UIPstream particleStream(neighbProci, pBufs);

                labelList receivePatchIndex(particleStream);

                IDLList<ParticleType> newParticles
                (
                    particleStream,
                    typename ParticleType::iNew(polyMesh_)
                );

                label pI = 0;

                forAllIter(typename Cloud<ParticleType>, newParticles, newpIter)
                {
                    ParticleType& newp = newpIter();

                    label patchi = procPatches[receivePatchIndex[pI++]];

                    newp.correctAfterParallelTransfer(patchi, td);

                    addParticle(newParticles.remove(&newp));
                }
            }
        }
    }
}


template<class ParticleType>
void CML::Cloud<ParticleType>::autoMap(const mapPolyMesh& mapper)
{
    if (!globalPositionsPtr_.valid())
    {
        FatalErrorInFunction
            << "Global positions are not available. "
            << "Cloud::storeGlobalPositions has not been called."
            << exit(FatalError);
    }

    // Ask for the tetBasePtIs to trigger all processors to build
    // them, otherwise, if some processors have no particles then
    // there is a comms mismatch.
    polyMesh_.tetBasePtIs();

    const vectorField& positions = globalPositionsPtr_();

    label i = 0;
    forAllIter(typename Cloud<ParticleType>, *this, iter)
    {
        iter().autoMap(positions[i], mapper);
        ++ i;
    }
}


template<class ParticleType>
void CML::Cloud<ParticleType>::writePositions() const
{
    OFstream pObj
    (
        this->db().time().path()/this->name() + "_positions.obj"
    );

    forAllConstIter(typename Cloud<ParticleType>, *this, pIter)
    {
        const ParticleType& p = pIter();
        pObj<< "v " << p.position().x() << " " << p.position().y() << " "
            << p.position().z() << nl;
    }

    pObj.flush();
}


template<class ParticleType>
void CML::Cloud<ParticleType>::storeGlobalPositions() const
{
    // Store the global positions for later use by autoMap. It would be
    // preferable not to need this. If the mapPolyMesh object passed to autoMap
    // had a copy of the old mesh then the global positions could be recovered
    // within autoMap, and this pre-processing would not be necessary.

    globalPositionsPtr_.reset(new vectorField(this->size()));

    vectorField& positions = globalPositionsPtr_();

    label i = 0;
    forAllConstIter(typename Cloud<ParticleType>, *this, iter)
    {
        positions[i] = iter().position();
        ++ i;
    }
}


// * * * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * //

#include "Time.hpp"
#include "IOPosition.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class ParticleType>
CML::word CML::Cloud<ParticleType>::cloudPropertiesName("cloudProperties");


// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class ParticleType>
void CML::Cloud<ParticleType>::readCloudUniformProperties()
{
    IOobject dictObj
    (
        cloudPropertiesName,
        time().timeName(),
        "uniform"/cloud::prefix/name(),
        db(),
        IOobject::MUST_READ_IF_MODIFIED,
        IOobject::NO_WRITE,
        false
    );

    if (dictObj.headerOk())
    {
        const IOdictionary uniformPropsDict(dictObj);

        const word procName("processor" + CML::name(Pstream::myProcNo()));
        if (uniformPropsDict.found(procName))
        {
            uniformPropsDict.subDict(procName).lookup("particleCount")
                >> ParticleType::particleCount_;
        }
    }
    else
    {
        ParticleType::particleCount_ = 0;
    }
}


template<class ParticleType>
void CML::Cloud<ParticleType>::writeCloudUniformProperties() const
{
    IOdictionary uniformPropsDict
    (
        IOobject
        (
            cloudPropertiesName,
            time().timeName(),
            "uniform"/cloud::prefix/name(),
            db(),
            IOobject::NO_READ,
            IOobject::NO_WRITE,
            false
        )
    );

    labelList np(Pstream::nProcs(), 0);
    np[Pstream::myProcNo()] = ParticleType::particleCount_;

    Pstream::listCombineGather(np, maxEqOp<label>());
    Pstream::listCombineScatter(np);

    forAll(np, i)
    {
        word procName("processor" + CML::name(i));
        uniformPropsDict.add(procName, dictionary());
        uniformPropsDict.subDict(procName).add("particleCount", np[i]);
    }

    uniformPropsDict.writeObject
    (
        IOstream::ASCII,
        IOstream::currentVersion,
        time().writeCompression()
    );
}


template<class ParticleType>
void CML::Cloud<ParticleType>::initCloud(const bool checkClass)
{
    readCloudUniformProperties();

    IOPosition<Cloud<ParticleType> > ioP(*this);

    if (ioP.headerOk())
    {
        ioP.readData(*this, checkClass);
        ioP.close();
    }
    else
    {
        if (debug)
        {
            Pout<< "Cannot read particle positions file:" << nl
                << "    " << ioP.objectPath() << nl
                << "Assuming the initial cloud contains 0 particles." << endl;
        }
    }

    // Ask for the tetBasePtIs to trigger all processors to build
    // them, otherwise, if some processors have no particles then
    // there is a comms mismatch.
    polyMesh_.tetBasePtIs();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ParticleType>
CML::Cloud<ParticleType>::Cloud
(
    const polyMesh& pMesh,
    const word& cloudName,
    const bool checkClass
)
:
    cloud(pMesh, cloudName),
    polyMesh_(pMesh),
    globalPositionsPtr_()
{
    checkPatches();

    polyMesh_.tetBasePtIs();
    polyMesh_.oldCellCentres();

    initCloud(checkClass);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ParticleType>
CML::IOobject CML::Cloud<ParticleType>::fieldIOobject
(
    const word& fieldName,
    const IOobject::readOption r
) const
{
    return IOobject
    (
        fieldName,
        time().timeName(),
        *this,
        r,
        IOobject::NO_WRITE,
        false
    );
}


template<class ParticleType>
template<class DataType>
void CML::Cloud<ParticleType>::checkFieldIOobject
(
    const Cloud<ParticleType>& c,
    const IOField<DataType>& data
) const
{
    if (data.size() != c.size())
    {
        FatalErrorInFunction
            << "Size of " << data.name()
            << " field " << data.size()
            << " does not match the number of particles " << c.size()
            << abort(FatalError);
    }
}


template<class ParticleType>
template<class DataType>
void CML::Cloud<ParticleType>::checkFieldFieldIOobject
(
    const Cloud<ParticleType>& c,
    const CompactIOField<Field<DataType>, DataType>& data
) const
{
    if (data.size() != c.size())
    {
        FatalErrorInFunction
            << "Size of " << data.name()
            << " field " << data.size()
            << " does not match the number of particles " << c.size()
            << abort(FatalError);
    }
}


template<class ParticleType>
void CML::Cloud<ParticleType>::writeFields() const
{
    if (this->size())
    {
        ParticleType::writeFields(*this);
    }
}


template<class ParticleType>
bool CML::Cloud<ParticleType>::writeObject
(
    IOstream::streamFormat fmt,
    IOstream::versionNumber ver,
    IOstream::compressionType cmp
) const
{
    writeCloudUniformProperties();

    if (this->size())
    {
        writeFields();
        return cloud::writeObject(fmt, ver, cmp);
    }
    else
    {
        return true;
    }
}


// * * * * * * * * * * * * * * * Ostream Operators * * * * * * * * * * * * * //

template<class ParticleType>
CML::Ostream& CML::operator<<(Ostream& os, const Cloud<ParticleType>& pc)
{
    pc.writeData(os);

    // Check state of Ostream
    os.check(FUNCTION_NAME);
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
