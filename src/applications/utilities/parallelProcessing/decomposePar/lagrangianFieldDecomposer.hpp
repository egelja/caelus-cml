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

Class
    CML::lagrangianFieldDecomposer

Description
    Lagrangian field decomposer.

SourceFiles
    lagrangianFieldDecomposer.cpp
    lagrangianFieldDecomposerDecomposeFields.cpp

\*---------------------------------------------------------------------------*/

#ifndef lagrangianFieldDecomposer_H
#define lagrangianFieldDecomposer_H

#include "Cloud.hpp"
#include "CompactIOField.hpp"
#include "indexedParticle.hpp"
#include "passiveParticle.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class IOobjectList;

/*---------------------------------------------------------------------------*\
                    Class lagrangianFieldDecomposer Declaration
\*---------------------------------------------------------------------------*/

class lagrangianFieldDecomposer
{
    // Private data

        //- Reference to processor mesh
        const polyMesh& procMesh_;

        //- Lagrangian positions for this processor
        Cloud<passiveParticle> positions_;

        //- The indices of the particles on this processor
        labelList particleIndices_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        lagrangianFieldDecomposer(const lagrangianFieldDecomposer&);

        //- Disallow default bitwise assignment
        void operator=(const lagrangianFieldDecomposer&);


public:

    // Constructors

        //- Construct from components
        lagrangianFieldDecomposer
        (
            const polyMesh& mesh,
            const polyMesh& procMesh,
            const labelList& faceProcAddressing,
            const labelList& cellProcAddressing,
            const word& cloudName,
            const Cloud<indexedParticle>& lagrangianPositions,
            const List<SLList<indexedParticle*>*>& cellParticles
        );


    // Member Functions

        // Read the fields and hold on the pointer list
        template<class Type>
        static void readFields
        (
            const label cloudI,
            const IOobjectList& lagrangianObjects,
            PtrList<PtrList<IOField<Type> > >& lagrangianFields
//            PtrList<IOField<Type> >& lagrangianFields
        );

        template<class Type>
        static void readFieldFields
        (
            const label cloudI,
            const IOobjectList& lagrangianObjects,
            PtrList
            <
                PtrList<CompactIOField<Field<Type>, Type> >
            >& lagrangianFields
//            PtrList<CompactIOField<Field<Type>, Type > >& lagrangianFields
        );


        //- Decompose volume field
        template<class Type>
        tmp<IOField<Type> > decomposeField
        (
            const word& cloudName,
            const IOField<Type>& field
        ) const;

        template<class Type>
        tmp<CompactIOField<Field<Type>, Type> > decomposeFieldField
        (
            const word& cloudName,
            const CompactIOField<Field<Type>, Type>& field
        ) const;


        template<class GeoField>
        void decomposeFields
        (
            const word& cloudName,
            const PtrList<GeoField>& fields
        ) const;

        template<class GeoField>
        void decomposeFieldFields
        (
            const word& cloudName,
            const PtrList<GeoField>& fields
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IOobjectList.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::lagrangianFieldDecomposer::readFields
(
    const label cloudI,
    const IOobjectList& lagrangianObjects,
    PtrList<PtrList<IOField<Type> > >& lagrangianFields
)
{
    // Search list of objects for lagrangian fields
    IOobjectList lagrangianTypeObjects
    (
        lagrangianObjects.lookupClass(IOField<Type>::typeName)
    );

    lagrangianFields.set
    (
        cloudI,
        new PtrList<IOField<Type> >
        (
            lagrangianTypeObjects.size()
        )
    );

    label lagrangianFieldi = 0;
    forAllIter(IOobjectList, lagrangianTypeObjects, iter)
    {
        lagrangianFields[cloudI].set
        (
            lagrangianFieldi++,
            new IOField<Type>(*iter())
        );
    }
}


template<class Type>
void CML::lagrangianFieldDecomposer::readFieldFields
(
    const label cloudI,
    const IOobjectList& lagrangianObjects,
    PtrList<PtrList<CompactIOField<Field<Type>, Type> > >& lagrangianFields
)
{
    // Search list of objects for lagrangian fields
    IOobjectList lagrangianTypeObjectsA
    (
        lagrangianObjects.lookupClass(IOField<Field<Type> >::typeName)
    );

    IOobjectList lagrangianTypeObjectsB
    (
        lagrangianObjects.lookupClass
        (
            CompactIOField<Field<Type>,
            Type>::typeName
        )
    );

    lagrangianFields.set
    (
        cloudI,
        new PtrList<CompactIOField<Field<Type>, Type> >
        (
            lagrangianTypeObjectsA.size() + lagrangianTypeObjectsB.size()
        )
    );

    label lagrangianFieldi = 0;

    forAllIter(IOobjectList, lagrangianTypeObjectsA, iter)
    {
        lagrangianFields[cloudI].set
        (
            lagrangianFieldi++,
            new CompactIOField<Field<Type>, Type>(*iter())
        );
    }

    forAllIter(IOobjectList, lagrangianTypeObjectsB, iter)
    {
        lagrangianFields[cloudI].set
        (
            lagrangianFieldi++,
            new CompactIOField<Field<Type>, Type>(*iter())
        );
    }
}


template<class Type>
CML::tmp<CML::IOField<Type> >
CML::lagrangianFieldDecomposer::decomposeField
(
    const word& cloudName,
    const IOField<Type>& field
) const
{
    // Create and map the internal field values
    Field<Type> procField(field, particleIndices_);

    // Create the field for the processor
    return tmp<IOField<Type> >
    (
        new IOField<Type>
        (
            IOobject
            (
                field.name(),
                procMesh_.time().timeName(),
                cloud::prefix/cloudName,
                procMesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            procField
        )
    );
}


template<class Type>
CML::tmp<CML::CompactIOField<CML::Field<Type>, Type> >
CML::lagrangianFieldDecomposer::decomposeFieldField
(
    const word& cloudName,
    const CompactIOField<Field<Type>, Type>& field
) const
{
    // Create and map the internal field values
    Field<Field<Type> > procField(field, particleIndices_);

    // Create the field for the processor
    return tmp<CompactIOField<Field<Type>, Type> >
    (
        new CompactIOField<Field<Type>, Type>
        (
            IOobject
            (
                field.name(),
                procMesh_.time().timeName(),
                cloud::prefix/cloudName,
                procMesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            procField
        )
    );
}


template<class GeoField>
void CML::lagrangianFieldDecomposer::decomposeFields
(
    const word& cloudName,
    const PtrList<GeoField>& fields
) const
{
    if (particleIndices_.size())
    {
        forAll(fields, fieldI)
        {
            decomposeField(cloudName, fields[fieldI])().write();
        }
    }
}


template<class GeoField>
void CML::lagrangianFieldDecomposer::decomposeFieldFields
(
    const word& cloudName,
    const PtrList<GeoField>& fields
) const
{
    if (particleIndices_.size())
    {
        forAll(fields, fieldI)
        {
            decomposeFieldField(cloudName, fields[fieldI])().write();
        }
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
