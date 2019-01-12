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

#include "word.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::word::word(Istream& is)
:
    string()
{
    is >> *this;
}


CML::Istream& CML::operator>>(Istream& is, word& w)
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
        // try a bit harder and convert string to word
        w = t.stringToken();
        string::stripInvalid<word>(w);

        // flag empty strings and bad chars as an error
        if (w.empty() || w.size() != t.stringToken().size())
        {
            is.setBad();
            FatalIOErrorInFunction(is)
                << "wrong token type - expected word, found "
                "non-word characters "
                << t.info()
                << exit(FatalIOError);
            return is;
        }
    }
    else
    {
        is.setBad();
        FatalIOErrorInFunction(is)
            << "wrong token type - expected word, found "
            << t.info()
            << exit(FatalIOError);

        return is;
    }

    // Check state of IOstream
    is.check("Istream& operator>>(Istream&, word&)");

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const word& w)
{
    os.write(w);
    os.check("Ostream& operator<<(Ostream&, const word&)");
    return os;
}


// ************************************************************************* //
