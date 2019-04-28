/*---------------------------------------------------------------------------*\
Copyright (C) 2011 David L. F. Gaden
Copyright (C) 2015-2018 Applied CCM
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
    CML::dbRegistrator

Description
    A wrapper class that allows you to retrieve non-IOobjects through the
    object registry.  Performs no input or output, simply for a const reference
    retrieval.

    Syntax:
    @verbatim
        // Your non-IOobject, e.g. Type = scalar
        Type myObject;
        
        // Register it to the object registry
        dbRegistrator<scalar> myIOobject
        (
            IOobject
            (
                "lookupName",
                instance,           // can be anything
                [local,]            // (optional) can be anything
                registry,           // can be anything
                IOobject::NO_READ,  // *must* be NO_READ
                IOobject::NO_WRITE  // *must* be NO_WRITE
            ),
            myObject
        );

        // (Now don't let myObject and myIOobject go out of scope)

        // In another part of your solver, e.g. in a custom boundary condition:
        Type const& myConstObject = registry.lookupObject<dbRegistrator<Type> >
        (
            "lookupName"
        )();

        // Now you have a local const reference to your non-IOobject
    @endverbatim

SourceFiles
    dbRegistrator.C

Author
    David L. F. Gaden

Modifications
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef dbRegistrator_HPP
#define dbRegistrator_HPP

#include "regIOobject.hpp"

namespace CML
{

template <typename Type>
class dbRegistrator : public regIOobject
{
    Type const* typePtr_;

public:

    // Construct from IOobject - pointer set to null
    dbRegistrator(IOobject const& io);

    // Construct from IOobject - pointer initialized
    dbRegistrator(IOobject const& io, Type const& reference);

    virtual ~dbRegistrator();

    //- A "do nothing" writeData function, required by regIOobject
    virtual bool writeData(Ostream&) const;
        
    //- Return the pointer as a reference
    Type const& operator()() const;
        
    //- Set the pointer
    void set(Type const& reference);

};
}

template <typename Type>
CML::dbRegistrator<Type>::dbRegistrator(IOobject const& io ) : regIOobject(io)
{
    if
    (
        (io.readOpt() != IOobject::NO_READ)
     || (io.writeOpt() != IOobject::NO_WRITE)
    )
    {
        FatalErrorInFunction
            << "dbRegistrator can only be NO_READ, NO_WRITE."
            << abort(FatalError);
    }
}


template <typename Type> CML::dbRegistrator<Type>::dbRegistrator
(
    IOobject const& io,
    Type const& reference
) :
    regIOobject(io),
    typePtr_(& reference)
{
    if
    (
        (io.readOpt() != IOobject::NO_READ)
     || (io.writeOpt() != IOobject::NO_WRITE)
    )
    {
        FatalErrorInFunction
            << "dbRegistrator can only be NO_READ, NO_WRITE."
            << abort(FatalError);
    }
}

template <typename Type>
CML::dbRegistrator<Type>::~dbRegistrator()
{}

template<typename Type>
bool CML::dbRegistrator<Type>::writeData(Ostream& os) const
{
    // do nothing
    return os.good();
}


template <typename Type>
Type const& CML::dbRegistrator<Type>::operator()() const
{
    if (!typePtr_)
    {
        FatalErrorInFunction
            << "Attempting to derefence a null typePtr - use dbRegistrator::set"
            << "first."
            << abort(FatalError);
    }
    return * typePtr_;
}


template <typename Type>
void CML::dbRegistrator<Type>::set
(
    Type const& reference
)
{
    typePtr_ = &reference;
}

#endif

