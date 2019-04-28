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

InClass
    CML::reconstructLagrangian

Description

SourceFiles
    reconstructLagrangianPositions.cpp

\*---------------------------------------------------------------------------*/

#ifndef reconstructLagrangian_H
#define reconstructLagrangian_H

#include "cloud_.hpp"
#include "polyMesh.hpp"
#include "IOobjectList.hpp"
#include "CompactIOField.hpp"
#include "fvMesh.hpp"
#include "IOField.hpp"
#include "CompactIOField.hpp"
#include "Time.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

void reconstructLagrangianPositions
(
    const polyMesh& mesh,
    const word& cloudName,
    const PtrList<fvMesh>& meshes,
    const PtrList<labelIOList>& faceProcAddressing,
    const PtrList<labelIOList>& cellProcAddressing
);


template<class Type>
tmp<IOField<Type> > reconstructLagrangianField
(
    const word& cloudName,
    const polyMesh& mesh,
    const PtrList<fvMesh>& meshes,
    const word& fieldName
);


template<class Type>
tmp<CompactIOField<Field<Type>, Type> > reconstructLagrangianFieldField
(
    const word& cloudName,
    const polyMesh& mesh,
    const PtrList<fvMesh>& meshes,
    const word& fieldName
);


template<class Type>
void reconstructLagrangianFields
(
    const word& cloudName,
    const polyMesh& mesh,
    const PtrList<fvMesh>& meshes,
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
);


template<class Type>
void reconstructLagrangianFieldFields
(
    const word& cloudName,
    const polyMesh& mesh,
    const PtrList<fvMesh>& meshes,
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::IOField<Type> > CML::reconstructLagrangianField
(
    const word& cloudName,
    const polyMesh& mesh,
    const PtrList<fvMesh>& meshes,
    const word& fieldName
)
{
    // Construct empty field on mesh
    tmp<IOField<Type> > tfield
    (
        new IOField<Type>
        (
            IOobject
            (
                fieldName,
                mesh.time().timeName(),
                cloud::prefix/cloudName,
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            Field<Type>(0)
        )
    );
    Field<Type>& field = tfield();

    forAll(meshes, i)
    {
        // Check object on local mesh
        IOobject localIOobject
        (
            fieldName,
            meshes[i].time().timeName(),
            cloud::prefix/cloudName,
            meshes[i],
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if (localIOobject.headerOk())
        {
            IOField<Type> fieldi(localIOobject);

            label offset = field.size();
            field.setSize(offset + fieldi.size());

            forAll(fieldi, j)
            {
                field[offset + j] = fieldi[j];
            }
        }
    }

    return tfield;
}


template<class Type>
CML::tmp<CML::CompactIOField<CML::Field<Type>, Type> >
CML::reconstructLagrangianFieldField
(
    const word& cloudName,
    const polyMesh& mesh,
    const PtrList<fvMesh>& meshes,
    const word& fieldName
)
{
    // Construct empty field on mesh
    tmp<CompactIOField<Field<Type>, Type > > tfield
    (
        new CompactIOField<Field<Type>, Type>
        (
            IOobject
            (
                fieldName,
                mesh.time().timeName(),
                cloud::prefix/cloudName,
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            Field<Field<Type> >(0)
        )
    );
    Field<Field<Type> >& field = tfield();

    forAll(meshes, i)
    {
        // Check object on local mesh
        IOobject localIOobject
        (
            fieldName,
            meshes[i].time().timeName(),
            cloud::prefix/cloudName,
            meshes[i],
            IOobject::MUST_READ,
            IOobject::NO_WRITE
        );

        if (localIOobject.headerOk())
        {
            CompactIOField<Field<Type>, Type> fieldi(localIOobject);

            label offset = field.size();
            field.setSize(offset + fieldi.size());

            forAll(fieldi, j)
            {
                field[offset + j] = fieldi[j];
            }
        }
    }

    return tfield;
}



template<class Type>
void CML::reconstructLagrangianFields
(
    const word& cloudName,
    const polyMesh& mesh,
    const PtrList<fvMesh>& meshes,
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
)
{
    const word fieldClassName(IOField<Type>::typeName);

    IOobjectList fields = objects.lookupClass(fieldClassName);

    if (fields.size())
    {
        Info<< "    Reconstructing lagrangian "
            << fieldClassName << "s\n" << endl;

        forAllConstIter(IOobjectList, fields, fieldIter)
        {
            if
            (
                selectedFields.empty()
             || selectedFields.found(fieldIter()->name())
            )
            {
                Info<< "        " << fieldIter()->name() << endl;
                reconstructLagrangianField<Type>
                (
                    cloudName,
                    mesh,
                    meshes,
                    fieldIter()->name()
                )().write();
            }
        }

        Info<< endl;
    }
}


template<class Type>
void CML::reconstructLagrangianFieldFields
(
    const word& cloudName,
    const polyMesh& mesh,
    const PtrList<fvMesh>& meshes,
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
)
{
    {
        const word fieldClassName(CompactIOField<Field<Type>, Type>::typeName);

        IOobjectList fields = objects.lookupClass(fieldClassName);

        if (fields.size())
        {
            Info<< "    Reconstructing lagrangian "
                << fieldClassName << "s\n" << endl;

            forAllConstIter(IOobjectList, fields, fieldIter)
            {
                if
                (
                    selectedFields.empty()
                 || selectedFields.found(fieldIter()->name())
                )
                {
                    Info<< "        " << fieldIter()->name() << endl;
                    reconstructLagrangianFieldField<Type>
                    (
                        cloudName,
                        mesh,
                        meshes,
                        fieldIter()->name()
                    )().write();
                }
            }

            Info<< endl;
        }
    }

    {
        const word fieldClassName(IOField<Field<Type> >::typeName);

        IOobjectList fields = objects.lookupClass(fieldClassName);

        if (fields.size())
        {
            Info<< "    Reconstructing lagrangian "
                << fieldClassName << "s\n" << endl;

            forAllConstIter(IOobjectList, fields, fieldIter)
            {
                if
                (
                    selectedFields.empty()
                 || selectedFields.found(fieldIter()->name())
                )
                {
                    Info<< "        " << fieldIter()->name() << endl;
                    reconstructLagrangianFieldField<Type>
                    (
                        cloudName,
                        mesh,
                        meshes,
                        fieldIter()->name()
                    )().write();
                }
            }

            Info<< endl;
        }
    }
}


#endif

// ************************************************************************* //
