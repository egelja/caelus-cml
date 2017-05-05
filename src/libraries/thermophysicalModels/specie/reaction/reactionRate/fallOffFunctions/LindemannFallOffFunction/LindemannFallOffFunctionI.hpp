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

inline CML::LindemannFallOffFunction::LindemannFallOffFunction()
{}


inline CML::LindemannFallOffFunction::LindemannFallOffFunction(Istream&)
{}


inline CML::LindemannFallOffFunction::LindemannFallOffFunction
(
    const dictionary& dict
)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::scalar CML::LindemannFallOffFunction::operator()
(
    const scalar,
    const scalar
) const
{
    return 1.0;
}


inline void CML::LindemannFallOffFunction::write(Ostream& os) const
{}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline CML::Ostream& CML::operator<<
(
    CML::Ostream& os,
    const CML::LindemannFallOffFunction&
)
{
    return os;
}


// ************************************************************************* //
