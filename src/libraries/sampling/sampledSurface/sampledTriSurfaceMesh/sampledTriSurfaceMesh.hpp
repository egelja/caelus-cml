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
    CML::sampledTriSurfaceMesh

Description
    A sampledSurface from a triSurfaceMesh. It samples on the points/triangles
    of the triSurface.

    - it either samples cells or (non-coupled) boundary faces

    - 4 different modes:
        - source=cells, interpolate=false:
            finds per triangle centre the nearest cell centre and uses its value
        - source=cells, interpolate=true
            finds per triangle centre the nearest cell centre.
            Per surface point checks if this nearest cell is the one containing
            point; otherwise projects the point onto the nearest point on
            the boundary of the cell (to make sure interpolateCellPoint
            gets a valid location)

        - source=boundaryFaces, interpolate=false:
            finds per triangle centre the nearest point on the boundary
            (uncoupled faces only) and uses the value (or 0 if the nearest
            is on an empty boundary)
        - source=boundaryFaces, interpolate=true:
            finds per triangle centre the nearest point on the boundary
            (uncoupled faces only).
            Per surface point projects the point onto this boundary face
            (to make sure interpolateCellPoint gets a valid location)

    - since it finds a nearest per triangle each triangle is guaranteed
    to be on one processor only. So after stitching (by sampledSurfaces)
    the original surface should be complete.

SourceFiles
    sampledTriSurfaceMesh.cpp

\*---------------------------------------------------------------------------*/

#ifndef sampledTriSurfaceMesh_H
#define sampledTriSurfaceMesh_H

#include "sampledSurface.hpp"
#include "triSurfaceMesh.hpp"
#include "MeshedSurface.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class treeDataFace;

/*---------------------------------------------------------------------------*\
                       Class sampledTriSurfaceMesh Declaration
\*---------------------------------------------------------------------------*/

class sampledTriSurfaceMesh
:
    public sampledSurface,
    public MeshedSurface<face>
{
public:
        //- Types of communications
        enum samplingSource
        {
            cells,
            boundaryFaces
        };

private:

    //- Private typedefs for convenience
        typedef MeshedSurface<face> MeshStorage;


    // Private data

        static const NamedEnum<samplingSource, 2> samplingSourceNames_;

        //- Surface to sample on
        const triSurfaceMesh surface_;

        //- Whether to sample internal cell values or boundary values
        const samplingSource sampleSource_;

        //- Track if the surface needs an update
        mutable bool needsUpdate_;

        //- Search tree for all non-coupled boundary faces
        mutable autoPtr<indexedOctree<treeDataFace> > boundaryTreePtr_;

        //- From local surface triangle to mesh cell/face.
        labelList sampleElements_;

        //- Local points to sample per point
        pointField samplePoints_;


    // Private Member Functions

        //- Get tree of all non-coupled boundary faces
        const indexedOctree<treeDataFace>& nonCoupledboundaryTree() const;

        //- sample field on faces
        template <class Type>
        tmp<Field<Type> > sampleField
        (
            const GeometricField<Type, fvPatchField, volMesh>& vField
        ) const;


        template <class Type>
        tmp<Field<Type> >
        interpolateField(const interpolation<Type>&) const;

public:

    //- Runtime type information
    TypeName("sampledTriSurfaceMesh");


    // Constructors

        //- Construct from components
        sampledTriSurfaceMesh
        (
            const word& name,
            const polyMesh& mesh,
            const word& surfaceName,
            const samplingSource sampleSource
        );

        //- Construct from dictionary
        sampledTriSurfaceMesh
        (
            const word& name,
            const polyMesh& mesh,
            const dictionary& dict
        );


    //- Destructor
    virtual ~sampledTriSurfaceMesh();


    // Member Functions

        //- Does the surface need an update?
        virtual bool needsUpdate() const;

        //- Mark the surface as needing an update.
        //  May also free up unneeded data.
        //  Return false if surface was already marked as expired.
        virtual bool expire();

        //- Update the surface as required.
        //  Do nothing (and return false) if no update was needed
        virtual bool update();


        //- Points of surface
        virtual const pointField& points() const
        {
            return MeshStorage::points();
        }

        //- Faces of surface
        virtual const faceList& faces() const
        {
            return MeshStorage::faces();
        }


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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template <class Type>
CML::tmp<CML::Field<Type> >
CML::sampledTriSurfaceMesh::sampleField
(
    const GeometricField<Type, fvPatchField, volMesh>& vField
) const
{
    // One value per face
    tmp<Field<Type> > tvalues(new Field<Type>(sampleElements_.size()));
    Field<Type>& values = tvalues();

    if (sampleSource_ == cells)
    {
        // Sample cells

        forAll(sampleElements_, triI)
        {
            values[triI] = vField[sampleElements_[triI]];
        }
    }
    else
    {
        // Sample boundary faces

        const polyBoundaryMesh& pbm = mesh().boundaryMesh();
        label nBnd = mesh().nFaces()-mesh().nInternalFaces();

        // Create flat boundary field

        Field<Type> bVals(nBnd, pTraits<Type>::zero);

        forAll(vField.boundaryField(), patchI)
        {
            label bFaceI = pbm[patchI].start() - mesh().nInternalFaces();

            SubList<Type>
            (
                bVals,
                vField.boundaryField()[patchI].size(),
                bFaceI
            ).assign(vField.boundaryField()[patchI]);
        }

        // Sample in flat boundary field

        forAll(sampleElements_, triI)
        {
            label faceI = sampleElements_[triI];
            values[triI] = bVals[faceI-mesh().nInternalFaces()];
        }
    }

    return tvalues;
}


template <class Type>
CML::tmp<CML::Field<Type> >
CML::sampledTriSurfaceMesh::interpolateField
(
    const interpolation<Type>& interpolator
) const
{
    // One value per vertex
    tmp<Field<Type> > tvalues(new Field<Type>(sampleElements_.size()));
    Field<Type>& values = tvalues();

    if (sampleSource_ == cells)
    {
        // Sample cells.

        forAll(sampleElements_, pointI)
        {
            values[pointI] = interpolator.interpolate
            (
                samplePoints_[pointI],
                sampleElements_[pointI]
            );
        }
    }
    else
    {
        // Sample boundary faces.

        forAll(samplePoints_, pointI)
        {
            label faceI = sampleElements_[pointI];

            values[pointI] = interpolator.interpolate
            (
                samplePoints_[pointI],
                mesh().faceOwner()[faceI],
                faceI
            );
        }
    }

    return tvalues;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
