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
    CML::sampledPatchInternalField

Description
    Variation of sampledPatch that samples the internalField (at a given
    normal distance from the patch) instead of the patchField.
    Note:
    - interpolate=false : get cell value on faces
    - interpolate=true  : interpolate inside cell and interpolate to points
    There is no option to get interpolated value inside the cell on the faces.

SourceFiles
    sampledPatchInternalField.cpp

\*---------------------------------------------------------------------------*/

#ifndef sampledPatchInternalField_H
#define sampledPatchInternalField_H

#include "sampledPatch.hpp"
#include "mappedPatchBase.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class sampledPatchInternalField Declaration
\*---------------------------------------------------------------------------*/

class sampledPatchInternalField
:
    public sampledPatch
{
    // Private data

        //- Mapping engines
        PtrList<mappedPatchBase> mappers_;


    // Private Member Functions

        //- sample field on faces
        template <class Type>
        tmp<Field<Type> > sampleField
        (
            const GeometricField<Type, fvPatchField, volMesh>& vField
        ) const;

        template <class Type>
        tmp<Field<Type> > interpolateField(const interpolation<Type>&) const;

public:

    //- Runtime type information
    TypeName("sampledPatchInternalField");


    // Constructors

        //- Construct from dictionary
        sampledPatchInternalField
        (
            const word& name,
            const polyMesh& mesh,
            const dictionary& dict
        );


    //- Destructor
    virtual ~sampledPatchInternalField();


    // Member Functions

        //- sample field on surface
        virtual tmp<scalarField> sample
        (
            const volScalarField&
        ) const;

        //- sample field on surface
        virtual tmp<vectorField> sample
        (
            const volVectorField&
        ) const;

        //- sample field on surface
        virtual tmp<sphericalTensorField> sample
        (
            const volSphericalTensorField&
        ) const;

        //- sample field on surface
        virtual tmp<symmTensorField> sample
        (
            const volSymmTensorField&
        ) const;

        //- sample field on surface
        virtual tmp<tensorField> sample
        (
            const volTensorField&
        ) const;


        //- interpolate field on surface
        virtual tmp<scalarField> interpolate
        (
            const interpolation<scalar>&
        ) const;


        //- interpolate field on surface
        virtual tmp<vectorField> interpolate
        (
            const interpolation<vector>&
        ) const;

        //- interpolate field on surface
        virtual tmp<sphericalTensorField> interpolate
        (
            const interpolation<sphericalTensor>&
        ) const;

        //- interpolate field on surface
        virtual tmp<symmTensorField> interpolate
        (
            const interpolation<symmTensor>&
        ) const;

        //- interpolate field on surface
        virtual tmp<tensorField> interpolate
        (
            const interpolation<tensor>&
        ) const;

        //- Write
        virtual void print(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "interpolationCellPoint.hpp"
#include "PrimitivePatchInterpolation_.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template <class Type>
CML::tmp<CML::Field<Type> >
CML::sampledPatchInternalField::sampleField
(
    const GeometricField<Type, fvPatchField, volMesh>& vField
) const
{
    // One value per face
    tmp<Field<Type> > tvalues(new Field<Type>(patchFaceLabels().size()));
    Field<Type>& values = tvalues();

    forAll(patchStart(), i)
    {
        // Get patchface wise data by sampling internal field
        Field<Type> interpVals = vField.internalField();
        mappers_[i].map().distribute(interpVals);

        // Store at correct position in values
        label end =
        (
            i < patchStart().size()-1
          ? patchStart()[i+1]
          : patchFaceLabels().size()
        );

        for (label triI = patchStart()[i]; triI < end; triI++)
        {
            values[triI] = interpVals[patchFaceLabels()[triI]];
        }
    }

    return tvalues;
}


template <class Type>
CML::tmp<CML::Field<Type> >
CML::sampledPatchInternalField::interpolateField
(
    const interpolation<Type>& interpolator
) const
{
    label sz = 0;
    forAll(patchIDs(), i)
    {
        sz += mesh().boundaryMesh()[patchIDs()[i]].size();
    }

    Field<Type> allPatchVals(sz);
    sz = 0;

    forAll(patchIDs(), i)
    {
        // See mappedFixedValueFvPatchField
        const mapDistribute& distMap = mappers_[i].map();

        // Send back sample points to processor that holds the cell.
        // Mark cells with point::max so we know which ones we need
        // to interpolate (since expensive).
        vectorField samples(mappers_[i].samplePoints());
        distMap.reverseDistribute(mesh().nCells(), point::max, samples);

        Field<Type> patchVals(mesh().nCells());

        forAll(samples, cellI)
        {
            if (samples[cellI] != point::max)
            {
                patchVals[cellI] = interpolator.interpolate
                (
                    samples[cellI],
                    cellI
                );
            }
        }

        distMap.distribute(patchVals);

        // Now patchVals holds the interpolated data in patch face order.
        // Collect.
        SubList<Type>(allPatchVals, patchVals.size(), sz).assign(patchVals);
        sz += patchVals.size();
    }

    // Interpolate to points. Reconstruct the patch of all faces to aid
    // interpolation.

    labelList meshFaceLabels(allPatchVals.size());
    sz = 0;
    forAll(patchIDs(), i)
    {
        const polyPatch& pp = mesh().boundaryMesh()[patchIDs()[i]];
        forAll(pp, i)
        {
            meshFaceLabels[sz++] = pp.start()+i;
        }
    }

    indirectPrimitivePatch allPatches
    (
        IndirectList<face>(mesh().faces(), meshFaceLabels),
        mesh().points()
    );

    return PrimitivePatchInterpolation<indirectPrimitivePatch>
    (
        allPatches
    ).faceToPointInterpolate(allPatchVals);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
