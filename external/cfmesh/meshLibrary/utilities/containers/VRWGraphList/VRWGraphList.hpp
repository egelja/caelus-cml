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
    VRWGraphList

Description
    This class is an implementation of a list of graphs
    with variable column width. The implementation is memory efficient.

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    VRWGraphList.C

\*---------------------------------------------------------------------------*/

#ifndef VRWGraphList_HPP
#define VRWGraphList_HPP

#include "VRWGraph.hpp"
#include "subGraph.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class VRWGraphList;

Ostream& operator<<(Ostream&, const VRWGraphList&);

/*---------------------------------------------------------------------------*\
                           Class VRWGraphList Declaration
\*---------------------------------------------------------------------------*/

class VRWGraphList
{
        
    // Private data
        //- graph containing the data
        VRWGraph data_;
    
        //- number of rows
        LongList<rowElement> rows_;

    // Private member functions
        //- check index
        inline void checkIndex
        (
            const label i,
            const label j,
            const label k
        ) const;
    
public:

    // Constructors

        //- Construct null
        inline VRWGraphList();

        //- Copy contructor
        inline VRWGraphList(const VRWGraphList&);

    // Destructor

        inline ~VRWGraphList();

    // Member Functions

        // Access

            //- Returns the number of graphs
            inline label size() const;
            
            //- Returns the number of rows in the graph at that position
            inline label sizeOfGraph(const label posI) const;
            
            //- Return the number of element in the row at the given position
            inline label sizeOfRow(const label posI, const label rowI) const;

            //- Clear the graph
            inline void clear();

    // Member Operators

        //- Append a graph at the end of the graphList
        template<class GraphType>
        inline void appendGraph(const GraphType& l);
        
        //- get and set operators
        inline label operator()
        (
            const label i,
            const label j,
            const label k
        ) const;
        
        inline label& operator()(const label i, const label j, const label k);
        
        inline const subGraph<const VRWGraph> operator[](const label i) const;
        
        //- Assignment operator
        inline void operator=(const VRWGraphList&);


    // IOstream operators

        // Write VRWGraphList to Ostream.
        friend Ostream& operator<<(Ostream&, const VRWGraphList&);

        //- Read from Istream, discarding contents of existing VRWGraphList.
/*        friend Istream& operator>> <T, width>
        (
            Istream&,
            VRWGraphList<T, width>&
        );
*/
};

    
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline void CML::VRWGraphList::checkIndex
(
    const label i,
    const label j,
    const label k
) const
{
    if( (i < 0) || (i >= rows_.size()) )
    {
        FatalErrorInFunction
            << "Position index " << CML::label(i)
            << " is not in range " << CML::label(0)
            << " and " << rows_.size() << abort(FatalError);
    }
    
    if( (j < 0) || (j >= rows_[i].size()) )
        FatalErrorInFunction
            << "Row index " << CML::label(j)
            << " is not in range " << CML::label(0)
            << " and " << rows_[i].size() << abort(FatalError);
    
    if( (k < 0) || (k >= data_.sizeOfRow(rows_[i].start()+j)) )
        FatalErrorInFunction
            << "Data index " << CML::label(k)
            << " is not in range " << CML::label(0)
            << " and " << data_.sizeOfRow(rows_[i].start()+j)
            << abort(FatalError);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct null
inline CML::VRWGraphList::VRWGraphList()
:
    data_(),
    rows_()
{
}

inline CML::VRWGraphList::VRWGraphList
(
    const VRWGraphList& ol
)
:
    data_(ol.data_),
    rows_(ol.rows_)
{
}

inline CML::VRWGraphList::~VRWGraphList()
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::VRWGraphList::size() const
{
    return rows_.size();
}

inline CML::label CML::VRWGraphList::sizeOfGraph(const label posI) const
{
    return rows_[posI].size();
}

inline CML::label CML::VRWGraphList::sizeOfRow
(
    const label posI,
    const label rowI
) const
{
    return data_.sizeOfRow(rows_[posI].start()+rowI);
}

inline void CML::VRWGraphList::clear()
{
    data_.setSize(0);
    rows_.setSize(0);
}

template<class GraphType>
inline void CML::VRWGraphList::appendGraph
(
    const GraphType& l
)
{
    rowElement re(data_.size(), l.size());
    
    for(label i=0;i<l.size();++i)
        data_.appendList(l[i]);
    
    rows_.append(re);
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline CML::label CML::VRWGraphList::operator()
(
    const label i,
    const label j,
    const label k
) const
{
    #ifdef FULLDEBUG
    checkIndex(i, j, k);
    #endif
        
    return data_(rows_[i].start() + j, k);
}


inline CML::label& CML::VRWGraphList::operator()
(
    const label i, const label j, const label k
)
{
    #ifdef FULLDEBUG
    checkIndex(i, j, k);
    #endif
        
    return data_(rows_[i].start() + j, k);
}

inline const CML::subGraph<const VRWGraph> CML::VRWGraphList::operator[]
(
    const label i
) const
{
    return CML::subGraph<const VRWGraph>(data_, i, rows_[i].size());
}

inline void CML::VRWGraphList::operator=(const VRWGraphList& l)
{
    data_ = l.data_;
    rows_ = l.rows_;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


#endif

// ************************************************************************* //
