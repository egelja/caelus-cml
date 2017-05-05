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

#include "argList.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::word& CML::argList::executable() const
{
    return executable_;
}


inline const CML::fileName& CML::argList::rootPath() const
{
    return rootPath_;
}


inline const CML::fileName& CML::argList::caseName() const
{
    return case_;
}


inline const CML::fileName& CML::argList::globalCaseName() const
{
    return globalCase_;
}


inline CML::fileName CML::argList::path() const
{
    return rootPath()/caseName();
}


inline const CML::stringList& CML::argList::args() const
{
    return args_;
}


inline const CML::string& CML::argList::arg(const label index) const
{
    return args_[index];
}


inline CML::label CML::argList::size() const
{
    return args_.size();
}


inline const CML::HashTable<CML::string>& CML::argList::options() const
{
    return options_;
}


inline const CML::string& CML::argList::option(const word& opt) const
{
    return options_[opt];
}


inline bool CML::argList::optionFound(const word& opt) const
{
    return options_.found(opt);
}


inline CML::IStringStream CML::argList::optionLookup(const word& opt) const
{
    return IStringStream(options_[opt]);
}


// * * * * * * * * * * * * Template Specializations  * * * * * * * * * * * * //

namespace CML
{
    // Template specialization for string
    template<>
    inline CML::string
    CML::argList::argRead<CML::string>(const label index) const
    {
        return args_[index];
    }

    // Template specialization for word
    template<>
    inline CML::word
    CML::argList::argRead<CML::word>(const label index) const
    {
        return args_[index];
    }

    // Template specialization for fileName
    template<>
    inline CML::fileName
    CML::argList::argRead<CML::fileName>(const label index) const
    {
        return args_[index];
    }

    // Template specialization for string
    template<>
    inline CML::string
    CML::argList::optionRead<CML::string>(const word& opt) const
    {
        return options_[opt];
    }

    // Template specialization for word
    template<>
    inline CML::word
    CML::argList::optionRead<CML::word>(const word& opt) const
    {
        return options_[opt];
    }

    // Template specialization for fileName
    template<>
    inline CML::fileName
    CML::argList::optionRead<CML::fileName>(const word& opt) const
    {
        return options_[opt];
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class T>
inline T CML::argList::argRead(const label index) const
{
    T val;

    IStringStream(args_[index])() >> val;
    return val;
}


template<class T>
inline T CML::argList::optionRead(const word& opt) const
{
    T val;

    optionLookup(opt)() >> val;
    return val;
}


template<class T>
inline bool CML::argList::optionReadIfPresent
(
    const word& opt,
    T& val
) const
{
    if (optionFound(opt))
    {
        val = optionRead<T>(opt);
        return true;
    }
    else
    {
        return false;
    }
}


template<class T>
inline bool CML::argList::optionReadIfPresent
(
    const word& opt,
    T& val,
    const T& deflt
) const
{
    if (optionReadIfPresent<T>(opt, val))
    {
        return true;
    }
    else
    {
        val = deflt;
        return false;
    }
}


template<class T>
inline T CML::argList::optionLookupOrDefault
(
    const word& opt,
    const T& deflt
) const
{
    if (optionFound(opt))
    {
        return optionRead<T>(opt);
    }
    else
    {
        return deflt;
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline const CML::string& CML::argList::operator[](const label index) const
{
    return args_[index];
}


inline const CML::string& CML::argList::operator[](const word& opt) const
{
    return options_[opt];
}


// ************************************************************************* //
