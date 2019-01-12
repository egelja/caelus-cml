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
    FRWGraph

Description
    This class is an implementation of a graph with the fixed row width.
    The implementation is memory efficient.

Author: Franjo Juretic (franjo.juretic@c-fields.com)


\*---------------------------------------------------------------------------*/

#ifndef FRWGraph_HPP
#define FRWGraph_HPP

#include "label.hpp"
#include "LongList.hpp"
#include "FixedList.hpp"
#include "bool.hpp"
#include "error.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //

template<class T, label width>
class FRWGraph;

template<class T, label width>
Ostream& operator<<
(
    Ostream&,
    const FRWGraph<T, width>&
);
template<class T, label width>
Istream& operator>>
(
    Istream&,
    FRWGraph<T, width>&
);

/*---------------------------------------------------------------------------*\
                           Class FRWGraph Declaration
\*---------------------------------------------------------------------------*/

template<class T, label width>
class FRWGraph
{
    // Private data
        //- list containing the data
        LongList<T> data_;

        //- number of rows
        label nRows_;

    // Private member functions
        //- check index
        void checkIndex(const label i, const label j) const;

public:

    // Constructors

        //- Construct null
        inline FRWGraph();

        //- Construct given size
        explicit inline FRWGraph(const label size);

        //- Construct to given size and initialize
        explicit inline FRWGraph(const label size, const T& t);

        //- Copy contructor
        inline FRWGraph(const FRWGraph<T, width>&);

    // Destructor

        inline ~FRWGraph();

    // Member Functions

        // Access

            //- Returns the number of rows
            inline label size() const;

            //- Returns the size of a given row (obsolete)
            inline label sizeOfRow(const label rowI) const;

        // Edit

            //- Reset the number of rows
            void setSize(const label);

            //- Clear the graph
            //  Allocated size does not change
            void clear();

    // Member Operators

        //- Append a row at the end of the graph
        inline void appendFixedList(const FixedList<T, width>& l);

        //- Set row with the list
        inline void setRow(const label rowI, const FixedList<T, width>& l);

        //- check if the element is in the given row (takes linear time)
        inline bool contains(const label rowI, const T& e) const;
        inline label containsAtPosition(const label rowI, const T& e) const;

        //- get and set operators
        inline const T& operator()(const label i, const label j) const;
        inline T& operator()(const label i, const label j);

        //- Assignment operator
        inline void operator=(const FRWGraph<T, width>&);


    // IOstream operators

        // Write FRWGraph to Ostream.
        friend Ostream& operator<< <T, width>
        (
            Ostream&,
            const FRWGraph<T, width>&
        );

        //- Read from Istream, discarding contents of existing FRWGraph.
/*        friend Istream& operator>> <T, width>
        (
            Istream&,
            FRWGraph<T, width>&
        );
*/
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class T, CML::label width>
void CML::FRWGraph<T, width>::checkIndex(const label i, const label j) const
{
    if( (i < 0) || (i >= nRows_) )
    {
        FatalErrorInFunction
            << "Row index " << i
            << " is not in range " << 0
            << " and " << nRows_ << abort(FatalError);
    }

    if( (j < 0) || (j >= width) )
        FatalErrorInFunction
            << "Column index " << j
            << " is not in range " << 0
            << " and " << width << abort(FatalError);
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct null
template<class T, CML::label width>
inline CML::FRWGraph<T,width>::FRWGraph()
:
    data_(),
    nRows_(0)
{}

//- Construct given size
template<class T, CML::label width>
inline CML::FRWGraph<T,width>::FRWGraph
(
    const label s
)
:
    data_(s * width),
    nRows_(s)
{}


//- Construct given size
template<class T, CML::label width>
inline CML::FRWGraph<T,width>::FRWGraph
(
    const label s,
    const T& t
)
:
    data_(s * width, t),
    nRows_(s)
{}

template<class T, CML::label width>
inline CML::FRWGraph<T,width>::FRWGraph
(
    const FRWGraph<T,width>& ol
)
:
    data_(ol.data_),
    nRows_(ol.nRows_)
{}

template<class T, CML::label width>
inline CML::FRWGraph<T,width>::~FRWGraph()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T, CML::label width>
inline CML::label CML::FRWGraph<T,width>::size() const
{
    return nRows_;
}

template<class T, CML::label width>
inline CML::label CML::FRWGraph<T,width>::sizeOfRow(const label) const
{
    return width;
}

template<class T, CML::label width>
inline void CML::FRWGraph<T,width>::setSize(const label i)
{
    data_.setSize(i * width);
    nRows_ = i;
}

template<class T, CML::label width>
inline void CML::FRWGraph<T,width>::clear()
{
    data_.clear();
    nRows_ = 0;
}

template<class T, CML::label width>
inline void CML::FRWGraph<T,width>::appendFixedList
(
    const FixedList<T, width>& l
)
{
    forAll(l, elI)
        data_.append(l[elI]);
    ++nRows_;
}

template<class T, CML::label width>
inline void CML::FRWGraph<T,width>::setRow
(
    const label rowI,
    const FixedList<T, width>& l
)
{
    const label start = rowI * width;
    forAll(l, elI)
        data_[start+elI] = l[elI];
}

template<class T, CML::label width>
inline bool CML::FRWGraph<T,width>::contains
(
    const label rowI,
    const T& e
) const
{
    const label start = rowI * width;

    for(label i=0;i<width;++i)
        if( data_[start+i] == e )
            return true;

    return false;
}

template<class T, CML::label width>
inline CML::label CML::FRWGraph<T,width>::containsAtPosition
(
    const label rowI,
    const T& e
) const
{
    const label start = rowI * width;

    for(label i=0;i<width;++i)
        if( data_[start+i] == e )
            return i;

    return -1;
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //
template<class T, CML::label width>
inline const T& CML::FRWGraph<T,width>::operator()
(
    const label i,
    const label j
) const
{
    #ifdef FULLDEBUG
    checkIndex(i, j);
    #endif

    return data_[i * width + j];
}

template<class T, CML::label width>
inline T& CML::FRWGraph<T,width>::operator()
(
    const label i, const label j
)
{
    #ifdef FULLDEBUG
    checkIndex(i, j);
    #endif

    return data_[i * width + j];
}

template<class T, CML::label width>
inline void CML::FRWGraph<T,width>::operator=
(
    const FRWGraph<T, width>& l
)
{
    data_ = l.data_;
    nRows_ = l.nRows_;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class T, CML::label width>
CML::Ostream& CML::operator<<
(
    CML::Ostream& os,
    const CML::FRWGraph<T, width>& DL
)
{
    os << DL.size() << "(" << nl;

    for(label i=0;i<DL.size();++i)
    {
        os << width << "(";

        for(label j=0;j<width;++j)
        {
            if( j )
            {
                os << " ";
            }

            os << DL(i, j);
        }

        os << ")" << nl;
    }

    os << ")";

    // Check state of IOstream
    os.check
    (
        "template<class T, CML::label width>CML::Ostream& CML::operator<<"
        "(CML::Ostream& os, const CML::FRWGraph<T, width>&)"
    );

    return os;
}

/*
template<class T, CML::label width>
CML::Istream& CML::operator>>
(
    CML::Istream& is,
    CML::FRWGraph<T, width>& DL
)
{
    label size;
    T e;
    is >> size;
    DL.setSize(size);
    for(IndexType i=0;i<size;++i)
    {
        is >> e;
        DL[i] = e;
    }

    return is;
}
*/


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
