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

Global
    CML::SetPatchFields

Description
    Helper routine to initialise a patch field to a constant value

SourceFiles
    SetPatchFields.cpp

\*---------------------------------------------------------------------------*/

#ifndef SetPatchFields_H
#define SetPatchFields_H

#include "PtrList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

//- Helper routine to read fields
template<class GeoField>
void SetPatchFields
(
    PtrList<GeoField>& fields,
    const label patchI,
    const typename GeoField::value_type& initVal
);

} // End namespace CML

// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class GeoField>
void CML::SetPatchFields
(
    PtrList<GeoField>& fields,
    const label patchI,
    const typename GeoField::value_type& initVal
)
{
    forAll(fields, i)
    {
        typename GeoField::PatchFieldType& pfld =
            fields[i].boundaryField()[patchI];
        pfld == initVal;
    }
}


// ************************************************************************* //


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
