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

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const TableBase<Type>& tbl
)
{
    if (os.format() == IOstream::ASCII)
    {
         os << token::SPACE << tbl.table_;
    }
    else
    {
        os.write
        (
            reinterpret_cast<const char*>(&tbl.table_),
            sizeof(tbl.table_)
        );
    }

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const TableBase<Type>&, const bool)"
    );

    return os;
}


template<class Type>
void CML::TableBase<Type>::writeData(Ostream& os) const
{
    os  << nl << indent << table_ << token::END_STATEMENT << nl;
    writeEntries(os);
}


template<class Type>
void CML::TableBase<Type>::writeEntries(Ostream& os) const
{
    if (boundsHandling_ != CLAMP)
    {
        os.writeKeyword("outOfBounds") << boundsHandlingToWord(boundsHandling_)
            << token::END_STATEMENT << nl;
    }
}


// ************************************************************************* //
