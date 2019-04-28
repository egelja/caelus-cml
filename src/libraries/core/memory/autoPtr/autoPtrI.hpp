/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "error.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
inline CML::autoPtr<T>::autoPtr(T* p)
:
    ptr_(p)
{}


template<class T>
inline CML::autoPtr<T>::autoPtr(const autoPtr<T>& ap)
:
    ptr_(ap.ptr_)
{
    ap.ptr_ = 0;
}


template<class T>
inline CML::autoPtr<T>::autoPtr(const autoPtr<T>& ap, const bool reuse)
{
    if (reuse)
    {
        ptr_ = ap.ptr_;
        ap.ptr_ = 0;
    }
    else if (ap.valid())
    {
        ptr_ = ap().clone().ptr();
    }
    else
    {
        ptr_ = 0;
    }
}


template<class T>
inline CML::autoPtr<T>::~autoPtr()
{
    clear();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline bool CML::autoPtr<T>::empty() const
{
    return !ptr_;
}


template<class T>
inline bool CML::autoPtr<T>::valid() const
{
    return ptr_;
}


template<class T>
inline T* CML::autoPtr<T>::ptr()
{
    T* ptr = ptr_;
    ptr_ = 0;
    return ptr;
}


template<class T>
inline void CML::autoPtr<T>::set(T* p)
{
    if (ptr_)
    {
        FatalErrorInFunction
            << "object already allocated"
            << abort(FatalError);
    }

    ptr_ = p;
}


template<class T>
inline void CML::autoPtr<T>::reset(T* p)
{
    if (ptr_)
    {
        delete ptr_;
    }

    ptr_ = p;
}


template<class T>
inline void CML::autoPtr<T>::clear()
{
    reset(0);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline T& CML::autoPtr<T>::operator()()
{
    if (!ptr_)
    {
        FatalErrorInFunction
            << "object is not allocated"
            << abort(FatalError);
    }

    return *ptr_;
}


template<class T>
inline const T& CML::autoPtr<T>::operator()() const
{
    if (!ptr_)
    {
        FatalErrorInFunction
            << "object is not allocated"
            << abort(FatalError);
    }

    return *ptr_;
}


template<class T>
inline CML::autoPtr<T>::operator const T&() const
{
    return operator()();
}


template<class T>
inline T* CML::autoPtr<T>::operator->()
{
    if (!ptr_)
    {
        FatalErrorInFunction
            << "object is not allocated"
            << abort(FatalError);
    }

    return ptr_;
}


template<class T>
inline const T* CML::autoPtr<T>::operator->() const
{
    return const_cast<autoPtr<T>&>(*this).operator->();
}


template<class T>
inline void CML::autoPtr<T>::operator=(T* p)
{
    reset(p);
}


template<class T>
inline void CML::autoPtr<T>::operator=(const autoPtr<T>& ap)
{
    if (this != &ap)
    {
        reset(const_cast<autoPtr<T>&>(ap).ptr());
    }
}


// ************************************************************************* //
