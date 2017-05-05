/*---------------------------------------------------------------------------*\
Copyright (C) 2014 OpenFOAM Foundation
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
    CML::nullSingleton

Description
    Singleton null-singleton class and instance

SourceFiles
    nullSingleton.C

\*---------------------------------------------------------------------------*/

#ifndef nullSingleton_H
#define nullSingleton_H

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class nullSingleton Declaration
\*---------------------------------------------------------------------------*/

class NullSingleton
{
    //- Private constructor
    NullSingleton()
    {}

    //- Prevent copy-construction
    NullSingleton(const NullSingleton&);

    //- Prevent assignment
    NullSingleton& operator=(const NullSingleton&);

public:

    //- The unique null singleton
    static const NullSingleton nullSingleton;
};


//- Pointer to the unique nullSingleton
extern const NullSingleton* nullSingletonPtr;


//- Return reference to the nullSingleton of type T
template <class T>
inline const T& NullSingletonRef();

//- Return pointer to the nullSingleton of type T
template <class T>
inline const T* NullSingletonPtr();


//- Return true if t is a reference to the nullSingleton of type T
template <class T>
inline bool isNull(const T& t);

//- Return true if t is not a reference to the nullSingleton of type T
template <class T>
inline bool notNull(const T& t);


//- Return true if t is a pointer to the nullSingleton of type T
template <class T>
inline bool isNull(const T* t);

//- Return true if t is not a pointer to the nullSingleton of type T
template <class T>
inline bool notNull(const T* t);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


template <class T>
inline const T& CML::NullSingletonRef()
{
    return *reinterpret_cast<const T*>(nullSingletonPtr);
}

template <class T>
inline const T* CML::NullSingletonPtr()
{
    return reinterpret_cast<const T*>(nullSingletonPtr);
}


template <class T>
inline bool CML::isNull(const T& t)
{
    return &t == NullSingletonPtr<T>();
}

template <class T>
inline bool CML::notNull(const T& t)
{
    return &t != NullSingletonPtr<T>();
}


template <class T>
inline bool CML::isNull(const T* t)
{
    return t == NullSingletonPtr<T>();
}

template <class T>
inline bool CML::notNull(const T* t)
{
    return t != NullSingletonPtr<T>();
}


#endif

// ************************************************************************* //
