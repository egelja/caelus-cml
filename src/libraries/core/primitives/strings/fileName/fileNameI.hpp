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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline void CML::fileName::stripInvalid()
{
    if (debug && string::stripInvalid<fileName>(*this))
    {
        std::cerr
            << "fileName::stripInvalid() called for invalid fileName "
            << this->c_str() << std::endl;

        if (debug > 1)
        {
            std::cerr
                << "    For debug level (= " << debug
                << ") > 1 this is considered fatal" << std::endl;
            std::abort();
        }

        removeRepeated('/');
        removeTrailing('/');
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::fileName::fileName()
:
    string()
{}

inline CML::fileName::fileName(const fileName& fn)
:
    string(fn)
{}

inline CML::fileName::fileName(const word& w)
:
    string(w)
{}


inline CML::fileName::fileName(const string& str)
:
    string(str)
{
    stripInvalid();
}


inline CML::fileName::fileName(const std::string& str)
:
    string(str)
{
    stripInvalid();
}


inline CML::fileName::fileName(const char* str)
:
    string(str)
{
    stripInvalid();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool CML::fileName::valid(char c)
{
    return
    (
    // Remove checking for space.
    // Under windows spaces are common in file paths,
    // so can't disallow them. Prefer also
    // to allow them under Linux too.
    //    !isspace(c)
     c != '"'   // string quote
     && c != '\''  // string quote
    );
}


// ************************************************************************* //
