/*---------------------------------------------------------------------------*\
Copyright (C) 2011 Symscape
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

#include "regExp.hpp"
#include "label.hpp"
#include "string.hpp"
#include "List.hpp"
#include "IOstreams.hpp"

// Alternative regular expression libraries to consider are:
// Boost http://www.boost.org/libs/regex/doc/
// GRETA http://research.microsoft.com/projects/greta/
// Henry Spencer's http://arglist.com/regex/
// 
// Chose DEELX http://www.regexlab.com/en/deelx/
// for its ease of integration - one header file
#include "deelx.h"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::regExp::regExp()
:
    preg_(0)
{}


CML::regExp::regExp(const char* pattern, const bool ignoreCase)
:
    preg_(0)
{
    set(pattern, ignoreCase);
}


CML::regExp::regExp(const std::string& pattern, const bool ignoreCase)
:
    preg_(0)
{
    set(pattern.c_str(), ignoreCase);
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::regExp::~regExp()
{
    clear();
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


void CML::regExp::set(const char* pattern, const bool ignoreCase) const
{
    clear();

    // Avoid nullptr and zero-length patterns
    if (pattern && *pattern)
    {
        int cflags = EXTENDED;
        if (ignoreCase)
        {
            cflags |= IGNORECASE;
        }

        preg_ = new CRegexpT<char>(pattern, cflags);
    }
}


void CML::regExp::set(const std::string& pattern, const bool ignoreCase) const
{
    return set(pattern.c_str(), ignoreCase);
}


bool CML::regExp::clear() const
{
    if (preg_)
    {
        delete preg_;
        preg_ = 0;

        return true;
    }

    return false;
}


std::string::size_type CML::regExp::find(const std::string& str) const
{
    std::string::size_type pos = std::string::npos;

    if (preg_ && !str.empty())
    {
        const MatchResult result = preg_->Match(str.c_str());

        if (0 < result.IsMatched())
        {
            pos = result.GetStart();
        }
    }

    return pos;
}


bool CML::regExp::match(const std::string& str) const
{
    bool isExactMatch = false;


    if (preg_ && !str.empty())
    {
        const MatchResult result = preg_->MatchExact(str.c_str());
        isExactMatch = (0 < result.IsMatched());
    }

    return isExactMatch;
}


bool CML::regExp::match(const string& str, List<string>& groups) const
{
    bool isMatch = false;

    if (preg_ && !str.empty())
    {
        const MatchResult results = preg_->MatchExact(str.c_str());
        isMatch = (0 < results.IsMatched());

        if (isMatch) 
        {
            int const notFound = -1;
            int start, end;
            const int groupsCount = results.MaxGroupNumber();
            groups.setSize(groupsCount);
        
            for (int i = 0; groupsCount > i; ++i)
            {
                start = results.GetGroupStart(i);
                end   = results.GetGroupEnd(i);
                  
                if ((notFound < start) && (notFound < end))
                {
                    groups[i] = str.substr(start, end - start);
                }
                else
                {
                    groups[i].clear();
                }
            }
        }
    }

    if (!isMatch)
    {
        groups.clear();
    }

    return isMatch;
}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //

void CML::regExp::operator=(const char* pat)
{
    set(pat);
}


void CML::regExp::operator=(const std::string& pat)
{
    set(pat);
}


// ************************************************************************* //
