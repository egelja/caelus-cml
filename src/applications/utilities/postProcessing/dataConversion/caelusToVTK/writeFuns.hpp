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
    CML::writeFuns

Description
    Various functions for collecting and writing binary data.

SourceFiles
    writeFuns.cpp

\*---------------------------------------------------------------------------*/

#ifndef writeFuns_H
#define writeFuns_H

#include "floatScalar.hpp"
#include "DynamicList.hpp"
#include "volFieldsFwd.hpp"
#include "pointFieldsFwd.hpp"
#include "vtkMesh.hpp"
#include "volPointInterpolation.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class writeFuns Declaration
\*---------------------------------------------------------------------------*/

class writeFuns
{
    // Private Member Functions

        // Swap halves of word.

            static void swapWord(label& word32);
            static void swapWords(const label nWords, label* words32);


public:

    // Write ascii or binary. If binary optionally in-place swaps argument

        static void write(std::ostream&, const bool, List<floatScalar>&);
        static void write(std::ostream&, const bool, DynamicList<floatScalar>&);
        static void write(std::ostream&, const bool, labelList&);
        static void write(std::ostream&, const bool, DynamicList<label>&);


    // Write header

        static void writeHeader
        (
            std::ostream&,
            const bool isBinary,
            const std::string& title
        );
        static void writeCellDataHeader
        (
            std::ostream&,
            const label nCells,
            const label nFields
        );
        static void writePointDataHeader
        (
            std::ostream&,
            const label nPoints,
            const label nFields
        );


    // Convert to VTK and store

        static void insert(const scalar, DynamicList<floatScalar>&);
        static void insert(const point&, DynamicList<floatScalar>&);
        static void insert(const sphericalTensor&, DynamicList<floatScalar>&);
        static void insert(const symmTensor&, DynamicList<floatScalar>&);
        static void insert(const tensor&, DynamicList<floatScalar>&);


    //- Append elements to DynamicList
    static void insert(const labelList&, DynamicList<label>&);
    template<class Type>
    static void insert(const List<Type>&, DynamicList<floatScalar>&);

    //- Write volField with cell values (including decomposed cells)
    template<class Type>
    static void write
    (
        std::ostream&,
        const bool binary,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const vtkMesh&
    );

    //- Write pointField on all mesh points. Interpolate to cell centre
    //  for decomposed cell centres.
    template<class Type>
    static void write
    (
        std::ostream&,
        const bool binary,
        const GeometricField<Type, pointPatchField, pointMesh>&,
        const vtkMesh&
    );

    //- Write interpolated field on points and original cell values on
    //  decomposed cell centres.
    template<class Type>
    static void write
    (
        std::ostream&,
        const bool binary,
        const GeometricField<Type, fvPatchField, volMesh>&,
        const GeometricField<Type, pointPatchField, pointMesh>&,
        const vtkMesh&
    );

    //- Write generic GeometricFields
    template<class Type, template<class> class PatchField, class GeoMesh>
    static void write
    (
        std::ostream&,
        const bool binary,
        const PtrList<GeometricField<Type, PatchField, GeoMesh> >&,
        const vtkMesh&
    );

    //- Interpolate and write volFields
    template<class Type>
    static void write
    (
        std::ostream&,
        const bool binary,
        const volPointInterpolation&,
        const PtrList<GeometricField<Type, fvPatchField, volMesh> >&,
        const vtkMesh&
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "interpolatePointToCell.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Store List in dest
template<class Type>
void CML::writeFuns::insert
(
    const List<Type>& source,
    DynamicList<floatScalar>& dest
)
{
    forAll(source, i)
    {
        insert(source[i], dest);
    }
}


//// Store List (indexed through map) in dest
//template<class Type>
//void CML::writeFuns::insert
//(
//    const labelList& map,
//    const List<Type>& source,
//    DynamicList<floatScalar>& dest
//)
//{
//    forAll(map, i)
//    {
//        insert(source[map[i]], dest);
//    }
//}


template<class Type>
void CML::writeFuns::write
(
    std::ostream& os,
    const bool binary,
    const GeometricField<Type, fvPatchField, volMesh>& vvf,
    const vtkMesh& vMesh
)
{
    const fvMesh& mesh = vMesh.mesh();

    const labelList& superCells = vMesh.topo().superCells();

    label nValues = mesh.nCells() + superCells.size();

    os  << vvf.name() << ' ' << int(pTraits<Type>::nComponents) << ' '
        << nValues << " float" << std::endl;

    DynamicList<floatScalar> fField(pTraits<Type>::nComponents*nValues);

    insert(vvf.internalField(), fField);

    forAll(superCells, superCelli)
    {
        label origCelli = superCells[superCelli];

        insert(vvf[origCelli], fField);
    }
    write(os, binary, fField);
}


template<class Type>
void CML::writeFuns::write
(
    std::ostream& os,
    const bool binary,
    const GeometricField<Type, pointPatchField, pointMesh>& pvf,
    const vtkMesh& vMesh
)
{
    const fvMesh& mesh = vMesh.mesh();
    const vtkTopo& topo = vMesh.topo();

    const labelList& addPointCellLabels = topo.addPointCellLabels();
    const label nTotPoints = mesh.nPoints() + addPointCellLabels.size();

    os  << pvf.name() << ' ' << int(pTraits<Type>::nComponents) << ' '
        << nTotPoints << " float" << std::endl;

    DynamicList<floatScalar> fField(pTraits<Type>::nComponents*nTotPoints);

    insert(pvf, fField);

    forAll(addPointCellLabels, api)
    {
        label origCelli = addPointCellLabels[api];

        insert(interpolatePointToCell(pvf, origCelli), fField);
    }
    write(os, binary, fField);
}


template<class Type>
void CML::writeFuns::write
(
    std::ostream& os,
    const bool binary,
    const GeometricField<Type, fvPatchField, volMesh>& vvf,
    const GeometricField<Type, pointPatchField, pointMesh>& pvf,
    const vtkMesh& vMesh
)
{
    const fvMesh& mesh = vMesh.mesh();
    const vtkTopo& topo = vMesh.topo();

    const labelList& addPointCellLabels = topo.addPointCellLabels();
    const label nTotPoints = mesh.nPoints() + addPointCellLabels.size();

    os  << vvf.name() << ' ' << int(pTraits<Type>::nComponents) << ' '
        << nTotPoints << " float" << std::endl;

    DynamicList<floatScalar> fField(pTraits<Type>::nComponents*nTotPoints);

    insert(pvf, fField);

    forAll(addPointCellLabels, api)
    {
        label origCelli = addPointCellLabels[api];

        insert(vvf[origCelli], fField);
    }
    write(os, binary, fField);
}


template<class Type, template<class> class PatchField, class GeoMesh>
void CML::writeFuns::write
(
    std::ostream& os,
    const bool binary,
    const PtrList<GeometricField<Type, PatchField, GeoMesh> >& flds,
    const vtkMesh& vMesh
)
{
    forAll(flds, i)
    {
        write(os, binary, flds[i], vMesh);
    }
}


template<class Type>
void CML::writeFuns::write
(
    std::ostream& os,
    const bool binary,
    const volPointInterpolation& pInterp,
    const PtrList<GeometricField<Type, fvPatchField, volMesh> >& flds,
    const vtkMesh& vMesh
)
{
    forAll(flds, i)
    {
        write(os, binary, flds[i], pInterp.interpolate(flds[i])(), vMesh);
    }
}

#endif

// ************************************************************************* //
