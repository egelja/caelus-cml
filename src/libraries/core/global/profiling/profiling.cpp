/*---------------------------------------------------------------------------*\
Copyright (C) 2009-2016 Bernhard Gschaider
Copyright (C) 2016 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "profiling.hpp"
#include "profilingSysInfo.hpp"
#include "OSspecific.hpp"
#include "IOstreams.hpp"
#include "dictionary.hpp"
#include "demandDrivenData.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

CML::profiling* CML::profiling::pool_(0);

CML::label CML::profiling::Information::nextId_(0);


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

// File-scope function
template<class T>
inline static void writeEntry
(
    CML::Ostream& os, const CML::word& key, const T& value
)
{
    os.writeKeyword(key) << value << CML::token::END_STATEMENT << '\n';
}


CML::label CML::profiling::Information::getNextId()
{
    return nextId_++;
}


void CML::profiling::Information::raiseID(label maxVal)
{
    if (nextId_ < maxVal)
    {
        nextId_ = maxVal;
    }
}


bool CML::profiling::active()
{
    return pool_;
}


bool CML::profiling::print(Ostream& os)
{
    if (pool_)
    {
        return pool_->writeData(os);
    }
    else
    {
        return false;
    }
}


bool CML::profiling::writeNow()
{
    if (pool_)
    {
        return pool_->write();
    }
    else
    {
        return false;
    }
}


void CML::profiling::initialize
(
    const IOobject& ioObj,
    const Time& owner
)
{
    if (!pool_)
    {
        pool_ = new profiling(ioObj, owner);

        Information *info = pool_->store
        (
            new Information()
        );

        pool_->push(info, pool_->clockTime_);
        Info<< "profiling initialized" << nl;
    }

    // silently ignore multiple initialization
    // eg, decomposePar use multiple simultaneous Times
}


void CML::profiling::initialize
(
    const dictionary& dict,
    const IOobject& ioObj,
    const Time& owner
)
{
    if (!pool_)
    {
        pool_ = new profiling(dict, ioObj, owner);

        Information *info = pool_->store
        (
            new Information()
        );

        pool_->push(info, pool_->clockTime_);
        Info<< "profiling initialized" << nl;
    }

    // silently ignore multiple initialization
    // eg, decomposePar use multiple simultaneous Times
}


void CML::profiling::stop(const Time& owner)
{
    if (pool_ && &owner == &(pool_->owner_))
    {
        delete pool_;
        pool_ = 0;
    }
}


CML::profiling::Information* CML::profiling::New
(
    const string& name,
    clockTime& timer
)
{
    Information *info = 0;

    if (pool_)
    {
        info = pool_->find(name);
        if (!info)
        {
            info = pool_->store
            (
                new Information(pool_->stack_.top(), name)
            );
        }

        pool_->push(info, timer);
    }

    return info;
}


void CML::profiling::unstack(const Information *info)
{
    if (pool_ && info)
    {
        Information *top = pool_->pop();

        if (info->id() != top->id())
        {
            FatalErrorIn("CML::profiling::unstack(const Information *info)")
                << "The profiling information to unstack has different"
                << " id than on the top of the profiling stack" << nl
                << "  info: " << info->id() << " (" << info->description()
                << ")\n"
                << "  top:  " << top->id()  << " (" << top->description()
                << ")\n" << endl
                << abort(FatalError);
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::profiling::profiling
(
    const IOobject& io,
    const Time& owner
)
:
    regIOobject(io),
    owner_(owner),
    clockTime_(),
    hash_(),
    stack_(),
    timers_(),
    sysInfo_(new sysInfo())
{}


CML::profiling::profiling
(
    const dictionary& dict,
    const IOobject& io,
    const Time& owner
)
:
    regIOobject(io),
    owner_(owner),
    clockTime_(),
    hash_(),
    stack_(),
    timers_(),
    sysInfo_
    (
        dict.lookupOrDefault<Switch>("sysInfo", true)
      ? new sysInfo() : 0
    )
{}


CML::profiling::Information::Information()
:
    id_(getNextId()),
    description_("application::main"),
    parent_(this),
    calls_(0),
    totalTime_(0),
    childTime_(0),
    onStack_(false)
{}


CML::profiling::Information::Information
(
    Information *parent,
    const string& descr
)
:
    id_(getNextId()),
    description_(descr),
    parent_(parent),
    calls_(0),
    totalTime_(0),
    childTime_(0),
    onStack_(false)
{}


CML::profiling::Trigger::Trigger(const char* name)
:
    clock_(),
    ptr_(profiling::New(name, clock_))
{}


CML::profiling::Trigger::Trigger(const string& name)
:
    clock_(),
    ptr_(profiling::New(name, clock_))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::profiling::~profiling()
{
    deleteDemandDrivenData(sysInfo_);

    if (pool_ == this)
    {
        pool_ = 0;
        Information::nextId_ = 0;
    }
}


CML::profiling::Information::~Information()
{}


CML::profiling::Trigger::~Trigger()
{
    stop();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

const CML::Time& CML::profiling::owner() const
{
    return owner_;
}

CML::label CML::profiling::size() const
{
    return stack_.size();
}


CML::profiling::Information* CML::profiling::find(const string& name)
{
    StorageContainer::iterator iter = hash_.find(name);
    return (iter != hash_.end() ? iter() : 0);
}


void CML::profiling::Information::update(const scalar& elapsed)
{
    ++calls_;
    totalTime_ += elapsed;

    if (id_ != parent().id())
    {
        parent().childTime_ += elapsed;
    }
}


bool CML::profiling::writeData(Ostream& os) const
{
    os  << indent << "profiling" << nl
        << indent << token::BEGIN_LIST << incrIndent << nl;

    // Add extra new line between entries
    label nTrigger = 0;

    // write on-stack items
    // newest is first on the stack, top-level is at the end
    // this is how the child times are summed
    {
        scalar oldElapsed = 0;
        forAllConstIter(StackContainer, stack_, iter)
        {
            const Information *info = *iter;
            scalar elapsed = timers_[info->id()]->elapsedTime();

            if (nTrigger++)
            {
                os << nl;
            }
            info->write(os, true, elapsed, oldElapsed);
            oldElapsed = elapsed;
        }
    }


    // write off-stack items
    // using an additional Map to sort by Id
    {
        typedef Map<const Information*> LookupContainer;
        LookupContainer lookup;

        forAllConstIter(StorageContainer, hash_, iter)
        {
            const Information *info = iter();

            if (!info->onStack())
            {
                lookup.set(info->id(), info);
            }
        }

        forAllConstIter(LookupContainer, lookup, iter)
        {
            if (nTrigger++)
            {
                os << nl;
            }
            iter()->write(os);
        }
    }

    os  << decrIndent
        << indent << token::END_LIST << token::END_STATEMENT << nl;


    if (sysInfo_)
    {
        os << nl;
        os << indent << "sysInfo" << nl
           << indent << token::BEGIN_BLOCK << incrIndent << nl;
        sysInfo_->write(os);
        os << decrIndent << indent << token::END_BLOCK << endl;
    }

    return os;
}


bool CML::profiling::writeObject
(
    IOstream::streamFormat,
    IOstream::versionNumber ver,
    IOstream::compressionType
) const
{
    return regIOobject::writeObject
    (
        IOstream::ASCII,
        ver,
        IOstream::UNCOMPRESSED
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::profiling::Information* CML::profiling::store(Information *info)
{
    hash_.insert(info->description(), info);
    return info;
}


void CML::profiling::push(Information *info, clockTime& timer)
{
    stack_.push(info);
    timers_.set(info->id(), &timer);
    info->push();                       // mark as on stack
}


CML::profiling::Information* CML::profiling::pop()
{
    Information *info = stack_.pop();
    timers_.erase(info->id());
    info->pop();                        // mark as off stack

    return info;
}


bool CML::profiling::Trigger::running() const
{
    return ptr_;
}


void CML::profiling::Trigger::stop()
{
    if (ptr_)
    {
        ptr_->update(clock_.elapsedTime());
        profiling::unstack(ptr_);
        // pointer is managed by pool storage -> thus no delete here
    }
    ptr_ = 0;
}


void CML::profiling::Information::push() const
{
    onStack_ = true;
}


void CML::profiling::Information::pop() const
{
    onStack_ = false;
}


CML::Ostream& CML::profiling::Information::write
(
    Ostream& os,
    const bool offset,
    const scalar& elapsedTime,
    const scalar& childTimes
) const
{
    // Write in dictionary format
    os << indent << token::BEGIN_BLOCK << incrIndent << nl;
    
    writeEntry(os, "id", id_);
    if (id_ != parent().id())
    {
        writeEntry(os, "parentId",  parent().id());
    }
    writeEntry(os, "description",   description());
    writeEntry(os, "calls",         calls()     + (offset ? 1 : 0));
    writeEntry(os, "totalTime",     totalTime() + elapsedTime);
    writeEntry(os, "childTime",     childTime() + childTimes);
    writeEntry(os, "onStack",       Switch(onStack()));

    os << decrIndent << indent << token::END_BLOCK << nl; 

    return os;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<
(
    Ostream& os,
    const profiling::Information& info
)
{
    return info.write(os);
}


// ************************************************************************* //
