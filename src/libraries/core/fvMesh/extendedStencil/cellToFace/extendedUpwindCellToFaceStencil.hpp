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
    CML::extendedUpwindCellToFaceStencil

Description
    Creates upwind stencil by shifting a centred stencil to upwind and downwind
    faces and optionally removing all non-(up/down)wind faces ('pureUpwind').

    Note: the minOpposedness parameter is to decide which upwind and
    downwind faces to combine the stencils from. If myArea is the
    local area and upwindArea
    the area of the possible upwind candidate it will be included if
        (upwindArea & myArea)/magSqr(myArea) > minOpposedness
    so this includes both cosine and area. WIP.

SourceFiles
    extendedUpwindCellToFaceStencil.cpp

\*---------------------------------------------------------------------------*/

#ifndef extendedUpwindCellToFaceStencil_H
#define extendedUpwindCellToFaceStencil_H

#include "extendedCellToFaceStencil.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class cellToFaceStencil;

/*---------------------------------------------------------------------------*\
               Class extendedUpwindCellToFaceStencil Declaration
\*---------------------------------------------------------------------------*/

class extendedUpwindCellToFaceStencil
:
    public extendedCellToFaceStencil
{
    // Private data

        //- Does stencil contain upwind points only
        const bool pureUpwind_;

        //- Swap map for getting neighbouring data
        autoPtr<mapDistribute> ownMapPtr_;
        autoPtr<mapDistribute> neiMapPtr_;

        //- Per face the stencil.
        labelListList ownStencil_;
        labelListList neiStencil_;



    // Private Member Functions

        //- Find most 'opposite' faces of cell
        void selectOppositeFaces
        (
            const boolList& nonEmptyFace,
            const scalar minOpposedness,
            const label faceI,
            const label cellI,
            DynamicList<label>& oppositeFaces
        ) const;

        //- Transport (centred) face stencil to 'opposite' face.
        void transportStencil
        (
            const boolList& nonEmptyFace,
            const labelListList& faceStencil,
            const scalar minOpposedness,
            const label faceI,
            const label cellI,
            const bool stencilHasNeighbour,

            DynamicList<label>& oppositeFaces,
            labelHashSet& faceStencilSet,
            labelList& transportedStencil
        ) const;

        //- Transport (centred) face stencil to 'opposite' faces.
        void transportStencils
        (
            const labelListList& faceStencil,
            const scalar minOpposedness,
            labelListList& ownStencil,
            labelListList& neiStencil
        );


        //- Disallow default bitwise copy construct
        extendedUpwindCellToFaceStencil(const extendedUpwindCellToFaceStencil&);

        //- Disallow default bitwise assignment
        void operator=(const extendedUpwindCellToFaceStencil&);


public:

    // Constructors

        //- Construct from mesh and uncompacted centred face stencil.
        //  Transports facestencil to create owner and neighbour versions.
        //  pureUpwind to remove any remaining downwind cells.
        extendedUpwindCellToFaceStencil
        (
            const cellToFaceStencil&,
            const bool pureUpwind,
            const scalar minOpposedness
        );

        //- Construct from mesh and uncompacted centred face stencil. Splits
        //  stencil into owner and neighbour (so always pure upwind)
        extendedUpwindCellToFaceStencil
        (
            const cellToFaceStencil&
        );


    // Member Functions

        bool pureUpwind() const
        {
            return pureUpwind_;
        }

        //- Return reference to the parallel distribution map
        const mapDistribute& ownMap() const
        {
            return ownMapPtr_();
        }

        //- Return reference to the parallel distribution map
        const mapDistribute& neiMap() const
        {
            return neiMapPtr_();
        }

        //- Return reference to the stencil
        const labelListList& ownStencil() const
        {
            return ownStencil_;
        }

        //- Return reference to the stencil
        const labelListList& neiStencil() const
        {
            return neiStencil_;
        }

        //- Sum vol field contributions to create face values
        template<class Type>
        tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > weightedSum
        (
            const surfaceScalarField& phi,
            const GeometricField<Type, fvPatchField, volMesh>& fld,
            const List<List<scalar> >& ownWeights,
            const List<List<scalar> >& neiWeights
        ) const;

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::tmp<CML::GeometricField<Type, CML::fvsPatchField, CML::surfaceMesh> >
CML::extendedUpwindCellToFaceStencil::weightedSum
(
    const surfaceScalarField& phi,
    const GeometricField<Type, fvPatchField, volMesh>& fld,
    const List<List<scalar> >& ownWeights,
    const List<List<scalar> >& neiWeights
) const
{
    const fvMesh& mesh = fld.mesh();

    // Collect internal and boundary values
    List<List<Type> > ownFld;
    collectData(ownMap(), ownStencil(), fld, ownFld);
    List<List<Type> > neiFld;
    collectData(neiMap(), neiStencil(), fld, neiFld);

    tmp<GeometricField<Type, fvsPatchField, surfaceMesh> > tsfCorr
    (
        new GeometricField<Type, fvsPatchField, surfaceMesh>
        (
            IOobject
            (
                fld.name(),
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh,
            dimensioned<Type>
            (
                fld.name(),
                fld.dimensions(),
                pTraits<Type>::zero
            )
        )
    );
    GeometricField<Type, fvsPatchField, surfaceMesh>& sf = tsfCorr();

    // Internal faces
    for (label faceI = 0; faceI < mesh.nInternalFaces(); faceI++)
    {
        if (phi[faceI] > 0)
        {
            // Flux out of owner. Use upwind (= owner side) stencil.
            const List<Type>& stField = ownFld[faceI];
            const List<scalar>& stWeight = ownWeights[faceI];

            forAll(stField, i)
            {
                sf[faceI] += stField[i]*stWeight[i];
            }
        }
        else
        {
            const List<Type>& stField = neiFld[faceI];
            const List<scalar>& stWeight = neiWeights[faceI];

            forAll(stField, i)
            {
                sf[faceI] += stField[i]*stWeight[i];
            }
        }
    }

    // Boundaries. Either constrained or calculated so assign value
    // directly (instead of nicely using operator==)
    typename GeometricField<Type, fvsPatchField, surfaceMesh>::
        GeometricBoundaryField& bSfCorr = sf.boundaryField();

    forAll(bSfCorr, patchi)
    {
        fvsPatchField<Type>& pSfCorr = bSfCorr[patchi];

        if (pSfCorr.coupled())
        {
            label faceI = pSfCorr.patch().start();

            forAll(pSfCorr, i)
            {
                if (phi.boundaryField()[patchi][i] > 0)
                {
                    // Flux out of owner. Use upwind (= owner side) stencil.
                    const List<Type>& stField = ownFld[faceI];
                    const List<scalar>& stWeight = ownWeights[faceI];

                    forAll(stField, j)
                    {
                        pSfCorr[i] += stField[j]*stWeight[j];
                    }
                }
                else
                {
                    const List<Type>& stField = neiFld[faceI];
                    const List<scalar>& stWeight = neiWeights[faceI];

                    forAll(stField, j)
                    {
                        pSfCorr[i] += stField[j]*stWeight[j];
                    }
                }
                faceI++;
            }
        }
    }

    return tsfCorr;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
