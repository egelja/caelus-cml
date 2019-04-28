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

#include <sys/types.h>

#include "regExp.hpp"
#include "label.hpp"
#include "string.hpp"
#include "List.hpp"
#include "IOstreams.hpp"

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

    // avoid nullptr pointer and zero-length patterns
    if (pattern && *pattern)
    {
        preg_ = new regex_t;

        int cflags = REG_EXTENDED;
        if (ignoreCase)
        {
            cflags |= REG_ICASE;
        }

        int err = regcomp(preg_, pattern, cflags);
        
        if (err != 0)
        {
            char errbuf[200];
            regerror(err, preg_, errbuf, sizeof(errbuf));

            FatalErrorInFunction
                << "Failed to compile regular expression '" << pattern << "'"
                << nl << errbuf
                << exit(FatalError);
        }
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
        regfree(preg_);
        delete preg_;
        preg_ = 0;

        return true;
    }

    return false;
}


std::string::size_type CML::regExp::find(const std::string& str) const
{
    if (preg_ && str.size())
    {
        size_t nmatch = 1;
        regmatch_t pmatch[1];

        if (regexec(preg_, str.c_str(), nmatch, pmatch, 0) == 0)
        {
            return pmatch[0].rm_so;
        }
    }

    return string::npos;
}


bool CML::regExp::match(const std::string& str) const
{
    if (preg_ && str.size())
    {
        size_t nmatch = 1;
        regmatch_t pmatch[1];

        // also verify that the entire string was matched
        // pmatch[0] is the entire match
        if
        (
            regexec(preg_, str.c_str(), nmatch, pmatch, 0) == 0
         && (pmatch[0].rm_so == 0 && pmatch[0].rm_eo == label(str.size()))
        )
        {
            return true;
        }
    }

    return false;
}


bool CML::regExp::match(const string& str, List<string>& groups) const
{
    if (preg_ && str.size())
    {
        size_t nmatch = ngroups() + 1;
        regmatch_t pmatch[nmatch];

        // also verify that the entire string was matched
        // pmatch[0] is the entire match
        // pmatch[1..] are the (...) sub-groups
        if
        (
            regexec(preg_, str.c_str(), nmatch, pmatch, 0) == 0
         && (pmatch[0].rm_so == 0 && pmatch[0].rm_eo == label(str.size()))
        )
        {
            groups.setSize(ngroups());
            label groupI = 0;

            for (size_t matchI = 1; matchI < nmatch; matchI++)
            {
                if (pmatch[matchI].rm_so != -1 && pmatch[matchI].rm_eo != -1)
                {
                    groups[groupI] = str.substr
                    (
                        pmatch[matchI].rm_so,
                        pmatch[matchI].rm_eo - pmatch[matchI].rm_so
                    );
                }
                else
                {
                    groups[groupI].clear();
                }
                groupI++;
            }

            return true;
        }
    }

    groups.clear();
    return false;
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
