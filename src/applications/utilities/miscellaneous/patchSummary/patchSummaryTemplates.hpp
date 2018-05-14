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

#ifndef patchSummaryTemplates_H
#define patchSummaryTemplates_H

#include "fvCFD.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
    template<class GeoField>
    void addToFieldList
    (
        PtrList<GeoField>& fieldList,
        const IOobject& obj,
        const label fieldI,
        const typename GeoField::Mesh& mesh
    );

    template<class GeoField>
    void outputFieldList
    (
        const PtrList<GeoField>& fieldList,
        const label patchI
    );

    template<class GeoField>
    void collectFieldList
    (
        const PtrList<GeoField>& fieldList,
        const label patchI,
        HashTable<word>& fieldToType
    );
} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IOmanip.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class GeoField>
void CML::addToFieldList
(
    PtrList<GeoField>& fieldList,
    const IOobject& obj,
    const label fieldI,
    const typename GeoField::Mesh& mesh
)
{

    if (obj.headerClassName() == GeoField::typeName)
    {
        fieldList.set
        (
            fieldI,
            new GeoField(obj, mesh)
        );
        Info<< "    " << GeoField::typeName << tab << obj.name() << endl;
    }
}


template<class GeoField>
void CML::outputFieldList
(
    const PtrList<GeoField>& fieldList,
    const label patchI
)
{
    forAll(fieldList, fieldI)
    {
        if (fieldList.set(fieldI))
        {
            Info<< "    " << pTraits<typename GeoField::value_type>::typeName
                << tab << tab
                << fieldList[fieldI].name() << tab << tab
                << fieldList[fieldI].boundaryField()[patchI].type() << nl;
        }
    }
}


template<class GeoField>
void CML::collectFieldList
(
    const PtrList<GeoField>& fieldList,
    const label patchI,
    HashTable<word>& fieldToType
)
{
    forAll(fieldList, fieldI)
    {
        if (fieldList.set(fieldI))
        {
            fieldToType.insert
            (
                fieldList[fieldI].name(),
                fieldList[fieldI].boundaryField()[patchI].type()
            );
        }
    }
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
