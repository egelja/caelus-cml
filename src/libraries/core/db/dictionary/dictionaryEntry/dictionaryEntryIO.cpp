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

Description
    DictionaryEntry constructor from Istream and Ostream output operator.

\*---------------------------------------------------------------------------*/

#include "keyType.hpp"
#include "dictionaryEntry.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::dictionaryEntry::dictionaryEntry
(
    const dictionary& parentDict,
    Istream& is
)
:
    entry(keyType(is)),
    dictionary(parentDict, is)
{
    is.fatalCheck
    (
        "dictionaryEntry::dictionaryEntry"
        "(const dictionary& parentDict, Istream&)"
    );
}


CML::dictionaryEntry::dictionaryEntry
(
    const keyType& key,
    const dictionary& parentDict,
    Istream& is
)
:
    entry(key),
    dictionary(key, parentDict, is)
{
    is.fatalCheck
    (
        "dictionaryEntry::dictionaryEntry"
        "(const keyType&, const dictionary& parentDict, Istream&)"
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::dictionaryEntry::write(Ostream& os) const
{
    // write keyword with indent but without trailing spaces
    os.indent();
    os.write(keyword());
    dictionary::write(os);
}


// * * * * * * * * * * * * * * Ostream operator  * * * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const dictionaryEntry& de)
{
    de.write(os);
    return os;
}

#if defined (__GNUC__)
template<>
#endif
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const InfoProxy<dictionaryEntry>& ip
)
{
    const dictionaryEntry& e = ip.t_;

    os  << "    dictionaryEntry '" << e.keyword() << "'" << endl;

    return os;
}


// ************************************************************************* //
