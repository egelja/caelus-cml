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
    CML::UIDLList

Description
    Intrusive doubly-linked list.

\*---------------------------------------------------------------------------*/

#ifndef UIDLList_H
#define UIDLList_H

#include "UILList.hpp"
#include "DLListBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class UIDLList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class UIDLList
:
    public UILList<DLListBase, T>
{

public:

    // Constructors

        //- Null construct
        UIDLList()
        {}

        //- Construct given initial T
        UIDLList(T a)
        :
            UILList<DLListBase, T>(a)
        {}

        //- Construct from Istream
        UIDLList(Istream& is)
        :
            UILList<DLListBase, T>(is)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
