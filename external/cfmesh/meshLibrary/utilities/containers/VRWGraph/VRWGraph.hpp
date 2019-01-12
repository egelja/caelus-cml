/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
Copyright (C) 2018 Applied CCM Pty Ltd
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
    VRWGraph

Description
    This class is an implementation of a graph with variable column width.
    The imoplementation is memory efficient.

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    VRWGraph.cpp

\*---------------------------------------------------------------------------*/

#ifndef VRWGraph_HPP
#define VRWGraph_HPP

#include "labelLongList.hpp"
#include "graphRow.hpp"
#include "DynList.hpp"
#include "bool.hpp"
#include "error.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class VRWGraphModifier;
class VRWGraph;

Ostream& operator<<(Ostream&, const VRWGraph&);


class rowElement
{
    // Private data
        //- starting element of the row
        label start_;

        //- number of elements in the row
        label size_;

    public:

        inline rowElement()
        :
            start_(),
            size_()
        {}

        inline rowElement(const label i, const label j)
        :
            start_(i),
            size_(j)
        {}

        inline ~rowElement()
        {}

        inline label start() const
        {
            return start_;
        }
        inline label& start()
        {
            return start_;
        }

        inline label size() const
        {
            return size_;
        }
        inline label& size()
        {
            return size_;
        }
};

/*---------------------------------------------------------------------------*\
                           Class VRWGraph Declaration
\*---------------------------------------------------------------------------*/

class VRWGraph
{
    // Private data
        //- list containing data
        labelLongList data_;

        //- number of rows
        LongList<rowElement> rows_;

    // Private member functions
        //- check index
        inline void checkIndex(const label i, const label j) const;

    // Enumerators
        enum typeOfEntries
        {
            NONE = 0,
            INVALIDROW=-10,
            FREEENTRY=-11,
            FREESTART=-12
        };

public:

    // Friend classes

        friend class VRWGraphSMPModifier;

    // Constructors

        //- Construct null
        inline VRWGraph();

        //- Construct given number of rows
        explicit inline VRWGraph(const label size);

        //- Construct given number of rows and row size
        explicit inline VRWGraph
        (
            const label nRows,
            const label nColumnsInRow
        );

        //- Construct to given number of rows, row size and initialize
        explicit inline VRWGraph
        (
            const label nRows,
            const label nColumnsInRow,
            const label t
        );

        //- Copy contructor
        inline VRWGraph(const VRWGraph&);

    // Destructor

        inline ~VRWGraph();

    // Member Functions

        // Access

            //- Returns the number of rows
            inline label size() const;

            //- Returns the number of elements in the given row
            inline label sizeOfRow(const label rowI) const;

        // Edit

            //- Reset the number of rows
            inline void setSize(const label);

            //- Reset the number of rows. The second argument specifies
            //- the reserved column width
            inline void setSizeAndColumnWidth
            (
                const label newNumRows,
                const label rcWidth
            );

            //- Set the number of rows and the size of each row
            template<class ListType>
            inline void setSizeAndRowSize(const ListType&);

            //- Reset the size of the given row
            inline void setRowSize(const label rowI, const label newSize);

            //- Clear the graph
            inline void clear();

    // Member Operators

        //- Append a list as a row at the end of the graph
        template<class ListType>
        inline void appendList(const ListType& l);

        //- Append an element to the given row
        inline void append(const label rowI, const label);

        //- Append an element to the given row if it does not exist there
        inline void appendIfNotIn(const label rowI, const label);

        //- Set row with the list
        template<class ListType>
        inline void setRow(const label rowI, const ListType& l);

        //- merge graphs with the identical number of rows
        //- into a single one. Use for SMP parallelisation
        inline void mergeGraphs(const List<VRWGraph>& graphParts);

        //- set the graph to the reverse of the original graph.
        //- the rows of such graph store the rows which contain the elements
        //- of the original graph
        template<class GraphType>
        inline void reverseAddressing
        (
            const label nRows,
            const GraphType& origGraph
        );

        template<class GraphType>
        inline void reverseAddressing(const GraphType& origGraph);

        inline void reverseAddressing
        (
            const label nRows,
            const VRWGraph& origGraph
        );

        inline void reverseAddressing(const VRWGraph& origGraph);

        //- optimize memory usage
        // this should be used once the graph will not be resized any more
        void optimizeMemoryUsage();

        //- check if the element is in the given row (takes linear time)
        inline bool contains(const label rowI, const label e) const;
        inline label containsAtPosition(const label rowI, const label e) const;

        //- get and set operators
        inline label operator()(const label i, const label j) const;
        inline label& operator()(const label i, const label j);

        inline constRow operator[](const label i) const;
        inline row operator[](const label i);

        //- Assignment operator
        inline void operator=(const VRWGraph&);


    // IOstream operators

        // Write VRWGraph to Ostream.
        friend Ostream& operator<<(Ostream&, const VRWGraph&);

        //- Read from Istream, discarding contents of existing VRWGraph.
/*        friend Istream& operator>> <T, width>
        (
            Istream&,
            VRWGraph<T, width>&
        );
*/
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


inline void CML::VRWGraph::checkIndex(const label i, const label j) const
{
    if( (i < 0) || (i >= rows_.size()) )
    {
        FatalErrorInFunction
            << "Row index " << i
            << " is not in range " << 0
            << " and " << rows_.size() << abort(FatalError);
    }

    if( (j < 0) || (j >= rows_[i].size()) )
        FatalErrorInFunction
            << "Column index " << j
            << " is not in range " << 0
            << " and " << rows_[i].size() << abort(FatalError);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct null
inline CML::VRWGraph::VRWGraph()
:
    data_(),
    rows_()
{}

//- Construct given size
inline CML::VRWGraph::VRWGraph
(
    const label size
)
:
    data_(),
    rows_(size)
{
    for(label rowI=0;rowI<size;++rowI)
    {
        rows_[rowI].start() = INVALIDROW;
        rows_[rowI].size() = NONE;
    }
}

inline CML::VRWGraph::VRWGraph
(
    const label nRows,
    const label nColumnsInRow
)
:
    data_(nRows * nColumnsInRow),
    rows_(nRows)
{
    for(label rowI=0;rowI<nRows;++rowI)
    {
        rows_[rowI].start() = rowI * nColumnsInRow;
        rows_[rowI].size() = nColumnsInRow;
    }
}

inline CML::VRWGraph::VRWGraph
(
    const label nRows,
    const label nColumnsInRow,
    const label t
)
:
    data_(nRows * nColumnsInRow, t),
    rows_(nRows)
{
    for(label rowI=0;rowI<nRows;++rowI)
    {
        rows_[rowI].start() = rowI * nColumnsInRow;
        rows_[rowI].size() = nColumnsInRow;
    }
}

inline CML::VRWGraph::VRWGraph
(
    const VRWGraph& ol
)
:
    data_(ol.data_),
    rows_(ol.rows_)
{}

inline CML::VRWGraph::~VRWGraph()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::VRWGraph::size() const
{
    return rows_.size();
}

inline CML::label CML::VRWGraph::sizeOfRow(const label rowI) const
{
    return rows_[rowI].size();
}

inline void CML::VRWGraph::setSize(const label size)
{
    if( size > rows_.size() )
    {
        rowElement rowInfo(INVALIDROW, NONE);

        for(label i=rows_.size();i<size;++i)
            rows_.append(rowInfo);
    }
    else
    {
        rows_.setSize(size);
    }
}

void CML::VRWGraph::setSizeAndColumnWidth
(
    const label newNumRows,
    const label rcWidth
)
{
    if( rows_.size() != 0 )
        FatalErrorInFunction
            << "This function should be used for empty graphs, only!"
            << exit(FatalError);

    data_.setSize(newNumRows * rcWidth);
    data_ = FREEENTRY;

    rows_.setSize(newNumRows);
    label start(0);

    for(label i=0;i<newNumRows;++i)
    {
        rows_[i].start() = start;
        rows_[i].size() = 0;
        data_[start] = FREESTART;

        start += rcWidth;
    }
}

template<class ListType>
inline void CML::VRWGraph::setSizeAndRowSize(const ListType& l)
{
    //- set the size of graph rows
    const label nRows = l.size();
    rows_.setSize(nRows);

    label start(0);
    for(label rowI=0;rowI<nRows;++rowI)
    {
        rows_[rowI].size() = l[rowI];

        if( rows_[rowI].size() != NONE )
        {
            rows_[rowI].start() = start;
        }
        else
        {
            rows_[rowI].start() = INVALIDROW;
        }

        start += rows_[rowI].size();
    }

    data_.setSize(start);
}

inline void CML::VRWGraph::setRowSize(const label rowI, const label newSize)
{
    # ifdef FULLDEBUG
    if( (rowI < 0) || (rowI >= rows_.size()) )
        FatalErrorInFunction
            << "Row index " << CML::label(rowI)
            << " is not in range " << CML::label(0)
            << " and " << rows_.size() << abort(FatalError);
    # endif

    const label start = rows_[rowI].start();
    if( start == INVALIDROW )
    {
        if( newSize > 0 )
        {
            rows_[rowI].start() = data_.size();
            for(label i=0;i<newSize;++i)
                data_.append(NONE);
            rows_[rowI].size() = newSize;
        }
    }
    else if( newSize > rows_[rowI].size() )
    {
        //- check if there is some unused space after the last element
        bool foundUnused(true);

        for(label i=rows_[rowI].size();i<newSize;++i)
        {
            const label j = start + i;
            if(
                (j >= data_.size()) ||
                (data_[j] != FREEENTRY) ||
                (data_[j] == FREESTART)
            )
            {
                foundUnused = false;
                break;
            }
        }

        if( foundUnused )
        {
            //- row can be extended without copying
            for(label i=rows_[rowI].size();i<newSize;++i)
                data_[start+i] = NONE;
        }
        else
        {
            //- row is copied at the end of the data list
            rows_[rowI].start() = data_.size();
            for(label i=0;i<rows_[rowI].size();++i)
            {
                data_.append(data_[start+i]);
                data_[start+i] = FREEENTRY;
            }
            for(label i=rows_[rowI].size();i<newSize;++i)
                data_.append(NONE);
        }

        rows_[rowI].size() = newSize;
    }
    else if( newSize < rows_[rowI].size() )
    {
        for(label i=newSize;i<rows_[rowI].size();++i)
            data_[start+i] = FREEENTRY;
        rows_[rowI].size() = newSize;
        if( newSize == 0 )
            rows_[rowI].start() = INVALIDROW;
    }
}

inline void CML::VRWGraph::clear()
{
    data_.setSize(0);
    rows_.setSize(0);
}

template<class ListType>
inline void CML::VRWGraph::appendList
(
    const ListType& l
)
{
    if( l.size() == 0 )
    {
        rows_.append(rowElement(INVALIDROW, 0));
        return;
    }

    rowElement rowInfo(data_.size(), l.size());
    const label size = l.size();
    for(label elI=0;elI<size;++elI)
        data_.append(l[elI]);
    rows_.append(rowInfo);
}

inline void CML::VRWGraph::append(const label rowI, const label el)
{
    rowElement& re = rows_[rowI];

    if( re.start() == INVALIDROW )
    {
        re.start() = data_.size();
        re.size() = 1;
        data_.append(el);
    }
    else
    {
        const label oldStart = re.start();
        const label oldSize = re.size();
         ++re.size();

        if( oldStart + oldSize < data_.size() )
        {
            if(
                (data_[oldStart+oldSize] == FREEENTRY) ||
                (data_[oldStart+oldSize] == FREESTART)
            )
            {
                data_[oldStart + oldSize] = el;
            }
            else
            {
                re.start() = data_.size();
                for(label i=0;i<oldSize;++i)
                {
                    data_.append(data_[oldStart+i]);
                    data_[oldStart+i] = FREEENTRY;
                }
                data_.append(el);
            }
        }
        else
        {
            data_.append(el);
        }
    }
}

inline void CML::VRWGraph::appendIfNotIn(const label rowI, const label el)
{
    if( !contains(rowI, el) )
        append(rowI, el);
}

template<class ListType>
inline void CML::VRWGraph::setRow
(
    const label rowI,
    const ListType& l
)
{
    this->setRowSize(rowI, l.size());
    const label start = rows_[rowI].start();
    const label size = l.size();
    for(label elI=0;elI<size;++elI)
        data_[start+elI] = l[elI];
}

inline void CML::VRWGraph::mergeGraphs(const List<VRWGraph>& graphParts)
{
    const label nGraphs = graphParts.size();
    const label nRows = graphParts[0].size();
    forAll(graphParts, i)
    {
        if( nRows != graphParts[i].size() )
            FatalErrorInFunction
                << "Cannot merge graphs" << abort(FatalError);
    }

    //- find the number of elements in each row
    labelLongList nElmtsInRow(nRows);
    for(label rowI=0;rowI<nRows;++rowI)
    {
        label sum(0);
        for(label i=0;i<nGraphs;++i)
            sum += graphParts[i].sizeOfRow(rowI);

        nElmtsInRow[rowI] = sum;
    }

    setSizeAndRowSize(nElmtsInRow);

    //- Finally, assemble the merged graph
    for(label rowI=0;rowI<nRows;++rowI)
    {
        forAll(graphParts, i)
        {
            const VRWGraph& gp = graphParts[i];
            for(label j=0;j<gp.sizeOfRow(rowI);++j)
                this->operator()(rowI, --nElmtsInRow[rowI]) = gp(rowI, j);
        }
    }
}

template<class GraphType>
inline void CML::VRWGraph::reverseAddressing
(
    const label nRows,
    const GraphType& origGraph
)
{
    const label origSize = origGraph.size();
    labelLongList nElmtsInRow(nRows);

    for(label rowI=0;rowI<nRows;++rowI)
        nElmtsInRow[rowI] = 0;

    for(label rowI=0;rowI<origSize;++rowI)
    {
        const label rowSize = origGraph[rowI].size();

        for(label i=0;i<rowSize;++i)
            ++nElmtsInRow[origGraph[rowI][i]];
    }

    setSizeAndRowSize(nElmtsInRow);
    nElmtsInRow = 0;

    //- finally fill in the data
    for(label rowI=0;rowI<origSize;++rowI)
    {
        const label rowSize = origGraph[rowI].size();

        for(label i=0;i<rowSize;++i)
        {
            const label el = origGraph[rowI][i];
            this->operator()(el, nElmtsInRow[el]++) = rowI;
        }
    }
}

template<class GraphType>
inline void CML::VRWGraph::reverseAddressing(const GraphType& origGraph)
{
    const label size = origGraph.size();
    label maxValue(-1);

    for(label rowI=0;rowI<size;++rowI)
    {
        const label rowSize = origGraph[rowI].size();
        for(label i=0;i<rowSize;++i)
            maxValue = CML::max(maxValue, origGraph[rowI][i]);
    }

    ++maxValue;
    reverseAddressing(maxValue, origGraph);
}

inline void CML::VRWGraph::reverseAddressing
(
    const label nRows,
    const VRWGraph& origGraph
)
{
    const label origSize = origGraph.size();
    labelLongList nElmtsInRow(nRows);

    for(label rowI=0;rowI<nRows;++rowI)
        nElmtsInRow[rowI] = 0;

    for(label rowI=0;rowI<origSize;++rowI)
    {
        const label rowSize = origGraph.sizeOfRow(rowI);

        for(label i=0;i<rowSize;++i)
            ++nElmtsInRow[origGraph(rowI, i)];
    }

    setSizeAndRowSize(nElmtsInRow);
    nElmtsInRow = 0;

    //- finally fill in the data
    for(label rowI=0;rowI<origSize;++rowI)
    {
        const label rowSize = origGraph.sizeOfRow(rowI);

        for(label i=0;i<rowSize;++i)
        {
            const label el = origGraph(rowI, i);
            this->operator()(el, nElmtsInRow[el]++) = rowI;
        }
    }
}

inline void CML::VRWGraph::reverseAddressing(const VRWGraph& origGraph)
{
    const label size = origGraph.size();
    label maxValue(-1);

    for(label rowI=0;rowI<size;++rowI)
    {
        const label rowSize = origGraph.sizeOfRow(rowI);
        for(label i=0;i<rowSize;++i)
            maxValue = CML::max(maxValue, origGraph(rowI, i));
    }

    ++maxValue;
    reverseAddressing(maxValue, origGraph);
}

inline bool CML::VRWGraph::contains
(
    const label rowI,
    const label e
) const
{
    const label start = rows_[rowI].start();
    if( start == INVALIDROW )
        return false;
    const label size = rows_[rowI].size();

    for(label i=0;i<size;++i)
        if( data_[start+i] == e )
            return true;

    return false;
}

inline CML::label CML::VRWGraph::containsAtPosition
(
    const label rowI,
    const label e
) const
{
    const label start = rows_[rowI].start();
    if( start == INVALIDROW )
        return -1;

    const label size = rows_[rowI].size();

    for(label i=0;i<size;++i)
        if( data_[start+i] == e )
            return i;

    return -1;
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline CML::label CML::VRWGraph::operator()
(
    const label i,
    const label j
) const
{
    #ifdef FULLDEBUG
    checkIndex(i, j);
    #endif

    return data_[rows_[i].start() + j];
}


inline CML::label& CML::VRWGraph::operator()
(
    const label i, const label j
)
{
    #ifdef FULLDEBUG
    checkIndex(i, j);
    #endif

    return data_[rows_[i].start() + j];
}

inline CML::constRow CML::VRWGraph::operator[](const label i) const
{
    return constRow(*this, i);
}

inline CML::row CML::VRWGraph::operator[](const label i)
{
    return row(*this, i);
}

inline void CML::VRWGraph::operator=
(
    const VRWGraph& l
)
{
    data_ = l.data_;
    rows_ = l.rows_;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define forAllRow(graph, rowI, index) \
    for(CML::label index=0;index<(graph).sizeOfRow(rowI);++index)

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
