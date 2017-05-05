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

inline CML::keyType::keyType()
:
    word(),
    isPattern_(false)
{}


inline CML::keyType::keyType(const keyType& s)
:
    word(s, false),
    isPattern_(s.isPattern())
{}


inline CML::keyType::keyType(const word& s)
:
    word(s, false),
    isPattern_(false)
{}


inline CML::keyType::keyType(const string& s)
:
    word(s, false),
    isPattern_(true)
{}


inline CML::keyType::keyType(const char* s)
:
    word(s, false),
    isPattern_(false)
{}


inline CML::keyType::keyType
(
    const std::string& s,
    const bool isPattern
)
:
    word(s, false),
    isPattern_(isPattern)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool CML::keyType::isPattern() const
{
    return isPattern_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline const CML::keyType& CML::keyType::operator=(const keyType& s)
{
    // Bypass checking
    string::operator=(s);
    isPattern_ = s.isPattern_;
    return *this;
}


inline const CML::keyType& CML::keyType::operator=(const word& s)
{
    word::operator=(s);
    isPattern_ = false;
    return *this;
}


inline const CML::keyType& CML::keyType::operator=(const string& s)
{
    // Bypass checking
    string::operator=(s);
    isPattern_ = true;
    return *this;
}


inline const CML::keyType& CML::keyType::operator=(const char* s)
{
    // Bypass checking
    string::operator=(s);
    isPattern_ = false;
    return *this;
}


// ************************************************************************* //
