/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::GAMGAgglomeration

Description
    Geometric agglomerated algebraic multigrid agglomeration class.

SourceFiles
    GAMGAgglomeration.cpp
    GAMGAgglomerationTemplates.cpp
    GAMGAgglomerate.cpp
    GAMGAgglomerateLduAddressing.cpp

\*---------------------------------------------------------------------------*/

#ifndef GAMGAgglomeration_H
#define GAMGAgglomeration_H

#include "MeshObject.hpp"
#include "lduPrimitiveMesh.hpp"
#include "lduInterfacePtrsList.hpp"
#include "primitiveFields.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class lduMesh;
class lduMatrix;

/*---------------------------------------------------------------------------*\
                    Class GAMGAgglomeration Declaration
\*---------------------------------------------------------------------------*/

class GAMGAgglomeration
:
    public MeshObject<lduMesh, GAMGAgglomeration>
{
protected:

    // Protected data

        //- Max number of levels
        label maxLevels_;

        //- Number of cells in coarsest level
        label nCellsInCoarsestLevel_;

        //- The number of cells in each level
        labelList nCells_;

        //- Cell restriction addressing array.
        //  Maps from the finer to the coarser level.
        PtrList<labelField> restrictAddressing_;

        //- Face restriction addressing array.
        //  Maps from the finer to the coarser level.
        //  Positive indices map the finer faces which form part of the boundary
        //  of the coarser cells to the corresponding coarser cell face.
        //  Negative indices map the finer faces which are internal to the
        //  coarser cells to minus the corresponding coarser cell index minus 1.
        PtrList<labelList> faceRestrictAddressing_;

        //- Hierarchy of mesh addressing
        PtrList<lduPrimitiveMesh> meshLevels_;

        //- Hierarchy interfaces.
        //  Warning: Needs to be deleted explicitly.
        PtrList<lduInterfacePtrsList> interfaceLevels_;

        //- Assemble coarse mesh addressing
        void agglomerateLduAddressing(const label fineLevelIndex);

        //- Shrink the number of levels to that specified
        void compactLevels(const label nCreatedLevels);

        //- Check the need for further agglomeration
        bool continueAgglomerating(const label nCoarseCells) const;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        GAMGAgglomeration(const GAMGAgglomeration&);

        //- Disallow default bitwise assignment
        void operator=(const GAMGAgglomeration&);


public:

    //- Runtime type information
    TypeName("GAMGAgglomeration");


    // Declare run-time constructor selection tables

        //- Runtime selection table for pure geometric agglomerators
        declareRunTimeSelectionTable
        (
            autoPtr,
            GAMGAgglomeration,
            lduMesh,
            (
                const lduMesh& mesh,
                const dictionary& controlDict
            ),
            (
                mesh,
                controlDict
            )
        );

        //- Runtime selection table for matrix or mixed geometric/matrix
        //  agglomerators
        declareRunTimeSelectionTable
        (
            autoPtr,
            GAMGAgglomeration,
            lduMatrix,
            (
                const lduMatrix& matrix,
                const dictionary& controlDict
            ),
            (
                matrix,
                controlDict
            )
        );


    // Constructors

        //- Construct given mesh and controls
        GAMGAgglomeration
        (
            const lduMesh& mesh,
            const dictionary& controlDict
        );


    // Selectors

        //- Return the selected geometric agglomerator
        static const GAMGAgglomeration& New
        (
            const lduMesh& mesh,
            const dictionary& controlDict
        );

        //- Return the selected matrix agglomerator
        static const GAMGAgglomeration& New
        (
            const lduMatrix& matrix,
            const dictionary& controlDict
        );


    //- Destructor
    ~GAMGAgglomeration();


    // Member Functions

        // Access

            label size() const
            {
                return meshLevels_.size();
            }

            //- Return LDU mesh of given level
            const lduMesh& meshLevel(const label leveli) const;

            //- Return LDU interface addressing of given level
            const lduInterfacePtrsList& interfaceLevel
            (
                const label leveli
            ) const;

            //- Return cell restrict addressing of given level
            const labelField& restrictAddressing(const label leveli) const
            {
                return restrictAddressing_[leveli];
            }

            //- Return face restrict addressing of given level
            const labelList& faceRestrictAddressing(const label leveli) const
            {
                return faceRestrictAddressing_[leveli];
            }


        // Restriction and prolongation

            //- Restrict (integrate by summation) cell field
            template<class Type>
            void restrictField
            (
                Field<Type>& cf,
                const Field<Type>& ff,
                const label fineLevelIndex
            ) const;

            //- Restrict (integrate by summation) face field
            template<class Type>
            void restrictFaceField
            (
                Field<Type>& cf,
                const Field<Type>& ff,
                const label fineLevelIndex
            ) const;

            //- Prolong (interpolate by injection) cell field
            template<class Type>
            void prolongField
            (
                Field<Type>& ff,
                const Field<Type>& cf,
                const label coarseLevelIndex
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
void CML::GAMGAgglomeration::restrictField
(
    Field<Type>& cf,
    const Field<Type>& ff,
    const label fineLevelIndex
) const
{
    const labelList& fineToCoarse = restrictAddressing_[fineLevelIndex];

    if (ff.size() != fineToCoarse.size())
    {
        FatalErrorInFunction
            << "field does not correspond to level " << fineLevelIndex
            << " sizes: field = " << ff.size()
            << " level = " << fineToCoarse.size()
            << abort(FatalError);
    }

    cf = pTraits<Type>::zero;

    forAll(ff, i)
    {
        cf[fineToCoarse[i]] += ff[i];
    }
}


template<class Type>
void CML::GAMGAgglomeration::restrictFaceField
(
    Field<Type>& cf,
    const Field<Type>& ff,
    const label fineLevelIndex
) const
{
    const labelList& fineToCoarse = faceRestrictAddressing_[fineLevelIndex];

    cf = pTraits<Type>::zero;

    forAll(fineToCoarse, ffacei)
    {
        label cFace = fineToCoarse[ffacei];

        if (cFace >= 0)
        {
            cf[cFace] += ff[ffacei];
        }
    }
}


template<class Type>
void CML::GAMGAgglomeration::prolongField
(
    Field<Type>& ff,
    const Field<Type>& cf,
    const label coarseLevelIndex
) const
{
    const labelList& fineToCoarse = restrictAddressing_[coarseLevelIndex];

    forAll(fineToCoarse, i)
    {
        ff[i] = cf[fineToCoarse[i]];
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
