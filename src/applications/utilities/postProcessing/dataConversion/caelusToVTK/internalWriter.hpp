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
    CML::internalWriter

Description
    Write fields (internal).

SourceFiles
    internalWriter.cpp
    internalWriterTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef internalWriter_H
#define internalWriter_H

#include "OFstream.hpp"
#include "volFields.hpp"
#include "pointFields.hpp"
#include "vtkMesh.hpp"

using namespace CML;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class volPointInterpolation;

/*---------------------------------------------------------------------------*\
                           Class internalWriter Declaration
\*---------------------------------------------------------------------------*/

class internalWriter
{
    const vtkMesh& vMesh_;

    const bool binary_;

    const fileName fName_;

    std::ofstream os_;

public:

    // Constructors

        //- Construct from components
        internalWriter
        (
            const vtkMesh&,
            const bool binary,
            const fileName&
        );


    // Member Functions

        std::ofstream& os()
        {
            return os_;
        }

        //- Write cellIDs
        void writeCellIDs();

        //- Write generic GeometricFields
        template<class Type, template<class> class PatchField, class GeoMesh>
        void write
        (
            const PtrList<GeometricField<Type, PatchField, GeoMesh> >&
        );

        //- Interpolate and write volFields
        template<class Type>
        void write
        (
            const volPointInterpolation&,
            const PtrList<GeometricField<Type, fvPatchField, volMesh> >&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "writeFuns.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type, template<class> class PatchField, class GeoMesh>
void CML::internalWriter::write
(
    const PtrList<GeometricField<Type, PatchField, GeoMesh> >& flds
)
{
    forAll(flds, i)
    {
        writeFuns::write(os_, binary_, flds[i], vMesh_);
    }
}


template<class Type>
void CML::internalWriter::write
(
    const volPointInterpolation& pInterp,
    const PtrList<GeometricField<Type, fvPatchField, volMesh> >& flds
)
{
    forAll(flds, i)
    {
        writeFuns::write
        (
            os_,
            binary_,
            flds[i],
            pInterp.interpolate(flds[i])(),
            vMesh_
        );
    }
}

#endif

// ************************************************************************* //
