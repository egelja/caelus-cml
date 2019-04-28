/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "wordRe.hpp"
#include "IOstreams.hpp"
#include "InfoProxy.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const CML::wordRe CML::wordRe::null;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::wordRe::wordRe(Istream& is)
:
    word(),
    re_(nullptr)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, wordRe& w)
{
    token t(is);

    if (!t.good())
    {
        is.setBad();
        return is;
    }

    if (t.isWord())
    {
        w = t.wordToken();
    }
    else if (t.isString())
    {
        // Auto-tests for regular expression
        w = t.stringToken();

        // flag empty strings as an error
        if (w.empty())
        {
            is.setBad();
            FatalIOErrorInFunction(is)
                << "empty word/expression "
                << exit(FatalIOError);
            return is;
        }
    }
    else
    {
        is.setBad();
        FatalIOErrorInFunction(is)
            << "wrong token type - expected word or string, found "
            << t.info()
            << exit(FatalIOError);

        return is;
    }

    // Check state of IOstream
    is.check("Istream& operator>>(Istream&, wordRe&)");

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const wordRe& w)
{
    os.writeQuoted(w, w.isPattern());
    os.check("Ostream& operator<<(Ostream&, const wordRe&)");
    return os;
}


CML::Ostream& CML::wordRe::info(Ostream& os) const
{
    if (isPattern())
    {
        os  << "wordRe(regex) " << *this;
    }
    else
    {
        os  << "wordRe(plain) \"" << *this << '"';
    }
    os.flush();

    return os;
}


// ************************************************************************* //
