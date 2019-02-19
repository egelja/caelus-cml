/*---------------------------------------------------------------------------*\
Copyright (C) 2015 Applied CCM
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
    CML::fv::CorrGaussGrad

Description
    Gradient based on the Gauss' theorem with corrections for fixed gradient
    type of boundaries

Author
    Aleksandar Jemcov
    Darrin Stephens

\*---------------------------------------------------------------------------*/

#ifndef corrGaussGrad_H
#define corrGaussGrad_H

#include "gradScheme.hpp"
#include "surfaceInterpolationScheme.hpp"
#include "linear.hpp"

namespace CML
{
namespace fv
{

template<class Type>
class CorrGaussGrad : public fv::gradScheme<Type>
{

    tmp<surfaceInterpolationScheme<Type> > tinterpScheme_;

    //- Number of iteration for correction loop
    label corrIter_;

    CorrGaussGrad(CorrGaussGrad const&);

    void operator=(CorrGaussGrad const&);
 

public:

    TypeName("corrGauss");

    CorrGaussGrad(fvMesh const& mesh) : gradScheme<Type>(mesh),
        tinterpScheme_(new linear<Type>(mesh)),
        corrIter_(1)
    {}

    CorrGaussGrad(fvMesh const& mesh, Istream& is) : gradScheme<Type>(mesh),
        tinterpScheme_(nullptr),
        corrIter_(0)
    {

        if (is.eof())
        {
            tinterpScheme_ =
                tmp<surfaceInterpolationScheme<Type> >
                (
                    new linear<Type>(mesh)
                );
        }
        else
        {
            
            tinterpScheme_ = tmp<surfaceInterpolationScheme<Type> >
            (
                surfaceInterpolationScheme<Type>::New(mesh, is)
            );
        }

        if (is.eof())
        {
            corrIter_ = 1;
        }
        else
        {
            corrIter_ = readLabel(is);
        }
        
    }

    static
    tmp
    <
        GeometricField
        <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > gradf
    (
        GeometricField<Type, fvsPatchField, surfaceMesh> const&,
        word const&,
        GeometricField<Type, fvPatchField, volMesh> const&,
        label const&
    );

    //- Return the gradient of the given field to the gradScheme::grad
    //  for optional caching
    virtual tmp
    <
        GeometricField
        <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>
    > calcGrad
    (
        GeometricField<Type, fvPatchField, volMesh> const& vsf,
        word const& name
    ) const;

    //- Correct the boundary values of the gradient using the patchField
    // snGrad functions
    static void correctBoundaryConditions
    (
        GeometricField<Type, fvPatchField, volMesh> const&,
        GeometricField
        <typename outerProduct<vector, Type>::type, fvPatchField, volMesh>&
    );
};

}
}

#include "extrapolatedCalculatedFvPatchFields.hpp"

template<class Type> CML::tmp
<
    CML::GeometricField
    <
        typename CML::outerProduct<CML::vector, Type>::type,
        CML::fvPatchField,
        CML::volMesh
    >
>
CML::fv::CorrGaussGrad<Type>::gradf
(
    GeometricField<Type, fvsPatchField, surfaceMesh> const& ssf,
    word const& name,
    GeometricField<Type, fvPatchField, volMesh> const& vsf,
    label const& corrIter
)
{
    typedef typename outerProduct<vector, Type>::type GradType;

    fvMesh const& mesh = ssf.mesh();

    tmp<GeometricField<GradType, fvPatchField, volMesh> > tgGrad
    (
        new GeometricField<GradType, fvPatchField, volMesh>
        (
            IOobject
            (
                name,
                ssf.instance(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimensioned<GradType>
            (
                "0",
                ssf.dimensions()/dimLength,
                pTraits<GradType>::zero
            ),
            extrapolatedCalculatedFvPatchField<GradType>::typeName
        )
    );

    GeometricField<GradType, fvPatchField, volMesh>& gGrad = tgGrad();

    labelUList const& owner = mesh.owner();
    labelUList const& neighbour = mesh.neighbour();
    vectorField const& Sf = mesh.Sf();

    Field<GradType>& igGrad = gGrad;
    Field<Type> const& issf = ssf;

    forAll(owner, facei)
    {
        GradType Sfssf = Sf[facei]*issf[facei];

        igGrad[owner[facei]] += Sfssf;
        igGrad[neighbour[facei]] -= Sfssf;
    }

    forAll(mesh.boundary(), patchi)
    {
        labelUList const& pFaceCells = mesh.boundary()[patchi].faceCells();

        vectorField const& pSf = mesh.Sf().boundaryField()[patchi];

        fvsPatchField<Type> const& pssf = ssf.boundaryField()[patchi];

        forAll(mesh.boundary()[patchi], facei)
        {
            igGrad[pFaceCells[facei]] += pSf[facei]*pssf[facei];
        }
    }

    igGrad /= mesh.V();

    gGrad.correctBoundaryConditions();

    volVectorField const&  C = mesh.C();

    surfaceVectorField const& Cf = mesh.Cf();

    const Field<Type>& vsfIn = vsf.internalField();

    forAll(mesh.boundary(), patchi)
    { 
        if (vsf.boundaryField()[patchi].fixesGradient())
        {
            vectorField const& pCf = Cf.boundaryField()[patchi];

            fvPatch const& p = mesh.boundary()[patchi];

            unallocLabelList const& pOwner = p.faceCells();
				
            // Make a copy of the current face values so we can update them
            fvsPatchField<Type> newpssf(ssf.boundaryField()[patchi]);

            // Access to current face value
            fvsPatchField<Type> const& pssf = ssf.boundaryField()[patchi];
			
            labelUList const& pFaceCells = mesh.boundary()[patchi].faceCells();

            vectorField const& pSf = mesh.Sf().boundaryField()[patchi];
			
            for (label i = 0; i<corrIter; ++i)
            {
                forAll(mesh.boundary()[patchi], facei)
                {
                    label const pown = pOwner[facei];
                    // Extrapolate new face value using current gradient
                    newpssf[facei]  = vsfIn[pFaceCells[facei]] 
                        + ((pCf[facei] - C[pown]) & igGrad[pown]);
                    igGrad[pFaceCells[facei]] += 
                        pSf[facei]*(newpssf[facei] - pssf[facei])
                        /mesh.V()[pown];
                }
            }
        }
    }

    gGrad.correctBoundaryConditions();   

    return tgGrad;
}

template<class Type> CML::tmp
<
    CML::GeometricField
    <
        typename CML::outerProduct<CML::vector, Type>::type,
        CML::fvPatchField,
        CML::volMesh
    >
>
CML::fv::CorrGaussGrad<Type>::calcGrad
(
    GeometricField<Type, fvPatchField, volMesh> const& vsf,
    word const& name
) const
{
    typedef typename outerProduct<vector, Type>::type GradType;

    tmp<GeometricField<GradType, fvPatchField, volMesh> > tgGrad
    (
        gradf(tinterpScheme_().interpolate(vsf), name, vsf, corrIter_)
    );
    GeometricField<GradType, fvPatchField, volMesh>& gGrad = tgGrad();

    correctBoundaryConditions(vsf, gGrad);

    return tgGrad;
}


template<class Type>
void CML::fv::CorrGaussGrad<Type>::correctBoundaryConditions
(
    GeometricField<Type, fvPatchField, volMesh> const& vsf,
    GeometricField
    <
        typename outerProduct<vector, Type>::type, fvPatchField, volMesh
    >& gGrad
)
{
    forAll(vsf.boundaryField(), patchi)
    {
        if (!vsf.boundaryField()[patchi].coupled())
        {
            vectorField const n
            (
                vsf.mesh().Sf().boundaryField()[patchi]
              / vsf.mesh().magSf().boundaryField()[patchi]
            );

            gGrad.boundaryField()[patchi] += n *
            (
                vsf.boundaryField()[patchi].snGrad()
              - (n & gGrad.boundaryField()[patchi])
            );
        }
     }
}

#endif

