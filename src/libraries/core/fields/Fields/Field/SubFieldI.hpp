/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
inline CML::SubField<Type>::SubField
(
    const SubList<Type>& list
)
:
    SubList<Type>(list)
{}


template<class Type>
inline CML::SubField<Type>::SubField
(
    const UList<Type>& list
)
:
    SubList<Type>(list, list.size())
{}


template<class Type>
inline CML::SubField<Type>::SubField
(
    const UList<Type>& list,
    const label subSize
)
:
    SubList<Type>(list, subSize)
{}


template<class Type>
inline CML::SubField<Type>::SubField
(
    const UList<Type>& list,
    const label subSize,
    const label startIndex
)
:
    SubList<Type>(list, subSize, startIndex)
{}


template<class Type>
inline CML::SubField<Type>::SubField
(
    const SubField<Type>& sfield
)
:
    refCount(),
    SubList<Type>(sfield)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
inline const CML::SubField<Type>& CML::SubField<Type>::null()
{
    return NullSingletonRef< SubField<Type> >();
}


template<class Type>
inline CML::tmp<CML::Field<typename CML::SubField<Type>::cmptType> >
CML::SubField<Type>::component
(
    const direction d
) const
{
    return (reinterpret_cast<const Field<Type>&>(*this)).component(d);
}


template<class Type>
inline CML::tmp<CML::Field<Type> > CML::SubField<Type>::T() const
{
    return (reinterpret_cast<const Field<Type>&>(*this)).T();
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
inline void CML::SubField<Type>::operator=(const SubField<Type>& rhs)
{
    UList<Type>::operator=(rhs);
}


template<class Type>
inline CML::SubField<Type>::operator const CML::Field<Type>&() const
{
    return *reinterpret_cast< const Field<Type>* >(this);
}


// ************************************************************************* //
