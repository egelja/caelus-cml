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
    CML::DLPtrList

Description
    Non-intrusive doubly-linked pointer list.

\*---------------------------------------------------------------------------*/

#ifndef DLPtrList_H
#define DLPtrList_H

#include "LPtrList.hpp"
#include "DLListBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class DLPtrList Declaration
\*---------------------------------------------------------------------------*/

template<class T>
class DLPtrList
:
    public LPtrList<DLListBase, T>
{

public:

    // Constructors

        //- Null construct
        DLPtrList()
        {}

        //- Construct given initial T
        DLPtrList(T a)
        :
            LPtrList<DLListBase, T>(a)
        {}

        //- Construct from Istream using given Istream constructor class
        template<class INew>
        DLPtrList(Istream& is, const INew& inewt)
        :
            LPtrList<DLListBase, T>(is, inewt)
        {}

        //- Construct from Istream
        DLPtrList(Istream& is)
        :
            LPtrList<DLListBase, T>(is)
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
