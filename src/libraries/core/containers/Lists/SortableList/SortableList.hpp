/*---------------------------------------------------------------------------*\
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
    CML::SortableList

Description
    A list that is sorted upon construction or when explicitly requested
    with the sort() method.

    Uses the CML::stableSort() algorithm.


\*---------------------------------------------------------------------------*/

#ifndef SortableList_H
#define SortableList_H

#include "labelList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class SortableList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class SortableList
:
    public List<T>
{
    // Private data

        //- Original indices
        labelList indices_;

        //- Resize, fill and sort the parameter according to the list values
        void sortIndices(List<label>&) const;

public:

    // Constructors

        //- Null constructor, sort later (eg, after assignment or transfer)
        SortableList();

        //- Construct from UList, sorting immediately.
        explicit SortableList(const UList<T>&);

        //- Construct from transferred List, sorting immediately.
        explicit SortableList(const Xfer<List<T> >&);

        //- Construct given size. Sort later on.
        //  The indices remain empty until the list is sorted
        explicit SortableList(const label size);

        //- Construct given size and initial value. Sort later on.
        //  The indices remain empty until the list is sorted
        SortableList(const label size, const T&);

        //- Construct as copy.
        SortableList(const SortableList<T>&);


    // Member Functions

        //- Return the list of sorted indices. Updated every sort.
        const labelList& indices() const
        {
            return indices_;
        }

        //- Return non-const access to the sorted indices. Updated every sort.
        labelList& indices()
        {
            return indices_;
        }

        //- Clear the list and the indices
        void clear();

        //- Clear the indices and return a reference to the underlying List
        List<T>& shrink();

        //- (stable) sort the list (if changed after construction time)
        //  also resizes the indices as required
        void sort();

        //- Reverse (stable) sort the list
        void reverseSort();

        //- Transfer contents to the Xfer container as a plain List
        inline Xfer<List<T> > xfer();


    // Member Operators

        //- Assignment of all entries to the given value
        inline void operator=(const T&);

        //- Assignment from UList operator. Takes linear time.
        inline void operator=(const UList<T>&);

        //- Assignment operator. Takes linear time.
        inline void operator=(const SortableList<T>&);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * * //

template<class T>
void CML::SortableList<T>::sortIndices(List<label>& order) const
{
    // list lengths must be identical
    if (order.size() != this->size())
    {
        // avoid copying any elements, they are overwritten anyhow
        order.clear();
        order.setSize(this->size());
    }

    forAll(order, elemI)
    {
        order[elemI] = elemI;
    }
    CML::stableSort(order, typename UList<T>::less(*this));
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
CML::SortableList<T>::SortableList()
{}


template<class T>
CML::SortableList<T>::SortableList(const UList<T>& values)
:
    List<T>(values)
{
    sort();
}


template<class T>
CML::SortableList<T>::SortableList(const Xfer<List<T> >& values)
:
    List<T>(values)
{
    sort();
}


template<class T>
CML::SortableList<T>::SortableList(const label size)
:
    List<T>(size)
{}


template<class T>
CML::SortableList<T>::SortableList(const label size, const T& val)
:
    List<T>(size, val)
{}


template<class T>
CML::SortableList<T>::SortableList(const SortableList<T>& lst)
:
    List<T>(lst),
    indices_(lst.indices())
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


template<class T>
void CML::SortableList<T>::clear()
{
    List<T>::clear();
    indices_.clear();
}


template<class T>
CML::List<T>& CML::SortableList<T>::shrink()
{
    indices_.clear();
    return static_cast<List<T>&>(*this);
}


template<class T>
void CML::SortableList<T>::sort()
{
    sortIndices(indices_);

    List<T> lst(this->size());
    forAll(indices_, i)
    {
        lst[i] = this->operator[](indices_[i]);
    }

    List<T>::transfer(lst);
}


template<class T>
void CML::SortableList<T>::reverseSort()
{
    sortIndices(indices_);

    List<T> lst(this->size());
    label endI = indices_.size();
    forAll(indices_, i)
    {
        lst[--endI] = this->operator[](indices_[i]);
    }

    List<T>::transfer(lst);
}


template<class T>
CML::Xfer<CML::List<T> > CML::SortableList<T>::xfer()
{
    return xferMoveTo<List<T> >(*this);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline void CML::SortableList<T>::operator=(const T& t)
{
    UList<T>::operator=(t);
}


template<class T>
inline void CML::SortableList<T>::operator=(const UList<T>& rhs)
{
    List<T>::operator=(rhs);
    indices_.clear();
}


template<class T>
inline void CML::SortableList<T>::operator=(const SortableList<T>& rhs)
{
    List<T>::operator=(rhs);
    indices_ = rhs.indices();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
