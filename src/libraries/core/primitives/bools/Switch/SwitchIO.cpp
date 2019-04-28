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

#include "Switch.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::Switch::Switch(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, Switch& s)
{
    token t(is);

    if (!t.good())
    {
        is.setBad();
        return is;
    }

    if (t.isLabel())
    {
        s = bool(t.labelToken());
    }
    else if (t.isWord())
    {
        // allow invalid values, but catch after for correct error message
        Switch sw(t.wordToken(), true);

        if (sw.valid())
        {
            s = sw;
        }
        else
        {
            is.setBad();
            FatalIOErrorInFunction(is)
                << "expected 'true/false', 'on/off' ... found " << t.wordToken()
                << exit(FatalIOError);

            return is;
        }
    }
    else
    {
        is.setBad();
        FatalIOErrorInFunction(is)
            << "wrong token type - expected bool, found " << t
            << exit(FatalIOError);

        return is;
    }


    // Check state of Istream
    is.check("Istream& operator>>(Istream&, Switch&)");

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const Switch& s)
{
    os << Switch::names[s.switch_];
    os.check("Ostream& operator<<(Ostream&, const Switch&)");
    return os;
}


// ************************************************************************* //
