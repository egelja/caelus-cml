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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
inline CML::UIndirectList<T>::UIndirectList
(
    const UList<T>& completeList,
    const labelUList& addr
)
:
    completeList_(const_cast<UList<T>&>(completeList)),
    addressing_(addr)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline CML::label CML::UIndirectList<T>::size() const
{
    return addressing_.size();
}


template<class T>
inline bool CML::UIndirectList<T>::empty() const
{
    return addressing_.empty();
}


template<class T>
inline T& CML::UIndirectList<T>::first()
{
    return completeList_[addressing_.first()];
}


template<class T>
inline const T& CML::UIndirectList<T>::first() const
{
    return completeList_[addressing_.first()];
}


template<class T>
inline T& CML::UIndirectList<T>::last()
{
    return completeList_[addressing_.last()];
}


template<class T>
inline const T& CML::UIndirectList<T>::last() const
{
    return completeList_[addressing_.last()];
}


template<class T>
inline const CML::UList<T>& CML::UIndirectList<T>::completeList() const
{
    return completeList_;
}


template<class T>
inline const CML::List<CML::label>& CML::UIndirectList<T>::addressing() const
{
    return addressing_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class T>
inline CML::List<T> CML::UIndirectList<T>::operator()() const
{
    List<T> result(size());

    forAll(*this, i)
    {
        result[i] = operator[](i);
    }

    return result;
}


template<class T>
inline T& CML::UIndirectList<T>::operator[](const label i)
{
    return completeList_[addressing_[i]];
}


template<class T>
inline const T& CML::UIndirectList<T>::operator[](const label i) const
{
    return completeList_[addressing_[i]];
}


template<class T>
inline void CML::UIndirectList<T>::operator=(const UList<T>& ae)
{
    if (addressing_.size() != ae.size())
    {
        FatalErrorInFunction
            << "Addressing and list of addressed elements "
               "have different sizes: "
            << addressing_.size() << " " << ae.size()
            << abort(FatalError);
    }

    forAll(addressing_, i)
    {
        completeList_[addressing_[i]] = ae[i];
    }
}


template<class T>
inline void CML::UIndirectList<T>::operator=(const UIndirectList<T>& ae)
{
    if (addressing_.size() != ae.size())
    {
        FatalErrorInFunction
            << "Addressing and list of addressed elements "
               "have different sizes: "
            << addressing_.size() << " " << ae.size()
            << abort(FatalError);
    }

    forAll(addressing_, i)
    {
        completeList_[addressing_[i]] = ae[i];
    }
}


template<class T>
inline void CML::UIndirectList<T>::operator=(const T& t)
{
    forAll(addressing_, i)
    {
        completeList_[addressing_[i]] = t;
    }
}


// ************************************************************************* //
