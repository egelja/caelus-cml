/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::mapDistribute

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

    - without transforms:
    Constructors using compact numbering: layout is
    - all my own elements first (whether used or not)
    - followed by used-only remote elements sorted by remote processor.
    So e.g 4 procs and on proc 1 the compact
    table will first have all globalIndex.localSize() elements from proc1
    followed by used-only elements of proc0, proc2, proc3.
    The constructed mapDistribute sends the local elements from and
    receives the remote elements into their compact position.
    compactMap[procI] is the position of elements from procI in the compact
    map. compactMap[myProcNo()] is empty since trivial addressing.

    It rewrites the input global indices into indices into the constructed
    data.


    - with transforms:
    This requires the precalculated set of possible transforms
    (globalIndexAndTransform). These are given as permutations (+, -, or none)
    of up to 3 independent transforms.
    The layout of the data is
    - all my own elements first (whether used or not)
    - followed by used-only remote elements sorted by remote processor.
    - followed by - for each transformation index - the set of local or
    remote elements with that transformation.
    The inputs for the constructor are
    - the set of untransformed local or remote indices in globalIndex
    numbering. These get rewritten to be indices into the layout of the data.
    - the set of transformed local or remote indices in globalIndexAndTransform
    encoding. These are labelPairs.

    Any distribute with transforms is now done as:
    1. exchange data with other processors and receive these into the
    slots for that processor
    2. for all transformations transform a subset of the data according
    to transformElements_[transformI] and store this starting from
    transformStart_[transformI]

    In the same way a reverse distribute will
    1. apply the inverse transform to the data starting at
    transformStart_[transformI] and copy the result back into the
    transformElements_[transformI]. These might be local or remote slots.
    2. the data in the remote slots will now be sent back to the correct
    location in the originating processor.

    E.g. a map to handle
    - mesh points on a mesh with
    - 1 cyclic so 3 permutations (+,-,none) will have layout
    - on e.g. processor 1 out of 2:

        +------+ <- transformStart[2]
        |      |
        |      | <- transform2 applied to data in local or remote slots
        |      |
        +------+ <- transformStart[1]
        |      |
        |      | <- transform1 applied to data in local or remote slots
        |      |
        +------+ <- transformStart[1]
        |      |
        |      | <- transform0 applied to data in local or remote slots
        |      |
        +------+ <- transformStart[0]
        |      |
        |      | <- data from proc2
        |      |
        +------+
        |      |
        |      | <- data from proc0
        |      |
        +------+ <- mesh.nPoints()
        |      |
        |      |
        |      |
        +------+ 0


SourceFiles
    mapDistribute.cpp
    mapDistributeTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef mapDistribute_H
#define mapDistribute_H

#include "transformList.hpp"
#include "labelList.hpp"
#include "labelPair.hpp"
#include "Pstream.hpp"
#include "boolList.hpp"
#include "Map.hpp"
#include "vectorTensorTransform.hpp"
#include "coupledPolyPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class mapPolyMesh;
class globalIndex;
class PstreamBuffers;
class globalIndexAndTransform;

/*---------------------------------------------------------------------------*\
                           Class mapDistribute Declaration
\*---------------------------------------------------------------------------*/

class mapDistribute
{
    // Private data

        //- Size of reconstructed data
        label constructSize_;

        //- Maps from subsetted data back to original data
        labelListList subMap_;

        //- Maps from subsetted data to new reconstructed data
        labelListList constructMap_;

        // Optional transformation

            //- For every globalIndexAndTransform::transformPermutations
            //  gives the elements that need to be transformed
            labelListList transformElements_;

            //- Destination in constructMap for transformed elements
            labelList transformStart_;


        //- Schedule
        mutable autoPtr<List<labelPair> > schedulePtr_;


   // Private Member Functions

        static void checkReceivedSize
        (
            const label procI,
            const label expectedSize,
            const label receivedSize
        );

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


        //- Helper function: copy transformElements without transformation
        template<class T>
        void applyDummyTransforms(List<T>& field) const;

        template<class T, class TransformOp>
        void applyTransforms
        (
            const globalIndexAndTransform& globalTransforms,
            List<T>& field,
            const TransformOp& top
        ) const;

        //- Helper function: copy transformElements without transformation
        template<class T>
        void applyDummyInverseTransforms(List<T>& field) const;

        template<class T, class TransformOp>
        void applyInverseTransforms
        (
            const globalIndexAndTransform& globalTransforms,
            List<T>& field,
            const TransformOp& top
        ) const;


public:

    // Public classes

        //- Default transformation behaviour
        class transform
        {
        public:

            template<class T>
            void operator()
            (
                const vectorTensorTransform& vt,
                const bool forward,
                List<T>& fld
            ) const
            {
                if (forward)
                {
                    transformList(vt.R(), fld);
                }
                else
                {
                    transformList(vt.R().T(), fld);
                }
            }

            //- Transform patch-based field
            template<class T>
            void operator()(const coupledPolyPatch& cpp, UList<T>& fld) const
            {
                if (!cpp.parallel())
                {
                    transformList(cpp.forwardT(), fld);
                }
            }

            //- Transform sparse field
            template<class T, template<class> class Container>
            void operator()(const coupledPolyPatch& cpp, Container<T>& map)
            const
            {
                if (!cpp.parallel())
                {
                    transformList(cpp.forwardT(), map);
                }
            }
        };

        //- Default transformation behaviour for position
        class transformPosition
        {
        public:

            void operator()
            (
                const vectorTensorTransform& vt,
                const bool forward,
                List<point>& fld
            ) const
            {
                pointField pfld(fld.xfer());
                if (forward)
                {
                    fld = vt.transformPosition(pfld);
                }
                else
                {
                    fld = vt.invTransformPosition(pfld);
                }
            }
            //- Transform patch-based field
            void operator()(const coupledPolyPatch& cpp, pointField& fld) const
            {
                cpp.transformPosition(fld);
            }
            template<template<class> class Container>
            void operator()(const coupledPolyPatch& cpp, Container<point>& map)
            const
            {
                Field<point> fld(map.size());
                label i = 0;
                forAllConstIter(typename Container<point>, map, iter)
                {
                    fld[i++] = iter();
                }
                cpp.transformPosition(fld);
                i = 0;
                forAllIter(typename Container<point>, map, iter)
                {
                    iter() = fld[i++];
                }
            }
        };



    // Declare name of the class and its debug switch
    ClassName("mapDistribute");


    // Constructors

        //- Construct null
        mapDistribute();

        //- Construct from components
        mapDistribute
        (
            const label constructSize,
            const Xfer<labelListList>& subMap,
            const Xfer<labelListList>& constructMap
        );

        //- Construct from components
        mapDistribute
        (
            const label constructSize,
            const Xfer<labelListList>& subMap,
            const Xfer<labelListList>& constructMap,
            const Xfer<labelListList>& transformElements,
            const Xfer<labelList>& transformStart
        );

        //- Construct from reverse addressing: per data item the send
        //  processor and the receive processor. All processors get same data.
        mapDistribute
        (
            const labelList& sendProcs,
            const labelList& recvProcs
        );

        //- Construct from list of (possibly) remote elements in globalIndex
        //  numbering (or -1). Determines compact numbering (see above) and
        //  distribute map to get data into this ordering and renumbers the
        //  elements to be in compact numbering.
        mapDistribute
        (
            const globalIndex&,
            labelList& elements,
            List<Map<label> >& compactMap,
            const int tag = Pstream::msgType()
        );

        //- Special variant that works with the info sorted into bins
        //  according to local indices. E.g. think cellCells where
        //  cellCells[localCellI] is a list of global cells
        mapDistribute
        (
            const globalIndex&,
            labelListList& cellCells,
            List<Map<label> >& compactMap,
            const int tag = Pstream::msgType()
        );

        //- Construct from list of (possibly remote) untransformed elements
        //  in globalIndex numbering (or -1) and (possibly remote)
        //  transformded elements in globalIndexAndTransform numbering.
        //  Determines compact numbering (see above) and
        //  distribute map to get data into this ordering and renumbers the
        //  elements to be in compact numbering.
        mapDistribute
        (
            const globalIndex&,
            labelList& untransformedElements,
            const globalIndexAndTransform&,
            const labelPairList& transformedElements,
            labelList& transformedIndices,
            List<Map<label> >& compactMap,
            const int tag = Pstream::msgType()
        );

        //- As above but with ListLists.
        mapDistribute
        (
            const globalIndex&,
            labelListList& cellCells,
            const globalIndexAndTransform&,
            const List<labelPairList>& transformedElements,
            labelListList& transformedIndices,
            List<Map<label> >& compactMap,
            const int tag = Pstream::msgType()
        );

        //- Construct by transferring parameter content
        mapDistribute(const Xfer<mapDistribute>&);

        //- Construct copy
        mapDistribute(const mapDistribute&);


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

            //- For every globalIndexAndTransform::transformPermutations
            //  gives the elements that need to be transformed
            const labelListList& transformElements() const
            {
                return transformElements_;
            }

            //- Destination in constructMap for transformed elements
            const labelList& transformStart() const
            {
                return transformStart_;
            }

            //- Find transform from transformElements
            label whichTransform(const label index) const;

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
            void transfer(mapDistribute&);

            //- Transfer contents to the Xfer container
            Xfer<mapDistribute> xfer();

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
            //  from maps.
            void compact(const boolList& elemIsUsed, const int tag);


            //- Distribute data. Note:schedule only used for Pstream::scheduled
            //  for now, all others just use send-to-all, receive-from-all.
            template<class T>
            static void distribute
            (
                const Pstream::commsTypes commsType,
                const List<labelPair>& schedule,
                const label constructSize,
                const labelListList& subMap,
                const labelListList& constructMap,
                List<T>&,
                const int tag = UPstream::msgType()
            );

            //- Distribute data. If multiple processors writing to same
            //  position adds contributions using cop.
            template<class T, class CombineOp>
            static void distribute
            (
                const Pstream::commsTypes commsType,
                const List<labelPair>& schedule,
                const label constructSize,
                const labelListList& subMap,
                const labelListList& constructMap,
                List<T>&,
                const CombineOp& cop,
                const T& nullValue,
                const int tag = UPstream::msgType()
            );

            //- Distribute data using default commsType.
            template<class T>
            void distribute
            (
                List<T>& fld,
                const bool dummyTransform = true,
                const int tag = UPstream::msgType()
            ) const;

            //- Same but with transforms
            template<class T, class TransformOp>
            void distribute
            (
                const globalIndexAndTransform&,
                List<T>& fld,
                const TransformOp& top,
                const int tag = UPstream::msgType()
            ) const;

            //- Reverse distribute data using default commsType.
            template<class T>
            void reverseDistribute
            (
                const label constructSize,
                List<T>&,
                const bool dummyTransform = true,
                const int tag = UPstream::msgType()
            ) const;

            //- Same but with transforms
            template<class T, class TransformOp>
            void reverseDistribute
            (
                const globalIndexAndTransform&,
                const label constructSize,
                List<T>& fld,
                const TransformOp& top,
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
                const bool dummyTransform = true,
                const int tag = UPstream::msgType()
            ) const;

            //- Same but with transforms
            template<class T, class TransformOp>
            void reverseDistribute
            (
                const globalIndexAndTransform&,
                const label constructSize,
                const T& nullValue,
                List<T>& fld,
                const TransformOp& top,
                const int tag = UPstream::msgType()
            ) const;

            //- Do all sends using PstreamBuffers
            template<class T>
            void send(PstreamBuffers&, const List<T>&) const;
            //- Do all receives using PstreamBuffers
            template<class T>
            void receive(PstreamBuffers&, List<T>&) const;

            //- Debug: print layout
            void printLayout(Ostream& os) const;

            //- Correct for topo change.
            void updateMesh(const mapPolyMesh&)
            {
                notImplemented
                (
                    "mapDistribute::updateMesh(const mapPolyMesh&)"
                );
            }

    // Member Operators

        void operator=(const mapDistribute&);


    // IOstream operators

        //- Read dictionary from Istream
        friend Istream& operator>>(Istream&, mapDistribute&);

        //- Write dictionary to Ostream
        friend Ostream& operator<<(Ostream&, const mapDistribute&);

};


template<>
void mapDistribute::transform::operator()
(
    const vectorTensorTransform&,
    const bool,
    List<label>&
) const;
template<>
void mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    UList<label>&
) const;
template<>
void mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    Map<label>&
) const;
template<>
void mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    EdgeMap<label>&
) const;

template<>
void mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    UList<scalar>&
) const;
template<>
void mapDistribute::transform::operator()
(
    const vectorTensorTransform&,
    const bool,
    List<scalar>&
) const;
template<>
void mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    Map<scalar>&
) const;
template<>
void mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    EdgeMap<scalar>&
) const;

template<>
void mapDistribute::transform::operator()
(
    const coupledPolyPatch& cpp,
    UList<bool>& fld
) const;
template<>
void mapDistribute::transform::operator()
(
    const vectorTensorTransform&,
    const bool,
    List<bool>&
) const;
template<>
void mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    Map<bool>&
) const;
template<>
void mapDistribute::transform::operator()
(
    const coupledPolyPatch&,
    EdgeMap<bool>&
) const;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Pstream.hpp"
#include "PstreamBuffers.hpp"
#include "PstreamCombineReduceOps.hpp"
#include "globalIndexAndTransform.hpp"
#include "transformField.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Distribute list.
template<class T>
void CML::mapDistribute::distribute
(
    const Pstream::commsTypes commsType,
    const List<labelPair>& schedule,
    const label constructSize,
    const labelListList& subMap,
    const labelListList& constructMap,
    List<T>& field,
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
            subField[i] = field[mySubMap[i]];
        }

        // Receive sub field from myself (subField)
        const labelList& map = constructMap[Pstream::myProcNo()];

        field.setSize(constructSize);

        forAll(map, i)
        {
            field[map[i]] = subField[i];
        }
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
                toNbr << UIndirectList<T>(field, map);
            }
        }

        // Subset myself
        const labelList& mySubMap = subMap[Pstream::myProcNo()];

        List<T> subField(mySubMap.size());
        forAll(mySubMap, i)
        {
            subField[i] = field[mySubMap[i]];
        }

        // Receive sub field from myself (subField)
        const labelList& map = constructMap[Pstream::myProcNo()];

        field.setSize(constructSize);

        forAll(map, i)
        {
            field[map[i]] = subField[i];
        }

        // Receive sub field from neighbour
        for (label domain = 0; domain < Pstream::nProcs(); domain++)
        {
            const labelList& map = constructMap[domain];

            if (domain != Pstream::myProcNo() && map.size())
            {
                IPstream fromNbr(Pstream::blocking, domain, 0, tag);
                List<T> subField(fromNbr);

                checkReceivedSize(domain, map.size(), subField.size());

                forAll(map, i)
                {
                    field[map[i]] = subField[i];
                }
            }
        }
    }
    else if (commsType == Pstream::scheduled)
    {
        // Need to make sure I don't overwrite field with received data
        // since the data might need to be sent to another processor. So
        // allocate a new field for the results.
        List<T> newField(constructSize);

        // Subset myself
        UIndirectList<T> subField(field, subMap[Pstream::myProcNo()]);

        // Receive sub field from myself (subField)
        const labelList& map = constructMap[Pstream::myProcNo()];

        forAll(map, i)
        {
            newField[map[i]] = subField[i];
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
                    OPstream toNbr(Pstream::scheduled, recvProc, 0, tag);
                    toNbr << UIndirectList<T>(field, subMap[recvProc]);
                }
                {
                    IPstream fromNbr(Pstream::scheduled, recvProc, 0, tag);
                    List<T> subField(fromNbr);

                    const labelList& map = constructMap[recvProc];

                    checkReceivedSize(recvProc, map.size(), subField.size());

                    forAll(map, i)
                    {
                        newField[map[i]] = subField[i];
                    }
                }
            }
            else
            {
                // I am receive first, send next
                {
                    IPstream fromNbr(Pstream::scheduled, sendProc, 0, tag);
                    List<T> subField(fromNbr);

                    const labelList& map = constructMap[sendProc];

                    checkReceivedSize(sendProc, map.size(), subField.size());

                    forAll(map, i)
                    {
                        newField[map[i]] = subField[i];
                    }
                }
                {
                    OPstream toNbr(Pstream::scheduled, sendProc, 0, tag);
                    toNbr << UIndirectList<T>(field, subMap[sendProc]);
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
                    toDomain << UIndirectList<T>(field, map);
                }
            }

            // Start receiving. Do not block.
            pBufs.finishedSends(false);

            {
                // Set up 'send' to myself
                const labelList& mySubMap = subMap[Pstream::myProcNo()];
                List<T> mySubField(mySubMap.size());
                forAll(mySubMap, i)
                {
                    mySubField[i] = field[mySubMap[i]];
                }
                // Combine bits. Note that can reuse field storage
                field.setSize(constructSize);
                // Receive sub field from myself
                {
                    const labelList& map = constructMap[Pstream::myProcNo()];

                    forAll(map, i)
                    {
                        field[map[i]] = mySubField[i];
                    }
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

                    forAll(map, i)
                    {
                        field[map[i]] = recvField[i];
                    }
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
                        subField[i] = field[map[i]];
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
                    subField[i] = field[map[i]];
                }
            }


            // Combine bits. Note that can reuse field storage

            field.setSize(constructSize);


            // Receive sub field from myself (sendFields[Pstream::myProcNo()])
            {
                const labelList& map = constructMap[Pstream::myProcNo()];
                const List<T>& subField = sendFields[Pstream::myProcNo()];

                forAll(map, i)
                {
                    field[map[i]] = subField[i];
                }
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

                    forAll(map, i)
                    {
                        field[map[i]] = subField[i];
                    }
                }
            }
        }
    }
    else
    {
        FatalErrorIn("mapDistribute::distribute(..)")
            << "Unknown communication schedule " << commsType
            << abort(FatalError);
    }
}


// Distribute list.
template<class T, class CombineOp>
void CML::mapDistribute::distribute
(
    const Pstream::commsTypes commsType,
    const List<labelPair>& schedule,
    const label constructSize,
    const labelListList& subMap,
    const labelListList& constructMap,
    List<T>& field,
    const CombineOp& cop,
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
            subField[i] = field[mySubMap[i]];
        }

        // Receive sub field from myself (subField)
        const labelList& map = constructMap[Pstream::myProcNo()];

        field.setSize(constructSize);
        field = nullValue;

        forAll(map, i)
        {
            cop(field[map[i]], subField[i]);
        }
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
                toNbr << UIndirectList<T>(field, map);
            }
        }

        // Subset myself
        const labelList& mySubMap = subMap[Pstream::myProcNo()];

        List<T> subField(mySubMap.size());
        forAll(mySubMap, i)
        {
            subField[i] = field[mySubMap[i]];
        }

        // Receive sub field from myself (subField)
        const labelList& map = constructMap[Pstream::myProcNo()];

        field.setSize(constructSize);
        field = nullValue;

        forAll(map, i)
        {
            cop(field[map[i]], subField[i]);
        }

        // Receive sub field from neighbour
        for (label domain = 0; domain < Pstream::nProcs(); domain++)
        {
            const labelList& map = constructMap[domain];

            if (domain != Pstream::myProcNo() && map.size())
            {
                IPstream fromNbr(Pstream::blocking, domain, 0, tag);
                List<T> subField(fromNbr);

                checkReceivedSize(domain, map.size(), subField.size());

                forAll(map, i)
                {
                    cop(field[map[i]], subField[i]);
                }
            }
        }
    }
    else if (commsType == Pstream::scheduled)
    {
        // Need to make sure I don't overwrite field with received data
        // since the data might need to be sent to another processor. So
        // allocate a new field for the results.
        List<T> newField(constructSize, nullValue);

        // Subset myself
        UIndirectList<T> subField(field, subMap[Pstream::myProcNo()]);

        // Receive sub field from myself (subField)
        const labelList& map = constructMap[Pstream::myProcNo()];

        forAll(map, i)
        {
            cop(newField[map[i]], subField[i]);
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
                    OPstream toNbr(Pstream::scheduled, recvProc, 0, tag);
                    toNbr << UIndirectList<T>(field, subMap[recvProc]);
                }
                {
                    IPstream fromNbr(Pstream::scheduled, recvProc, 0, tag);
                    List<T> subField(fromNbr);
                    const labelList& map = constructMap[recvProc];

                    checkReceivedSize(recvProc, map.size(), subField.size());

                    forAll(map, i)
                    {
                        cop(newField[map[i]], subField[i]);
                    }
                }
            }
            else
            {
                // I am receive first, send next
                {
                    IPstream fromNbr(Pstream::scheduled, sendProc, 0, tag);
                    List<T> subField(fromNbr);
                    const labelList& map = constructMap[sendProc];

                    checkReceivedSize(sendProc, map.size(), subField.size());

                    forAll(map, i)
                    {
                        cop(newField[map[i]], subField[i]);
                    }
                }
                {
                    OPstream toNbr(Pstream::scheduled, sendProc, 0, tag);
                    toNbr << UIndirectList<T>(field, subMap[sendProc]);
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
                    toDomain << UIndirectList<T>(field, map);
                }
            }

            // Start receiving. Do not block.
            pBufs.finishedSends(false);

            {
                // Set up 'send' to myself
                List<T> mySubField(field, subMap[Pstream::myProcNo()]);
                // Combine bits. Note that can reuse field storage
                field.setSize(constructSize);
                field = nullValue;
                // Receive sub field from myself
                {
                    const labelList& map = constructMap[Pstream::myProcNo()];

                    forAll(map, i)
                    {
                        cop(field[map[i]], mySubField[i]);
                    }
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

                    forAll(map, i)
                    {
                        cop(field[map[i]], recvField[i]);
                    }
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
                        subField[i] = field[map[i]];
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
                    subField[i] = field[map[i]];
                }
            }


            // Combine bits. Note that can reuse field storage

            field.setSize(constructSize);
            field = nullValue;

            // Receive sub field from myself (subField)
            {
                const labelList& map = constructMap[Pstream::myProcNo()];
                const List<T>& subField = sendFields[Pstream::myProcNo()];

                forAll(map, i)
                {
                    cop(field[map[i]], subField[i]);
                }
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

                    forAll(map, i)
                    {
                        cop(field[map[i]], subField[i]);
                    }
                }
            }
        }
    }
    else
    {
        FatalErrorIn("mapDistribute::distribute(..)")
            << "Unknown communication schedule " << commsType
            << abort(FatalError);
    }
}


template<class T>
void CML::mapDistribute::send(PstreamBuffers& pBufs, const List<T>& field)
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
            toDomain << UIndirectList<T>(field, map);
        }
    }

    // Start sending and receiving but do not block.
    pBufs.finishedSends(false);
}


template<class T>
void CML::mapDistribute::receive(PstreamBuffers& pBufs, List<T>& field) const
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
                FatalErrorIn
                (
                    "template<class T>\n"
                    "void mapDistribute::receive\n"
                    "(\n"
                    "    PstreamBuffers&,\n"
                    "    List<T>&\n"
                    ")\n"
                )   << "Expected from processor " << domain
                    << " " << map.size() << " but received "
                    << recvField.size() << " elements."
                    << abort(FatalError);
            }

            forAll(map, i)
            {
                field[map[i]] = recvField[i];
            }
        }
    }
}


// In case of no transform: copy elements
template<class T>
void CML::mapDistribute::applyDummyTransforms(List<T>& field) const
{
    forAll(transformElements_, trafoI)
    {
        const labelList& elems = transformElements_[trafoI];

        label n = transformStart_[trafoI];

        forAll(elems, i)
        {
            field[n++] = field[elems[i]];
        }
    }
}


// In case of no transform: copy elements
template<class T>
void CML::mapDistribute::applyDummyInverseTransforms(List<T>& field) const
{
    forAll(transformElements_, trafoI)
    {
        const labelList& elems = transformElements_[trafoI];
        label n = transformStart_[trafoI];

        forAll(elems, i)
        {
            field[elems[i]] = field[n++];
        }
    }
}


// Calculate transformed elements.
template<class T, class TransformOp>   //, class CombineOp>
void CML::mapDistribute::applyTransforms
(
    const globalIndexAndTransform& globalTransforms,
    List<T>& field,
    const TransformOp& top
) const
{
    const List<vectorTensorTransform>& totalTransform =
        globalTransforms.transformPermutations();

    forAll(totalTransform, trafoI)
    {
        const vectorTensorTransform& vt = totalTransform[trafoI];
        const labelList& elems = transformElements_[trafoI];
        label n = transformStart_[trafoI];

        // Could be optimised to avoid memory allocations
        List<T> transformFld(UIndirectList<T>(field, elems));
        top(vt, true, transformFld);

        forAll(transformFld, i)
        {
            //cop(field[n++], transformFld[i]);
            field[n++] = transformFld[i];
        }
    }
}


// Calculate transformed elements.
template<class T, class TransformOp>   //, class CombineOp>
void CML::mapDistribute::applyInverseTransforms
(
    const globalIndexAndTransform& globalTransforms,
    List<T>& field,
    const TransformOp& top
) const
{
    const List<vectorTensorTransform>& totalTransform =
        globalTransforms.transformPermutations();

    forAll(totalTransform, trafoI)
    {
        const vectorTensorTransform& vt = totalTransform[trafoI];
        const labelList& elems = transformElements_[trafoI];
        label n = transformStart_[trafoI];

        // Could be optimised to avoid memory allocations
        List<T> transformFld(SubList<T>(field, elems.size(), n));
        top(vt, false, transformFld);

        forAll(transformFld, i)
        {
            //cop(field[elems[i]], transformFld[i]);
            field[elems[i]] = transformFld[i];
        }
    }
}


//- Distribute data using default commsType.
template<class T>
void CML::mapDistribute::distribute
(
    List<T>& fld,
    const bool dummyTransform,
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
            constructMap_,
            fld,
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
            constructMap_,
            fld,
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
            constructMap_,
            fld,
            tag
        );
    }

    //- Fill in transformed slots with copies
    if (dummyTransform)
    {
        applyDummyTransforms(fld);
    }
}


//- Reverse distribute data using default commsType.
template<class T>
void CML::mapDistribute::reverseDistribute
(
    const label constructSize,
    List<T>& fld,
    const bool dummyTransform,
    const int tag
) const
{
    if (dummyTransform)
    {
        applyDummyInverseTransforms(fld);
    }

    if (Pstream::defaultCommsType == Pstream::nonBlocking)
    {
        distribute
        (
            Pstream::nonBlocking,
            List<labelPair>(),
            constructSize,
            constructMap_,
            subMap_,
            fld,
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
            subMap_,
            fld,
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
            subMap_,
            fld,
            tag
        );
    }
}


//- Reverse distribute data using default commsType.
//  Since constructSize might be larger than supplied size supply
//  a nullValue
template<class T>
void CML::mapDistribute::reverseDistribute
(
    const label constructSize,
    const T& nullValue,
    List<T>& fld,
    const bool dummyTransform,
    const int tag
) const
{
    if (dummyTransform)
    {
        applyDummyInverseTransforms(fld);
    }

    if (Pstream::defaultCommsType == Pstream::nonBlocking)
    {
        distribute
        (
            Pstream::nonBlocking,
            List<labelPair>(),
            constructSize,
            constructMap_,
            subMap_,
            fld,
            eqOp<T>(),
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
            subMap_,
            fld,
            eqOp<T>(),
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
            subMap_,
            fld,
            eqOp<T>(),
            nullValue,
            tag
        );
    }
}


//- Distribute data using default commsType.
template<class T, class TransformOp>
void CML::mapDistribute::distribute
(
    const globalIndexAndTransform& git,
    List<T>& fld,
    const TransformOp& top,
    const int tag
) const
{
    // Distribute. Leave out dummy transforms since we're doing them ourselves
    distribute(fld, false, tag);
    // Do transforms
    applyTransforms(git, fld, top);
}


template<class T, class TransformOp>
void CML::mapDistribute::reverseDistribute
(
    const globalIndexAndTransform& git,
    const label constructSize,
    List<T>& fld,
    const TransformOp& top,
    const int tag
) const
{
    // Fill slots with reverse-transformed data. Note that it also copies
    // back into the non-remote part of fld even though these values are not
    // used.
    applyInverseTransforms(git, fld, top);

    // And send back (the remote slots). Disable dummy transformations.
    reverseDistribute(constructSize, fld, false, tag);
}


template<class T, class TransformOp>
void CML::mapDistribute::reverseDistribute
(
    const globalIndexAndTransform& git,
    const label constructSize,
    const T& nullValue,
    List<T>& fld,
    const TransformOp& top,
    const int tag
) const
{
    // Fill slots with reverse-transformed data Note that it also copies
    // back into the non-remote part of fld even though these values are not
    // used.
    applyInverseTransforms(git, fld, top);   //, eqOp<T>());

    // And send back (the remote slots) Disable dummy transformations.
    reverseDistribute(constructSize, nullValue, fld, false, tag);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
