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
    subGraph

Description
    This class provides access to a row of a graph

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles
    subGraphI.H

\*---------------------------------------------------------------------------*/

#ifndef subGraph_HPP
#define subGraph_HPP

#include "bool.hpp"
#include "Ostream.hpp"
#include "error.hpp"
#include "graphRow.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class graphType> class subGraph;
template<class graphType>
Ostream& operator<<(Ostream&, const subGraph<graphType>&);

/*---------------------------------------------------------------------------*\
                           Class subGraph Declaration
\*---------------------------------------------------------------------------*/

template<class graphType>
class subGraph
{
    // Private data
        //- reference to the graph
        graphType& data_;
    
        //- starts at row
        const label start_;
    
        //- number of rows in the subGraph
        const label size_;

    // Private member functions
        //- check index
        inline void checkIndex(const label i) const;
    
public:

    // Constructors

        //- Construct from graph and start row and size
        inline subGraph(graphType&, const label, const label);

        //- Copy contructor
        inline subGraph(const subGraph<graphType>&);

    // Destructor

        inline ~subGraph();

    // Member Functions

        //- Returns the number of rows
        inline label size() const;

        //- Returns the size of a given row
        inline label sizeOfRow(const label rowI) const;

    // Member Operators
        
        //- Append an element to the given row
        inline void append(const label rowI, const label);
        
        //- Append an element to the given row if it does not exist there
        inline void appendIfNotIn(const label rowI, const label);
        
        //- check if the element is in the given row (takes linear time)
        inline bool contains(const label rowI, const label e) const;
        inline label containsAtPosition(const label rowI, const label e) const;
        
        //- set and get operators
        inline label operator()(const label i, const label j) const;
        inline label& operator()(const label i, const label j);
        inline const graphRow<const graphType> operator[](const label) const;
        inline graphRow<graphType> operator[](const label);

    // IOstream operators

        // Write subGraph to Ostream.
        friend Ostream& operator<< <graphType>
        (
            Ostream&,
            const subGraph<graphType>&
        );
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class graphType>
inline void CML::subGraph<graphType>::checkIndex(const label i) const
{
    if( (i < 0) || (i >= size_) )
    {
        FatalErrorInFunction
            << "Row index " << i
            << " is not in range " << CML::label(0)
            << " and " << size_ << abort(FatalError);
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class graphType>
inline CML::subGraph<graphType>::subGraph
(
    graphType& g,
    const label start,
    const label size
)
:
    data_(g),
    start_(start),
    size_(size)
{
}

template<class graphType>
inline CML::subGraph<graphType>::subGraph
(
    const subGraph<graphType>& ol
)
:
    data_(ol.data_),
    start_(ol.start_),
    size_(ol.size_)
{
}

template<class graphType>
inline CML::subGraph<graphType>::~subGraph()
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class graphType>
inline CML::label CML::subGraph<graphType>::size() const
{
    return size_;
}

template<class graphType>
inline CML::label CML::subGraph<graphType>::sizeOfRow(const label rowI) const
{
    return data_.sizeOfRow(start_+rowI);
}

template<class graphType>
inline void CML::subGraph<graphType>::append(const label rowI, const label el)
{
    data_.append(start_+rowI, el);
}

template<class graphType>
inline void CML::subGraph<graphType>::appendIfNotIn
(
    const label rowI,
    const label el
)
{
    data_.appendIfNotIn(start_+rowI, el);
}

template<class graphType>
inline bool CML::subGraph<graphType>::contains
(
    const label rowI,
    const label e
) const
{
    return data_.contains(start_+rowI, e);
}

template<class graphType>
inline CML::label CML::subGraph<graphType>::containsAtPosition
(
    const label rowI,
    const label e
) const
{
    return data_.containsAtPosition(start_+rowI, e);
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class graphType>
inline CML::label CML::subGraph<graphType>::operator()
(
    const label i,
    const label j
) const
{
    # ifdef FULLDEBUG
    checkIndex(i);
    # endif
    
    return data_(start_+i, j);
}

template<class graphType>
inline CML::label& CML::subGraph<graphType>::operator()
(
    const label i,
    const label j
)
{
    return data_(start_+i, j);
}


template<class graphType>
inline const CML::graphRow<const graphType>
CML::subGraph<graphType>::operator[]
(
    const label i
) const
{
    return data_[start_+i];
}

template<class graphType>
inline CML::graphRow<graphType>
CML::subGraph<graphType>::operator[](const label i)
{
    return data_[start_+i];
}

template<class graphType>
inline CML::Ostream& operator<<
(
    CML::Ostream& os,
    const CML::subGraph<graphType>& sg
)
{
    os << sg.size() << "\n" << "(";
    for(CML::label i=0;i<sg.size();++i)
    {
        os << "\n" << sg.sizeOfRow(i) << "(";
        for(CML::label j=0;j<sg.sizeOfRow(i);++j)
        {
            if( j > 0 ) os << " ";
            
            os << sg(i, j);
        }
        
        os << ")";
    }
    
    os << "\n" << ")";

    return os;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
