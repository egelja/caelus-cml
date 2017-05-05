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
    CML::SLList

Description
    Non-intrusive singly-linked list.

\*---------------------------------------------------------------------------*/

#ifndef SLList_H
#define SLList_H

#include "LList.hpp"
#include "SLListBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class SLList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class SLList
:
    public LList<SLListBase, T>
{

public:

    // Constructors

        //- Null construct
        SLList()
        {}

        //- Construct given initial T
        SLList(T a)
        :
            LList<SLListBase, T>(a)
        {}

        //- Construct from Istream
        SLList(Istream& is)
        :
            LList<SLListBase, T>(is)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
