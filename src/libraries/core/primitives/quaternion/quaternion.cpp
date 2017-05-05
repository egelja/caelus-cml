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

#include "quaternion.hpp"
#include "IOstreams.hpp"
#include "OStringStream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

const char* const CML::quaternion::typeName = "quaternion";
const CML::quaternion CML::quaternion::zero(0, vector::zero);
const CML::quaternion CML::quaternion::I(1, vector::zero);

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::quaternion::quaternion(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

CML::word CML::name(const quaternion& q)
{
    OStringStream buf;
    buf << '(' << q.w() << ',' << q.v() << ')';
    return buf.str();
}


CML::quaternion CML::slerp
(
    const quaternion& qa,
    const quaternion& qb,
    const scalar t
)
{
    label sign = 1;

    if ((qa & qb) < 0)
    {
        sign = -1;
    }

    return qa*pow((inv(qa)*sign*qb), t);
}


CML::quaternion CML::average
(
    const UList<quaternion>& qs,
    const UList<scalar> w
)
{
    quaternion qa(w[0]*qs[0]);

    for (label i=1; i<qs.size(); i++)
    {
        // Invert quaternion if it has the opposite sign to the average
        if ((qa & qs[i]) > 0)
        {
            qa += w[i]*qs[i];
        }
        else
        {
            qa -= w[i]*qs[i];
        }
    }

    return qa;
}


CML::quaternion CML::exp(const quaternion& q)
{
    const scalar magV = mag(q.v());

    if (magV == 0)
    {
        return quaternion(1, vector::zero);
    }

    const scalar expW = exp(q.w());

    return quaternion
    (
        expW*cos(magV),
        expW*sin(magV)*q.v()/magV
    );
}


CML::quaternion CML::pow(const quaternion& q, const label power)
{
    const scalar magQ = mag(q);
    const scalar magV = mag(q.v());

    quaternion powq(q.v());

    if (magV != 0 && magQ != 0)
    {
        powq /= magV;
        powq *= power*acos(q.w()/magQ);
    }

    return pow(magQ, power)*exp(powq);
}


CML::quaternion CML::pow(const quaternion& q, const scalar power)
{
    const scalar magQ = mag(q);
    const scalar magV = mag(q.v());

    quaternion powq(q.v());

    if (magV != 0 && magQ != 0)
    {
        powq /= magV;
        powq *= power*acos(q.w()/magQ);
    }

    return pow(magQ, power)*exp(powq);
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Istream& CML::operator>>(Istream& is, quaternion& q)
{
    // Read beginning of quaternion
    is.readBegin("quaternion");

    is  >> q.w() >> q.v();

    // Read end of quaternion
    is.readEnd("quaternion");

    // Check state of Istream
    is.check("operator>>(Istream&, quaternion&)");

    return is;
}


CML::Ostream& CML::operator<<(Ostream& os, const quaternion& q)
{
    os  << token::BEGIN_LIST
        << q.w() << token::SPACE << q.v()
        << token::END_LIST;

    return os;
}


// ************************************************************************* //
