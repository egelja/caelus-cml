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
    template<class Type>
    void addToFieldList
    (
        PtrList<GeometricField<Type, fvPatchField, volMesh> >& fieldList,
        const IOobject& obj,
        const label fieldI,
        const fvMesh& mesh
    );

    template<class Type>
    void outputFieldList
    (
        PtrList<GeometricField<Type, fvPatchField, volMesh> >& fieldList,
        const label patchI
    );
} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IOmanip.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void CML::addToFieldList
(
    PtrList<GeometricField<Type, fvPatchField, volMesh> >& fieldList,
    const IOobject& obj,
    const label fieldI,
    const fvMesh& mesh
)
{
    typedef GeometricField<Type, fvPatchField, volMesh> fieldType;

    if (obj.headerClassName() == fieldType::typeName)
    {
        fieldList.set
        (
            fieldI,
            new fieldType(obj, mesh)
        );
        Info<< "    " << fieldType::typeName << tab << obj.name() << endl;
    }
}


template<class Type>
void CML::outputFieldList
(
    PtrList<GeometricField<Type, fvPatchField, volMesh> >& fieldList,
    const label patchI
)
{
    forAll(fieldList, fieldI)
    {
        if (fieldList.set(fieldI))
        {
            Info<< "    " << pTraits<Type>::typeName << tab << tab
                << fieldList[fieldI].name() << tab << tab
                << fieldList[fieldI].boundaryField()[patchI].type() << nl;
        }
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
