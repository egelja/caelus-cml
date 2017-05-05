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
    CML::interpolationCellPointWallModified

Description
    Same as interpolationCellPoint, but if interpolating a wall face, uses
    cell centre value instead

\*---------------------------------------------------------------------------*/

#ifndef interpolationCellPointWallModified_H
#define interpolationCellPointWallModified_H

#include "interpolationCellPoint.hpp"
#include "cellPointWeightWallModified.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class interpolationCellPoint Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class interpolationCellPointWallModified
:
    public interpolationCellPoint<Type>
{
public:

    //- Runtime type information
    TypeName("cellPointWallModified");


    // Constructors

        //- Construct from components
        interpolationCellPointWallModified
        (
            const GeometricField<Type, fvPatchField, volMesh>& psi
        );


    // Member Functions

        //- Interpolate field for the given cellPointWeight
        inline Type interpolate(const cellPointWeightWallModified& cpw) const;

        //- Interpolate field to the given point in the given cell
        inline Type interpolate
        (
            const vector& position,
            const label cellI,
            const label faceI = -1
        ) const;

        //- Interpolate field to the given point in the tetrahedron
        //  defined by the given indices.
        inline Type interpolate
        (
            const vector& position,
            const tetIndices& tetIs,
            const label faceI = -1
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
inline Type CML::interpolationCellPointWallModified<Type>::interpolate
(
    const cellPointWeightWallModified& cpw
) const
{
    const List<scalar>& weights = cpw.weights();
    const List<label>& faceVertices = cpw.faceVertices();

    Type t = this->psi_[cpw.cell()]*weights[0];
    t += this->psip_[faceVertices[0]]*weights[1];
    t += this->psip_[faceVertices[1]]*weights[2];
    t += this->psip_[faceVertices[2]]*weights[3];

    return t;
}


template<class Type>
inline Type CML::interpolationCellPointWallModified<Type>::interpolate
(
    const vector& position,
    const label cellI,
    const label faceI
) const
{
    return interpolate
    (
        cellPointWeightWallModified
        (
            this->pMesh_,
            position,
            cellI,
            faceI
        )
    );
}


template<class Type>
inline Type CML::interpolationCellPointWallModified<Type>::interpolate
(
    const vector& position,
    const tetIndices& tetIs,
    const label faceI
) const
{
    if (faceI >= 0)
    {
        if (faceI != tetIs.face())
        {
            FatalErrorIn
            (
                "inline Type "
                "CML::interpolationCellPointWallModifie<Type>::interpolate"
                "("
                    "const vector& position, "
                    "const tetIndices& tetIs, "
                    "const label faceI"
                ") const"
            )
                << "specified face " << faceI << " inconsistent with the face "
                << "stored by tetIndices: " << tetIs.face()
                << exit(FatalError);
        }

        const polyBoundaryMesh& bm = this->pMesh_.boundaryMesh();
        label patchI = bm.whichPatch(faceI);

        if (patchI != -1)
        {
            if (isA<wallPolyPatch>(bm[patchI]))
            {
                Type t = this->psi_[tetIs.cell()];

                return t;
            }
        }
    }

    // If the wall face selection did not return, then use the normal
    // interpolate method

    return interpolationCellPoint<Type>::interpolate
    (
        position,
        tetIs,
        faceI
    );
}


// * * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * //

template<class Type>
CML::interpolationCellPointWallModified<Type>::
interpolationCellPointWallModified
(
    const GeometricField<Type, fvPatchField, volMesh>& psi
)
:
    interpolationCellPoint<Type>(psi)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
