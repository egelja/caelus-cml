/*---------------------------------------------------------------------------*\
Copyright (C) 2015-2017 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::mapDistributeBase

Description
    Class containing processor-to-processor mapping information.

    We store mapping from the bits-to-send to the complete starting list
    (subXXXMap) and from the received bits to their location in the new
    list (constructXXXMap).

Note:
    Schedule is a list of processor pairs (one send, one receive. One of
    them will be myself) which forms a scheduled (i.e. non-buffered) exchange.
    See distribute on how to use it.
    Note2: number of items sent on one processor have to equal the number
    of items received on the other processor.

    To aid constructing these maps there are the constructors from global
    numbering, either with or without transforms.

    Constructors using compact numbering: layout is
    - all my own elements first (whether used or not)
    - followed by used-only remote elements sorted by remote processor.
    So e.g 4 procs and on proc 1 the compact
    table will first have all globalIndex.localSize() elements from proc1
    followed by used-only elements of proc0, proc2, proc3.
    The constructed mapDistributeBase sends the local elements from and
    receives the remote elements into their compact position.
    compactMap[proci] is the position of elements from proci in the compact
    map. compactMap[myProcNo()] is empty since trivial addressing.

    It rewrites the input global indices into indices into the constructed
    data.

    When constructing from components optionally a 'flip' on
    the maps can be specified. This will interpret the map
    values as index+flip, similar to e.g. faceProcAddressing. The flip
    will only be applied to fieldTypes (scalar, vector, .. triad)


SourceFiles
    mapDistributeBase.cpp
    mapDistributeBaseTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef mapDistributeBase_HPP
#define mapDistributeBase_HPP

#include "labelList.hpp"
#include "labelPair.hpp"
#include "Pstream.hpp"
#include "boolList.hpp"
#include "Map.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class mapPolyMesh;
class globalIndex;
class PstreamBuffers;


// Forward declaration of friend functions and operators

class mapDistributeBase;

Istream& operator>>(Istream&, mapDistributeBase&);
Ostream& operator<<(Ostream&, const mapDistributeBase&);


/*---------------------------------------------------------------------------*\
                           Class mapDistributeBase Declaration
\*---------------------------------------------------------------------------*/

class mapDistributeBase
{
protected:

    // Protected data

        //- Size of reconstructed data
        label constructSize_;

        //- Maps from subsetted data back to original data
        labelListList subMap_;

        //- Maps from subsetted data to new reconstructed data
        labelListList constructMap_;

        //- Whether subMap includes flip or not
        bool subHasFlip_;

        //- Whether constructMap includes flip or not
        bool constructHasFlip_;


        //- Schedule
        mutable autoPtr<List<labelPair> > schedulePtr_;


    // Private Member Functions

        static void checkReceivedSize
        (
            const label proci,
            const label expectedSize,
            const label receivedSize
        );

        //- Construct per processor compact addressing of the global elements
        //  needed. The ones from the local processor are not included since
        //  these are always all needed.
        void calcCompactAddressing
        (
            const globalIndex& globalNumbering,
            const labelList& elements,
            List<Map<label> >& compactMap
        ) const;

        void calcCompactAddressing
        (
            const globalIndex& globalNumbering,
            const labelListList& elements,
            List<Map<label> >& compactMap
        ) const;

        void exchangeAddressing
        (
            const int tag,
            const globalIndex& globalNumbering,
            labelList& elements,
            List<Map<label> >& compactMap,
            labelList& compactStart
        );
        void exchangeAddressing
        (
            const int tag,
            const globalIndex& globalNumbering,
            labelListList& elements,
            List<Map<label> >& compactMap,
            labelList& compactStart
        );

        template<class T, class CombineOp, class negateOp>
        static void flipAndCombine
        (
            const UList<label>& map,
            const bool hasFlip,
            const UList<T>& rhs,
            const CombineOp& cop,
            const negateOp& negOp,
            List<T>& lhs
        );

        template<class T, class negateOp>
        static T accessAndFlip
        (
            const UList<T>& fld,
            const label index,
            const bool hasFlip,
            const negateOp& negOp
        );

public:

    // Declare name of the class and its debug switch
    ClassName("mapDistributeBase");


    // Constructors

        //- Construct null
        mapDistributeBase();

        //- Construct from components
        mapDistributeBase
        (
            const label constructSize,
            const Xfer<labelListList>& subMap,
            const Xfer<labelListList>& constructMap,
            const bool subHasFlip = false,
            const bool constructHasFlip = false
        );

        //- Construct from reverse addressing: per data item the send
        //  processor and the receive processor. (note: data is not stored
        //  sorted per processor so cannot use printLayout).
        mapDistributeBase
        (
            const labelList& sendProcs,
            const labelList& recvProcs
        );

        //- Construct from list of (possibly) remote elements in globalIndex
        //  numbering (or -1). Determines compact numbering (see above) and
        //  distribute map to get data into this ordering and renumbers the
        //  elements to be in compact numbering.
        mapDistributeBase
        (
            const globalIndex&,
            labelList& elements,
            List<Map<label> >& compactMap,
            const int tag = Pstream::msgType()
        );

        //- Special variant that works with the info sorted into bins
        //  according to local indices. E.g. think cellCells where
        //  cellCells[localCellI] is a list of global cells
        mapDistributeBase
        (
            const globalIndex&,
            labelListList& cellCells,
            List<Map<label> >& compactMap,
            const int tag = Pstream::msgType()
        );

        //- Construct by transferring parameter content
        mapDistributeBase(const Xfer<mapDistributeBase>&);

        //- Construct copy
        mapDistributeBase(const mapDistributeBase&);

        //- Construct from Istream
        mapDistributeBase(Istream&);


    // Member Functions

        // Access

            //- Constructed data size
            label constructSize() const
            {
                return constructSize_;
            }

            //- Constructed data size
            label& constructSize()
            {
                return constructSize_;
            }

            //- From subsetted data back to original data
            const labelListList& subMap() const
            {
                return subMap_;
            }

            //- From subsetted data back to original data
            labelListList& subMap()
            {
                return subMap_;
            }

            //- From subsetted data to new reconstructed data
            const labelListList& constructMap() const
            {
                return constructMap_;
            }

            //- From subsetted data to new reconstructed data
            labelListList& constructMap()
            {
                return constructMap_;
            }

            //- Does subMap include a sign
            bool subHasFlip() const
            {
                return subHasFlip_;
            }

            //- Does subMap include a sign
            bool& subHasFlip()
            {
                return subHasFlip_;
            }

            //- Does constructMap include a sign
            bool constructHasFlip() const
            {
                return constructHasFlip_;
            }

            //- Does constructMap include a sign
            bool& constructHasFlip()
            {
                return constructHasFlip_;
            }

            //- Calculate a schedule. See above.
            static List<labelPair> schedule
            (
                const labelListList& subMap,
                const labelListList& constructMap,
                const int tag
            );

            //- Return a schedule. Demand driven. See above.
            const List<labelPair>& schedule() const;


        // Other

            //- Transfer the contents of the argument and annul the argument.
            void transfer(mapDistributeBase&);

            //- Transfer contents to the Xfer container
            Xfer<mapDistributeBase> xfer();

            //- Helper for construct from globalIndex. Renumbers element
            //  (in globalIndex numbering) into compact indices.
            static label renumber
            (
                const globalIndex&,
                const List<Map<label> >& compactMap,
                const label globalElement
            );

            //- Compact maps. Gets per field a bool whether it is used (locally)
            //  and works out itself what this side and sender side can remove
            //  from maps. Only compacts non-local elements (i.e. the stuff
            //  that gets sent over), does not change the local layout
            void compact
            (
                const boolList& elemIsUsed,
                const int tag = UPstream::msgType()
            );

            //- Compact all maps and layout. Returns compaction maps for
            //  subMap and constructMap
            void compact
            (
                const boolList& elemIsUsed,
                const label localSize,            // max index for subMap
                labelList& oldToNewSub,
                labelList& oldToNewConstruct,
                const int tag = UPstream::msgType()
            );

            //- Distribute data. Note:schedule only used for
            //  Pstream::scheduled for now, all others just use
            //  send-to-all, receive-from-all.
            template<class T, class negateOp>
            static void distribute
            (
                const Pstream::commsTypes commsType,
                const List<labelPair>& schedule,
                const label constructSize,
                const labelListList& subMap,
                const bool subHasFlip,
                const labelListList& constructMap,
                const bool constructHasFlip,
                List<T>&,
                const negateOp& negOp,
                const int tag = UPstream::msgType()
            );

            //- Distribute data. If multiple processors writing to same
            //  position adds contributions using cop.
            template<class T, class CombineOp, class negateOp>
            static void distribute
            (
                const Pstream::commsTypes commsType,
                const List<labelPair>& schedule,
                const label constructSize,
                const labelListList& subMap,
                const bool subHasFlip,
                const labelListList& constructMap,
                const bool constructHasFlip,
                List<T>&,
                const CombineOp& cop,
                const negateOp& negOp,
                const T& nullValue,
                const int tag = UPstream::msgType()
            );

            //- Distribute data using default commsType.
            template<class T>
            void distribute
            (
                List<T>& fld,
                const int tag = UPstream::msgType()
            ) const;

            //- Distribute data using default commsType.
            template<class T, class negateOp>
            void distribute
            (
                List<T>& fld,
                const negateOp& negOp,
                const int tag = UPstream::msgType()
            ) const;

            //- Distribute data using default commsType.
            template<class T>
            void distribute
            (
                DynamicList<T>& fld,
                const int tag = UPstream::msgType()
            ) const;

            //- Reverse distribute data using default commsType.
            template<class T>
            void reverseDistribute
            (
                const label constructSize,
                List<T>&,
                const int tag = UPstream::msgType()
            ) const;

            //- Reverse distribute data using default commsType.
            //  Since constructSize might be larger than supplied size supply
            //  a nullValue
            template<class T>
            void reverseDistribute
            (
                const label constructSize,
                const T& nullValue,
                List<T>& fld,
                const int tag = UPstream::msgType()
            ) const;

            //- Do all sends using PstreamBuffers
            template<class T>
            void send(PstreamBuffers&, const List<T>&) const;
            //- Do all receives using PstreamBuffers
            template<class T>
            void receive(PstreamBuffers&, List<T>&) const;

            //- Debug: print layout. Can only be used on maps with sorted
            //  storage (local data first, then non-local data)
            void printLayout(Ostream& os) const;

            //- Correct for topo change.
            void updateMesh(const mapPolyMesh&)
            {
                NotImplemented;
            }

    // Member Operators

        void operator=(const mapDistributeBase&);

    // IOstream operators

        //- Read dictionary from Istream
        friend Istream& operator>>(Istream&, mapDistributeBase&);

        //- Write dictionary to Ostream
        friend Ostream& operator<<(Ostream&, const mapDistributeBase&);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#include "Pstream.hpp"
#include "PstreamBuffers.hpp"
#include "PstreamCombineReduceOps.hpp"
#include "flipOp.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class T, class CombineOp, class negateOp>
void CML::mapDistributeBase::flipAndCombine
(
    const UList<label>& map,
    const bool hasFlip,
    const UList<T>& rhs,
    const CombineOp& cop,
    const negateOp& negOp,
    List<T>& lhs
)
{
    if (hasFlip)
    {
        forAll(map, i)
        {
            if (map[i] > 0)
            {
                label index = map[i]-1;
                cop(lhs[index], rhs[i]);
            }
            else if (map[i] < 0)
            {
                label index = -map[i]-1;
                cop(lhs[index], negOp(rhs[i]));
            }
            else
            {
                FatalErrorInFunction
                    << "At index " << i << " out of " << map.size()
                    << " have illegal index " << map[i]
                    << " for field " << rhs.size() << " with flipMap"
                    << exit(FatalError);
            }
        }
    }
    else
    {
        forAll(map, i)
        {
            cop(lhs[map[i]], rhs[i]);
        }
    }
}


template<class T, class negateOp>
T CML::mapDistributeBase::accessAndFlip
(
    const UList<T>& fld,
    const label index,
    const bool hasFlip,
    const negateOp& negOp
)
{
    T t;
    if (hasFlip)
    {
        if (index > 0)
        {
            t = fld[index-1];
        }
        else if (index < 0)
        {
            t = negOp(fld[-index-1]);
        }
        else
        {
            FatalErrorInFunction
                << "Illegal index " << index
                << " into field of size " << fld.size()
                << " with face-flipping"
                << exit(FatalError);
            t = fld[index];
        }
    }
    else
    {
        t = fld[index];
    }
    return t;
}


// Distribute list.
template<class T, class negateOp>
void CML::mapDistributeBase::distribute
(
    const Pstream::commsTypes commsType,
    const List<labelPair>& schedule,
    const label constructSize,
    const labelListList& subMap,
    const bool subHasFlip,
    const labelListList& constructMap,
    const bool constructHasFlip,
    List<T>& field,
    const negateOp& negOp,
    const int tag
)
{
    if (!Pstream::parRun())
    {
        // Do only me to me.

        const labelList& mySubMap = subMap[Pstream::myProcNo()];

        List<T> subField(mySubMap.size());
        forAll(mySubMap, i)
        {
            subField[i] = accessAndFlip(field, mySubMap[i], subHasFlip, negOp);
        }

        // Receive sub field from myself (subField)
        const labelList& map = constructMap[Pstream::myProcNo()];

        field.setSize(constructSize);

        flipAndCombine
        (
            map,
            constructHasFlip,
            subField,
            eqOp<T>(),
            negOp,
            field
        );

        return;
    }

    if (commsType == Pstream::blocking)
    {
        // Since buffered sending can reuse the field to collect the
        // received data.

        // Send sub field to neighbour
        for (label domain = 0; domain < Pstream::nProcs(); domain++)
        {
            const labelList& map = subMap[domain];

            if (domain != Pstream::myProcNo() && map.size())
            {
                OPstream toNbr(Pstream::blocking, domain, 0, tag);

                List<T> subField(map.size());
                forAll(subField, i)
                {
                    subField[i] = accessAndFlip
                    (
                        field,
                        map[i],
                        subHasFlip,
                        negOp
                    );
                }
                toNbr << subField;
            }
        }

        // Subset myself
        const labelList& mySubMap = subMap[Pstream::myProcNo()];

        List<T> subField(mySubMap.size());
        forAll(mySubMap, i)
        {
            subField[i] = accessAndFlip(field, mySubMap[i], subHasFlip, negOp);
        }

        // Receive sub field from myself (subField)
        const labelList& map = constructMap[Pstream::myProcNo()];

        field.setSize(constructSize);

        flipAndCombine
        (
            map,
            constructHasFlip,
            subField,
            eqOp<T>(),
            negOp,
            field
        );

        // Receive sub field from neighbour
        for (label domain = 0; domain < Pstream::nProcs(); domain++)
        {
            const labelList& map = constructMap[domain];

            if (domain != Pstream::myProcNo() && map.size())
            {
                IPstream fromNbr(Pstream::blocking, domain, 0, tag);
                List<T> subField(fromNbr);

                checkReceivedSize(domain, map.size(), subField.size());

                flipAndCombine
                (
                    map,
                    constructHasFlip,
                    subField,
                    eqOp<T>(),
                    negOp,
                    field
                );
            }
        }
    }
    else if (commsType == Pstream::scheduled)
    {
        // Need to make sure I don't overwrite field with received data
        // since the data might need to be sent to another processor. So
        // allocate a new field for the results.
        List<T> newField(constructSize);

        // Receive sub field from myself
        {
            const labelList& mySubMap = subMap[Pstream::myProcNo()];

            List<T> subField(mySubMap.size());
            forAll(subField, i)
            {
                subField[i] = accessAndFlip
                (
                    field,
                    mySubMap[i],
                    subHasFlip,
                    negOp
                );
            }

            // Receive sub field from myself (subField)
            flipAndCombine
            (
                constructMap[Pstream::myProcNo()],
                constructHasFlip,
                subField,
                eqOp<T>(),
                negOp,
                newField
            );
        }

        // Schedule will already have pruned 0-sized comms
        forAll(schedule, i)
        {
            const labelPair& twoProcs = schedule[i];
            // twoProcs is a swap pair of processors. The first one is the
            // one that needs to send first and then receive.

            label sendProc = twoProcs[0];
            label recvProc = twoProcs[1];

            if (Pstream::myProcNo() == sendProc)
            {
                // I am send first, receive next
                {
                    OPstream toNbr
                    (
                        Pstream::scheduled,
                        recvProc,
                        0,
                        tag
                    );

                    const labelList& map = subMap[recvProc];
                    List<T> subField(map.size());
                    forAll(subField, i)
                    {
                        subField[i] = accessAndFlip
                        (
                            field,
                            map[i],
                            subHasFlip,
                            negOp
                        );
                    }
                    toNbr << subField;
                }
                {
                    IPstream fromNbr
                    (
                        Pstream::scheduled,
                        recvProc,
                        0,
                        tag
                    );
                    List<T> subField(fromNbr);

                    const labelList& map = constructMap[recvProc];

                    checkReceivedSize(recvProc, map.size(), subField.size());

                    flipAndCombine
                    (
                        map,
                        constructHasFlip,
                        subField,
                        eqOp<T>(),
                        negOp,
                        newField
                    );
                }
            }
            else
            {
                // I am receive first, send next
                {
                    IPstream fromNbr
                    (
                        Pstream::scheduled,
                        sendProc,
                        0,
                        tag
                    );
                    List<T> subField(fromNbr);

                    const labelList& map = constructMap[sendProc];

                    checkReceivedSize(sendProc, map.size(), subField.size());

                    flipAndCombine
                    (
                        map,
                        constructHasFlip,
                        subField,
                        eqOp<T>(),
                        negOp,
                        newField
                    );
                }
                {
                    OPstream toNbr
                    (
                        Pstream::scheduled,
                        sendProc,
                        0,
                        tag
                    );

                    const labelList& map = subMap[sendProc];
                    List<T> subField(map.size());
                    forAll(subField, i)
                    {
                        subField[i] = accessAndFlip
                        (
                            field,
                            map[i],
                            subHasFlip,
                            negOp
                        );
                    }
                    toNbr << subField;
                }
            }
        }
        field.transfer(newField);
    }
    else if (commsType == Pstream::nonBlocking)
    {
        label nOutstanding = Pstream::nRequests();

        if (!contiguous<T>())
        {
            PstreamBuffers pBufs(Pstream::nonBlocking, tag);

            // Stream data into buffer
            for (label domain = 0; domain < Pstream::nProcs(); domain++)
            {
                const labelList& map = subMap[domain];

                if (domain != Pstream::myProcNo() && map.size())
                {
                    // Put data into send buffer
                    UOPstream toDomain(domain, pBufs);

                    List<T> subField(map.size());
                    forAll(subField, i)
                    {
                        subField[i] = accessAndFlip
                        (
                            field,
                            map[i],
                            subHasFlip,
                            negOp
                        );
                    }
                    toDomain << subField;
                }
            }

            // Start receiving. Do not block.
            pBufs.finishedSends(false);

            {
                // Set up 'send' to myself
                const labelList& mySub = subMap[Pstream::myProcNo()];
                List<T> mySubField(mySub.size());
                forAll(mySub, i)
                {
                    mySubField[i] = accessAndFlip
                    (
                        field,
                        mySub[i],
                        subHasFlip,
                        negOp
                    );
                }
                // Combine bits. Note that can reuse field storage
                field.setSize(constructSize);
                // Receive sub field from myself
                {
                    const labelList& map = constructMap[Pstream::myProcNo()];

                    flipAndCombine
                    (
                        map,
                        constructHasFlip,
                        mySubField,
                        eqOp<T>(),
                        negOp,
                        field
                    );
                }
            }

            // Block ourselves, waiting only for the current comms
            Pstream::waitRequests(nOutstanding);

            // Consume
            for (label domain = 0; domain < Pstream::nProcs(); domain++)
            {
                const labelList& map = constructMap[domain];

                if (domain != Pstream::myProcNo() && map.size())
                {
                    UIPstream str(domain, pBufs);
                    List<T> recvField(str);

                    checkReceivedSize(domain, map.size(), recvField.size());

                    flipAndCombine
                    (
                        map,
                        constructHasFlip,
                        recvField,
                        eqOp<T>(),
                        negOp,
                        field
                    );
                }
            }
        }
        else
        {
            // Set up sends to neighbours

            List<List<T > > sendFields(Pstream::nProcs());

            for (label domain = 0; domain < Pstream::nProcs(); domain++)
            {
                const labelList& map = subMap[domain];

                if (domain != Pstream::myProcNo() && map.size())
                {
                    List<T>& subField = sendFields[domain];
                    subField.setSize(map.size());
                    forAll(map, i)
                    {
                        subField[i] = accessAndFlip
                        (
                            field,
                            map[i],
                            subHasFlip,
                            negOp
                        );
                    }

                    OPstream::write
                    (
                        Pstream::nonBlocking,
                        domain,
                        reinterpret_cast<const char*>(subField.begin()),
                        subField.byteSize(),
                        tag
                    );
                }
            }

            // Set up receives from neighbours

            List<List<T > > recvFields(Pstream::nProcs());

            for (label domain = 0; domain < Pstream::nProcs(); domain++)
            {
                const labelList& map = constructMap[domain];

                if (domain != Pstream::myProcNo() && map.size())
                {
                    recvFields[domain].setSize(map.size());
                    IPstream::read
                    (
                        Pstream::nonBlocking,
                        domain,
                        reinterpret_cast<char*>(recvFields[domain].begin()),
                        recvFields[domain].byteSize(),
                        tag
                    );
                }
            }


            // Set up 'send' to myself

            {
                const labelList& map = subMap[Pstream::myProcNo()];

                List<T>& subField = sendFields[Pstream::myProcNo()];
                subField.setSize(map.size());
                forAll(map, i)
                {
                    subField[i] = accessAndFlip
                    (
                        field,
                        map[i],
                        subHasFlip,
                        negOp
                    );
                }
            }


            // Combine bits. Note that can reuse field storage

            field.setSize(constructSize);


            // Receive sub field from myself (sendFields[Pstream::myProcNo()])
            {
                const labelList& map = constructMap[Pstream::myProcNo()];
                const List<T>& subField = sendFields[Pstream::myProcNo()];

                flipAndCombine
                (
                    map,
                    constructHasFlip,
                    subField,
                    eqOp<T>(),
                    negOp,
                    field
                );
            }


            // Wait for all to finish

            Pstream::waitRequests(nOutstanding);


            // Collect neighbour fields

            for (label domain = 0; domain < Pstream::nProcs(); domain++)
            {
                const labelList& map = constructMap[domain];

                if (domain != Pstream::myProcNo() && map.size())
                {
                    const List<T>& subField = recvFields[domain];

                    checkReceivedSize(domain, map.size(), subField.size());

                    flipAndCombine
                    (
                        map,
                        constructHasFlip,
                        subField,
                        eqOp<T>(),
                        negOp,
                        field
                    );
                }
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "Unknown communication schedule " << int(commsType)
            << abort(FatalError);
    }
}


// Distribute list.
template<class T, class CombineOp, class negateOp>
void CML::mapDistributeBase::distribute
(
    const Pstream::commsTypes commsType,
    const List<labelPair>& schedule,
    const label constructSize,
    const labelListList& subMap,
    const bool subHasFlip,
    const labelListList& constructMap,
    const bool constructHasFlip,
    List<T>& field,
    const CombineOp& cop,
    const negateOp& negOp,
    const T& nullValue,
    const int tag
)
{
    if (!Pstream::parRun())
    {
        // Do only me to me.

        const labelList& mySubMap = subMap[Pstream::myProcNo()];

        List<T> subField(mySubMap.size());
        forAll(mySubMap, i)
        {
            subField[i] = accessAndFlip(field, mySubMap[i], subHasFlip, negOp);
        }

        // Receive sub field from myself (subField)
        const labelList& map = constructMap[Pstream::myProcNo()];

        field.setSize(constructSize);
        field = nullValue;

        flipAndCombine(map, constructHasFlip, subField, cop, negOp, field);

        return;
    }

    if (commsType == Pstream::blocking)
    {
        // Since buffered sending can reuse the field to collect the
        // received data.

        // Send sub field to neighbour
        for (label domain = 0; domain < Pstream::nProcs(); domain++)
        {
            const labelList& map = subMap[domain];

            if (domain != Pstream::myProcNo() && map.size())
            {
                OPstream toNbr(Pstream::blocking, domain, 0, tag);
                List<T> subField(map.size());
                forAll(subField, i)
                {
                    subField[i] = accessAndFlip
                    (
                        field,
                        map[i],
                        subHasFlip,
                        negOp
                    );
                }
                toNbr << subField;
            }
        }

        // Subset myself
        const labelList& mySubMap = subMap[Pstream::myProcNo()];

        List<T> subField(mySubMap.size());
        forAll(mySubMap, i)
        {
            subField[i] = accessAndFlip(field, mySubMap[i], subHasFlip, negOp);
        }

        // Receive sub field from myself (subField)
        const labelList& map = constructMap[Pstream::myProcNo()];

        field.setSize(constructSize);
        field = nullValue;

        flipAndCombine(map, constructHasFlip, subField, cop, negOp, field);

        // Receive sub field from neighbour
        for (label domain = 0; domain < Pstream::nProcs(); domain++)
        {
            const labelList& map = constructMap[domain];

            if (domain != Pstream::myProcNo() && map.size())
            {
                IPstream fromNbr(Pstream::blocking, domain, 0, tag);
                List<T> subField(fromNbr);

                checkReceivedSize(domain, map.size(), subField.size());

                flipAndCombine
                (
                    map,
                    constructHasFlip,
                    subField,
                    cop,
                    negOp,
                    field
                );
            }
        }
    }
    else if (commsType == Pstream::scheduled)
    {
        // Need to make sure I don't overwrite field with received data
        // since the data might need to be sent to another processor. So
        // allocate a new field for the results.
        List<T> newField(constructSize, nullValue);

        {
            const labelList& mySubMap = subMap[Pstream::myProcNo()];

            // Subset myself
            List<T> subField(mySubMap.size());
            forAll(subField, i)
            {
                subField[i] = accessAndFlip
                (
                    field,
                    mySubMap[i],
                    subHasFlip,
                    negOp
                );
            }

            // Receive sub field from myself (subField)
            const labelList& map = constructMap[Pstream::myProcNo()];

            flipAndCombine
            (
                map,
                constructHasFlip,
                subField,
                cop,
                negOp,
                newField
            );
        }


        // Schedule will already have pruned 0-sized comms
        forAll(schedule, i)
        {
            const labelPair& twoProcs = schedule[i];
            // twoProcs is a swap pair of processors. The first one is the
            // one that needs to send first and then receive.

            label sendProc = twoProcs[0];
            label recvProc = twoProcs[1];

            if (Pstream::myProcNo() == sendProc)
            {
                // I am send first, receive next
                {
                    OPstream toNbr
                    (
                        Pstream::scheduled,
                        recvProc,
                        0,
                        tag
                    );

                    const labelList& map = subMap[recvProc];

                    List<T> subField(map.size());
                    forAll(subField, i)
                    {
                        subField[i] = accessAndFlip
                        (
                            field,
                            map[i],
                            subHasFlip,
                            negOp
                        );
                    }
                    toNbr << subField;
                }
                {
                    IPstream fromNbr
                    (
                        Pstream::scheduled,
                        recvProc,
                        0,
                        tag
                    );
                    List<T> subField(fromNbr);
                    const labelList& map = constructMap[recvProc];

                    checkReceivedSize(recvProc, map.size(), subField.size());

                    flipAndCombine
                    (
                        map,
                        constructHasFlip,
                        subField,
                        cop,
                        negOp,
                        newField
                    );
                }
            }
            else
            {
                // I am receive first, send next
                {
                    IPstream fromNbr
                    (
                        Pstream::scheduled,
                        sendProc,
                        0,
                        tag
                    );
                    List<T> subField(fromNbr);
                    const labelList& map = constructMap[sendProc];

                    checkReceivedSize(sendProc, map.size(), subField.size());

                    flipAndCombine
                    (
                        map,
                        constructHasFlip,
                        subField,
                        cop,
                        negOp,
                        newField
                    );
                }
                {
                    OPstream toNbr
                    (
                        Pstream::scheduled,
                        sendProc,
                        0,
                        tag
                    );

                    const labelList& map = subMap[sendProc];

                    List<T> subField(map.size());
                    forAll(subField, i)
                    {
                        subField[i] = accessAndFlip
                        (
                            field,
                            map[i],
                            subHasFlip,
                            negOp
                        );
                    }
                    toNbr << subField;
                }
            }
        }
        field.transfer(newField);
    }
    else if (commsType == Pstream::nonBlocking)
    {
        label nOutstanding = Pstream::nRequests();

        if (!contiguous<T>())
        {
            PstreamBuffers pBufs(Pstream::nonBlocking, tag);

            // Stream data into buffer
            for (label domain = 0; domain < Pstream::nProcs(); domain++)
            {
                const labelList& map = subMap[domain];

                if (domain != Pstream::myProcNo() && map.size())
                {
                    // Put data into send buffer
                    UOPstream toDomain(domain, pBufs);

                    List<T> subField(map.size());
                    forAll(subField, i)
                    {
                        subField[i] = accessAndFlip
                        (
                            field,
                            map[i],
                            subHasFlip,
                            negOp
                        );
                    }
                    toDomain << subField;
                }
            }

            // Start receiving. Do not block.
            pBufs.finishedSends(false);

            {
                // Set up 'send' to myself
                const labelList& myMap = subMap[Pstream::myProcNo()];

                List<T> mySubField(myMap.size());
                forAll(myMap, i)
                {
                    mySubField[i] = accessAndFlip
                    (
                        field,
                        myMap[i],
                        subHasFlip,
                        negOp
                    );
                }

                // Combine bits. Note that can reuse field storage
                field.setSize(constructSize);
                field = nullValue;
                // Receive sub field from myself
                {
                    const labelList& map = constructMap[Pstream::myProcNo()];

                    flipAndCombine
                    (
                        map,
                        constructHasFlip,
                        mySubField,
                        cop,
                        negOp,
                        field
                    );
                }
            }

            // Block ourselves, waiting only for the current comms
            Pstream::waitRequests(nOutstanding);

            // Consume
            for (label domain = 0; domain < Pstream::nProcs(); domain++)
            {
                const labelList& map = constructMap[domain];

                if (domain != Pstream::myProcNo() && map.size())
                {
                    UIPstream str(domain, pBufs);
                    List<T> recvField(str);

                    checkReceivedSize(domain, map.size(), recvField.size());

                    flipAndCombine
                    (
                        map,
                        constructHasFlip,
                        recvField,
                        cop,
                        negOp,
                        field
                    );
                }
            }
        }
        else
        {
            // Set up sends to neighbours

            List<List<T > > sendFields(Pstream::nProcs());

            for (label domain = 0; domain < Pstream::nProcs(); domain++)
            {
                const labelList& map = subMap[domain];

                if (domain != Pstream::myProcNo() && map.size())
                {
                    List<T>& subField = sendFields[domain];
                    subField.setSize(map.size());
                    forAll(map, i)
                    {
                        subField[i] = accessAndFlip
                        (
                            field,
                            map[i],
                            subHasFlip,
                            negOp
                        );
                    }

                    OPstream::write
                    (
                        Pstream::nonBlocking,
                        domain,
                        reinterpret_cast<const char*>(subField.begin()),
                        subField.size()*sizeof(T),
                        tag
                    );
                }
            }

            // Set up receives from neighbours

            List<List<T > > recvFields(Pstream::nProcs());

            for (label domain = 0; domain < Pstream::nProcs(); domain++)
            {
                const labelList& map = constructMap[domain];

                if (domain != Pstream::myProcNo() && map.size())
                {
                    recvFields[domain].setSize(map.size());
                    UIPstream::read
                    (
                        Pstream::nonBlocking,
                        domain,
                        reinterpret_cast<char*>(recvFields[domain].begin()),
                        recvFields[domain].size()*sizeof(T),
                        tag
                    );
                }
            }

            // Set up 'send' to myself

            {
                const labelList& map = subMap[Pstream::myProcNo()];

                List<T>& subField = sendFields[Pstream::myProcNo()];
                subField.setSize(map.size());
                forAll(map, i)
                {
                    subField[i] = accessAndFlip
                    (
                        field,
                        map[i],
                        subHasFlip,
                        negOp
                    );
                }
            }


            // Combine bits. Note that can reuse field storage

            field.setSize(constructSize);
            field = nullValue;

            // Receive sub field from myself (subField)
            {
                const labelList& map = constructMap[Pstream::myProcNo()];
                const List<T>& subField = sendFields[Pstream::myProcNo()];

                flipAndCombine
                (
                    map,
                    constructHasFlip,
                    subField,
                    cop,
                    negOp,
                    field
                );
            }


            // Wait for all to finish

            Pstream::waitRequests(nOutstanding);


            // Collect neighbour fields

            for (label domain = 0; domain < Pstream::nProcs(); domain++)
            {
                const labelList& map = constructMap[domain];

                if (domain != Pstream::myProcNo() && map.size())
                {
                    const List<T>& subField = recvFields[domain];

                    checkReceivedSize(domain, map.size(), subField.size());

                    flipAndCombine
                    (
                        map,
                        constructHasFlip,
                        subField,
                        cop,
                        negOp,
                        field
                    );
                }
            }
        }
    }
    else
    {
        FatalErrorInFunction
            << "Unknown communication schedule " << commsType
            << abort(FatalError);
    }
}


template<class T>
void CML::mapDistributeBase::send(PstreamBuffers& pBufs, const List<T>& field)
const
{
    // Stream data into buffer
    for (label domain = 0; domain < Pstream::nProcs(); domain++)
    {
        const labelList& map = subMap_[domain];

        if (map.size())
        {
            // Put data into send buffer
            UOPstream toDomain(domain, pBufs);

            List<T> subField(map.size());
            forAll(subField, i)
            {
                subField[i] = accessAndFlip
                (
                    field,
                    map[i],
                    subHasFlip_,
                    flipOp()
                );
            }
            toDomain << subField;
        }
    }

    // Start sending and receiving but do not block.
    pBufs.finishedSends(false);
}


template<class T>
void CML::mapDistributeBase::receive(PstreamBuffers& pBufs, List<T>& field)
const
{
    // Consume
    field.setSize(constructSize_);

    for (label domain = 0; domain < Pstream::nProcs(); domain++)
    {
        const labelList& map = constructMap_[domain];

        if (map.size())
        {
            UIPstream str(domain, pBufs);
            List<T> recvField(str);

            if (recvField.size() != map.size())
            {
                FatalErrorInFunction
                    << "Expected from processor " << domain
                    << " " << map.size() << " but received "
                    << recvField.size() << " elements."
                    << abort(FatalError);
            }

            flipAndCombine
            (
                map,
                constructHasFlip_,
                recvField,
                eqOp<T>(),
                flipOp(),
                field
            );
        }
    }
}


//- Distribute data using default commsType.
template<class T, class negateOp>
void CML::mapDistributeBase::distribute
(
    List<T>& fld,
    const negateOp& negOp,
    const int tag
) const
{
    if (Pstream::defaultCommsType == Pstream::nonBlocking)
    {
        distribute
        (
            Pstream::nonBlocking,
            List<labelPair>(),
            constructSize_,
            subMap_,
            subHasFlip_,
            constructMap_,
            constructHasFlip_,
            fld,
            negOp,
            tag
        );
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        distribute
        (
            Pstream::scheduled,
            schedule(),
            constructSize_,
            subMap_,
            subHasFlip_,
            constructMap_,
            constructHasFlip_,
            fld,
            negOp,
            tag
        );
    }
    else
    {
        distribute
        (
            Pstream::blocking,
            List<labelPair>(),
            constructSize_,
            subMap_,
            subHasFlip_,
            constructMap_,
            constructHasFlip_,
            fld,
            negOp,
            tag
        );
    }
}


//- Distribute data using default commsType.
template<class T>
void CML::mapDistributeBase::distribute
(
    List<T>& fld,
    const int tag
) const
{
    distribute(fld, flipOp(), tag);
}


//- Distribute data using default commsType.
template<class T>
void CML::mapDistributeBase::distribute
(
    DynamicList<T>& fld,
    const int tag
) const
{
    fld.shrink();

    List<T>& fldList = static_cast<List<T>& >(fld);

    distribute(fldList, tag);

    fld.setCapacity(fldList.size());
}


//- Reverse distribute data using default commsType.
template<class T>
void CML::mapDistributeBase::reverseDistribute
(
    const label constructSize,
    List<T>& fld,
    const int tag
) const
{
    if (Pstream::defaultCommsType == Pstream::nonBlocking)
    {
        distribute
        (
            Pstream::nonBlocking,
            List<labelPair>(),
            constructSize,
            constructMap_,
            constructHasFlip_,
            subMap_,
            subHasFlip_,
            fld,
            flipOp(),
            tag
        );
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        distribute
        (
            Pstream::scheduled,
            schedule(),
            constructSize,
            constructMap_,
            constructHasFlip_,
            subMap_,
            subHasFlip_,
            fld,
            flipOp(),
            tag
        );
    }
    else
    {
        distribute
        (
            Pstream::blocking,
            List<labelPair>(),
            constructSize,
            constructMap_,
            constructHasFlip_,
            subMap_,
            subHasFlip_,
            fld,
            flipOp(),
            tag
        );
    }
}


//- Reverse distribute data using default commsType.
//  Since constructSize might be larger than supplied size supply
//  a nullValue
template<class T>
void CML::mapDistributeBase::reverseDistribute
(
    const label constructSize,
    const T& nullValue,
    List<T>& fld,
    const int tag
) const
{
    if (Pstream::defaultCommsType == Pstream::nonBlocking)
    {
        distribute
        (
            Pstream::nonBlocking,
            List<labelPair>(),
            constructSize,
            constructMap_,
            constructHasFlip_,
            subMap_,
            subHasFlip_,
            fld,
            eqOp<T>(),
            flipOp(),
            nullValue,
            tag
        );
    }
    else if (Pstream::defaultCommsType == Pstream::scheduled)
    {
        distribute
        (
            Pstream::scheduled,
            schedule(),
            constructSize,
            constructMap_,
            constructHasFlip_,
            subMap_,
            subHasFlip_,
            fld,
            eqOp<T>(),
            flipOp(),
            nullValue,
            tag
        );
    }
    else
    {
        distribute
        (
            Pstream::blocking,
            List<labelPair>(),
            constructSize,
            constructMap_,
            constructHasFlip_,
            subMap_,
            subHasFlip_,
            fld,
            eqOp<T>(),
            flipOp(),
            nullValue,
            tag
        );
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
