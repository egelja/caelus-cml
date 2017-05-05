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
    CML::CallbackRegistry

Description
    Base class with which callbacks are registered.

    Derive from this class and extend by adding the appropriate callback
    functions that loop and call the callback functions for each entry.

\*---------------------------------------------------------------------------*/

#ifndef CallbackRegistry_H
#define CallbackRegistry_H

#include "UIDLList.hpp"
#include "className.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

template<class CallbackType>
class Callback;


/*---------------------------------------------------------------------------*\
                       Class CallbackRegistry Declaration
\*---------------------------------------------------------------------------*/

TemplateName(CallbackRegistry);


/*---------------------------------------------------------------------------*\
                       Class CallbackRegistry Declaration
\*---------------------------------------------------------------------------*/

template<class CallbackType>
class CallbackRegistry
:
    public CallbackRegistryName,
    public UIDLList<CallbackType>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        CallbackRegistry(const CallbackRegistry&);

        //- Disallow default bitwise assignment
        void operator=(const CallbackRegistry&);


public:

    // Constructors

        //- Construct null
        CallbackRegistry();


    //- Destructor
    virtual ~CallbackRegistry();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CallbackType>
CML::CallbackRegistry<CallbackType>::CallbackRegistry()
:
    CallbackRegistryName(),
    UIDLList<CallbackType>()
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CallbackType>
CML::CallbackRegistry<CallbackType>::~CallbackRegistry()
{
    forAllIter(typename CallbackRegistry<CallbackType>, *this, iter)
    {
        iter().Callback<CallbackType>::checkOut();
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
