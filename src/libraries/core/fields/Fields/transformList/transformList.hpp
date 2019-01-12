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

InClass
    CML

Description
    Spatial transformation functions for primitive fields.


\*---------------------------------------------------------------------------*/

#ifndef transformList_H
#define transformList_H

#include "transform.hpp"
#include "List.hpp"
#include "Map.hpp"
#include "EdgeMap.hpp"
#include "tensorField.hpp"


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Extend transform to work on list.
template<class T>
List<T> transform
(
    const tensor& rotTensor,
    const UList<T>& field
);

//- Apply transformation to list. Either single transformation tensor
//  or one tensor per element.
template<class T>
void transformList(const tensor&, UList<T>&);
template<class T>
void transformList(const tensorField&, UList<T>&);

template<class T>
void transformList(const tensor&, Map<T>&);
template<class T>
void transformList(const tensorField&, Map<T>&);

template<class T>
void transformList(const tensor&, EdgeMap<T>&);
template<class T>
void transformList(const tensorField&, EdgeMap<T>&);


// Specialisations for bool
template<>
inline void transformList(const tensor&, UList<bool>&)
{}
template<>
inline void transformList(const tensorField&, UList<bool>&)
{}
template<>
inline void transformList(const tensor&, Map<bool>&)
{}
template<>
inline void transformList(const tensorField&, Map<bool>&)
{}
template<>
inline void transformList(const tensor&, EdgeMap<bool>&)
{}
template<>
inline void transformList(const tensorField&, EdgeMap<bool>&)
{}


// Specialisations for label
template<>
inline void transformList(const tensor&, labelUList&)
{}
template<>
inline void transformList(const tensorField&, labelUList&)
{}
template<>
inline void transformList(const tensor&, Map<label>&)
{}
template<>
inline void transformList(const tensorField&, Map<label>&)
{}
template<>
inline void transformList(const tensor&, EdgeMap<label>&)
{}
template<>
inline void transformList(const tensorField&, EdgeMap<label>&)
{}


// Specialisations for scalar
template<>
inline void transformList(const tensor&, UList<scalar>&)
{}
template<>
inline void transformList(const tensorField&, UList<scalar>&)
{}
template<>
inline void transformList(const tensor&, Map<scalar>&)
{}
template<>
inline void transformList(const tensorField&, Map<scalar>&)
{}
template<>
inline void transformList(const tensor&, EdgeMap<scalar>&)
{}
template<>
inline void transformList(const tensorField&, EdgeMap<scalar>&)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template <class T>
CML::List<T> CML::transform
(
    const tensor& rotTensor,
    const UList<T>& field
)
{
    List<T> newField(field.size());

    forAll(field, i)
    {
        newField[i] = transform(rotTensor, field[i]);
    }

    return newField;
}


template <class T>
void CML::transformList(const tensor& rotTensor, UList<T>& field)
{
    forAll(field, i)
    {
        field[i] = transform(rotTensor, field[i]);
    }
}


template <class T>
void CML::transformList(const tensorField& rotTensor, UList<T>& field)
{
    if (rotTensor.size() == 1)
    {
        forAll(field, i)
        {
            field[i] = transform(rotTensor[0], field[i]);
        }
    }
    else if (rotTensor.size() == field.size())
    {
        forAll(field, i)
        {
            field[i] = transform(rotTensor[i], field[i]);
        }
    }
    else
    {
        FatalErrorInFunction
            << "Sizes of field and transformation not equal. field:"
            << field.size() << " transformation:" << rotTensor.size()
            << abort(FatalError);
    }
}


template <class T>
void CML::transformList(const tensor& rotTensor, Map<T>& field)
{
    forAllIter(typename Map<T>, field, iter)
    {
        iter() = transform(rotTensor[0], iter());
    }
}


template <class T>
void CML::transformList(const tensorField& rotTensor, Map<T>& field)
{
    if (rotTensor.size() == 1)
    {
        forAllIter(typename Map<T>, field, iter)
        {
            iter() = transform(rotTensor[0], iter());
        }
    }
    else
    {
        FatalErrorInFunction
            << "Multiple transformation tensors not supported. field:"
            << field.size() << " transformation:" << rotTensor.size()
            << abort(FatalError);
    }
}


template <class T>
void CML::transformList(const tensor& rotTensor, EdgeMap<T>& field)
{
    forAllIter(typename EdgeMap<T>, field, iter)
    {
        iter() = transform(rotTensor[0], iter());
    }
}


template <class T>
void CML::transformList(const tensorField& rotTensor, EdgeMap<T>& field)
{
    if (rotTensor.size() == 1)
    {
        forAllIter(typename EdgeMap<T>, field, iter)
        {
            iter() = transform(rotTensor[0], iter());
        }
    }
    else
    {
        FatalErrorInFunction
            << "Multiple transformation tensors not supported. field:"
            << field.size() << " transformation:" << rotTensor.size()
            << abort(FatalError);
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
