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
    CML::Dictionary

Description
    Gerneral purpose template dictionary class which manages the storage
    associated with it.

    It is derived from DictionaryBase instantiated on a memory managed form
    of intrusive doubly-linked list of \<T\>.


\*---------------------------------------------------------------------------*/

#ifndef Dictionary__H
#define Dictionary__H

#include "DictionaryBase.hpp"
#include "IDLList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class Dictionary Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class Dictionary
:
    public DictionaryBase<IDLList<T>, T>
{

public:

    // Constructors

        //- Null constructor
        Dictionary();

        //- Copy construct
        Dictionary(const Dictionary&);


    // Member functions

        //- Remove an entry specified by keyword and delete the pointer.
        //  Returns true if the keyword was found
        bool erase(const word& keyword);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class T>
CML::Dictionary<T>::Dictionary()
{}


template<class T>
CML::Dictionary<T>::Dictionary(const Dictionary& dict)
:
    DictionaryBase<IDLList<T>, T>(dict)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
bool CML::Dictionary<T>::erase(const word& keyword)
{
    T* tPtr = this->remove(keyword);
    
    if (tPtr)
    {
        delete tPtr;
        return true;
    }
    else
    {
        return false;
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
