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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * * //

template<class T>
inline const CML::Xfer<T>& CML::Xfer<T>::null()
{
    return *reinterpret_cast< Xfer<T>* >(0);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
inline CML::Xfer<T>::Xfer(T* p)
:
    ptr_(p ? p : new T)
{}


template<class T>
inline CML::Xfer<T>::Xfer(T& t, bool allowTransfer)
:
    ptr_(new T)
{
    if (allowTransfer)
    {
        ptr_->transfer(t);
    }
    else
    {
        ptr_->operator=(t);
    }
}


template<class T>
inline CML::Xfer<T>::Xfer(const T& t)
:
    ptr_(new T)
{
    ptr_->operator=(t);
}


template<class T>
inline CML::Xfer<T>::Xfer(const Xfer<T>& t)
:
    ptr_(new T)
{
    ptr_->transfer(*(t.ptr_));
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class T>
inline CML::Xfer<T>::~Xfer()
{
    delete ptr_;
    ptr_ = 0;
}


// * * * * * * * * * * * * *  Member Functions * * * * * * * * * * * * * * * //


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline void CML::Xfer<T>::operator=(T& t)
{
    ptr_->transfer(t);
}


template<class T>
inline void CML::Xfer<T>::operator=(const Xfer<T>& t)
{
    // silently ignore attempted copy to self
    if (this != &t)
    {
        ptr_->transfer(*(t.ptr_));
    }
}


template<class T>
inline T& CML::Xfer<T>::operator()() const
{
    return *ptr_;
}


template<class T>
inline T* CML::Xfer<T>::operator->() const
{
    return ptr_;
}


// * * * * * * * * * * * * *  Helper Functions * * * * * * * * * * * * * * * //


template<class T>
inline CML::Xfer<T> CML::xferCopy(const T& t)
{
    return CML::Xfer<T>(t);
}


template<class T>
inline CML::Xfer<T> CML::xferMove(T& t)
{
    return CML::Xfer<T>(t, true);
}


template<class T>
inline CML::Xfer<T> CML::xferTmp(CML::tmp<T>& tt)
{
    return CML::Xfer<T>(tt(), tt.isTmp());
}


template<class To, class From>
inline CML::Xfer<To> CML::xferCopyTo(const From& t)
{
    CML::Xfer<To> xf;
    xf() = t;
    return xf;
}


template<class To, class From>
inline CML::Xfer<To> CML::xferMoveTo(From& t)
{
    CML::Xfer<To> xf;
    xf().transfer(t);
    return xf;
}


// ************************************************************************* //
