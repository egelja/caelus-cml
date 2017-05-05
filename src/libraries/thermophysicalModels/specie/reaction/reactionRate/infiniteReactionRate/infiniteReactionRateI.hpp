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

inline CML::infiniteReactionRate::infiniteReactionRate()
{}


inline CML::infiniteReactionRate::infiniteReactionRate
(
    const speciesTable&,
    const dictionary&
)
{}


inline void CML::infiniteReactionRate::write(Ostream& os) const
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::scalar CML::infiniteReactionRate::operator()
(
    const scalar,
    const scalar,
    const scalarField&
) const
{
    return (1);
}


inline CML::Ostream& CML::operator<<
(
    Ostream& os,
    const infiniteReactionRate& rr
)
{
    os  << token::BEGIN_LIST
        << token::END_LIST;
    return os;
}


// ************************************************************************* //
