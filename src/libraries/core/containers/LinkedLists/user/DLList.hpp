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
    CML::DLList

Description
    Non-intrusive doubly-linked list.

\*---------------------------------------------------------------------------*/

#ifndef DLList_H
#define DLList_H

#include "LList.hpp"
#include "DLListBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class DLList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class DLList
:
    public LList<DLListBase, T>
{

public:

    // Constructors

        //- Null construct
        DLList()
        {}

        //- Construct given initial T
        DLList(T a)
        :
            LList<DLListBase, T>(a)
        {}

        //- Construct from Istream
        DLList(Istream& is)
        :
            LList<DLListBase, T>(is)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
