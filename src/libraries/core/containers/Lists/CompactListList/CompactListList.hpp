/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
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
    CML::CompactListList

Description
    A packed storage unstructured matrix of objects of type \<T\>
    using an offset table for access.

    The offset table is the size of the number of rows+1
    whose elements are the
    accumulated sizes of the rows, i.e.
      - offset[i] gives the index of first element of row i
      - offset[i+1] - offset[i] is the number of elements in row i

    Storage is allocated on free-store during construction.

    As a special case a null-contructed CompactListList has an empty
    offsets_ (instead of size 1).
 

\*---------------------------------------------------------------------------*/

#ifndef CompactListList_H
#define CompactListList_H

#include "labelList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class T, class Container> class CompactListList;

template<class T, class Container> Istream& operator>>
(
    Istream&,
    CompactListList<T, Container>&
);
template<class T, class Container> Ostream& operator<<
(
    Ostream&,
    const CompactListList<T, Container>&
);


/*---------------------------------------------------------------------------*\
                       Class CompactListList Declaration
\*---------------------------------------------------------------------------*/

template<class T, class Container = List<T> >
class CompactListList
{
    // Private data

        label size_;

        //- Offset table
        List<label> offsets_;

        //- Packed matrix of data
        List<T> m_;


public:

    // Static Member Functions

        //- Return a null CompactListList
        inline static const CompactListList<T, Container>& null();

    // Constructors

        //- Null constructor.
        inline CompactListList();

        //- Construct by converting given List<List<T> >
        explicit CompactListList(const List<Container>&);

        //- Construct given size of offset table (number of rows)
        //  and number of data.
        inline CompactListList(const label nRows, const label nData);

        //- Construct given size of offset table (number of rows),
        //  the number of data and a value for all elements.
        inline CompactListList(const label nRows, const label nData, const T&);

        //- Construct given list of row-sizes.
        explicit CompactListList(const labelUList& rowSizes);

        //- Construct given list of row-sizes
        CompactListList(const labelUList& rowSizes, const T&);

        //- Construct by transferring the parameter contents
        explicit CompactListList(const Xfer<CompactListList<T, Container> >&);

        //- Construct as copy or re-use as specified.
        CompactListList(CompactListList<T, Container>&, bool reUse);

        //- Construct from Istream.
        CompactListList(Istream&);

        //- Clone
        inline autoPtr<CompactListList<T, Container> > clone() const;


    // Member Functions

        // Access

            //- Return the primary size, i.e. the number of rows
            inline label size() const;

            //- Return true if the number of rows is zero
            inline bool empty() const;

            //- Return the offset table (= size()+1)
            inline const List<label>& offsets() const;

            //- Return non-const access to the offset table
            inline List<label>& offsets();

            //- Return the packed matrix of data
            inline const List<T>& m() const;

            //- Return non-const access to the packed matrix of data
            inline List<T>& m();


        // Edit

            //- Reset size of CompactListList.
            //  This form only allows contraction of the CompactListList.
            void setSize(const label nRows);

            //- Reset size of CompactListList.
            void setSize(const label nRows, const label nData);

            //- Reset sizes of CompactListList and value for new elements.
            void setSize(const label nRows, const label nData, const T&);

            //- Reset size of CompactListList.
            void setSize(const labelUList& rowSizes);

            //- Reset size of CompactListList.
            //  This form only allows contraction of the CompactListList.
            inline void resize(const label nRows);

            //- Reset size of CompactListList.
            inline void resize(const label nRows, const label nData);

            //- Reset sizes of CompactListList and value for new elements.
            inline void resize(const label nRows, const label nData, const T&);

            //- Reset size of CompactListList.
            inline void resize(const labelUList& rowSizes);

            //- Clear the CompactListList, i.e. set sizes to zero.
            void clear();

            //- Return sizes (to be used e.g. for construction)
            labelList sizes() const;

            //- Transfer the contents of the argument CompactListList
            //  into this CompactListList and annul the argument list.
            void transfer(CompactListList<T, Container>&);

            //- Transfer the contents to the Xfer container
            inline Xfer<CompactListList<T, Container> > xfer();

        // Other

            //- Return index into m
            inline label index(const label row, const label col) const;

            //- Get row for index into m.
            inline label whichRow(const label index) const;

            //- Get column index (j) given above row
            inline label whichColumn(const label row, const label index) const;


    // Member operators

        //- Return subscript-checked row as UList.
        inline UList<T> operator[](const label i);

        //- Return const subscript-checked row as UList.
        inline const UList<T> operator[](const label i) const;

        //- Return subscript-checked element.
        inline T& operator()(const label i, const label j);

        //- Return const subscript-checked element.
        inline const T& operator()(const label i, const label j) const;

        //- Return as List<Container>
        List<Container> operator()() const;

        //- Assignment of all entries to the given value
        inline void operator=(const T&);


    // Istream operator

        //- Read CompactListList from Istream, discarding contents
        //  of existing CompactListList.
        friend Istream& operator>> <T, Container>
        (
            Istream&,
            CompactListList<T, Container>&
        );

        // Write CompactListList to Ostream.
        friend Ostream& operator<< <T, Container>
        (
            Ostream&,
            const CompactListList<T, Container>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "ListOps.hpp"
#include "SubList.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T, class Container>
inline CML::CompactListList<T, Container>::CompactListList()
:
    size_(0)
{}


template<class T, class Container>
inline CML::CompactListList<T, Container>::CompactListList
(
    const label nRows,
    const label nData
)
:
    size_(nRows),
    offsets_(nRows+1, 0),
    m_(nData)
{}


template<class T, class Container>
inline CML::CompactListList<T, Container>::CompactListList
(
    const label nRows,
    const label nData,
    const T& t
)
:
    size_(nRows),
    offsets_(nRows+1, 0),
    m_(nData, t)
{}


template<class T, class Container>
inline CML::autoPtr<CML::CompactListList<T, Container> >
CML::CompactListList<T, Container>::clone() const
{
    return autoPtr<CompactListList<T, Container> >
    (
        new CompactListList<T, Container>(*this)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T, class Container>
inline const CML::CompactListList<T, Container>&
CML::CompactListList<T, Container>::null()
{
    return NullSingletonRef< CompactListList<T, Container> >();
}


template<class T, class Container>
inline CML::label CML::CompactListList<T, Container>::size() const
{
    return size_;
}


template<class T, class Container>
inline bool CML::CompactListList<T, Container>::empty() const
{
    return !size_;
}


template<class T, class Container>
inline const CML::List<CML::label>&
CML::CompactListList<T, Container>::offsets() const
{
    return offsets_;
}


template<class T, class Container>
inline CML::List<CML::label>& CML::CompactListList<T, Container>::offsets()
{
    return offsets_;
}


template<class T, class Container>
inline const CML::List<T>& CML::CompactListList<T, Container>::m()
const
{
    return m_;
}


template<class T, class Container>
inline CML::List<T>& CML::CompactListList<T, Container>::m()
{
    return m_;
}


template<class T, class Container>
inline CML::label CML::CompactListList<T, Container>::index
(
    const label i,
    const label j
) const
{
    return offsets_[i] + j;
}


template<class T, class Container>
inline CML::label CML::CompactListList<T, Container>::whichRow(const label i)
const
{
    if (i < 0 || i >= m_.size())
    {
        FatalErrorInFunction
            << "Index " << i << " outside 0.." << m_.size()
            << abort(FatalError);
    }

    return findLower(offsets_, i+1);
}


template<class T, class Container>
inline CML::label CML::CompactListList<T, Container>::whichColumn
(
    const label row,
    const label i
) const
{
    return i - index(row, 0);
}


template<class T, class Container>
inline CML::Xfer<CML::CompactListList<T, Container> >
CML::CompactListList<T, Container>::xfer()
{
    return xferMove(*this);
}


template<class T, class Container>
inline void CML::CompactListList<T, Container>::resize(const label nRows)
{
    this->setSize(nRows);
}


template<class T, class Container>
inline void CML::CompactListList<T, Container>::resize
(
    const label nRows,
    const label nData
)
{
    this->setSize(nRows, nData);
}


template<class T, class Container>
inline void CML::CompactListList<T, Container>::resize
(
    const label nRows,
    const label nData,
    const T& t
)
{
    this->setSize(nRows, nData, t);
}


template<class T, class Container>
inline void CML::CompactListList<T, Container>::resize
(
    const labelUList& rowSizes
)
{
    this->setSize(rowSizes);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, class Container>
inline CML::UList<T> CML::CompactListList<T, Container>::operator[]
(
    const label i
)
{
    label start = offsets_[i];
    return UList<T>(m_.begin() + start, offsets_[i+1] - start);
}


template<class T, class Container>
inline const CML::UList<T>
CML::CompactListList<T, Container>::operator[]
(
    const label i
) const
{
    label start = offsets_[i];
    return UList<T>
    (
        const_cast<T*>(m_.begin() + start),
        offsets_[i+1] - start
    );
}


template<class T, class Container>
inline T& CML::CompactListList<T, Container>::operator()
(
    const label i,
    const label j
)
{
    return m_[index(i, j)];
}


template<class T, class Container>
inline const T& CML::CompactListList<T, Container>::operator()
(
    const label i,
    const label j
) const
{
    return m_[index(i, j)];
}


template<class T, class Container>
inline void CML::CompactListList<T, Container>::operator=(const T& t)
{
    m_ = t;
}


// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class T, class Container>
CML::CompactListList<T, Container>::CompactListList(const List<Container>& ll)
:
    size_(ll.size()),
    offsets_(ll.size()+1)
{
    label sumSize = 0;
    offsets_[0] = 0;
    forAll(ll, i)
    {
        sumSize += ll[i].size();
        offsets_[i+1] = sumSize;
    }

    m_.setSize(sumSize);

    label k = 0;
    forAll(ll, i)
    {
        const Container& lli = ll[i];

        forAll(lli, j)
        {
            m_[k++] = lli[j];
        }
    }
}


template<class T, class Container>
CML::CompactListList<T, Container>::CompactListList
(
    const labelUList& rowSizes
)
:
    size_(rowSizes.size()),
    offsets_(rowSizes.size()+1)
{
    label sumSize = 0;
    offsets_[0] = 0;
    forAll(rowSizes, i)
    {
        sumSize += rowSizes[i];
        offsets_[i+1] = sumSize;
    }

    m_.setSize(sumSize);
}


template<class T, class Container>
CML::CompactListList<T, Container>::CompactListList
(
    const labelUList& rowSizes,
    const T& t
)
:
    size_(rowSizes.size()),
    offsets_(rowSizes.size()+1)
{
    label sumSize = 0;
    offsets_[0] = 0;
    forAll(rowSizes, i)
    {
        sumSize += rowSizes[i];
        offsets_[i+1] = sumSize;
    }

    m_.setSize(sumSize, t);
}


template<class T, class Container>
CML::CompactListList<T, Container>::CompactListList
(
    const Xfer<CompactListList<T, Container> >& lst
)
{
    transfer(lst());
}


template<class T, class Container>
CML::CompactListList<T, Container>::CompactListList
(
    CompactListList<T, Container>& lst,
    bool reUse
)
:
    size_(lst.size()),
    offsets_(lst.offsets_, reUse),
    m_(lst.m_, reUse)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T, class Container>
void CML::CompactListList<T, Container>::setSize(const label nRows)
{
    if (nRows == 0)
    {
        clear();
    }
    if (nRows < size())
    {
        size_ = nRows;
        offsets_.setSize(nRows+1);
        m_.setSize(offsets_[nRows]);
    }
    else if (nRows > size())
    {
        FatalErrorInFunction
            << "Cannot be used to extend the list from " << offsets_.size()
            << " to " << nRows << nl
            << "    Please use one of the other setSize member functions"
            << abort(FatalError);
    }
}


template<class T, class Container>
void CML::CompactListList<T, Container>::setSize
(
    const label nRows,
    const label nData
)
{
    size_ = nRows;
    offsets_.setSize(nRows+1);
    m_.setSize(nData);
}


template<class T, class Container>
void CML::CompactListList<T, Container>::setSize
(
    const label nRows,
    const label nData,
    const T& t
)
{
    size_ = nRows;
    offsets_.setSize(nRows+1);
    m_.setSize(nData, t);
}


template<class T, class Container>
void CML::CompactListList<T, Container>::setSize(const labelUList& rowSizes)
{
    size_ = rowSizes.size();
    offsets_.setSize(rowSizes.size()+1);

    label sumSize = 0;
    offsets_[0] = 0;
    forAll(rowSizes, i)
    {
        sumSize += rowSizes[i];
        offsets_[i+1] = sumSize;
    }

    m_.setSize(sumSize);
}


template<class T, class Container>
CML::labelList CML::CompactListList<T, Container>::sizes() const
{
    labelList rowSizes(size());

    if (rowSizes.size() > 0)
    {
        forAll(rowSizes, i)
        {
            rowSizes[i] = offsets_[i+1] - offsets_[i];
        }
    }
    return rowSizes;
}


template<class T, class Container>
void CML::CompactListList<T, Container>::clear()
{
    size_ = 0;
    offsets_.clear();
    m_.clear();
}


template<class T, class Container>
void CML::CompactListList<T, Container>::transfer
(
    CompactListList<T, Container>& a
)
{
    size_ = a.size_;
    offsets_.transfer(a.offsets_);
    m_.transfer(a.m_);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, class Container>
CML::List<Container> CML::CompactListList<T, Container>::operator()()
const
{
    List<Container> ll(size());

    forAll(ll, i)
    {
        ll[i] = Container(operator[](i));
    }

    return ll;
}


// * * * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * //

#include "Istream.hpp"

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class T, class Container>
CML::CompactListList<T, Container>::CompactListList(Istream& is)
{
    operator>>(is, *this);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class T, class Container>
CML::Istream& CML::operator>>(Istream& is, CompactListList<T, Container>& lst)
{
    is  >> lst.offsets_ >> lst.m_;
    // Note: empty list gets output as two empty lists
    if (lst.offsets_.size() == 0)
    {
        lst.size_ = 0;
    }
    else
    {
        lst.size_ = lst.offsets_.size()-1;
    }
    return is;
}


template<class T, class Container>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const CompactListList<T, Container>& lst
)
{
    os  << lst.offsets_ << lst.m_;
    return os;
}




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
