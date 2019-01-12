/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
Copyright (C) 2015-2017 OpenCFD Ltd.
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

Class
    CML::parLagrangianRedistributor

Description
    Lagrangian field redistributor.

    Runs in parallel. Redistributes from fromMesh to toMesh.

SourceFiles
    parLagrangianRedistributor.cpp

\*---------------------------------------------------------------------------*/

#ifndef parLagrangianRedistributor_HPP
#define parLagrangianRedistributor_HPP

#include "PtrList.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class mapDistributePolyMesh;
class mapDistributeBase;
class IOobjectList;
class passivePositionParticleCloud;

/*---------------------------------------------------------------------------*\
                    Class parLagrangianRedistributor Declaration
\*---------------------------------------------------------------------------*/

class parLagrangianRedistributor
{
    // Private data

        //- Source mesh reference
        const fvMesh& srcMesh_;

        //- Destination mesh reference
        const fvMesh& tgtMesh_;

        //- Distribution map reference
        const mapDistributePolyMesh& distMap_;

        //- For every src cell the target processor
        labelList destinationProcID_;

        //- For every src cell the target cell
        labelList destinationCell_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        parLagrangianRedistributor(const parLagrangianRedistributor&);

        //- Disallow default bitwise assignment
        void operator=(const parLagrangianRedistributor&);


public:

    // Constructors

        //- Construct from components
        parLagrangianRedistributor
        (
            const fvMesh& srcMesh,
            const fvMesh& tgtMesh,
            const label nOldCells,
            const mapDistributePolyMesh& distMap
        );


    // Member Functions

        //- Find all clouds (on all processors) and for each cloud all
        //  the objects. Result will be synchronised on all processors
        static void findClouds
        (
            const fvMesh&,
            wordList& cloudNames,
            List<wordList>& objectNames
        );

        //- Redistribute and write lagrangian positions
        autoPtr<mapDistributeBase> redistributeLagrangianPositions
        (
            passivePositionParticleCloud& cloud
        ) const;

        //- Read, redistribute and write lagrangian positions
        autoPtr<mapDistributeBase> redistributeLagrangianPositions
        (
            const word& cloudName
        ) const;

        //- Pick up any fields of a given type
        template<class Type>
        static wordList filterObjects
        (
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        );

        //- Read, redistribute and write all/selected lagrangian fields
        template<class Type>
        void redistributeLagrangianFields
        (
            const mapDistributeBase& map,
            const word& cloudName,
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        ) const;

        //- Read, redistribute and write all/selected lagrangian fieldFields
        template<class Type>
        void redistributeLagrangianFieldFields
        (
            const mapDistributeBase& map,
            const word& cloudName,
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        ) const;

        //- Read and store all fields of a cloud
        template<class Container>
        static void readLagrangianFields
        (
            const passivePositionParticleCloud& cloud,
            const IOobjectList& objects,
            const HashSet<word>& selectedFields
        );

        //- Redistribute and write stored lagrangian fields
        template<class Container>
        void redistributeStoredLagrangianFields
        (
            const mapDistributeBase& map,
            passivePositionParticleCloud& cloud
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Time.hpp"
#include "IOobjectList.hpp"
#include "mapDistributePolyMesh.hpp"
#include "cloud_.hpp"
#include "CompactIOField.hpp"
#include "passivePositionParticleCloud.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Container>
CML::wordList CML::parLagrangianRedistributor::filterObjects
(
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
)
{
    const word fieldClassName(Container::typeName);

    // Parallel synchronise
    wordList fieldNames(objects.names(fieldClassName));
    Pstream::combineGather(fieldNames, ListUniqueEqOp<word>());
    Pstream::combineScatter(fieldNames);

    if (!selectedFields.empty())
    {
        DynamicList<word> selectedNames(fieldNames.size());
        forAll(fieldNames, i)
        {
            if (selectedFields.found(fieldNames[i]))
            {
                selectedNames.append(fieldNames[i]);
            }
        }
        fieldNames.transfer(selectedNames);
    }
    return fieldNames;
}


template<class Type>
void CML::parLagrangianRedistributor::redistributeLagrangianFields
(
    const mapDistributeBase& map,
    const word& cloudName,
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
) const
{
    const wordList objectNames
    (
        filterObjects<IOField<Type> >
        (
            objects,
            selectedFields
        )
    );

    if (objectNames.size())
    {
        const word fieldClassName(IOField<Type>::typeName);

        Info<< "    Redistributing lagrangian "
            << fieldClassName << "s\n" << endl;

        forAll(objectNames, i)
        {
            Info<< "        " <<  objectNames[i] << endl;

            // Read if present
            IOField<Type> field
            (
                IOobject
                (
                    objectNames[i],
                    srcMesh_.time().timeName(),
                    cloud::prefix/cloudName,
                    srcMesh_,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE,
                    false
                ),
                label(0)
            );

            map.distribute(field);


            if (field.size())
            {
                IOField<Type>
                (
                    IOobject
                    (
                        objectNames[i],
                        tgtMesh_.time().timeName(),
                        cloud::prefix/cloudName,
                        tgtMesh_,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE,
                        false
                    ),
                    xferMove<Field<Type> >(field)
                ).write();
            }
        }

        Info<< endl;
    }
}


template<class Type>
void CML::parLagrangianRedistributor::redistributeLagrangianFieldFields
(
    const mapDistributeBase& map,
    const word& cloudName,
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
) const
{
    wordList objectNames
    (
        filterObjects<CompactIOField<Field<Type>, Type> >
        (
            objects,
            selectedFields
        )
    );

    // Append IOField names
    {
        const wordList ioFieldNames
        (
            filterObjects<IOField<Field<Type> > >
            (
                objects,
                selectedFields
            )
        );
        objectNames.append(ioFieldNames);
    }


    if (objectNames.size())
    {
        const word fieldClassName(CompactIOField<Field<Type>, Type>::typeName);

        Info<< "    Redistributing lagrangian "
            << fieldClassName << "s\n" << endl;

        forAll(objectNames, i)
        {
            Info<< "        " <<  objectNames[i] << endl;

            // Read if present
            CompactIOField<Field<Type>, Type > field
            (
                IOobject
                (
                    objectNames[i],
                    srcMesh_.time().timeName(),
                    cloud::prefix/cloudName,
                    srcMesh_,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE,
                    false
                ),
                label(0)
            );

            // Distribute
            map.distribute(field);

            // Write
            if (field.size())
            {
                CompactIOField<Field<Type>, Type>
                (
                    IOobject
                    (
                        objectNames[i],
                        tgtMesh_.time().timeName(),
                        cloud::prefix/cloudName,
                        tgtMesh_,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE,
                        false
                    ),
                    xferMove<Field<Field<Type> > >(field)
                ).write();
            }
        }
    }
}


template<class Container>
void CML::parLagrangianRedistributor::readLagrangianFields
(
    const passivePositionParticleCloud& cloud,
    const IOobjectList& objects,
    const HashSet<word>& selectedFields
)
{
    const wordList objectNames
    (
        filterObjects<Container>
        (
            objects,
            selectedFields
        )
    );

    if (objectNames.size())
    {
        const word fieldClassName(Container::typeName);

        Info<< "    Reading lagrangian "
            << fieldClassName << "s\n" << endl;

        forAll(objectNames, i)
        {
            Info<< "        " <<  objectNames[i] << endl;

            // Read if present
            Container* fieldPtr = new Container
            (
                IOobject
                (
                    objectNames[i],
                    cloud.time().timeName(),
                    cloud,
                    IOobject::READ_IF_PRESENT,
                    IOobject::NO_WRITE
                ),
                label(0)
            );

            fieldPtr->store();
        }
    }
}


template<class Container>
void CML::parLagrangianRedistributor::redistributeStoredLagrangianFields
(
    const mapDistributeBase& map,
    passivePositionParticleCloud& cloud
) const
{
    HashTable<Container*> fields
    (
        cloud.lookupClass<Container >()
    );

    if (fields.size())
    {
        const word fieldClassName(Container::typeName);

        Info<< "    Redistributing lagrangian "
            << fieldClassName << "s\n" << endl;

        forAllIter(typename HashTable<Container*>, fields, iter)
        {
            Container& field = *iter();

            Info<< "        " <<  field.name() << endl;

            map.distribute(field);

            if (field.size())
            {
                Container
                (
                    IOobject
                    (
                        field.name(),
                        tgtMesh_.time().timeName(),
                        cloud::prefix/cloud.name(),
                        tgtMesh_,
                        IOobject::NO_READ,
                        IOobject::NO_WRITE,
                        false
                    ),
                    xferMove<Field<typename Container::value_type> >(field)
                ).write();
            }
        }
    }
}
#endif

// ************************************************************************* //
