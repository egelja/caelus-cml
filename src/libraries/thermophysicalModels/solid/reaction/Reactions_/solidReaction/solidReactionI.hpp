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

#include "solidReaction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const List<label>& solidReaction::slhs() const
{
    return slhs_;
}


inline const List<label>& solidReaction::srhs() const
{
    return srhs_;
}


inline const List<label>& solidReaction::grhs() const
{
    return grhs_;
}


inline const speciesTable& solidReaction::pyrolisisGases() const
{
    return pyrolisisGases_;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline Ostream& operator<<
(
    Ostream& os,
    const solidReaction& r
)
{
    r.write(os);
    return os;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
