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
    graphRow

Description
    This class provides access to a row of a graph

Author: Franjo Juretic (franjo.juretic@c-fields.com)


\*---------------------------------------------------------------------------*/

#ifndef graphRow_HPP
#define graphRow_HPP

#include "bool.hpp"
#include "Ostream.hpp"
#include "error.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    
class VRWGraph;

template<class graphType> class graphRow;
template<class graphType>
Ostream& operator<<(Ostream&, const graphRow<graphType>&);

/*---------------------------------------------------------------------------*\
                           Class graphRow Declaration
\*---------------------------------------------------------------------------*/

template<class graphType>
class graphRow
{
    // Private data
        //- reference to the graph
        graphType& data_;
    
        //- row number
        const label rowI_;

    // Private member functions
        //- check index
        inline void checkIndex(const label i) const;
    
public:

    // Constructors

        //- Construct from graph and row number
        inline graphRow(graphType&, const label);

        //- Copy contructor
        inline graphRow(const graphRow<graphType>&);

    // Destructor

        inline ~graphRow();

    // Member Functions

        //- Returns the number of rows
        inline label size() const;

        //- Reset the number of rows
        inline void setSize(const label size);

        //- Clear the graph
        inline void clear();

    // Member Operators
        
        //- Append an element to the given row
        inline void append(const label);
        
        //- Append an element to the given row if it does not exist there
        inline void appendIfNotIn(const label);
        
        //- check if the element is in the given row (takes linear time)
        inline bool contains(const label e) const;
        inline label containsAtPosition(const label e) const;
        
        //- set and get operators
        inline label operator[](const label) const;
        inline label& operator[](const label);
        
        //- Assignment operator
        inline void operator=(const graphRow<graphType>&);
        template<class listType>
        inline void operator=(const listType&);

    // IOstream operators

        // Write graphRow to Ostream.
        friend Ostream& operator<< <graphType>
        (
            Ostream&,
            const graphRow<graphType>&
        );

        //- Read from Istream, discarding contents of existing graphRow.
/*        friend Istream& operator>> <T, width>
        (
            Istream&,
            graphRow<T, width>&
        );
*/
};

typedef const graphRow<const VRWGraph> constRow;
typedef graphRow<VRWGraph> row;


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


template<class graphType>
inline void CML::graphRow<graphType>::checkIndex(const label i) const
{
    if( (i < 0) || (i >=data_.sizeOfRow(rowI_)) )
    {
        FatalErrorInFunction
            << "Row index " << rowI_
            << " is not in range " << CML::label(0)
            << " and " << data_.sizeOfRow(rowI_) << abort(FatalError);
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class graphType>
inline CML::graphRow<graphType>::graphRow(graphType& g, const label i)
:
    data_(g),
    rowI_(i)
{
}

template<class graphType>
inline CML::graphRow<graphType>::graphRow
(
    const graphRow<graphType>& ol
)
:
    data_(ol.data_),
    rowI_(ol.rowI_)
{
}

template<class graphType>
inline CML::graphRow<graphType>::~graphRow()
{
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class graphType>
inline CML::label CML::graphRow<graphType>::size() const
{
    return data_.sizeOfRow(rowI_);
}

template<class graphType>
inline void CML::graphRow<graphType>::setSize(const label s)
{
    data_.setRowSize(rowI_, s);
}

template<class graphType>
inline void CML::graphRow<graphType>::clear()
{
    data_.setRowSize(rowI_, 0);
}

template<class graphType>
inline void CML::graphRow<graphType>::append(const label el)
{
    data_.append(rowI_, el);
}

template<class graphType>
inline void CML::graphRow<graphType>::appendIfNotIn(const label el)
{
    data_.appendIfNotIn(rowI_, el);
}

template<class graphType>
inline bool CML::graphRow<graphType>::contains(const label e) const
{
    return data_.contains(rowI_, e);
}

template<class graphType>
inline CML::label CML::graphRow<graphType>::containsAtPosition
(
    const label e
) const
{
    return data_.containsAtPosition(rowI_, e);
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class graphType>
inline CML::label CML::graphRow<graphType>::operator[](const label i) const
{
    return data_(rowI_, i);
}

template<class graphType>
inline CML::label& CML::graphRow<graphType>::operator[](const label i)
{
    return data_(rowI_, i);
}

template<class graphType>
inline void CML::graphRow<graphType>::operator=
(
    const graphRow<graphType>& l
)
{
    data_.setRowSize(rowI_, l.size());
    for(label i=0;i<l.size();++i)
        data_(rowI_, i) = l[i];
}

template<class graphType>
template<class listType>
inline void CML::graphRow<graphType>::operator=(const listType& l)
{
    data_.setRowSize(rowI_, l.size());
    for(label i=0;i<l.size();++i)
        data_(rowI_, i) = l[i];
}

template<class graphType>
inline CML::Ostream& operator<<
(
    CML::Ostream& os,
    const CML::graphRow<graphType>& r
)
{
    os << r.size() << "(";
    for(CML::label i=0;i<r.size();++i)
        os << r[i] << " ";
    os << ")";
    
    return os;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
