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
    CML::wallDistData

Description
    Wall distance calculation. Like wallDist but also transports extra
    data (template argument).

    Used for e.g reflection vector calculation or vanDriest damping.

    Templated on two parameters:
        - TransferType: type of overall data transported
          (e.g. wallPointData\<vector\>)

SourceFiles
    wallDistData.cpp

\*---------------------------------------------------------------------------*/

#ifndef wallDistData_H
#define wallDistData_H

#include "cellDistFuncs.hpp"
#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class wallDistData Declaration
\*---------------------------------------------------------------------------*/

template<class TransferType>
class wallDistData
:
    public volScalarField,
    public cellDistFuncs
{

private:

    typedef typename TransferType::dataType Type;


    // Private Member Data

        //- Reference to field whose data to use (on walls) and update
        //  (every cell and non-wall face)
        GeometricField<Type, fvPatchField, volMesh>& field_;

        //- Do accurate distance calculation for near-wall cells.
        bool correctWalls_;

        //- Number of unset cells and faces.
        label nUnset_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        wallDistData(const wallDistData&);

        //- Disallow default bitwise assignment
        void operator=(const wallDistData&);


public:

    // Constructors

        //- Construct from mesh and flag whether or not to correct wall.
        //  Calculate for all cells. correctWalls : correct wall (face&point)
        //  cells for correct distance, searching neighbours.
        wallDistData
        (
            const fvMesh& mesh,
            GeometricField<Type, fvPatchField, volMesh>&,
            bool correctWalls = true
        );


    //- Destructor
    virtual ~wallDistData();


    // Member Functions

        const volScalarField& y() const
        {
            return *this;
        }

        label nUnset() const
        {
            return nUnset_;
        }

        //- Access field
        const GeometricField<Type, fvPatchField, volMesh>& data() const
        {
            return field_;
        }

        //- Correct for mesh geom/topo changes
        virtual void correct();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "patchDataWave.hpp"
#include "wallPolyPatch.hpp"
#include "emptyFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

// Construct from components
template<class TransferType>
CML::wallDistData<TransferType>::wallDistData
(
    const CML::fvMesh& mesh,
    GeometricField<Type, fvPatchField, volMesh>& field,
    const bool correctWalls
)
:
    volScalarField
    (
        IOobject
        (
            "y",
            mesh.time().timeName(),
            mesh
        ),
        mesh,
        dimensionedScalar("y", dimLength, GREAT)
    ),
    cellDistFuncs(mesh),
    field_(field),
    correctWalls_(correctWalls),
    nUnset_(0)
{
    correct();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class TransferType>
CML::wallDistData<TransferType>::~wallDistData()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

// Correct for mesh geom/topo changes
template<class TransferType>
void CML::wallDistData<TransferType>::correct()
{
    const polyMesh& mesh = cellDistFuncs::mesh();

    //
    // Fill data on wall patches with initial values
    //

    // Get patchids of walls
    labelHashSet wallPatchIDs(getPatchIDs<wallPolyPatch>());

    // Collect pointers to data on patches
    UPtrList<Field<Type> > patchData(mesh.boundaryMesh().size());

    forAll(field_.boundaryField(), patchI)
    {
        patchData.set(patchI, &field_.boundaryField()[patchI]);
    }

    // Do mesh wave
    patchDataWave<TransferType> wave
    (
        mesh,
        wallPatchIDs,
        patchData,
        correctWalls_
    );

    // Transfer cell values from wave into *this and field_
    transfer(wave.distance());

    field_.transfer(wave.cellData());

    // Transfer values on patches into boundaryField of *this and field_
    forAll(boundaryField(), patchI)
    {
        scalarField& waveFld = wave.patchDistance()[patchI];

        if (!isA<emptyFvPatchScalarField>(boundaryField()[patchI]))
        {
            boundaryField()[patchI].transfer(waveFld);

            Field<Type>& wavePatchData = wave.patchData()[patchI];

            field_.boundaryField()[patchI].transfer(wavePatchData);
        }
    }

    // Transfer number of unset values
    nUnset_ = wave.nUnset();
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
