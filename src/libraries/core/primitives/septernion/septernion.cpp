/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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

#include "septernion.hpp"
#include "IOstreams.hpp"
#include "OStringStream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const char* const CML::septernion::typeName = "septernion";
const CML::septernion CML::septernion::zero
(
    vector(0, 0, 0),
    quaternion(0, vector(0, 0, 0))
);
const CML::septernion CML::septernion::I
(
    vector(0, 0, 0),
    quaternion(1, vector(0, 0, 0))
);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::septernion::septernion(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::word CML::name(const septernion& s)
{
    OStringStream buf;
    buf << '(' << s.t() << ',' << s.r() << ')';
    return buf.str();
}


CML::septernion CML::slerp
(
    const septernion& sa,
    const septernion& sb,
    const scalar t
)
{
    return septernion((1 - t)*sa.t() + t*sb.t(), slerp(sa.r(), sb.r(), t));
}


CML::septernion CML::average
(
    const UList<septernion>& ss,
    const UList<scalar> w
)
{
    septernion sa(w[0]*ss[0]);

    for (label i=1; i<ss.size(); i++)
    {
        sa.t() += w[i]*ss[i].t();

        // Invert quaternion if it has the opposite sign to the average
        if ((sa.r() & ss[i].r()) > 0)
        {
            sa.r() += w[i]*ss[i].r();
        }
        else
        {
            sa.r() -= w[i]*ss[i].r();
        }
    }

    sa.r().normalize();

    return sa;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, septernion& s)
{
    // Read beginning of septernion
    is.readBegin("septernion");

    is  >> s.t() >> s.r();

    // Read end of septernion
    is.readEnd("septernion");

    // Check state of Istream
    is.check("operator>>(Istream&, septernion&)");

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const septernion& s)
{
    os  << token::BEGIN_LIST
        << s.t() << token::SPACE << s.r()
        << token::END_LIST;

    return os;
}


// ************************************************************************* //
