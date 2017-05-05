/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::triad::triad()
:
    Vector<vector>(triad::unset)
{}


inline CML::triad::triad(const Vector<vector>& vv)
:
    Vector<vector>(vv)
{}


inline CML::triad::triad(const vector& x, const vector& y, const vector& z)
:
    Vector<vector>(x, y, z)
{}


inline CML::triad::triad(const vector& pa)
{
    operator=(triad::unset);
    operator[](primaryDirection(pa)) = pa;
}


inline CML::triad::triad(Istream& is)
:
    Vector<vector>(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool CML::triad::set(const direction d) const
{
    return operator[](d)[0] < GREAT;
}


inline bool CML::triad::set() const
{
    return set(0) && set(1) && set(2);
}


inline CML::direction CML::triad::primaryDirection(const vector& v)
{
    if (mag(v.x()) > mag(v.y()) && mag(v.x()) > mag(v.z()))
    {
        return triad::X;
    }
    else if (mag(v.y()) > mag(v.z()))
    {
        return triad::Y;
    }
    else
    {
        return triad::Z;
    }
}


inline CML::vector CML::triad::orthogonal
(
    const vector& v1,
    const vector& v2
)
{
    vector v3 = v1 ^ v2;

    scalar magV3 = mag(v3);

    if (magV3 > 0.5)
    {
        return v3/magV3;
    }
    else
    {
        return triad::unset[0];
    }
}


inline void CML::triad::normalize()
{
    if (set(0)) operator[](0) /= mag(operator[](0));
    if (set(1)) operator[](1) /= mag(operator[](1));
    if (set(2)) operator[](2) /= mag(operator[](2));
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void CML::triad::operator=(const Vector<vector>& vv)
{
    Vector<vector>::operator=(vv);
}


// * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * * //

inline CML::Istream& CML::operator>>(Istream& is, triad& t)
{
    is >> static_cast<Vector<vector>&>(t);
    return is;
}


inline CML::Ostream& CML::operator<<(Ostream& os, const triad& t)
{
    os << static_cast<const Vector<vector>&>(t);
    return os;
}


// ************************************************************************* //
