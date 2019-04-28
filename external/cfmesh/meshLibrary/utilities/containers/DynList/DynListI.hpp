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

Author: Franjo Juretic (franjo.juretic@c-fields.com)

\*---------------------------------------------------------------------------*/


template<class T, CML::label staticSize>
inline void CML::DynList<T, staticSize>::allocateSize(const label s)
{
    if( s > UList<T>::size()  )
    {
        T* newData = new T[s];

        for(label i=0;i<nextFree_;++i)
            newData[i] = this->operator[](i);

        T* data = UList<T>::begin();
        if( data && (data != staticData_) )
            delete [] data;

        //UList<T>::reset(newData, s);
        this->UList<T>::operator=(UList<T>(newData, s));
    }
    else if( (s > staticSize) && (s < UList<T>::size()) )
    {
        T* newData = new T[s];

        for(label i=0;i<s;++i)
            newData[i] = this->operator[](i);

        T* data = UList<T>::begin();
        delete [] data;

        //UList<T>::reset(newData, s);
        this->UList<T>::operator=(UList<T>(newData, s));
    }
    else if( (s <= staticSize) && (UList<T>::size() > staticSize) )
    {
        for(label i=0;i<s;++i)
            staticData_[i] = UList<T>::operator[](i);

        T* data = UList<T>::begin();
        if( data && (data != staticData_) )
            delete [] data;

        //UList<T>::reset(staticData_, staticSize);
        this->UList<T>::operator=(UList<T>(staticData_, staticSize));
    }
}

template<class T, CML::label staticSize>
inline void CML::DynList<T, staticSize>::checkIndex(const label i) const
{
    if( (i < 0) || (i >= nextFree_) )
    {
        FatalErrorInFunction
            << "Index " << i << " is not in range " << 0
            << " and " << nextFree_ << abort(FatalError);
    }
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

//- Construct null
template<class T, CML::label staticSize>
inline CML::DynList<T, staticSize>::DynList()
:
    UList<T>(staticData_, staticSize),
    nextFree_(0)
{}


template<class T, CML::label staticSize>
inline CML::DynList<T, staticSize>::DynList(const label s)
:
    UList<T>(staticData_, staticSize),
    nextFree_(0)
{
    setSize(s);
}

template<class T, CML::label staticSize>
inline CML::DynList<T, staticSize>::DynList(const label s, const T& val)
:
    UList<T>(staticData_, staticSize),
    nextFree_(0)
{
    setSize(s);

    for(label i=0;i<s;++i)
        this->operator[](i) = val;
}

template<class T, CML::label staticSize>
inline CML::DynList<T, staticSize>::DynList(const UList<T>& ul)
:
    UList<T>(staticData_, staticSize),
    nextFree_(0)
{
    setSize(ul.size());

    forAll(ul, i)
        this->operator[](i) = ul[i];
}

template<class T, CML::label staticSize>
template<class ListType>
inline CML::DynList<T, staticSize>::DynList(const ListType& l)
:
    UList<T>(staticData_, staticSize),
    nextFree_(0)
{
    setSize(l.size());
    for(label i=0;i<nextFree_;++i)
        this->operator[](i) = l[i];
}

//- Copy construct
template<class T, CML::label staticSize>
inline CML::DynList<T, staticSize>::DynList
(
    const DynList<T, staticSize>& dl
)
:
    UList<T>(staticData_, staticSize),
    nextFree_(0)
{
    setSize(dl.size());
    for(label i=0;i<nextFree_;++i)
        this->operator[](i) = dl[i];
}

template<class T, CML::label staticSize>
inline CML::DynList<T, staticSize>::~DynList()
{
    allocateSize(0);
    //UList<T>::reset(nullptr, 0);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T, CML::label staticSize>
inline CML::label CML::DynList<T, staticSize>::size() const
{
    return nextFree_;
}

template<class T, CML::label staticSize>
inline CML::label CML::DynList<T, staticSize>::byteSize() const
{
    if( !contiguous<T>() )
    {
        FatalErrorInFunction
            << "Cannot return the binary size of a list of "
               "non-primitive elements"
            << abort(FatalError);
    }

    return nextFree_*sizeof(T);

}

template<class T, CML::label staticSize>
inline void CML::DynList<T, staticSize>::setSize(const label s)
{
    allocateSize(s);
    nextFree_ = s;
}


template<class T, CML::label staticSize>
inline void CML::DynList<T, staticSize>::clear()
{
    nextFree_ = 0;
}


template<class T, CML::label staticSize>
void CML::DynList<T, staticSize>::shrink()
{
    allocateSize(nextFree_);
}

template<class T, CML::label staticSize>
inline void CML::DynList<T, staticSize>::append(const T& e)
{
    if( nextFree_ >= UList<T>::size() )
    {
        const label newSize = 2*UList<T>::size()+2;
        allocateSize(newSize);
    }

    UList<T>::operator[](nextFree_++) = e;
}

template<class T, CML::label staticSize>
inline void CML::DynList<T, staticSize>::appendIfNotIn(const T& e)
{
    if( !contains(e) )
         append(e);
}

template<class T, CML::label staticSize>
inline bool CML::DynList<T, staticSize>::contains(const T& e) const
{
    for(label i=0;i<nextFree_;++i)
    {
        if( UList<T>::operator[](i) == e )
            return true;
    }

    return false;
}

template<class T, CML::label staticSize>
inline CML::label CML::DynList<T, staticSize>::containsAtPosition
(
    const T& e
) const
{
    for(label i=0;i<nextFree_;++i)
    {
        if( UList<T>::operator[](i) == e )
            return i;
    }

    return -1;
}

template<class T, CML::label staticSize>
inline const T& CML::DynList<T, staticSize>::lastElement() const
{
    return this->operator[](nextFree_-1);
}

template<class T, CML::label staticSize>
inline T CML::DynList<T, staticSize>::removeLastElement()
{
    if( nextFree_ == 0 )
    {
        FatalErrorInFunction
            << "List is empty" << abort(FatalError);
    }

    T el = UList<T>::operator[](--nextFree_);
    return el;
}

template<class T, CML::label staticSize>
inline T CML::DynList<T, staticSize>::removeElement(const label i)
{
    if( nextFree_ == 0 )
    {
        FatalErrorInFunction
            << "List is empty" << abort(FatalError);
    }

    T el = this->operator[](i);
    this->operator[](i) = this->operator[](nextFree_-1);
    --nextFree_;

    return el;
}

template<class T, CML::label staticSize>
inline T& CML::DynList<T, staticSize>::newElmt(const label i)
{
    return this->operator()(i);
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T, CML::label staticSize>
inline T& CML::DynList<T, staticSize>::operator()(const label i)
{
    nextFree_ = CML::max(nextFree_, i + 1);

    if( nextFree_ >= UList<T>::size() )
    {
        allocateSize(2 * nextFree_+1);
    }

    return this->operator[](i);
}

template<class T, CML::label staticSize>
inline const T& CML::DynList<T, staticSize>::operator[](const label i) const
{
    # ifdef FULLDEBUG
    checkIndex(i);
    # endif

    return UList<T>::operator[](i);
}

template<class T, CML::label staticSize>
inline T& CML::DynList<T, staticSize>::operator[](const label i)
{
    # ifdef FULLDEBUG
    checkIndex(i);
    # endif

    return UList<T>::operator[](i);
}

template<class T, CML::label staticSize>
inline CML::label CML::DynList<T, staticSize>::fcIndex
(
    const label index,
    const label offset
) const
{
    return (index + offset) % nextFree_;
}

template<class T, CML::label staticSize>
inline CML::label CML::DynList<T, staticSize>::rcIndex
(
    const label index,
    const label offset
) const
{
    return (index + nextFree_ - offset) % nextFree_;
}

template<class T, CML::label staticSize>
inline const T& CML::DynList<T, staticSize>::fcElement
(
    const label index,
    const label offset
) const
{
    return operator[](fcIndex(index, offset));
}

template<class T, CML::label staticSize>
inline const T& CML::DynList<T, staticSize>::rcElement
(
    const label index,
    const label offset
) const
{
    return operator[](rcIndex(index, offset));
}

template<class T, CML::label staticSize>
inline void CML::DynList<T, staticSize>::operator=(const T& t)
{
    UList<T>::operator=(t);
}

template<class T, CML::label staticSize>
inline void CML::DynList<T, staticSize>::operator=
(
    const DynList<T, staticSize>& dl
)
{
    allocateSize(dl.size());
    nextFree_ = dl.size();

    for(label i=0;i<nextFree_;++i)
        this->operator[](i) = dl[i];
}

template<class T, CML::label staticSize>
template<class ListType>
inline void CML::DynList<T, staticSize>::operator=(const ListType& l)
{
    allocateSize(l.size());
    nextFree_ = l.size();

    for(label i=0;i<nextFree_;++i)
        this->operator[](i) = l[i];
}


// ************************************************************************* //
