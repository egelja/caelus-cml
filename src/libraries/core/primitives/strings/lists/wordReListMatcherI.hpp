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


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::wordReListMatcher::wordReListMatcher
(
    const UList<wordRe>& lst
)
:
    reList_(lst)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::wordReListMatcher::size() const
{
    return reList_.size();
}


inline bool CML::wordReListMatcher::empty() const
{
    return reList_.empty();
}


inline const CML::UList<CML::wordRe>&
CML::wordReListMatcher::operator()() const
{
    return reList_;
}


inline bool CML::wordReListMatcher::match
(
    const string& str,
    bool literalMatch
) const
{
    const label nElem = reList_.size();
    for (label elemI = 0; elemI < nElem; ++elemI)
    {
        if (reList_[elemI].match(str, literalMatch))
        {
            return true;
        }
    }

    return false;
}


// ************************************************************************* //
