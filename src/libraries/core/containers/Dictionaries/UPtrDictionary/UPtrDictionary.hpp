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
    CML::UPtrDictionary

Description
    Template dictionary class which does not manages the storage
    associated with it.

    It is derived from DictionaryBase instantiated on a non-memory managed
    form of intrusive doubly-linked list of \<T\>.

SourceFiles
    UPtrDictionary.cpp

\*---------------------------------------------------------------------------*/

#ifndef UPtrDictionary_H
#define UPtrDictionary_H

#include "DictionaryBase.hpp"
#include "DLList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class UPtrDictionary Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class UPtrDictionary
:
    public DictionaryBase<DLList<T*>, T>
{

public:

    // Constructors

        //- Null constructor
        UPtrDictionary();

        //- Copy construct
        UPtrDictionary(const UPtrDictionary&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
CML::UPtrDictionary<T>::UPtrDictionary()
{}


template<class T>
CML::UPtrDictionary<T>::UPtrDictionary(const UPtrDictionary& dict)
:
    DictionaryBase<DLList<T*>, T>(dict)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
