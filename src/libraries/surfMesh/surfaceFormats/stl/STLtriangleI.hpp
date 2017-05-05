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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::STLtriangle::STLtriangle()
{}


inline CML::STLtriangle::STLtriangle
(
    const STLpoint& normal,
    const STLpoint& a,
    const STLpoint& b,
    const STLpoint& c,
    unsigned short attrib
)
:
    normal_(normal),
    a_(a),
    b_(b),
    c_(c),
    attrib_(attrib)
{}


inline CML::STLtriangle::STLtriangle(istream& is)
{
    read(is);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::STLpoint& CML::STLtriangle::normal() const
{
    return normal_;
}


inline const CML::STLpoint& CML::STLtriangle::a() const
{
    return a_;
}


inline const CML::STLpoint& CML::STLtriangle::b() const
{
    return b_;
}


inline const CML::STLpoint& CML::STLtriangle::c() const
{
    return c_;
}


inline unsigned short CML::STLtriangle::attrib() const
{
    return attrib_;
}


inline void CML::STLtriangle::read(istream& is)
{
    is.read(reinterpret_cast<char*>(this), 4*sizeof(STLpoint));
    is.read(reinterpret_cast<char*>(&attrib_), sizeof(STLattrib));
}


inline void CML::STLtriangle::write(ostream& os)
{
    os.write(reinterpret_cast<char*>(this), 4*sizeof(STLpoint));
    os.write(reinterpret_cast<char*>(&attrib_), sizeof(STLattrib));
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline CML::Ostream& CML::operator<<(Ostream& os, const STLtriangle& t)
{
    os  << t.normal_ << token::SPACE
        << t.a_ << token::SPACE
        << t.b_ << token::SPACE
        << t.c_ << token::SPACE
        << t.attrib_;

    return os;
}


// ************************************************************************* //
