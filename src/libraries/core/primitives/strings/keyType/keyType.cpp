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

Description
    Istream constructor and IOstream operators for keyType.

\*---------------------------------------------------------------------------*/

#include "keyType.hpp"
#include "regExp.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const CML::keyType CML::keyType::null;


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::keyType::keyType(Istream& is)
:
    word(),
    isPattern_(false)
{
    is  >> *this;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::keyType::match
(
    const std::string& str,
    bool literalMatch
) const
{
    if (literalMatch || !isPattern_)
    {
        // check as string
        return (str == *this);
    }
    else
    {
        // check as regex
        return regExp(*this).match(str);
    }
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, keyType& kw)
{
    token t(is);

    if (!t.good())
    {
        is.setBad();
        return is;
    }

    if (t.isWord())
    {
        kw = t.wordToken();
    }
    else if (t.isString())
    {
        // Assign from string. Set as regular expression.
        kw = t.stringToken();
        kw.isPattern_ = true;

        // flag empty strings as an error
        if (kw.empty())
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
    is.check("Istream& operator>>(Istream&, keyType&)");

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const keyType& kw)
{
    os.write(kw);
    os.check("Ostream& operator<<(Ostream&, const keyType&)");
    return os;
}


// ************************************************************************* //
