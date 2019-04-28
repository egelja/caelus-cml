/*---------------------------------------------------------------------------*\
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

\*---------------------------------------------------------------------------*/

#include "IOobject.hpp"
#include "Time.hpp"
#include "IFstream.hpp"
#include "StaticHashTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(IOobject, 0);
}


static CML::StaticHashTable<CML::word> replacedFileNames_;

// * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * * //

// Return components following the IOobject requirements
//
// behaviour
//    input               IOobject(instance, local, name)
//    -----               ------
//    "foo"               ("", "", "foo")
//    "foo/bar"           ("foo", "", "bar")
//    "/XXX"              ERROR - no absolute path
//    "foo/bar/"          ERROR - no name
//    "foo/xxx/bar"       ("foo", "xxx", "bar")
//    "foo/xxx/yyy/bar"   ("foo", "xxx/yyy", "bar")
bool CML::IOobject::fileNameComponents
(
    const fileName& path,
    fileName& instance,
    fileName& local,
    word& name
)
{
    instance.clear();
    local.clear();
    name.clear();

    // called with directory
    if (isDir(path))
    {
        WarningInFunction
            << " called with directory: " << path << endl;

        return false;
    }

    if (path.isAbsolute())
    {
        string::size_type last = path.rfind('/');
        instance = path.substr(0, last);

        // check afterwards
        name.string::operator=(path.substr(last+1));
    }
    else
    {
        string::size_type first = path.find('/');

        if (first == string::npos)
        {
            // no '/' found - no instance or local

            // check afterwards
            name.string::operator=(path);
        }
        else
        {
            instance = path.substr(0, first);

            string::size_type last = path.rfind('/');
            if (last > first)
            {
                // with local
                local = path.substr(first+1, last-first-1);
            }

            // check afterwards
            name.string::operator=(path.substr(last+1));
        }
    }


    // check for valid (and stripped) name, regardless of the debug level
    if (name.empty() || string::stripInvalid<word>(name))
    {
        WarningInFunction
            << "has invalid word for name: \"" << name
            << "\"\nwhile processing path: " << path << endl;

        return false;
    }

    return true;
}


CML::word CML::IOobject::group(const word& name)
{
    word::size_type i = name.find_last_of('.');

    if (i == word::npos || i == 0)
    {
        return word::null;
    }
    else
    {
        return name.substr(i+1, word::npos);
    }
}


CML::word CML::IOobject::member(const word& name)
{
    word::size_type i = name.find_last_of('.');

    if (i == word::npos || i == 0)
    {
        return name;
    }
    else
    {
        return name.substr(0, i);
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::IOobject::IOobject
(
    const word& name,
    const fileName& instance,
    const objectRegistry& registry,
    readOption ro,
    writeOption wo,
    bool registerObject
)
:
    name_(name),
    headerClassName_(typeName),
    note_(),
    instance_(instance),
    local_(),
    db_(registry),
    rOpt_(ro),
    wOpt_(wo),
    registerObject_(registerObject),
    objState_(GOOD)
{
    if (objectRegistry::debug)
    {
        Info<< "Constructing IOobject called " << name_
            << " of type " << headerClassName_
            << endl;
    }
}


CML::IOobject::IOobject
(
    const word& name,
    const fileName& instance,
    const fileName& local,
    const objectRegistry& registry,
    readOption ro,
    writeOption wo,
    bool registerObject
)
:
    name_(name),
    headerClassName_(typeName),
    note_(),
    instance_(instance),
    local_(local),
    db_(registry),
    rOpt_(ro),
    wOpt_(wo),
    registerObject_(registerObject),
    objState_(GOOD)
{
    if (objectRegistry::debug)
    {
        Info<< "Constructing IOobject called " << name_
            << " of type " << headerClassName_
            << endl;
    }
}


CML::IOobject::IOobject
(
    const fileName& path,
    const objectRegistry& registry,
    readOption ro,
    writeOption wo,
    bool registerObject
)
:
    name_(),
    headerClassName_(typeName),
    note_(),
    instance_(),
    local_(),
    db_(registry),
    rOpt_(ro),
    wOpt_(wo),
    registerObject_(registerObject),
    objState_(GOOD)
{
    if (!fileNameComponents(path, instance_, local_, name_))
    {
        FatalErrorInFunction
            << " invalid path specification"
            << exit(FatalError);
    }

    if (objectRegistry::debug)
    {
        Info<< "Constructing IOobject called " << name_
            << " of type " << headerClassName_
            << endl;
    }
}


// * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * * //

CML::IOobject::~IOobject()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::objectRegistry& CML::IOobject::db() const
{
    return db_;
}


const CML::Time& CML::IOobject::time() const
{
    return db_.time();
}


const CML::fileName& CML::IOobject::caseName() const
{
    return time().caseName();
}

CML::word CML::IOobject::group() const
{
    return group(name_);
}


CML::word CML::IOobject::member() const
{
    return member(name_);
}


const CML::fileName& CML::IOobject::rootPath() const
{
    return time().rootPath();
}


CML::fileName CML::IOobject::path() const
{
    if (instance().isAbsolute())
    {
        return instance();
    }
    else
    {
        return rootPath()/caseName()/instance()/db_.dbDir()/local();
    }
}


CML::fileName CML::IOobject::path
(
    const word& instance,
    const fileName& local
) const
{
    // Note: can only be called with relative instance since is word type
    return rootPath()/caseName()/instance/db_.dbDir()/local;
}


void CML::IOobject::replaceFileName(const CML::word & from, 
                                     const CML::word & to)
{
    replacedFileNames_.insert(from, to);
}


const CML::word & CML::IOobject::uniqueFileName() const
{
    StaticHashTable<word>::const_iterator findIt = 
      replacedFileNames_.find(name());

    const word & diskFileName = (findIt == replacedFileNames_.end()) ?
      name() : *findIt;

    return diskFileName;
}


CML::fileName CML::IOobject::filePath() const
{
    if (instance().isAbsolute())
    {
        fileName objectPath = instance()/uniqueFileName();
        if (isFile(objectPath))
        {
            if (objectRegistry::debug)
            {
                Pout<< "filePath : returning absolute:" << objectPath
                    << endl;
            }
            return objectPath;
        }
        else
        {
            if (objectRegistry::debug)
            {
                Pout<< "filePath : absolute not found:" << objectPath
                    << endl;
            }
            return fileName::null;
        }
    }
    else
    {
        fileName path = this->path();
        fileName objectPath = path/uniqueFileName();

        if (isFile(objectPath))
        {
            if (objectRegistry::debug)
            {
                Pout<< "filePath : returning time:" << objectPath << endl;
            }
            return objectPath;
        }
        else
        {
            if
            (
                time().processorCase()
             && (
                    instance() == time().system()
                 || instance() == time().constant()
                )
            )
            {
                fileName parentObjectPath =
                    rootPath()/time().globalCaseName()
                   /instance()/db_.dbDir()/local()/uniqueFileName();

                if (isFile(parentObjectPath))
                {
                    if (objectRegistry::debug)
                    {
                        Pout<< "filePath : returning parent:"
                            << parentObjectPath << endl;
                    }
                    return parentObjectPath;
                }
            }

            if (!isDir(path))
            {
                word newInstancePath = time().findInstancePath
                (
                    instant(instance())
                );

                if (newInstancePath.size())
                {
                    fileName fName
                    (
                        rootPath()/caseName()
                       /newInstancePath/db_.dbDir()/local()/uniqueFileName()
                    );

                    if (isFile(fName))
                    {
                        if (objectRegistry::debug)
                        {
                            Pout<< "filePath : returning similar time:"
                                << fName << endl;
                        }
                        return fName;
                    }
                }
            }
        }

        return fileName::null;
    }
}


CML::Istream* CML::IOobject::objectStream()
{
    return objectStream(filePath());
}


CML::Istream* CML::IOobject::objectStream(const fileName& fName)
{
    if (fName.size())
    {
        IFstream* isPtr = new IFstream(fName);

        if (isPtr->good())
        {
            return isPtr;
        }
        else
        {
            delete isPtr;
            return nullptr;
        }
    }
    else
    {
        return nullptr;
    }
}


bool CML::IOobject::headerOk()
{
    bool ok = true;

    Istream* isPtr = objectStream();

    // If the stream has failed return
    if (!isPtr)
    {
        if (objectRegistry::debug)
        {
            Info
                << "IOobject::headerOk() : "
                << "file " << objectPath() << " could not be opened"
                << endl;
        }

        ok = false;
    }
    else
    {
        // Try reading header
        if (!readHeader(*isPtr))
        {
            if (objectRegistry::debug)
            {
                IOWarningInFunction((*isPtr))
                    << "failed to read header of file " << objectPath()
                    << endl;
            }

            ok = false;
        }
    }

    delete isPtr;

    return ok;
}


void CML::IOobject::setBad(const string& s)
{
    if (objState_ != GOOD)
    {
        FatalErrorInFunction
            << "recurrent failure for object " << s
            << exit(FatalError);
    }

    if (error::level)
    {
        Info<< "IOobject::setBad(const string&) : "
            << "broken object " << s << info() << endl;
    }

    objState_ = BAD;
}


void CML::IOobject::operator=(const IOobject& io)
{
    name_ = io.name_;
    headerClassName_ = io.headerClassName_;
    note_ = io.note_;
    instance_ = io.instance_;
    local_ = io.local_;
    rOpt_ = io.rOpt_;
    wOpt_ = io.wOpt_;
    objState_ = io.objState_;
}


// ************************************************************************* //
