/*---------------------------------------------------------------------------*\
Copyright (C) 2017 OpenCFD Ltd.
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

Class
    CML::Enum

Description
    A Enum is a wrapper around a list of names/values that represent
    particular enumeration values.

SourceFiles
    Enum.C

\*---------------------------------------------------------------------------*/

#ifndef Enum_HPP
#define Enum_HPP

#include "wordList.hpp"
#include <initializer_list>
#include <utility>
#include "dictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
// Forward declarations
class dictionary;
template<class EnumType> class Enum;

template<class EnumType>
Ostream& operator<<(Ostream& os, const Enum<EnumType>& wrapped);

/*---------------------------------------------------------------------------*\
                            Class Enum Declaration
\*---------------------------------------------------------------------------*/

template<class EnumType>
class Enum
{
    // Private Member Data

        //- The names for the enum
        List<word> names_;

        //- The values for the enum
        List<int> values_;


    // Private Member Functions

        //- The index for the given name. -1 if not found.
        label getIndex(const word& enumName) const;

        //- The index for the given enumeration. -1 if not found.
        label getIndex(const EnumType e) const;

        //- Lookup enumeration corresponding to the given name.
        //  FatalError if not found.
        EnumType getEnum(const word& enumName) const;

        //- Lookup name corresponding to the given enumeration.
        //  Return empty word if not found.
        const word& getName(const EnumType e) const;


        //- Disallow default bitwise copy construct
        Enum(const Enum&) = delete;

        //- Disallow default bitwise assignment
        void operator=(const Enum&) = delete;


public:

    //- The type of enumeration wrapped by Enum
    typedef EnumType value_type;


    // Constructors

        //- Construct from a values/names list.
        //  Duplicate values are permitted (eg, for aliases).
        //  Duplicate names are permitted, but won't make much sense.
        explicit Enum(std::initializer_list<std::pair<EnumType, word>> lst);

        //- Construct from a list of names with values incremented from the
        //  specified start value.
        Enum(const EnumType start, std::initializer_list<word> lst);


    // Member Functions

      // Access

        //- The number of lookup names for the enumeration
        inline label size() const;

        //- The list of enum names, in construction order
        inline const List<word>& names() const;

        //- The list of enum names, in construction order
        inline const List<word>& toc() const;

        //- The sorted list of enum names
        List<word> sortedToc() const;

        //- The list of enum values, in construction order
        inline const List<int>& values() const;


      // Query

        //- Test if there is an enumeration corresponding to the given name.
        inline bool hasEnum(const word& enumName) const;

        //- Test if there is a name corresponding to the given enumeration.
        inline bool hasName(const EnumType e) const;


      // Lookup

        //- Lookup the key in the dictionary and return the corresponding
        //  enumeration element based on its name.
        //  Fatal if anything is incorrect.
        EnumType lookup
        (
            const word& key,
            const dictionary& dict
        ) const;

        //- Find the key in the dictionary and return the corresponding
        //  enumeration element based on its name.
        //  Return the default value if the key was not found in the dictionary.
        //  Fatal if the enumerated name was incorrect.
        EnumType lookupOrDefault
        (
            const word& key,
            const dictionary& dict,
            const EnumType deflt
        ) const;


        //- Find the key in the dictionary and return the corresponding
        //  enumeration element based on its name.
        //  Return the default value if the key was not found in the dictionary
        //  or if the enumerated name was incorrect (emit warning)
        EnumType lookupOrFailsafe
        (
            const word& key,
            const dictionary& dict,
            const EnumType deflt
        ) const;


      // IO

        //- Read a word from Istream and return the corresponding enumeration
        EnumType read(Istream& is) const;

        //- Write the name representation of the enumeration to an Ostream
        //  A noop if the enumeration wasn't found.
        void write(const EnumType e, Ostream& os) const;

        //- Write the names as a list to an Ostream
        inline Ostream& writeList
        (
            Ostream& os,
            const label shortListLen=0
        ) const;


    // Member Operators

        //- Return the enumeration corresponding to the given name
        //  Fatal if the name cannot be found.
        inline const EnumType operator[](const word& name) const;

        //- Return the first name corresponding to the given enumeration.
        //  Returns an empty word on failure.
        inline const word& operator[](const EnumType e) const;


    // IOstream operators

        //- Write names to Ostream, as per writeList() with shortListLen=10
        friend Ostream& operator<< <EnumType>
        (
            Ostream& os,
            const Enum<EnumType>& wrapped
        );

};


} // End namespace CML


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EnumType>
inline CML::label CML::Enum<EnumType>::size() const
{
    return names_.size();
}


template<class EnumType>
inline const CML::wordList& CML::Enum<EnumType>::names() const
{
    return names_;
}


template<class EnumType>
inline const CML::wordList& CML::Enum<EnumType>::toc() const
{
    return names_;
}


template<class EnumType>
inline const CML::List<int>& CML::Enum<EnumType>::values() const
{
    return values_;
}


template<class EnumType>
inline bool CML::Enum<EnumType>::hasEnum(const word& enumName) const
{
    return getIndex(enumName) >= 0;
}


template<class EnumType>
inline bool CML::Enum<EnumType>::hasName(const EnumType e) const
{
    return getIndex(e) >= 0;
}


template<class EnumType>
inline CML::Ostream& CML::Enum<EnumType>::writeList
(
    Ostream& os,
    const label shortListLen
) const
{
    return names_.writeList(os, shortListLen);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class EnumType>
inline const EnumType CML::Enum<EnumType>::operator[]
(
    const word& name
) const
{
    return getEnum(name);
}


template<class EnumType>
inline const CML::word& CML::Enum<EnumType>::operator[]
(
    const EnumType e
) const
{
    return getName(e);
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class EnumType>
CML::label CML::Enum<EnumType>::getIndex(const word& enumName) const
{
    const label n = size();
    for (label idx=0; idx < n; ++idx)
    {
        if (names_[idx] == enumName)
        {
            return idx;
        }
    }

    return -1;
}


template<class EnumType>
CML::label CML::Enum<EnumType>::getIndex(const EnumType e) const
{
    const int val = int(e);

    const label n = size();
    for (label idx=0; idx < n; ++idx)
    {
        if (values_[idx] == val)
        {
            return idx;
        }
    }

    return -1;
}


template<class EnumType>
EnumType CML::Enum<EnumType>::getEnum(const word& enumName) const
{
    const label idx = getIndex(enumName);

    if (idx < 0)
    {
        FatalErrorInFunction
            << enumName << " is not in enumeration: "
            << names_ << exit(FatalError);
    }

    return EnumType(values_[idx]);
}


template<class EnumType>
const CML::word& CML::Enum<EnumType>::getName(const EnumType e) const
{
    const label idx = getIndex(e);

    if (idx < 0)
    {
        return word::null;
    }
    else
    {
        return names_[idx];
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class EnumType>
CML::Enum<EnumType>::Enum
(
    std::initializer_list<std::pair<EnumType, word>> lst
)
:
    names_(lst.size()),
    values_(lst.size())
{
    int idx = 0;
    for (const auto& pair : lst)
    {
        names_[idx]  = pair.second;
        values_[idx] = int(pair.first);

        ++idx;
    }
}


template<class EnumType>
CML::Enum<EnumType>::Enum
(
    const EnumType start,
    std::initializer_list<word> lst
)
:
    names_(lst.size()),
    values_(lst.size())
{
    int val = int(start);

    int idx = 0;
    for (const auto& key : lst)
    {
        names_[idx]  = key;
        values_[idx] = val;

        ++val;
        ++idx;
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class EnumType>
CML::List<CML::word> CML::Enum<EnumType>::sortedToc() const
{
    wordList lst(names_);
    CML::sort(lst);

    return lst;
}


template<class EnumType>
EnumType CML::Enum<EnumType>::read(Istream& is) const
{
    const word enumName(is);
    const label idx = getIndex(enumName);

    if (idx < 0)
    {
        FatalIOErrorInFunction(is)
            << enumName << " is not in enumeration: "
            << names_ << nl
            << exit(FatalIOError);
    }

    return EnumType(values_[idx]);
}


template<class EnumType>
void CML::Enum<EnumType>::write(const EnumType e, Ostream& os) const
{
    const label idx = getIndex(e);
    if (idx >= 0)
    {
        os << names_[idx];
    }
}


template<class EnumType>
EnumType CML::Enum<EnumType>::lookup
(
    const word& key,
    const dictionary& dict
) const
{
    const word enumName(dict.lookup(key));
    const label idx = getIndex(enumName);

    if (idx < 0)
    {
        FatalIOErrorInFunction(dict)
            << enumName << " is not in enumeration: "
            << names_ << nl
            << exit(FatalIOError);
    }

    return EnumType(values_[idx]);
}


template<class EnumType>
EnumType CML::Enum<EnumType>::lookupOrDefault
(
    const word& key,
    const dictionary& dict,
    const EnumType deflt
) const
{
    if (dict.found(key))
    {
        return lookup(key, dict);
    }

    return deflt;
}


template<class EnumType>
EnumType CML::Enum<EnumType>::lookupOrFailsafe
(
    const word& key,
    const dictionary& dict,
    const EnumType deflt
) const
{
    if (dict.found(key))
    {
        const word enumName(dict.lookup(key));
        const label idx = getIndex(enumName);

        if (idx < 0)
        {
            IOWarningInFunction(dict)
                << "bad " << key <<" specifier " << enumName
                << " using " << getName(deflt) << endl;
        }
        else
        {
            return EnumType(values_[idx]);
        }
    }

    return deflt;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class EnumType>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const Enum<EnumType>& wrapped
)
{
    return wrapped.names().writeList(os, 10);
}


#endif
