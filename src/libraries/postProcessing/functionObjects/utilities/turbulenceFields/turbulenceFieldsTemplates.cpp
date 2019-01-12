/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2015 OpenFOAM Foundation
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

#include "volFields.hpp"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class Type>
void CML::turbulenceFields::processField
(
    const word& fieldName,
    const tmp<GeometricField<Type, fvPatchField, volMesh> >& tvalue
)
{
    typedef GeometricField<Type, fvPatchField, volMesh> FieldType;

    const word scopedName = modelName + ':' + fieldName;

    if (obr_.foundObject<FieldType>(scopedName))
    {
        FieldType& fld =
            const_cast<FieldType&>(obr_.lookupObject<FieldType>(scopedName));
        fld == tvalue();
    }
    else if (obr_.found(scopedName))
    {
        WarningInFunction
            << "Cannot store turbulence field " << scopedName
            << " since an object with that name already exists"
            << nl << endl;
    }
    else
    {
        obr_.store
        (
            new FieldType
            (
                IOobject
                (
                    scopedName,
                    obr_.time().timeName(),
                    obr_,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE
                ),
                tvalue
            )
        );
    }
}


// ************************************************************************* //
