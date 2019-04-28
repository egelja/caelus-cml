/*---------------------------------------------------------------------------*\
Copyright (C) 2009-2016 Bernhard Gschaider
Copyright (C) 2016-2017 OpenCFD Ltd.
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
#include "profilingInformation.hpp"
#include "profilingSysInfo.hpp"
#include "demandDrivenData.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

CML::profiling* CML::profiling::pool_(0);


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

CML::profilingInformation* CML::profiling::find(const string& name)
{
    StorageContainer::iterator iter = hash_.find(name);
    return (iter != hash_.end() ? iter() : 0);
}


CML::profilingInformation* CML::profiling::store(profilingInformation *info)
{
    hash_.insert(info->description(), info);
    return info;
}


void CML::profiling::push(profilingInformation *info, clockTime& timer)
{
    stack_.push(info);
    timers_.set(info->id(), &timer);
    info->push();                       // mark as on stack
}


CML::profilingInformation* CML::profiling::pop()
{
    profilingInformation *info = stack_.pop();
    timers_.erase(info->id());
    info->pop();                        // mark as off stack

    return info;
}


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

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

        profilingInformation *info = pool_->store
        (
            new profilingInformation()
        );

        pool_->push(info, pool_->clockTime_);
        Info<< "profiling initialized" << nl;
    }

    // silently ignore multiple initializations
    // eg, decomposePar uses multiple simultaneous Times
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

        profilingInformation *info = pool_->store
        (
            new profilingInformation()
        );

        pool_->push(info, pool_->clockTime_);
        Info<< "profiling initialized" << nl;
    }

    // silently ignore multiple initializations
    // eg, decomposePar uses multiple simultaneous Times
}


void CML::profiling::stop(const Time& owner)
{
    if (pool_ && &owner == &(pool_->owner_))
    {
        delete pool_;
        pool_ = 0;
    }
}


CML::profilingInformation* CML::profiling::New
(
    const string& descr,
    clockTime& timer
)
{
    profilingInformation *info = 0;

    if (pool_)
    {
        profilingInformation *parent = pool_->stack_.top();

        info = pool_->find(descr);
        if (!info)
        {
            info = pool_->store(new profilingInformation(descr, parent));
        }

        pool_->push(info, timer);
    }

    return info;
}


void CML::profiling::unstack(const profilingInformation *info)
{
    if (pool_ && info)
    {
        profilingInformation *top = pool_->pop();

        if (info->id() != top->id())
        {
            FatalErrorInFunction
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
    sysInfo_(new profilingSysInfo())
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
      ? new profilingSysInfo() : 0
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::profiling::~profiling()
{
    deleteDemandDrivenData(sysInfo_);

    if (pool_ == this)
    {
        pool_ = 0;
        profilingInformation::nextId_ = 0;
    }
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
            const profilingInformation *info = *iter;
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
            const profilingInformation *info = iter();

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


// ************************************************************************* //
