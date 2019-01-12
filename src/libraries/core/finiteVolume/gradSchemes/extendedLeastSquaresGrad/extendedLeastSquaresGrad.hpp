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
    CML::fv::extendedLeastSquaresGrad

Description
    Second-order gradient scheme using least-squares.


\*---------------------------------------------------------------------------*/

#ifndef extendedLeastSquaresGrad_H
#define extendedLeastSquaresGrad_H

#include "gradScheme.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace fv
{

/*---------------------------------------------------------------------------*\
                      Class extendedLeastSquaresGrad Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class extendedLeastSquaresGrad
:
    public fv::gradScheme<Type>
{
    // Private Data

        //- Minimum determinant criterion to choose extra cells
        scalar minDet_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        extendedLeastSquaresGrad(const extendedLeastSquaresGrad&);

        //- Disallow default bitwise assignment
        void operator=(const extendedLeastSquaresGrad&);


public:

    //- Runtime type information
    TypeName("extendedLeastSquares");


    // Constructors

        //- Construct from Istream
        extendedLeastSquaresGrad(const fvMesh& mesh, Istream& schemeData)
        :
            gradScheme<Type>(mesh),
            minDet_(readScalar(schemeData))
        {
            if (minDet_ < 0)    //-for facearea weighted: || minDet_ > 8)
            {
                FatalIOErrorInFunction(schemeData)
                    << "Minimum determinant = " << minDet_
                    << " should be >= 0"    // and <= 8"
                    << exit(FatalIOError);
            }
        }


    // Member Functions

        //- Return the gradient of the given field to the gradScheme::grad
        //  for optional caching
        virtual tmp
        <
            GeometricField
            <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
        > calcGrad
        (
            const GeometricField<Type, fvPatchField, volMesh>& vsf,
            const word& name
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "extendedLeastSquaresVectors.hpp"
#include "gaussGrad.hpp"
#include "fvMesh.hpp"
#include "volMesh.hpp"
#include "surfaceMesh.hpp"
#include "GeometricField.hpp"
#include "zeroGradientFvPatchField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class Type>
CML::tmp
<
    CML::GeometricField
    <
        typename CML::outerProduct<CML::vector, Type>::type,
        CML::fvPatchField,
    CML::volMesh
    >
>
CML::fv::extendedLeastSquaresGrad<Type>::calcGrad
(
    const GeometricField<Type, fvPatchField, volMesh>& vsf,
    const word& name
) const
{
    typedef typename outerProduct<vector, Type>::type GradType;

    const fvMesh& mesh = vsf.mesh();

    tmp<GeometricField<GradType, fvPatchField, volMesh> > tlsGrad
    (
        new GeometricField<GradType, fvPatchField, volMesh>
        (
            IOobject
            (
                name,
                vsf.instance(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimensioned<GradType>
            (
                "zero",
                vsf.dimensions()/dimLength,
                pTraits<GradType>::zero
            ),
            zeroGradientFvPatchField<GradType>::typeName
        )
    );
    GeometricField<GradType, fvPatchField, volMesh>& lsGrad = tlsGrad();

    // Get reference to least square vectors
    const extendedLeastSquaresVectors& lsv = extendedLeastSquaresVectors::New
    (
        mesh,
        minDet_
    );

    const surfaceVectorField& ownLs = lsv.pVectors();
    const surfaceVectorField& neiLs = lsv.nVectors();

    const labelUList& owner = mesh.owner();
    const labelUList& neighbour = mesh.neighbour();

    forAll(owner, facei)
    {
        label own = owner[facei];
        label nei = neighbour[facei];

        Type deltaVsf = vsf[nei] - vsf[own];

        lsGrad[own] += ownLs[facei]*deltaVsf;
        lsGrad[nei] -= neiLs[facei]*deltaVsf;
    }

    // Boundary faces
    forAll(vsf.boundaryField(), patchi)
    {
        const fvsPatchVectorField& patchOwnLs = ownLs.boundaryField()[patchi];

        const labelUList& faceCells =
            lsGrad.boundaryField()[patchi].patch().faceCells();

        if (vsf.boundaryField()[patchi].coupled())
        {
            const Field<Type> neiVsf
            (
                vsf.boundaryField()[patchi].patchNeighbourField()
            );

            forAll(neiVsf, patchFaceI)
            {
                lsGrad[faceCells[patchFaceI]] +=
                    patchOwnLs[patchFaceI]
                   *(neiVsf[patchFaceI] - vsf[faceCells[patchFaceI]]);
            }
        }
        else
        {
            const fvPatchField<Type>& patchVsf = vsf.boundaryField()[patchi];

            forAll(patchVsf, patchFaceI)
            {
                lsGrad[faceCells[patchFaceI]] +=
                     patchOwnLs[patchFaceI]
                    *(patchVsf[patchFaceI] - vsf[faceCells[patchFaceI]]);
            }
        }
    }


    const List<labelPair>& additionalCells = lsv.additionalCells();
    const vectorField& additionalVectors = lsv.additionalVectors();

    forAll(additionalCells, i)
    {
        lsGrad[additionalCells[i][0]] +=
            additionalVectors[i]
           *(vsf[additionalCells[i][1]] - vsf[additionalCells[i][0]]);
    }


    lsGrad.correctBoundaryConditions();
    gaussGrad<Type>::correctBoundaryConditions(vsf, lsGrad);

    return tlsGrad;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
