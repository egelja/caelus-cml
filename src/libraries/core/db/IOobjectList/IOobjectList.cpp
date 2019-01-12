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

\*---------------------------------------------------------------------------*/

#include "IOobjectList.hpp"
#include "Time.hpp"
#include "OSspecific.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::IOobjectList::IOobjectList(const label nIoObjects)
:
    HashPtrTable<IOobject>(nIoObjects)
{}


CML::IOobjectList::IOobjectList
(
    const objectRegistry& db,
    const fileName& instance,
    const fileName& local,
    IOobject::readOption r,
    IOobject::writeOption w,
    bool registerObject
)
:
    HashPtrTable<IOobject>()
{
    word newInstance = instance;

    if (!isDir(db.path(instance)))
    {
        newInstance = db.time().findInstancePath(instant(instance));

        if (newInstance.empty())
        {
            return;
        }
    }

    // Create a list of file names in this directory
    fileNameList ObjectNames =
        readDir(db.path(newInstance, db.dbDir()/local), fileName::FILE);

    forAll(ObjectNames, i)
    {
        IOobject* objectPtr = new IOobject
        (
            ObjectNames[i],
            newInstance,
            local,
            db,
            r,
            w,
            registerObject
        );

        if (objectPtr->headerOk())
        {
            insert(ObjectNames[i], objectPtr);
        }
        else
        {
            delete objectPtr;
        }
    }
}


CML::IOobjectList::IOobjectList(const IOobjectList& ioOL)
:
    HashPtrTable<IOobject>(ioOL)
{}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

CML::IOobjectList::~IOobjectList()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::IOobjectList::add(IOobject& io)
{
    return insert(io.name(), &io);
}


bool CML::IOobjectList::remove(IOobject& io)
{
    HashPtrTable<IOobject>::iterator iter =
        HashPtrTable<IOobject>::find(io.name());

    if (iter != end())
    {
        return erase(iter);
    }
    else
    {
        return false;
    }
}


CML::IOobject* CML::IOobjectList::lookup(const word& name) const
{
    HashPtrTable<IOobject>::const_iterator iter = find(name);

    if (iter != end())
    {
        if (IOobject::debug)
        {
            Info<< "IOobjectList::lookup : found "
                << name << endl;
        }

        return const_cast<IOobject*>(*iter);
    }
    else
    {
        if (IOobject::debug)
        {
            Info<< "IOobjectList::lookup : could not find "
                << name << endl;
        }

        return nullptr;
    }
}


CML::IOobjectList CML::IOobjectList::lookup(const wordRe& name) const
{
    IOobjectList objectsOfName(size());

    forAllConstIter(HashPtrTable<IOobject>, *this, iter)
    {
        if (name.match(iter()->name()))
        {
            if (IOobject::debug)
            {
                Info<< "IOobjectList::lookupRe : found " << iter.key() << endl;
            }

            objectsOfName.insert(iter.key(), new IOobject(*iter()));
        }
    }

    return objectsOfName;
}


CML::IOobjectList CML::IOobjectList::lookup(const wordReList& patterns) const
{
    wordReListMatcher names(patterns);

    IOobjectList objectsOfName(size());

    forAllConstIter(HashPtrTable<IOobject>, *this, iter)
    {
        if (names.match(iter()->name()))
        {
            if (IOobject::debug)
            {
                Info<< "IOobjectList::lookupRe : found " << iter.key() << endl;
            }

            objectsOfName.insert(iter.key(), new IOobject(*iter()));
        }
    }

    return objectsOfName;
}


CML::IOobjectList CML::IOobjectList::lookupClass(const word& ClassName) const
{
    IOobjectList objectsOfClass(size());

    forAllConstIter(HashPtrTable<IOobject>, *this, iter)
    {
        if (iter()->headerClassName() == ClassName)
        {
            if (IOobject::debug)
            {
                Info<< "IOobjectList::lookupClass : found "
                    << iter.key() << endl;
            }

            objectsOfClass.insert(iter.key(), new IOobject(*iter()));
        }
    }

    return objectsOfClass;
}


CML::wordList CML::IOobjectList::names() const
{
    return HashPtrTable<IOobject>::toc();
}


CML::wordList CML::IOobjectList::sortedNames() const
{
    return HashPtrTable<IOobject>::sortedToc();
}


CML::wordList CML::IOobjectList::names(const word& ClassName) const
{
    wordList objectNames(size());

    label count = 0;
    forAllConstIter(HashPtrTable<IOobject>, *this, iter)
    {
        if (iter()->headerClassName() == ClassName)
        {
            objectNames[count++] = iter.key();
        }
    }

    objectNames.setSize(count);

    return objectNames;
}


CML::wordList CML::IOobjectList::sortedNames(const word& ClassName) const
{
    wordList sortedLst = names(ClassName);
    sort(sortedLst);

    return sortedLst;
}


// ************************************************************************* //
