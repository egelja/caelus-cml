/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::objectRegistry

Description
    Registry of regIOobjects

SourceFiles
    objectRegistry.cpp

\*---------------------------------------------------------------------------*/

#ifndef objectRegistry_H
#define objectRegistry_H

#include "HashTable.hpp"
#include "regIOobject.hpp"
#include "wordReList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class objectRegistry Declaration
\*---------------------------------------------------------------------------*/

class objectRegistry
:
    public regIOobject,
    public HashTable<regIOobject*>
{
    // Private Data

        //- Master time objectRegistry
        const Time& time_;

        //- Parent objectRegistry
        const objectRegistry& parent_;

        //- Local directory path of this objectRegistry relative to time
        fileName dbDir_;

        //- Current event
        mutable label event_;


    // Private Member Functions

        //- Is the objectRegistry parent_ different from time_
        //  Used to terminate searching within the ancestors
        bool parentNotTime() const;

        //- Disallow Copy constructor
        objectRegistry(const objectRegistry&);

        //- Disallow default bitwise copy construct and assignment
        void operator=(const objectRegistry&);


public:

    //- Declare type name for this IOobject
    TypeName("objectRegistry");


    // Constructors

        //- Construct the time objectRegistry given an initial estimate
        //  for the number of entries
        explicit objectRegistry
        (
            const Time& db,
            const label nIoObjects = 128
        );

        //- Construct a sub-registry given an IObject to describe the registry
        //  and an initial estimate for the number of entries
        explicit objectRegistry
        (
            const IOobject& io,
            const label nIoObjects = 128
        );


    //- Destructor
    virtual ~objectRegistry();


    // Member functions

        // Access

            //- Return time
            const Time& time() const
            {
                return time_;
            }

            //- Return the parent objectRegistry
            const objectRegistry& parent() const
            {
                return parent_;
            }

            //- Local directory path of this objectRegistry relative to the time
            virtual const fileName& dbDir() const
            {
                return dbDir_;
            }

            //- Return the list of names of the IOobjects
            wordList names() const;

            //- Return the sorted list of names of the IOobjects
            wordList sortedNames() const;

            //- Return the list of names of IOobjects of given class name
            wordList names(const word& className) const;

            //- Return the sorted list of names of IOobjects of given class name
            wordList sortedNames(const word& className) const;

            //- Return the list of names of the IOobjects of given type
            template<class Type>
            wordList names() const;

            //- Return the list of objects whose name matches the input regExp
            template<class Type>
            wordList names(const wordRe& name) const;

            //- Return the list of objects whose name matches the input regExp
            template<class Type>
            wordList names(const wordReList& name) const;

            //- Lookup and return a const sub-objectRegistry. Optionally create
            //  it if it does not exist.
            const objectRegistry& subRegistry
            (
                const word& name,
                const bool forceCreate = false
            ) const;

            //- Lookup and return all objects of the given Type
            template<class Type>
            HashTable<const Type*> lookupClass(const bool strict = false) const;

            //- Lookup and return all objects of the given Type
            template<class Type>
            HashTable<Type*> lookupClass(const bool strict = false);

            //- Is the named Type found?
            template<class Type>
            bool foundObject(const word& name) const;

            //- Lookup and return the object of the given Type
            template<class Type>
            const Type& lookupObject(const word& name) const;

            //- Return new event number.
            label getEvent() const;


        // Edit

            //- Rename
            virtual void rename(const word& newName);

            //- Add an regIOobject to registry
            bool checkIn(regIOobject&) const;

            //- Remove an regIOobject from registry
            bool checkOut(regIOobject&) const;

        // Reading

            //- Return true if any of the object's files have been modified
            virtual bool modified() const;

            //- Read the objects that have been modified
            void readModifiedObjects();

            //- Read object if modified
            virtual bool readIfModified();


        // Writing

            //- writeData function required by regIOobject but not used
            //  for this class, write is used instead
            virtual bool writeData(Ostream&) const
            {
                NotImplemented;

                return false;
            }

            //- Write the objects
            virtual bool writeObject
            (
                IOstream::streamFormat fmt,
                IOstream::versionNumber ver,
                IOstream::compressionType cmp
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "stringListOps.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::wordList
CML::objectRegistry::names() const
{
    wordList objectNames(size());

    label count=0;
    forAllConstIter(HashTable<regIOobject*>, *this, iter)
    {
        if (isA<Type>(*iter()))
        {
            objectNames[count++] = iter()->name();
        }
    }

    objectNames.setSize(count);

    return objectNames;
}


template<class Type>
CML::wordList
CML::objectRegistry::names(const wordRe& name) const
{
    wordList objectNames(size());

    label count = 0;
    forAllConstIter(HashTable<regIOobject*>, *this, iter)
    {
        if (isA<Type>(*iter()))
        {
            const word& objectName = iter()->name();

            if (name.match(objectName))
            {
                objectNames[count++] = objectName;
            }
        }
    }

    objectNames.setSize(count);

    return objectNames;
}


template<class Type>
CML::wordList
CML::objectRegistry::names(const wordReList& patterns) const
{
    wordList names(this->names<Type>());

    return wordList(names, findStrings(patterns, names));
}


template<class Type>
CML::HashTable<const Type*> CML::objectRegistry::lookupClass
(
    const bool strict
) const
{
    HashTable<const Type*> objectsOfClass(size());

    forAllConstIter(HashTable<regIOobject*>, *this, iter)
    {
        if
        (
            (strict && isType<Type>(*iter()))
         || (!strict && isA<Type>(*iter()))
        )
        {
            objectsOfClass.insert
            (
                iter()->name(),
                dynamic_cast<const Type*>(iter())
            );
        }
    }

    return objectsOfClass;
}


template<class Type>
CML::HashTable<Type*> CML::objectRegistry::lookupClass
(
    const bool strict
)
{
    HashTable<Type*> objectsOfClass(size());

    forAllIter(HashTable<regIOobject*>, *this, iter)
    {
        if
        (
            (strict && isType<Type>(*iter()))
         || (!strict && isA<Type>(*iter()))
        )
        {
            objectsOfClass.insert
            (
                iter()->name(),
                dynamic_cast<Type*>(iter())
            );
        }
    }

    return objectsOfClass;
}


template<class Type>
bool CML::objectRegistry::foundObject(const word& name) const
{
    const_iterator iter = find(name);

    if (iter != end())
    {
        const Type* vpsiPtr_ = dynamic_cast<const Type*>(iter());

        if (vpsiPtr_)
        {
            return true;
        }
    }
    else if (this->parentNotTime())
    {
        return parent_.foundObject<Type>(name);
    }

    return false;
}


template<class Type>
const Type& CML::objectRegistry::lookupObject(const word& name) const
{
    const_iterator iter = find(name);

    if (iter != end())
    {
        const Type* vpsiPtr_ = dynamic_cast<const Type*>(iter());

        if (vpsiPtr_)
        {
            return *vpsiPtr_;
        }

        FatalErrorInFunction
            << nl
            << "    lookup of " << name << " from objectRegistry "
            << this->name()
            << " successful\n    but it is not a " << Type::typeName
            << ", it is a " << iter()->type()
            << abort(FatalError);
    }
    else
    {
        if (this->parentNotTime())
        {
            return parent_.lookupObject<Type>(name);
        }

        FatalErrorInFunction
            << nl
            << "    request for " << Type::typeName
            << " " << name << " from objectRegistry " << this->name()
            << " failed\n    available objects of type " << Type::typeName
            << " are" << nl
            << names<Type>()
            << abort(FatalError);
    }

    return NullSingletonRef< Type >();
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
