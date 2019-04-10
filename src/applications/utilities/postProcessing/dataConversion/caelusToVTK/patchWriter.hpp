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
    CML::patchWriter

Description
    Write patch fields

SourceFiles
    patchWriter.cpp
    patchWriterTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef patchWriter_H
#define patchWriter_H

#include "pointMesh.hpp"
#include "OFstream.hpp"
#include "volFields.hpp"
#include "pointFields.hpp"
#include "vtkMesh.hpp"
#include "indirectPrimitivePatch.hpp"
#include "PrimitivePatchInterpolation_.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class volPointInterpolation;

/*---------------------------------------------------------------------------*\
                           Class patchWriter Declaration
\*---------------------------------------------------------------------------*/

class patchWriter
{
    const vtkMesh& vMesh_;

    const bool binary_;

    const bool nearCellValue_;

    const fileName fName_;

    const labelList patchIDs_;

    std::ofstream os_;

    label nPoints_;

    label nFaces_;

public:

    // Constructors

        //- Construct from components
        patchWriter
        (
            const vtkMesh&,
            const bool binary,
            const bool nearCellValue,
            const fileName&,
            const labelList& patchIDs
        );


    // Member Functions

        std::ofstream& os()
        {
            return os_;
        }

        label nPoints() const
        {
            return nPoints_;
        }

        label nFaces() const
        {
            return nFaces_;
        }

        //- Write cellIDs
        void writePatchIDs();

        //- Write volFields
        template<class Type>
        void write
        (
            const PtrList<GeometricField<Type, fvPatchField, volMesh> >&
        );

        //- Write pointFields
        template<class Type>
        void write
        (
            const PtrList<GeometricField<Type, pointPatchField, pointMesh> >&
        );

        //- Interpolate and write volFields
        template<class Type>
        void write
        (
            const PrimitivePatchInterpolation<primitivePatch>&,
            const PtrList<GeometricField<Type, fvPatchField, volMesh> >&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "writeFuns.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void CML::patchWriter::write
(
    const PtrList<GeometricField<Type, fvPatchField, volMesh> >& flds
)
{
    forAll(flds, fieldi)
    {
        const GeometricField<Type, fvPatchField, volMesh>& fld = flds[fieldi];

        os_ << fld.name() << ' ' << int(pTraits<Type>::nComponents) << ' '
            << nFaces_ << " float" << std::endl;

        DynamicList<floatScalar> fField(pTraits<Type>::nComponents*nFaces_);

        forAll(patchIDs_, j)
        {
            label patchi = patchIDs_[j];

            const fvPatchField<Type>& pfld = fld.boundaryField()[patchi];

            if (nearCellValue_)
            {
                writeFuns::insert(pfld.patchInternalField()(), fField);
            }
            else
            {
                writeFuns::insert(pfld, fField);
            }
        }
        writeFuns::write(os_, binary_, fField);
    }
}


template<class Type>
void CML::patchWriter::write
(
    const PtrList<GeometricField<Type, pointPatchField, pointMesh> >& flds
)
{
    forAll(flds, fieldi)
    {
        const GeometricField<Type, pointPatchField, pointMesh>& fld =
            flds[fieldi];

        os_ << fld.name() << ' ' << int(pTraits<Type>::nComponents) << ' '
            << nPoints_ << " float" << std::endl;

        DynamicList<floatScalar> fField(pTraits<Type>::nComponents*nPoints_);

        forAll(patchIDs_, j)
        {
            label patchi = patchIDs_[j];

            const pointPatchField<Type>& pfld = fld.boundaryField()[patchi];

            writeFuns::insert(pfld.patchInternalField()(), fField);
        }
        writeFuns::write(os_, binary_, fField);
    }
}


template<class Type>
void CML::patchWriter::write
(
    const PrimitivePatchInterpolation<primitivePatch>& pInter,
    const PtrList<GeometricField<Type, fvPatchField, volMesh> >& flds
)
{
    forAll(flds, fieldi)
    {
        const GeometricField<Type, fvPatchField, volMesh>& fld = flds[fieldi];

        os_ << fld.name() << ' ' << int(pTraits<Type>::nComponents) << ' '
            << nPoints_ << " float" << std::endl;

        DynamicList<floatScalar> fField(pTraits<Type>::nComponents*nPoints_);

        forAll(patchIDs_, j)
        {
            label patchi = patchIDs_[j];

            const fvPatchField<Type>& pfld = fld.boundaryField()[patchi];

            if (nearCellValue_)
            {
                writeFuns::insert
                (
                    pInter.faceToPointInterpolate
                    (
                        pfld.patchInternalField()()
                    )(),
                    fField
                );
            }
            else
            {
                writeFuns::insert
                (
                    pInter.faceToPointInterpolate(pfld)(),
                    fField
                );
            }
        }
        writeFuns::write(os_, binary_, fField);
    }
}

#endif

// ************************************************************************* //
