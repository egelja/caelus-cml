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

#include "entry.hpp"
#include "dictionary.hpp"
#include "OStringStream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

int CML::entry::disableFunctionEntries
(
    CML::debug::infoSwitch("disableFunctionEntries", 0)
);


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::entry::entry(const keyType& keyword)
:
    IDLList<entry>::link(),
    keyword_(keyword)
{}


CML::entry::entry(const entry& e)
:
    IDLList<entry>::link(),
    keyword_(e.keyword_)
{}


CML::autoPtr<CML::entry> CML::entry::clone() const
{
    return clone(dictionary::null);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::entry::operator=(const entry& e)
{
    // check for assignment to self
    if (this == &e)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    keyword_ = e.keyword_;
}


bool CML::entry::operator==(const entry& e) const
{
    if (keyword_ != e.keyword_)
    {
        return false;
    }
    else
    {
        OStringStream oss1;
        oss1 << *this;

        OStringStream oss2;
        oss2 << e;

        return oss1.str() == oss2.str();
    }
}


bool CML::entry::operator!=(const entry& e) const
{
    return !operator==(e);
}


// ************************************************************************* //
