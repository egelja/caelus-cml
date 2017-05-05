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

inline CML::solidArrheniusReactionRate::solidArrheniusReactionRate
(
    const scalar A,
    const scalar Ta,
    const scalar Tcrit
)
:
    A_(A),
    Ta_(Ta),
    Tcrit_(Tcrit)
{}


inline CML::solidArrheniusReactionRate::solidArrheniusReactionRate
(
    const speciesTable&,
    Istream& is
)
:
    A_(readScalar(is.readBegin("solidArrheniusReaction(Istream&)"))),
    Ta_(readScalar(is)),
    Tcrit_(readScalar(is))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::scalar CML::solidArrheniusReactionRate::operator()
(
    const scalar T,
    const scalar,
    const scalarField&
) const
{
    scalar ak = A_;

    if (T < Tcrit_)
    {
        ak *= 0.0;
    }
    else
    {
        ak *= exp(-Ta_/T);
    }

    return ak;
}


inline CML::Ostream& CML::operator<<
(
    Ostream& os,
    const solidArrheniusReactionRate& arr
)
{
    os  << token::BEGIN_LIST
        << arr.A_ << token::SPACE << arr.Ta_ << token::SPACE << arr.Tcrit_
        << token::END_LIST;
    return os;
}


// ************************************************************************* //
