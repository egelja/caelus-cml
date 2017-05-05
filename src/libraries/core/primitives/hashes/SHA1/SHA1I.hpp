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

#include "SHA1.hpp"
#include <cstring>

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::SHA1::SHA1()
{
    clear();
}


inline CML::SHA1::SHA1(const std::string& str)
{
    clear();
    append(str);
}


inline CML::SHA1::SHA1(const char* str)
{
    clear();
    append(str);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::SHA1& CML::SHA1::append(const char* data, size_t len)
{
    processBytes(data, len);
    return *this;
}


inline CML::SHA1& CML::SHA1::append(const std::string& str)
{
    processBytes(str.data(), str.size());
    return *this;
}


inline CML::SHA1& CML::SHA1::append(const char* str)
{
    if (str)
    {
        processBytes(str, strlen(str));
    }
    return *this;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline bool CML::SHA1::operator==(const SHA1& rhs) const
{
    return this->digest() == rhs.digest();
}


inline bool CML::SHA1::operator==(const SHA1Digest& rhs) const
{
    return this->digest() == rhs;
}


inline bool CML::SHA1::operator==(const std::string& hexdigits) const
{
    return this->digest() == hexdigits;
}


inline bool CML::SHA1::operator==(const char* hexdigits) const
{
    return this->digest() == hexdigits;
}


inline bool CML::SHA1::operator!=(const SHA1& rhs) const
{
    return !this->operator==(rhs);
}


inline bool CML::SHA1::operator!=(const SHA1Digest& rhs) const
{
    return !this->operator==(rhs);
}


inline bool CML::SHA1::operator!=(const std::string& rhs) const
{
    return !this->operator==(rhs);
}


inline bool CML::SHA1::operator!=(const char* rhs) const
{
    return !this->operator==(rhs);
}


inline CML::SHA1::operator CML::SHA1Digest() const
{
    return digest();
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline CML::Ostream& CML::operator<<(Ostream& os, const SHA1& sha)
{
    return os  << sha.digest();
}


// ************************************************************************* //
