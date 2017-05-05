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

#include "polynomial.hpp"

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<
(
    Ostream& os,
    const polynomial& poly
)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << static_cast<const DataEntry<scalar>& >(poly)
            << token::SPACE << poly.coeffs_;
    }
    else
    {
        os  << static_cast<const DataEntry<scalar>& >(poly);
        os.write
        (
            reinterpret_cast<const char*>(&poly.coeffs_),
            sizeof(poly.coeffs_)
        );
    }

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const polynomial&)"
    );

    return os;
}


void CML::polynomial::writeData(Ostream& os) const
{
    DataEntry<scalar>::writeData(os);

    os  << nl << indent << coeffs_ << token::END_STATEMENT << nl;
}


// ************************************************************************* //
