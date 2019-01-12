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

#include "profilingInformation.hpp"
#include "Switch.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

CML::label CML::profilingInformation::nextId_(0);


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


CML::label CML::profilingInformation::getNextId()
{
    return nextId_++;
}


void CML::profilingInformation::raiseId(label maxVal)
{
    if (nextId_ < maxVal)
    {
        nextId_ = maxVal;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::profilingInformation::profilingInformation()
:
    id_(getNextId()),
    description_("application::main"),
    parent_(this),
    calls_(0),
    totalTime_(0),
    childTime_(0),
    onStack_(false)
{}


CML::profilingInformation::profilingInformation
(
    const string& descr,
    profilingInformation *parent
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


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::profilingInformation::~profilingInformation()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::profilingInformation::update(const scalar elapsed)
{
    ++calls_;
    totalTime_ += elapsed;

    if (id_ != parent().id())
    {
        parent().childTime_ += elapsed;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::profilingInformation::push() const
{
    onStack_ = true;
}


void CML::profilingInformation::pop() const
{
    onStack_ = false;
}


CML::Ostream& CML::profilingInformation::write
(
    Ostream& os,
    const bool offset,
    const scalar elapsedTime,
    const scalar childTimes
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
    const profilingInformation& info
)
{
    return info.write(os);
}


// ************************************************************************* //
