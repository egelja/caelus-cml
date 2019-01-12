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
    CML::interpolationCellPatchConstrained

Description
    Uses the cell value for any point in the cell apart from a boundary face
    where it uses the boundary value directly.
    Note: will not work on an empty patch.

\*---------------------------------------------------------------------------*/

#ifndef interpolationCellPatchConstrained_H
#define interpolationCellPatchConstrained_H

#include "interpolation.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;

/*---------------------------------------------------------------------------*\
                           Class interpolationCellPatchConstrained Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class interpolationCellPatchConstrained
:
    public interpolation<Type>
{

public:

    //- Runtime type information
    TypeName("cellPatchConstrained");


    // Constructors

        //- Construct from components
        interpolationCellPatchConstrained
        (
            const GeometricField<Type, fvPatchField, volMesh>& psi
        );


    // Member Functions

        //- Interpolate field to the given point in the given cell
        Type interpolate
        (
            const vector& position,
            const label celli,
            const label facei = -1
        ) const;

        //- Interpolate field to the given coordinates in the tetrahedron
        //  defined by the given indices. This is an optimisation which skips
        //  calculating the position, as cell interpolation doesn't need it.
        inline Type interpolate
        (
            const barycentric& coordinates,
            const tetIndices& tetIs,
            const label facei = -1
        ) const
        {
            return interpolate(vector::zero, tetIs.cell(), facei);
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "volFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * //

template<class Type>
interpolationCellPatchConstrained<Type>::interpolationCellPatchConstrained
(
    const GeometricField<Type, fvPatchField, volMesh>& psi
)
:
    interpolation<Type>(psi)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Type interpolationCellPatchConstrained<Type>::interpolate
(
    const vector& pt,
    const label cellI,
    const label faceI
) const
{
    if (faceI >= 0 && faceI >= this->psi_.mesh().nInternalFaces())
    {
        // Use boundary value
        const polyBoundaryMesh& pbm = this->psi_.mesh().boundaryMesh();
        label patchI = pbm.patchID()[faceI-this->psi_.mesh().nInternalFaces()];
        label patchFaceI = pbm[patchI].whichFace(faceI);

        return this->psi_.boundaryField()[patchI][patchFaceI];
    }
    else
    {
        return this->psi_[cellI];
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
