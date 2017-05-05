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

#include "word.hpp"
#include "Ostream.hpp"
#include "token.hpp"
#include "keyType.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Decrement the indent level
void CML::Ostream::decrIndent()
{
    if (indentLevel_ == 0)
    {
        cerr<< "Ostream::decrIndent() : attempt to decrement 0 indent level"
            << std::endl;
    }
    else
    {
        indentLevel_--;
    }
}


// Write keyType
// write regular expression as quoted string
// write plain word as word (unquoted)
CML::Ostream& CML::Ostream::write(const keyType& kw)
{
    return writeQuoted(kw, kw.isPattern());
}


// Write the keyword followed by appropriate indentation
CML::Ostream& CML::Ostream::writeKeyword(const keyType& kw)
{
    indent();
    write(kw);

    label nSpaces = entryIndentation_ - label(kw.size());

    // pattern is surrounded by quotes
    if (kw.isPattern())
    {
        nSpaces -= 2;
    }

    // could also increment by indentSize_ ...
    if (nSpaces < 1)
    {
        nSpaces = 1;
    }

    while (nSpaces--)
    {
        write(char(token::SPACE));
    }

    return *this;
}


// ************************************************************************* //
