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

// * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * * //

inline CML::forceSuSp::forceSuSp()
{}


inline CML::forceSuSp::forceSuSp
(
    const Tuple2<vector, scalar>& fs
)
:
    Tuple2<vector, scalar>(fs)
{}


inline CML::forceSuSp::forceSuSp(const vector& Su, const scalar Sp)
{
    first() = Su;
    second() = Sp;
}


inline CML::forceSuSp::forceSuSp(Istream& is)
:
    Tuple2<vector, scalar>(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::vector& CML::forceSuSp::Su() const
{
    return first();
}


inline CML::scalar CML::forceSuSp::Sp() const
{
    return second();
}


inline CML::vector& CML::forceSuSp::Su()
{
    return first();
}


inline CML::scalar& CML::forceSuSp::Sp()
{
    return second();
}


// * * * * * * * * * * * * * * * Operators * * * * * * * * * * * * * * * * * //

inline void CML::forceSuSp::operator=(const forceSuSp& susp)
{
    first() = susp.first();
    second() = susp.second();
}


inline void CML::forceSuSp::operator+=(const forceSuSp& susp)
{
    first() += susp.first();
    second() += susp.second();
}


inline void CML::forceSuSp::operator-=(const forceSuSp& susp)
{
    first() -= susp.first();
    second() -= susp.second();
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline CML::forceSuSp CML::operator+
(
    const forceSuSp& susp1,
    const forceSuSp& susp2
)
{
    return forceSuSp
    (
        susp1.first() + susp2.first(),
        susp1.second() + susp2.second()
    );
}


inline CML::forceSuSp CML::operator*
(
    const scalar s,
    const forceSuSp& susp
)
{
    return forceSuSp(susp.first()*s, susp.second()*s);
}


// ************************************************************************* //
