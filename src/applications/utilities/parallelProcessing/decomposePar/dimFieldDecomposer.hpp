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
    CML::dimFieldDecomposer

Description
    Dimensioned field decomposer.

SourceFiles
    dimFieldDecomposer.cpp
    dimFieldDecomposerDecomposeFields.cpp

\*---------------------------------------------------------------------------*/

#ifndef dimFieldDecomposer_H
#define dimFieldDecomposer_H

#include "fvMesh.hpp"
#include "fvPatchFieldMapper.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class IOobjectList;

/*---------------------------------------------------------------------------*\
                    Class fvFieldDecomposer Declaration
\*---------------------------------------------------------------------------*/

class dimFieldDecomposer
{
private:

    // Private data

        //- Reference to complete mesh
        const fvMesh& completeMesh_;

        //- Reference to processor mesh
        const fvMesh& procMesh_;

        //- Reference to face addressing
        const labelList& faceAddressing_;

        //- Reference to cell addressing
        const labelList& cellAddressing_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        dimFieldDecomposer(const dimFieldDecomposer&);

        //- Disallow default bitwise assignment
        void operator=(const dimFieldDecomposer&);


public:

    // Constructors

        //- Construct from components
        dimFieldDecomposer
        (
            const fvMesh& completeMesh,
            const fvMesh& procMesh,
            const labelList& faceAddressing,
            const labelList& cellAddressing
        );


    //- Destructor
    ~dimFieldDecomposer();


    // Member Functions

        //- Decompose field
        template<class Type>
        tmp<DimensionedField<Type, volMesh> > decomposeField
        (
            const DimensionedField<Type, volMesh>& field
        ) const;


        //- Decompose llist of fields
        template<class GeoField>
        void decomposeFields(const PtrList<GeoField>& fields) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::DimensionedField<Type, CML::volMesh> >
CML::dimFieldDecomposer::decomposeField
(
    const DimensionedField<Type, volMesh>& field
) const
{
    // Create and map the internal field values
    Field<Type> mappedField(field, cellAddressing_);

    // Create the field for the processor
    return tmp<DimensionedField<Type, volMesh> >
    (
        new DimensionedField<Type, volMesh>
        (
            IOobject
            (
                field.name(),
                procMesh_.time().timeName(),
                procMesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            procMesh_,
            field.dimensions(),
            mappedField
        )
    );
}


template<class GeoField>
void CML::dimFieldDecomposer::decomposeFields
(
    const PtrList<GeoField>& fields
) const
{
    forAll(fields, fieldI)
    {
        decomposeField(fields[fieldI])().write();
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
