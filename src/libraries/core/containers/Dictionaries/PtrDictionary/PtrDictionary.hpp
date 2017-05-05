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
    CML::PtrDictionary

Description
    Template dictionary class which does not manages the storage
    associated with it.

    It is derived from DictionaryBase instantiated on a non-memory managed
    form of intrusive doubly-linked list of T.


\*---------------------------------------------------------------------------*/

#ifndef PtrDictionary_H
#define PtrDictionary_H

#include "DictionaryBase.hpp"
#include "DLPtrList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class PtrDictionary Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class PtrDictionary
:
    public DictionaryBase<DLPtrList<T>, T>
{

public:

    // Constructors

        //- Null constructor
        PtrDictionary();

        //- Copy construct
        PtrDictionary(const PtrDictionary&);

        //- Construct from Istream using given Istream constructor class
        template<class INew>
        PtrDictionary(Istream&, const INew&);

        //- Construct from Istream
        PtrDictionary(Istream&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
CML::PtrDictionary<T>::PtrDictionary()
{}


template<class T>
CML::PtrDictionary<T>::PtrDictionary(const PtrDictionary& dict)
:
    DictionaryBase<DLPtrList<T>, T>(dict)
{}


template<class T>
template<class INew>
CML::PtrDictionary<T>::PtrDictionary(Istream& is, const INew& iNew)
:
    DictionaryBase<DLPtrList<T>, T>(is, iNew)
{}


template<class T>
CML::PtrDictionary<T>::PtrDictionary(Istream& is)
:
    DictionaryBase<DLPtrList<T>, T>(is)
{}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
