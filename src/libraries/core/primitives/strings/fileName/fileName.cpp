/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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

#include "fileName.hpp"
#include "wordList.hpp"
#include "DynamicList.hpp"
#include "debug.hpp"
#include "OSspecific.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const char* const CML::fileName::typeName = "fileName";
int CML::fileName::debug(debug::debugSwitch(fileName::typeName, 0));
const CML::fileName CML::fileName::null;

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::fileName::fileName(const wordList& lst)
{
    forAll(lst, elemI)
    {
        operator=((*this)/lst[elemI]);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::fileName::Type CML::fileName::type() const
{
    return ::CML::type(*this);
}


bool CML::fileName::isAbsolute() const
{
#ifdef windows
  // Cater for absolute Windows paths, e.g., C:\..., and file share \\machine_name
  return ((3 < size()) && (operator[](1) == ':' || 
                           (operator[](0) == '\\' && operator[](1) == '\\')));
#else
    return !empty() && operator[](0) == '/';
#endif
}


CML::fileName& CML::fileName::toAbsolute()
{
    fileName& f = *this;

    if (!f.isAbsolute())
    {
        f = cwd()/f;
        f.clean();
    }

    return f;
}


bool CML::fileName::clean()
{
    // The top slash - we are never allowed to go above it
    register string::size_type top = this->find('/');

    // No slashes - nothing to do
    if (top == string::npos)
    {
        return false;
    }

    // Start with the '/' found:
    register char prev = '/';
    register string::size_type nChar  = top+1;
    register string::size_type maxLen = this->size();

    for
    (
        register string::size_type src = nChar;
        src < maxLen;
    )
    {
        register char c = operator[](src++);

        if (prev == '/')
        {
            // Repeated '/' - skip it
            if (c == '/')
            {
                continue;
            }

            // Could be '/./' or '/../'
            if (c == '.')
            {
                // Found trailing '/.' - skip it
                if (src >= maxLen)
                {
                    continue;
                }


                // Peek at the next character
                register char c1 = operator[](src);

                // found '/./' - skip it
                if (c1 == '/')
                {
                    src++;
                    continue;
                }

                // It is '/..' or '/../'
                if (c1 == '.' && (src+1 >= maxLen || operator[](src+1) == '/'))
                {
                    string::size_type parent;

                    // Backtrack to find the parent directory
                    // Minimum of 3 characters:  '/x/../'
                    // Strip it, provided it is above the top point
                    if
                    (
                        nChar > 2
                     && (parent = this->rfind('/', nChar-2)) != string::npos
                     && parent >= top
                    )
                    {
                        nChar = parent + 1;   // Retain '/' from the parent
                        src += 2;
                        continue;
                    }

                    // Bad resolution, eg 'abc/../../'
                    // Retain the sequence, but move the top to avoid it being
                    // considered a valid parent later
                    top = nChar + 2;
                }
            }
        }
        operator[](nChar++) = prev = c;
    }

    // Remove trailing slash
    if (nChar > 1 && operator[](nChar-1) == '/')
    {
        nChar--;
    }

    this->resize(nChar);

    return (nChar != maxLen);
}


CML::fileName CML::fileName::clean() const
{
    fileName fName(*this);
    fName.clean();
    return fName;
}


CML::word CML::fileName::name() const
{
    size_type i = rfind('/');

    if (i == npos)
    {
        return *this;
    }
    else
    {
        return substr(i+1, npos);
    }
}


CML::string CML::fileName::caseName() const
{
    string cName = *this;

    const string caseStr(getEnv("CAELUS_CASE"));

    const size_type i = find(caseStr);

    if (i == npos)
    {
        return cName;
    }
    else
    {
        return cName.replace(i, caseStr.size(), string("$CAELUS_CASE"));
    }
}


CML::word CML::fileName::name(const bool noExt) const
{
    if (noExt)
    {
        size_type beg = rfind('/');
        if (beg == npos)
        {
            beg = 0;
        }
        else
        {
            ++beg;
        }

        size_type dot = rfind('.');
        if (dot != npos && dot <= beg)
        {
            dot = npos;
        }

        if (dot == npos)
        {
            return substr(beg, npos);
        }
        else
        {
            return substr(beg, dot - beg);
        }
    }
    else
    {
        return this->name();
    }
}


CML::fileName CML::fileName::path() const
{
    size_type i = rfind('/');

    if (i == npos)
    {
        return ".";
    }
    else if (i)
    {
        return substr(0, i);
    }
    else
    {
        return "/";
    }
}


CML::fileName CML::fileName::lessExt() const
{
    size_type i = find_last_of("./");

    if (i == npos || i == 0 || operator[](i) == '/')
    {
        return *this;
    }
    else
    {
        return substr(0, i);
    }
}


CML::word CML::fileName::ext() const
{
    size_type i = find_last_of("./");

    if (i == npos || i == 0 || operator[](i) == '/')
    {
        return word::null;
    }
    else
    {
        return substr(i+1, npos);
    }
}


CML::wordList CML::fileName::components(const char delimiter) const
{
    DynamicList<word> wrdList(20);

    size_type beg=0, end=0;

    while ((end = find(delimiter, beg)) != npos)
    {
        // Avoid empty element (caused by doubled slashes)
        if (beg < end)
        {
            wrdList.append(substr(beg, end-beg));
        }
        beg = end + 1;
    }

    // Avoid empty trailing element
    if (beg < size())
    {
        wrdList.append(substr(beg, npos));
    }

    // Transfer to wordList
    return wordList(wrdList.xfer());
}


CML::word CML::fileName::component
(
    const size_type cmpt,
    const char delimiter
) const
{
    return components(delimiter)[cmpt];
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::fileName::operator=(const fileName& str)
{
    string::operator=(str);
}


void CML::fileName::operator=(const word& str)
{
    string::operator=(str);
}


void CML::fileName::operator=(const string& str)
{
    string::operator=(str);
    stripInvalid();
}


void CML::fileName::operator=(const std::string& str)
{
    string::operator=(str);
    stripInvalid();
}


void CML::fileName::operator=(const char* str)
{
    string::operator=(str);
    stripInvalid();
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::fileName CML::operator/(const string& a, const string& b)
{
    if (a.size())           // First string non-null
    {
        if (b.size())       // Second string non-null
        {
            return fileName(a + '/' + b);
        }
        else                // Second string null
        {
            return a;
        }
    }
    else                    // First string null
    {
        if (b.size())       // Second string non-null
        {
            return b;
        }
        else                // Second string null
        {
            return fileName();
        }
    }
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

CML::fileName CML::search(const word& file, const fileName& directory)
{
    // Search the current directory for the file
    fileNameList files(readDir(directory));
    forAll(files, i)
    {
        if (files[i] == file)
        {
            return directory/file;
        }
    }

    // If not found search each of the sub-directories
    fileNameList dirs(readDir(directory, fileName::DIRECTORY));
    forAll(dirs, i)
    {
        fileName path = search(file, directory/dirs[i]);
        if (path != fileName::null)
        {
            return path;
        }
    }

    return fileName::null;
}


// ************************************************************************* //
