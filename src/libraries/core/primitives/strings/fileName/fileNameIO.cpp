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

#include "fileName.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::fileName::fileName(Istream& is)
:
    string()
{
    is >> *this;
}


CML::Istream& CML::operator>>(Istream& is, fileName& fn)
{
    token t(is);

    if (!t.good())
    {
        is.setBad();
        return is;
    }

    if (t.isString())
    {
        fn = t.stringToken();
    }
    else
    {
        is.setBad();
        FatalIOErrorInFunction(is)
            << "wrong token type - expected string, found " << t.info()
            << exit(FatalIOError);

        return is;
    }

    fn.stripInvalid();

    // Check state of Istream
    is.check("Istream& operator>>(Istream&, fileName&)");

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const fileName& fn)
{
    os.write(fn);
    os.check("Ostream& operator<<(Ostream&, const fileName&)");
    return os;
}


// ************************************************************************* //


