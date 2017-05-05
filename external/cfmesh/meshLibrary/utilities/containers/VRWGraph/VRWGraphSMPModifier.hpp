/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Class
    VRWGraphModifier

Description
    This class is a modifier for VRWGraph which allows for multi-threaded
    execution of most time-consuimg functions

Author: Franjo Juretic (franjo.juretic@c-fields.com)
    
SourceFiles
    VRWGraphSMPModifier.H
    VRWGraphSMPModifier.C

\*---------------------------------------------------------------------------*/

#ifndef VRWGraphSMPModifier_HPP
#define VRWGraphSMPModifier_HPP

#include "VRWGraph.hpp"
#include "labelPair.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class VRWGraphSMPModifier Declaration
\*---------------------------------------------------------------------------*/

class VRWGraphSMPModifier
{
    // Private data
        //- list containing the data
        VRWGraph& graph_;

    // Private member functions

        //- Disallow default construct
        VRWGraphSMPModifier();

        //- Disallow bitwise copy construct
        VRWGraphSMPModifier(const VRWGraphSMPModifier&);

        //- Disallow bitwise assignment
        void operator=(const VRWGraphSMPModifier&);
    
public:

    // Constructor

        //- Construct from reference to VRWGraph
        VRWGraphSMPModifier(VRWGraph&);

    // Destructor

        ~VRWGraphSMPModifier();

    // Member Functions

        //- set the size and row sizes
        template<class ListType>
        void setSizeAndRowSize(const ListType&);
        
        //- merge graphs with the identical number of rows
        //- into a single one. Use for SMP parallelisation
        void mergeGraphs(const List<VRWGraph>& graphParts);
        
        //- set the graph to the reverse of the original graph.
        //- the rows of such graph store the rows which contain the elements
        //- of the original graph
        template<class GraphType>
        void reverseAddressing(const GraphType& origGraph);
        
        void reverseAddressing(const VRWGraph& origGraph);
        
        //- set the graph to the reverse of the original graph and mapped
        //- to another space.
        template<class ListType, class GraphType>
        void reverseAddressing(const ListType&, const GraphType&);
        
        template<class ListType>
        void reverseAddressing(const ListType&, const VRWGraph&);
        
        //- optimize memory usage
        // this should be used once the graph will not be resized any more
        void optimizeMemoryUsage();

        //- Assignment operator
        void operator=(const VRWGraph&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


# ifdef USE_OMP
#include <omp.h>
# endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class ListType>
void VRWGraphSMPModifier::setSizeAndRowSize(const ListType& s)
{
    graph_.rows_.setSize(s.size());

    # ifdef USE_OMP
    label nThreads = 3 * omp_get_num_procs();
    if( s.size() < 1000 )
        nThreads = 1;
    # else
    const label nThreads(1);
    # endif

    label nEntries(0);
    DynList<label> procEntries;
    procEntries.setSize(nThreads);

    # ifdef USE_OMP
    # pragma omp parallel num_threads(nThreads)
    # endif
    {
        # ifdef USE_OMP
        label& nLocalEntries = procEntries[omp_get_thread_num()];
        # else
        label& nLocalEntries = procEntries[0];
        # endif
        nLocalEntries = 0;

        # ifdef USE_OMP
        # pragma omp for schedule(static)
        # endif
        forAll(s, i)
            nLocalEntries += s[i];

        # ifdef USE_OMP
        # pragma omp critical
        # endif
        nEntries += nLocalEntries;

        # ifdef USE_OMP
        # pragma omp barrier

        # pragma omp master
        # endif
        {
            graph_.data_.setSize(nEntries);
        }

        # ifdef USE_OMP
        # pragma omp barrier
        # endif

        label start(0);
        # ifdef USE_OMP
        for(label i=0;i<omp_get_thread_num();++i)
            start += procEntries[i];
        # endif

        # ifdef USE_OMP
        # pragma omp for schedule(static)
        # endif
        forAll(s, i)
        {
            graph_.rows_[i].start() = start;
            graph_.rows_[i].size() = s[i];
            start += s[i];
        }
    }
}

template<class GraphType>
void VRWGraphSMPModifier::reverseAddressing(const GraphType& origGraph)
{
    graph_.setSize(0);
    labelLongList nAppearances;

    # ifdef USE_OMP
    label nThreads = 3 * omp_get_num_procs();
    if( origGraph.size() < 1000 )
        nThreads = 1;
    # else
    const label nThreads(1);
    # endif

    label minRow(INT_MAX), maxRow(-1);
    List<List<LongList<labelPair> > > dataForOtherThreads(nThreads);

    # ifdef USE_OMP
    # pragma omp parallel num_threads(nThreads)
    # endif
    {
        # ifdef USE_OMP
        const label threadI = omp_get_thread_num();
        # else
        const label threadI(0);
        # endif

        List<LongList<labelPair> >& dot = dataForOtherThreads[threadI];
        dot.setSize(nThreads);

        //- find min and max entry in the graph
        //- they are used for assigning ranges of values local for each process
        label localMinRow(INT_MAX), localMaxRow(-1);
        # ifdef USE_OMP
        # pragma omp for schedule(static)
        # endif
        forAll(origGraph, rowI)
        {
            forAll(origGraph[rowI], i)
            {
                const label entryI = origGraph[rowI][i];
                localMaxRow = CML::max(localMaxRow, entryI);
                localMinRow = CML::min(localMinRow, entryI);
            }
        }

        ++localMaxRow;

        # ifdef USE_OMP
        # pragma omp critical
        # endif
        {
            minRow = CML::min(minRow, localMinRow);
            maxRow = CML::max(maxRow, localMaxRow);

            nAppearances.setSize(maxRow);
        }

        # ifdef USE_OMP
        # pragma omp barrier

        //- initialise appearances
        # pragma omp for schedule(static)
        # endif
        for(label i=0;i<maxRow;++i)
            nAppearances[i] = 0;

        # ifdef USE_OMP
        # pragma omp barrier
        # endif

        const label range = (maxRow - minRow) / nThreads + 1;
        const label localMin = minRow + threadI * range;
        const label localMax = CML::min(localMin + range, maxRow);

        //- find the number of appearances of each element in the original graph
        # ifdef USE_OMP
        # pragma omp for schedule(static)
        # endif
        forAll(origGraph, rowI)
        {
            forAll(origGraph[rowI], j)
            {
                const label entryI = origGraph[rowI][j];

                const label threadNo = (entryI - minRow) / range;

                if( threadNo == threadI )
                {
                    ++nAppearances[entryI];
                }
                else
                {
                    dot[threadNo].append(labelPair(entryI, rowI));
                }
            }
        }

        # ifdef USE_OMP
        # pragma omp barrier
        # endif

        //- count the appearances which are not local to the processor
        for(label i=0;i<nThreads;++i)
        {
            const LongList<labelPair>& data =
                dataForOtherThreads[i][threadI];

            forAll(data, j)
                ++nAppearances[data[j].first()];
        }

        # ifdef USE_OMP
        # pragma omp barrier

        //- allocate graph
        # pragma omp master
        # endif
        setSizeAndRowSize(nAppearances);

        # ifdef USE_OMP
        # pragma omp barrier
        # endif

        for(label i=localMin;i<localMax;++i)
        {
            nAppearances[i] = 0;
        }

        //- start filling reverse addressing graph
        //- update data from processors with smaller labels
        for(label i=0;i<threadI;++i)
        {
            const LongList<labelPair>& data =
                dataForOtherThreads[i][threadI];

            forAll(data, j)
            {
                const label entryI = data[j].first();
                graph_(entryI, nAppearances[entryI]++) = data[j].second();
            }
        }

        //- update data local to the processor
        # ifdef USE_OMP
        # pragma omp for schedule(static)
        # endif
        forAll(origGraph, rowI)
        {
            forAll(origGraph[rowI], j)
            {
                const label entryI = origGraph[rowI][j];

                if( (entryI >= localMin) && (entryI < localMax) )
                    graph_(entryI, nAppearances[entryI]++) = rowI;
            }
        }

        //- update data from the processors with higher labels
        for(label i=threadI+1;i<nThreads;++i)
        {
            const LongList<labelPair>& data =
                dataForOtherThreads[i][threadI];

            forAll(data, j)
            {
                const label entryI = data[j].first();
                graph_(entryI, nAppearances[entryI]++) = data[j].second();
            }
        }
    }
}

template<class ListType, class GraphType>
void VRWGraphSMPModifier::reverseAddressing
(
    const ListType& mapper,
    const GraphType& origGraph
)
{
    ListType nAppearances;

    # ifdef USE_OMP
    label nThreads = 3 * omp_get_num_procs();
    if( origGraph.size() < 1000 )
        nThreads = 1;
    # else
    const label nThreads(1);
    # endif

    label minRow(INT_MAX), maxRow(-1);
    List<List<LongList<labelPair> > > dataForOtherThreads(nThreads);

    # ifdef USE_OMP
    # pragma omp parallel num_threads(nThreads)
    # endif
    {
        # ifdef USE_OMP
        const label threadI = omp_get_thread_num();
        # else
        const label threadI(0);
        # endif

        List<LongList<labelPair> >& dot = dataForOtherThreads[threadI];
        dot.setSize(nThreads);

        //- find min and max entry in the graph
        //- they are used for assigning ranges of values local for each process
        label localMinRow(INT_MAX), localMaxRow(-1);
        # ifdef USE_OMP
        # pragma omp for schedule(static)
        # endif
        forAll(origGraph, rowI)
        {
            forAll(origGraph[rowI], i)
            {
                const label entryI = mapper[origGraph[rowI][i]];
                localMaxRow = CML::max(localMaxRow, entryI);
                localMinRow = CML::min(localMinRow, entryI);
            }
        }

        ++localMaxRow;

        # ifdef USE_OMP
        # pragma omp critical
        # endif
        {
            minRow = CML::min(minRow, localMinRow);
            maxRow = CML::max(maxRow, localMaxRow);
            nAppearances.setSize(maxRow);
        }

        # ifdef USE_OMP
        # pragma omp barrier

        //- initialise appearances
        # pragma omp for schedule(static)
        # endif
        for(label i=0;i<maxRow;++i)
            nAppearances[i] = 0;

        # ifdef USE_OMP
        # pragma omp barrier
        # endif

        const label range = (maxRow - minRow) / nThreads + 1;
        const label localMin = minRow + threadI * range;
        const label localMax = CML::min(localMin + range, maxRow);

        //- find the number of appearances of each element in the original graph
        # ifdef USE_OMP
        # pragma omp for schedule(static)
        # endif
        forAll(origGraph, rowI)
        {
            forAll(origGraph[rowI], i)
            {
                const label entryI = mapper[origGraph[rowI][i]];

                const label threadNo = (entryI - minRow) / range;

                if( threadNo == threadI )
                {
                    ++nAppearances[entryI];
                }
                else
                {
                    dot[threadNo].append(labelPair(entryI, rowI));
                }
            }
        }

        # ifdef USE_OMP
        # pragma omp barrier
        # endif

        //- count the appearances which are not local to the processor
        for(label i=0;i<nThreads;++i)
        {
            const LongList<labelPair>& data =
                dataForOtherThreads[i][threadI];

            forAll(data, j)
                ++nAppearances[data[j].first()];
        }

        # ifdef USE_OMP
        # pragma omp barrier

        //- allocate graph
        # pragma omp master
        # endif
        {
            setSizeAndRowSize(nAppearances);
        }

        # ifdef USE_OMP
        # pragma omp barrier
        # endif

        for(label i=localMin;i<localMax;++i)
        {
            nAppearances[i] = 0;
        }

        //- start filling reverse addressing graph
        //- update data from processors with smaller labels
        for(label i=0;i<threadI;++i)
        {
            const LongList<labelPair>& data =
                dataForOtherThreads[i][threadI];

            forAll(data, j)
            {
                const label entryI = data[j].first();
                graph_(entryI, nAppearances[entryI]++) = data[j].second();
            }
        }

        //- update data local to the processor
        # ifdef USE_OMP
        # pragma omp for schedule(static)
        # endif
        forAll(origGraph, rowI)
        {
            forAll(origGraph[rowI], j)
            {
                const label entryI = mapper[origGraph[rowI][j]];

                if( (entryI >= localMin) && (entryI < localMax) )
                    graph_(entryI, nAppearances[entryI]++) = rowI;
            }
        }

        //- update data from the processors with higher labels
        for(label i=threadI+1;i<nThreads;++i)
        {
            const LongList<labelPair>& data =
                dataForOtherThreads[i][threadI];

            forAll(data, j)
            {
                const label entryI = data[j].first();
                graph_(entryI, nAppearances[entryI]++) = data[j].second();
            }
        }
    }
}

template<class ListType>
void VRWGraphSMPModifier::reverseAddressing
(
    const ListType& mapper,
    const VRWGraph& origGraph
)
{
    ListType nAppearances;

    # ifdef USE_OMP
    label nThreads = 3 * omp_get_num_procs();
    if( origGraph.size() < 1000 )
        nThreads = 1;
    # else
    const label nThreads(1);
    # endif

    label minRow(INT_MAX), maxRow(-1);
    List<List<LongList<labelPair> > > dataForOtherThreads(nThreads);

    # ifdef USE_OMP
    # pragma omp parallel num_threads(nThreads)
    # endif
    {
        # ifdef USE_OMP
        const label threadI = omp_get_thread_num();
        # else
        const label threadI(0);
        # endif

        List<LongList<labelPair> >& dot = dataForOtherThreads[threadI];
        dot.setSize(nThreads);

        //- find min and max entry in the graph
        //- they are used for assigning ranges of values local for each process
        label localMinRow(INT_MAX), localMaxRow(-1);
        # ifdef USE_OMP
        # pragma omp for schedule(static)
        # endif
        forAll(origGraph, rowI)
        {
            forAllRow(origGraph, rowI, i)
            {
                const label entryI = mapper[origGraph(rowI, i)];
                localMaxRow = CML::max(localMaxRow, entryI);
                localMinRow = CML::min(localMinRow, entryI);
            }
        }

        ++localMaxRow;

        # ifdef USE_OMP
        # pragma omp critical
        # endif
        {
            minRow = CML::min(minRow, localMinRow);
            maxRow = CML::max(maxRow, localMaxRow);
            nAppearances.setSize(maxRow);
        }

        # ifdef USE_OMP
        # pragma omp barrier

        //- initialise appearances
        # pragma omp for schedule(static)
        # endif
        for(label i=0;i<maxRow;++i)
            nAppearances[i] = 0;

        # ifdef USE_OMP
        # pragma omp barrier
        # endif

        const label range = (maxRow - minRow) / nThreads + 1;
        const label localMin = minRow + threadI * range;
        const label localMax = CML::min(localMin + range, maxRow);

        //- find the number of appearances of each element in the original graph
        # ifdef USE_OMP
        # pragma omp for schedule(static)
        # endif
        forAll(origGraph, rowI)
        {
            forAllRow(origGraph, rowI, i)
            {
                const label entryI = mapper[origGraph(rowI, i)];

                const label threadNo = (entryI - minRow) / range;

                if( threadNo == threadI )
                {
                    ++nAppearances[entryI];
                }
                else
                {
                    dot[threadNo].append(labelPair(entryI, rowI));
                }
            }
        }

        # ifdef USE_OMP
        # pragma omp barrier
        # endif

        //- count the appearances which are not local to the processor
        for(label i=0;i<nThreads;++i)
        {
            const LongList<labelPair>& data =
                dataForOtherThreads[i][threadI];

            forAll(data, j)
                ++nAppearances[data[j].first()];
        }

        # ifdef USE_OMP
        # pragma omp barrier

        //- allocate graph
        # pragma omp master
        # endif
        {
            setSizeAndRowSize(nAppearances);
        }

        # ifdef USE_OMP
        # pragma omp barrier
        # endif
        for(label i=localMin;i<localMax;++i)
            nAppearances[i] = 0;

        //- start filling reverse addressing graph
        //- update data from processors with smaller labels
        for(label i=0;i<threadI;++i)
        {
            const LongList<labelPair>& data =
                dataForOtherThreads[i][threadI];

            forAll(data, j)
            {
                const label entryI = data[j].first();
                graph_(entryI, nAppearances[entryI]++) = data[j].second();
            }
        }

        //- update data local to the processor
        # ifdef USE_OMP
        # pragma omp for schedule(static)
        # endif
        forAll(origGraph, rowI)
        {
            forAllRow(origGraph, rowI, j)
            {
                const label entryI = mapper[origGraph(rowI, j)];

                if( (entryI >= localMin) && (entryI < localMax) )
                    graph_(entryI, nAppearances[entryI]++) = rowI;
            }
        }

        //- update data from the processors with higher labels
        for(label i=threadI+1;i<nThreads;++i)
        {
            const LongList<labelPair>& data =
                dataForOtherThreads[i][threadI];

            forAll(data, j)
            {
                const label entryI = data[j].first();
                graph_(entryI, nAppearances[entryI]++) = data[j].second();
            }
        }
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
