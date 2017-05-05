/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

#include "fieldValue.hpp"
#include "ListListOps.hpp"
#include "Pstream.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::fieldValue::combineFields(Field<Type>& field)
{
    List<Field<Type> > allValues(Pstream::nProcs());

    allValues[Pstream::myProcNo()] = field;

    Pstream::gatherList(allValues);

    if (Pstream::master())
    {
        field =
            ListListOps::combine<Field<Type> >
            (
                allValues,
                accessOp<Field<Type> >()
            );
    }
}


template<class Type>
void CML::fieldValue::combineFields(tmp<Field<Type> >& field)
{
    combineFields(field());
}


// ************************************************************************* //
