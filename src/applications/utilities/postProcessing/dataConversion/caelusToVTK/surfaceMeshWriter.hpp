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
    CML::surfaceMeshWriter

Description
    Write faces with fields

SourceFiles
    surfaceMeshWriter.cpp
    surfaceMeshWriterTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef surfaceMeshWriter_H
#define surfaceMeshWriter_H

#include "pointMesh.hpp"
#include "OFstream.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "vtkMesh.hpp"
#include "indirectPrimitivePatch.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class volPointInterpolation;

/*---------------------------------------------------------------------------*\
                      Class surfaceMeshWriter Declaration
\*---------------------------------------------------------------------------*/

class surfaceMeshWriter
{
    const bool binary_;

    const indirectPrimitivePatch& pp_;

    const fileName fName_;

    std::ofstream os_;


public:

    // Constructors

        //- Construct from components
        surfaceMeshWriter
        (
            const bool binary,
            const indirectPrimitivePatch& pp,
            const word& name,
            const fileName&
        );


    // Member Functions

        std::ofstream& os()
        {
            return os_;
        }

        //- Extract face data
        template<class Type>
        tmp<Field<Type> > getFaceField
        (
            const GeometricField<Type, fvsPatchField, surfaceMesh>&
        ) const;

        //- Write surfaceFields
        template<class Type>
        void write
        (
            const PtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "writeFuns.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
CML::tmp<Field<Type> > CML::surfaceMeshWriter::getFaceField
(
    const GeometricField<Type, fvsPatchField, surfaceMesh>& sfld
) const
{
    const polyBoundaryMesh& patches = sfld.mesh().boundaryMesh();

    tmp<Field<Type> > tfld(new Field<Type>(pp_.size()));
    Field<Type>& fld = tfld();

    forAll(pp_.addressing(), i)
    {
        label facei = pp_.addressing()[i];

        label patchi = patches.whichPatch(facei);

        if (patchi == -1)
        {
            fld[i] = sfld[facei];
        }
        else
        {
            label localFacei = facei - patches[patchi].start();
            fld[i] = sfld.boundaryField()[patchi][localFacei];
        }
    }

    return tfld;
}


template<class Type>
void CML::surfaceMeshWriter::write
(
    const PtrList<GeometricField<Type, fvsPatchField, surfaceMesh> >& sflds
)
{
    forAll(sflds, fieldi)
    {
        const GeometricField<Type, fvsPatchField, surfaceMesh>& fld =
            sflds[fieldi];

        os_ << fld.name() << ' ' << pTraits<Type>::nComponents << ' '
            << pp_.size() << " float" << std::endl;

        DynamicList<floatScalar> fField(pTraits<Type>::nComponents*pp_.size());
        writeFuns::insert(getFaceField(fld)(), fField);
        writeFuns::write(os_, binary_, fField);
    }
}

#endif

// ************************************************************************* //
