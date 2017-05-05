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
    CML::Callback

Description
    Abstract class to handle automatic call-back registration with the
    CallbackRegistry.  Derive from this class and extend by adding the
    appropriate callback functions.

SourceFiles
    Callback.cpp

\*---------------------------------------------------------------------------*/

#ifndef Callback_H
#define Callback_H

#include "CallbackRegistry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class Callback Declaration
\*---------------------------------------------------------------------------*/

template<class CallbackType>
class Callback
:
    public CallbackRegistry<CallbackType>::link
{
    // Private data

        //- CallbackRegistry reference
        CallbackRegistry<CallbackType>& cbr_;


    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const Callback<CallbackType>&);


public:

    // Constructors

        //- Construct and register with CallbackRegistry
        Callback(CallbackRegistry<CallbackType>&);

        //- Construct as copy
        Callback(const Callback<CallbackType>&);


    //- Destructor
    virtual ~Callback();


    // Member functions

        // Registration

            //- Register Callback with CallbackRegistry
            bool checkIn();

            //- Check-out Callback from CallbackRegistry
            bool checkOut();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CallbackType>
CML::Callback<CallbackType>::Callback(CallbackRegistry<CallbackType>& cbr)
:
    cbr_(cbr)
{
    checkIn();
}


template<class CallbackType>
CML::Callback<CallbackType>::Callback(const Callback<CallbackType>& cb)
:
    cbr_(cb.cbr_)
{
    checkIn();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CallbackType>
CML::Callback<CallbackType>::~Callback()
{
    checkOut();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class CallbackType>
bool CML::Callback<CallbackType>::checkIn()
{
    if (!Callback<CallbackType>::link::registered())
    {
        cbr_.append(static_cast<CallbackType*>(this));
        return true;
    }
    else
    {
        return false;
    }
}


template<class CallbackType>
bool CML::Callback<CallbackType>::checkOut()
{
    if (Callback<CallbackType>::link::registered())
    {
        CallbackType* cbPtr = cbr_.remove(static_cast<CallbackType*>(this));

        if (cbPtr)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
