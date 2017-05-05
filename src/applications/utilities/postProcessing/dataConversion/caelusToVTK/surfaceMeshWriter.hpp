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
    const vtkMesh& vMesh_;

    const bool binary_;

    const indirectPrimitivePatch& pp_;

    const fileName fName_;

    std::ofstream os_;

//    label nPoints_;
//
//    label nFaces_;

public:

    // Constructors

        //- Construct from components
        surfaceMeshWriter
        (
            const vtkMesh&,
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

//        label nPoints() const
//        {
//            return nPoints_;
//        }
//
//        label nFaces() const
//        {
//            return nFaces_;
//        }
//
//        //- Write cellIDs
//        void writePatchIDs();

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
        label faceI = pp_.addressing()[i];

        label patchI = patches.whichPatch(faceI);

        if (patchI == -1)
        {
            fld[i] = sfld[faceI];
        }
        else
        {
            label localFaceI = faceI - patches[patchI].start();
            fld[i] = sfld.boundaryField()[patchI][localFaceI];
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
    forAll(sflds, fieldI)
    {
        const GeometricField<Type, fvsPatchField, surfaceMesh>& fld =
            sflds[fieldI];

        os_ << fld.name() << ' ' << pTraits<Type>::nComponents << ' '
            << pp_.size() << " float" << std::endl;

        DynamicList<floatScalar> fField(pTraits<Type>::nComponents*pp_.size());
        writeFuns::insert(getFaceField(fld)(), fField);
        writeFuns::write(os_, binary_, fField);
    }
}




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
