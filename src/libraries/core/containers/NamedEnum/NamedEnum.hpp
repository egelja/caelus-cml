/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
Copyright (C) 2011-2018 OpenFOAM Foundation
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

Class
    CML::NamedEnum

Description
    Initialise the NamedEnum HashTable from the static list of names.


\*---------------------------------------------------------------------------*/

#ifndef NamedEnum_H
#define NamedEnum_H

#include "HashTable.hpp"
#include "stringList.hpp"
#include "wordList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                          Class NamedEnum Declaration
\*---------------------------------------------------------------------------*/

template<class Enum, unsigned int nEnum>
class NamedEnum
:
    public HashTable<unsigned int>
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        NamedEnum(const NamedEnum&);

        //- Disallow default bitwise assignment
        void operator=(const NamedEnum&);


public:

    // Static data members

        //- The set of names corresponding to the enumeration Enum
        static const char* names[nEnum];


    // Constructors

        //- Construct from names
        NamedEnum();


    // Member Functions

        //- Read a word from Istream and return the corresponding
        //  enumeration element
        Enum read(Istream&) const;

        //- Write the name representation of the enumeration to an Ostream
        void write(const Enum e, Ostream&) const;

        //- The set of names as a list of strings
        static stringList strings();

        //- The set of names as a list of words
        static wordList words();


    // Member Operators

        //- Return the enumeration element corresponding to the given name
        const Enum operator[](const char* name) const
        {
            return Enum(HashTable<unsigned int>::operator[](name));
        }

        //- Return the enumeration element corresponding to the given name
        const Enum operator[](const word& name) const
        {
            return Enum(HashTable<unsigned int>::operator[](name));
        }

        //- Return the name of the given enumeration element
        const char* operator[](const Enum e) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Enum, unsigned int nEnum>
CML::NamedEnum<Enum, nEnum>::NamedEnum()
:
    HashTable<unsigned int>(2*nEnum)
{
    for (unsigned int enumI = 0; enumI < nEnum; ++enumI)
    {
        if (!names[enumI] || names[enumI][0] == '\0')
        {
            stringList goodNames(enumI);

            for (unsigned int i = 0; i < enumI; ++i)
            {
                goodNames[i] = names[i];
            }

            FatalErrorInFunction
                << "Illegal enumeration name at position " << enumI << endl
                << "after entries " << goodNames << ".\n"
                << "Possibly your NamedEnum<Enum, nEnum>::names array"
                << " is not of size " << nEnum << endl
                << abort(FatalError);
        }
        insert(names[enumI], enumI);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Enum, unsigned int nEnum>
Enum CML::NamedEnum<Enum, nEnum>::read(Istream& is) const
{
    const word name(is);

    HashTable<unsigned int>::const_iterator iter = find(name);

    if (iter == HashTable<unsigned int>::end())
    {
        FatalIOErrorInFunction(is)
            << name << " is not in enumeration: "
            << sortedToc() << exit(FatalIOError);
    }

    return Enum(iter());
}


template<class Enum, unsigned int nEnum>
void CML::NamedEnum<Enum, nEnum>::write(const Enum e, Ostream& os) const
{
    os  << operator[](e);
}


template<class Enum, unsigned int nEnum>
CML::stringList CML::NamedEnum<Enum, nEnum>::strings()
{
    stringList lst(nEnum);

    label nElem = 0;
    for (unsigned int enumI = 0; enumI < nEnum; ++enumI)
    {
        if (names[enumI] && names[enumI][0])
        {
            lst[nElem++] = names[enumI];
        }
    }

    lst.setSize(nElem);
    return lst;
}


template<class Enum, unsigned int nEnum>
CML::wordList CML::NamedEnum<Enum, nEnum>::words()
{
    wordList lst(nEnum);

    label nElem = 0;
    for (unsigned int enumI = 0; enumI < nEnum; ++enumI)
    {
        if (names[enumI] && names[enumI][0])
        {
            lst[nElem++] = names[enumI];
        }
    }

    lst.setSize(nElem);
    return lst;
}


template<class Enum, unsigned int nEnum>
const char* CML::NamedEnum<Enum, nEnum>::operator[](const Enum e) const
{
    unsigned int ue = unsigned(e);

    if (ue < nEnum)
    {
        return names[ue];
    }
    else
    {
        FatalErrorInFunction
            << "names array index " << ue << " out of range 0-"
            << nEnum - 1
            << exit(FatalError);

        return names[0];
    }
}
#endif

// ************************************************************************* //
