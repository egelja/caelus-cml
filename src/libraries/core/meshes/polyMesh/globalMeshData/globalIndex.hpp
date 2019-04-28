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
    CML::globalIndex

Description
    Calculates a unique integer (label so might not have enough room - 2G max)
    for processor + local index. E.g.

    globalIndex globalFaces(mesh.nFaces());
    label globalFaceI = globalFaces.toGlobal(faceI);


SourceFiles
    globalIndex.cpp

\*---------------------------------------------------------------------------*/

#ifndef globalIndex_H
#define globalIndex_H

#include "Pstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class globalIndex;

Istream& operator>>(Istream& is, globalIndex& gi);
Ostream& operator<<(Ostream& os, const globalIndex& gi);


/*---------------------------------------------------------------------------*\
                           Class globalIndex Declaration
\*---------------------------------------------------------------------------*/

class globalIndex
{
    // Private data

        //- Start of proci. Size is nProcs()+1. (so like CompactListList)
        labelList offsets_;


public:

    // Constructors

        //- Construct null
        inline globalIndex();

        //- Construct from local max size
        globalIndex(const label localSize, const int tag = Pstream::msgType());

        //- Construct from components
        globalIndex(const labelList& offsets);

        //- Construct from components
        inline globalIndex(const Xfer<labelList>& offsets);

        //- Construct from Istream
        globalIndex(Istream& is);


    // Member Functions

        // Edit

            //- Change after construction
            inline labelList& offsets();


        // Queries relating to my processor (using world communicator)

            //- My local size
            inline label localSize() const;

            //- From local to global
            inline label toGlobal(const label i) const;

            //- Is on local processor
            inline bool isLocal(const label i) const;

            //- From global to local on current processor.
            //  FatalError if not on local processor.
            inline label toLocal(const label i) const;


        // Global queries

            //- Global sum of localSizes
            inline label size() const;

            //- Size of proci data
            inline label localSize(const label proci) const;

            //- From local to global on proci
            inline label toGlobal(const label proci, const label i) const;

            //- Is on processor proci
            inline bool isLocal(const label proci, const label i) const;

            //- From global to local on proci
            inline label toLocal(const label proci, const label i) const;

            //- Which processor does global come from? Binary search.
            inline label whichProcID(const label i) const;

            //- Start of proci data
            inline label offset(const label proci) const;


        // Other

            //- Collect data in processor order on master (== procIDs[0]).
            //  Needs offsets only on master.
            template<class Type>
            static void gather
            (
                const labelUList& offsets,
                const labelList& procIDs,
                const UList<Type>& fld,
                List<Type>& allFld,
                const int tag = UPstream::msgType(),
                const Pstream::commsTypes commsType = Pstream::nonBlocking
            );

            //- Collect data in processor order on master (== procIDs[0]).
            //  Needs offsets only on master.
            template<class Type>
            void gather
            (
                const labelList& procIDs,
                const UList<Type>& fld,
                List<Type>& allFld,
                const int tag = UPstream::msgType(),
                const Pstream::commsTypes commsType = Pstream::nonBlocking
            ) const
            {
                gather(offsets_, procIDs, fld, allFld, tag, commsType);
            }

            //- Inplace collect data in processor order on master
            //  (== procIDs[0]). Needs offsets only on master.
            template<class Type>
            static void gather
            (
                const labelUList& offsets,
                const labelList& procIDs,
                List<Type>& fld,
                const int tag = UPstream::msgType(),
                const Pstream::commsTypes commsType = Pstream::nonBlocking
            );

            //- Inplace collect data in processor order on master
            //  (== procIDs[0]). Needs offsets only on master.
            template<class Type>
            void gather
            (
                const labelList& procIDs,
                List<Type>& fld,
                const int tag = UPstream::msgType(),
                const Pstream::commsTypes commsType = Pstream::nonBlocking
            ) const
            {
                gather(offsets_, procIDs, fld, tag, commsType);
            }

            //- Distribute data in processor order. Requires fld to be sized!
            template<class Type>
            static void scatter
            (
                const labelUList& offsets,
                const labelList& procIDs,
                const UList<Type>& allFld,
                UList<Type>& fld,
                const int tag = UPstream::msgType(),
                const Pstream::commsTypes commsType = Pstream::nonBlocking
            );

            //- Distribute data in processor order. Requires fld to be sized!
            template<class Type>
            void scatter
            (
                const labelList& procIDs,
                const UList<Type>& allFld,
                UList<Type>& fld,
                const int tag = UPstream::msgType(),
                const Pstream::commsTypes commsType = Pstream::nonBlocking
            ) const
            {
                scatter(offsets_, procIDs, allFld, fld, tag, commsType);
            }


    // IOstream Operators

        friend Istream& operator>>(Istream& is, globalIndex& gi);
        friend Ostream& operator<<(Ostream& os, const globalIndex& gi);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::globalIndex::gather
(
    const labelUList& off,
    const labelList& procIDs,
    const UList<Type>& fld,
    List<Type>& allFld,
    const int tag,
    const Pstream::commsTypes commsType
)
{
    if (Pstream::myProcNo() == procIDs[0])
    {
        allFld.setSize(off.last());

        // Assign my local data
        SubList<Type>(allFld, fld.size(), 0) = fld;

        if (commsType == Pstream::scheduled || commsType == Pstream::blocking)
        {
            for (label i = 1; i < procIDs.size(); i++)
            {
                SubList<Type> procSlot(allFld, off[i+1]-off[i], off[i]);

                if (contiguous<Type>())
                {
                    IPstream::read
                    (
                        commsType,
                        procIDs[i],
                        reinterpret_cast<char*>(procSlot.begin()),
                        procSlot.byteSize(),
                        tag
                    );
                }
                else
                {
                    IPstream fromSlave
                    (
                        commsType,
                        procIDs[i],
                        0,
                        tag
                    );
                    fromSlave >> procSlot;
                }
            }
        }
        else
        {
            // nonBlocking

            if (!contiguous<Type>())
            {
                FatalErrorInFunction
                    << "nonBlocking not supported for non-contiguous data"
                    << exit(FatalError);
            }

            label startOfRequests = Pstream::nRequests();

            // Set up reads
            for (label i = 1; i < procIDs.size(); i++)
            {
                SubList<Type> procSlot(allFld, off[i+1]-off[i], off[i]);

                IPstream::read
                (
                    commsType,
                    procIDs[i],
                    reinterpret_cast<char*>(procSlot.begin()),
                    procSlot.byteSize(),
                    tag
                );
            }

            // Wait for all to finish
            Pstream::waitRequests(startOfRequests);
        }
    }
    else
    {
        if (commsType == Pstream::scheduled || commsType == Pstream::blocking)
        {
            if (contiguous<Type>())
            {
                OPstream::write
                (
                    commsType,
                    procIDs[0],
                    reinterpret_cast<const char*>(fld.begin()),
                    fld.byteSize(),
                    tag
                );
            }
            else
            {
                OPstream toMaster
                (
                    commsType,
                    procIDs[0],
                    0,
                    tag
                );
                toMaster << fld;
            }
        }
        else
        {
            // nonBlocking

            if (!contiguous<Type>())
            {
                FatalErrorInFunction
                    << "nonBlocking not supported for non-contiguous data"
                    << exit(FatalError);
            }

            label startOfRequests = Pstream::nRequests();

            // Set up write
            OPstream::write
            (
                commsType,
                procIDs[0],
                reinterpret_cast<const char*>(fld.begin()),
                fld.byteSize(),
                tag
            );

            // Wait for all to finish
            Pstream::waitRequests(startOfRequests);
        }
    }
}


template<class Type>
void CML::globalIndex::gather
(
    const labelUList& off,
    const labelList& procIDs,
    List<Type>& fld,
    const int tag,
    const Pstream::commsTypes commsType
)
{
    List<Type> allFld;

    gather(off, procIDs, fld, allFld, tag, commsType);

    if (Pstream::myProcNo() == procIDs[0])
    {
        fld.transfer(allFld);
    }
}


template<class Type>
void CML::globalIndex::scatter
(
    const labelUList& off,
    const labelList& procIDs,
    const UList<Type>& allFld,
    UList<Type>& fld,
    const int tag,
    const Pstream::commsTypes commsType
)
{
    if (Pstream::myProcNo() == procIDs[0])
    {
        fld.deepCopy(SubList<Type>(allFld, off[1]-off[0]));

        if (commsType == Pstream::scheduled || commsType == Pstream::blocking)
        {
            for (label i = 1; i < procIDs.size(); i++)
            {
                const SubList<Type> procSlot
                (
                    allFld,
                    off[i+1]-off[i],
                    off[i]
                );

                if (contiguous<Type>())
                {
                    OPstream::write
                    (
                        commsType,
                        procIDs[i],
                        reinterpret_cast<const char*>(procSlot.begin()),
                        procSlot.byteSize(),
                        tag
                    );
                }
                else
                {
                    OPstream toSlave
                    (
                        commsType,
                        procIDs[i],
                        0,
                        tag
                    );
                    toSlave << procSlot;
                }
            }
        }
        else
        {
            // nonBlocking

            if (!contiguous<Type>())
            {
                FatalErrorInFunction
                    << "nonBlocking not supported for non-contiguous data"
                    << exit(FatalError);
            }

            label startOfRequests = Pstream::nRequests();

            // Set up writes
            for (label i = 1; i < procIDs.size(); i++)
            {
                const SubList<Type> procSlot
                (
                    allFld,
                    off[i+1]-off[i],
                    off[i]
                );

                OPstream::write
                (
                    commsType,
                    procIDs[i],
                    reinterpret_cast<const char*>(procSlot.begin()),
                    procSlot.byteSize(),
                    tag
                );
            }

            // Wait for all to finish
            Pstream::waitRequests(startOfRequests);
        }
    }
    else
    {
        if (commsType == Pstream::scheduled || commsType == Pstream::blocking)
        {
            if (contiguous<Type>())
            {
                IPstream::read
                (
                    commsType,
                    procIDs[0],
                    reinterpret_cast<char*>(fld.begin()),
                    fld.byteSize(),
                    tag
                );
            }
            else
            {
                IPstream fromMaster
                (
                    commsType,
                    procIDs[0],
                    0,
                    tag
                );
                fromMaster >> fld;
            }
        }
        else
        {
            // nonBlocking

            if (!contiguous<Type>())
            {
                FatalErrorInFunction
                    << "nonBlocking not supported for non-contiguous data"
                    << exit(FatalError);
            }

            label startOfRequests = Pstream::nRequests();

            // Set up read
            IPstream::read
            (
                commsType,
                procIDs[0],
                reinterpret_cast<char*>(fld.begin()),
                fld.byteSize(),
                tag
            );

            // Wait for all to finish
            Pstream::waitRequests(startOfRequests);
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "globalIndexI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
