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
    MDLimiter

Description
    Multidimensional limiter class

SourceFiles


References

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef MDLIMITER_HPP_
#define MDLIMITER_HPP_

#include "fvm.hpp"
#include "zeroGradientFvPatchFields.hpp"
#include "bjLimiter.hpp"
#include "vLimiter.hpp"

namespace CML
{

template <typename LimiterFunctionType, typename FieldType>
class MDLimiter
{};

template <typename LimiterFunctionType>
class MDLimiter<LimiterFunctionType,volScalarField>
{
    fvMesh           const& mesh_;
    volScalarField   const& phi_;
    volScalarField          phiLimiter_;
    volScalarField          phiMinValue_;
    volScalarField          phiMaxValue_;

public:

    MDLimiter
    (
        fvMesh         const& mesh,
        volScalarField const& phi
    )
    :
        mesh_(mesh),
        phi_(phi),
        phiLimiter_
        (
            IOobject
            (
                "phiLimiter",
		 this->mesh_.time().timeName(),
                 this->mesh_,
                 IOobject::NO_READ,
                 IOobject::AUTO_WRITE
            ),
            this->mesh_,
            dimensionedScalar("pLimiter", dimless, 1.0),
            zeroGradientFvPatchVectorField::typeName
	),
        phiMinValue_
        (
            IOobject
            (
                "phiMinValue",
                this->mesh_.time().timeName(),
                this->mesh_,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            this->mesh_,
            dimensionedScalar("pMinValue", dimless, 0.0),
            zeroGradientFvPatchScalarField::typeName
	),
        phiMaxValue_
        (
            IOobject
            (
                "pMaxValue",
                this->mesh_.time().timeName(),
                this->mesh_,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
             ),
             this->mesh_,
             dimensionedScalar("pMaxValue", dimless, 0.0),
             zeroGradientFvPatchScalarField::typeName
        )
    {
        unallocLabelList const& owner = this->mesh_.owner();
        unallocLabelList const& neighbour = this->mesh_.neighbour();

        forAll (owner, faceI)
        {
            label const own = owner[faceI];
            label const nei = neighbour[faceI];

            // min values
            this->phiMinValue_[own] = min(phi_[own], phi_[nei]);
            this->phiMinValue_[nei] = min(phi_[nei], phi_[own]);

            // max values
            this->phiMaxValue_[own] = max(phi_[own], phi_[nei]);
            this->phiMaxValue_[nei] = max(phi_[nei], phi_[own]);
        }
    }

public:

    volScalarField const& getPhiLimiter() const
    {
        return this->phiLimiter_;
    }

    void limitField
    (
        volVectorField const& gradPhi
    )
    {
        unallocLabelList const& owner = this->mesh_.owner();
        unallocLabelList const& neighbour = this->mesh_.neighbour();
        
        this->phiLimiter_ = 1.0;

        forAll(owner, faceI)
        {
            label const own = owner[faceI];
            label const nei = neighbour[faceI];

	    // min values
            this->phiMinValue_[own] = min(this->phiMinValue_[own], phi_[nei]);
            this->phiMinValue_[nei] = min(this->phiMinValue_[nei], phi_[own]);

	    // max values
            this->phiMaxValue_[own] = max(this->phiMaxValue_[own], phi_[nei]);
            this->phiMaxValue_[nei] = max(this->phiMaxValue_[nei], phi_[own]);
        }

        // take care of boundaries
        this->phiMinValue_.correctBoundaryConditions();
        this->phiMaxValue_.correctBoundaryConditions();

        volScalarField cellVolume
        (
            IOobject
            (
                "cellVolume",
                this->mesh_.time().timeName(),
                this->mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            this->mesh_,
            dimVolume,
            zeroGradientFvPatchScalarField::typeName
        );

        cellVolume.internalField() = this->mesh_.V();
        cellVolume.correctBoundaryConditions();

        volVectorField const& cellCenter = mesh_.C();
        surfaceVectorField const& faceCenter = mesh_.Cf();

        LimiterFunctionType limitFunction;

        // Compute limiter values
        forAll(owner, faceI)
        {
            label const own = owner[faceI];
            label const nei = neighbour[faceI];

            vector const deltaRLeft  = faceCenter[faceI] - cellCenter[own];
            vector const deltaRRight = faceCenter[faceI] - cellCenter[nei];

	    scalar phiOwnerLimiter = limitFunction.limiter
            (
                cellVolume[own],
                this->phiMaxValue_[own]-this->phi_[own],
                this->phiMinValue_[own]-this->phi_[own],
                gradPhi[own],
                gradPhi[own],
                deltaRLeft
	    );

            scalar phiNeighbourLimiter = limitFunction.limiter
            (
                cellVolume[nei],
                this->phiMaxValue_[nei]-this->phi_[nei],
                this->phiMinValue_[nei]-this->phi_[nei],
                gradPhi[nei],
                gradPhi[nei],
                deltaRRight
            );
		
            // find minimal limiter value in each cell
            this->phiLimiter_[own] =
                min(this->phiLimiter_[own], phiOwnerLimiter);

            this->phiLimiter_[nei] =
                min(this->phiLimiter_[nei], phiNeighbourLimiter);
        }

        this->phiLimiter_.correctBoundaryConditions();
    }
};

template <typename LimiterFunctionType>
class MDLimiter<LimiterFunctionType,volVectorField>
{
    fvMesh           const& mesh_;
    volVectorField   const& phi_;
    volVectorField          phiLimiter_;
    volVectorField          phiMinValue_;
    volVectorField          phiMaxValue_;

public:

    MDLimiter
    (
        fvMesh         const& mesh,
        volVectorField const& phi
    )
    :
        mesh_(mesh),
        phi_(phi),
        phiLimiter_
        (
            IOobject
            (
                "phiLimiter",
		 this->mesh_.time().timeName(),
                 this->mesh_,
                 IOobject::NO_READ,
                 IOobject::AUTO_WRITE
            ),
            this->mesh_,
            dimensionedVector("phiLimiter", dimless, vector::one),
            zeroGradientFvPatchVectorField::typeName
	),
        phiMinValue_
        (
            IOobject
            (
                "phiMinValue",
                this->mesh_.time().timeName(),
                this->mesh_,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
            ),
            this->mesh_,
            dimensionedVector("phiMinValue", dimless, vector::zero),
            zeroGradientFvPatchScalarField::typeName
	),
        phiMaxValue_
        (
            IOobject
            (
                "phiMaxValue",
                this->mesh_.time().timeName(),
                this->mesh_,
                IOobject::NO_READ,
                IOobject::AUTO_WRITE
             ),
             this->mesh_,
	    dimensionedVector("phiMaxValue", dimless, vector::zero),
             zeroGradientFvPatchScalarField::typeName
        )
    {
        unallocLabelList const& owner = this->mesh_.owner();
        unallocLabelList const& neighbour = this->mesh_.neighbour();

        forAll (owner, faceI)
        {
            label const own = owner[faceI];
            label const nei = neighbour[faceI];

            // min values
            this->phiMinValue_[own] = min(phi_[own], phi_[nei]);
            this->phiMinValue_[nei] = min(phi_[nei], phi_[own]);

            // max values
            this->phiMaxValue_[own] = max(phi_[own], phi_[nei]);
            this->phiMaxValue_[nei] = max(phi_[nei], phi_[own]);
        }
    }

public:

    volVectorField const& getPhiLimiter() const
    {
        return this->phiLimiter_;
    }

    void limitField
    (
        volTensorField const& gradPhi
    )
    {
        unallocLabelList const& owner = this->mesh_.owner();
        unallocLabelList const& neighbour = this->mesh_.neighbour();
        
        this->phiLimiter_ = vector::one;

        forAll(owner, faceI)
        {
            label const own = owner[faceI];
            label const nei = neighbour[faceI];

	    // min values
            this->phiMinValue_[own] = min(this->phiMinValue_[own], phi_[nei]);
            this->phiMinValue_[nei] = min(this->phiMinValue_[nei], phi_[own]);

	    // max values
            this->phiMaxValue_[own] = max(this->phiMaxValue_[own], phi_[nei]);
            this->phiMaxValue_[nei] = max(this->phiMaxValue_[nei], phi_[own]);
        }

        // take care of boundaries
        this->phiMinValue_.correctBoundaryConditions();
        this->phiMaxValue_.correctBoundaryConditions();

        volScalarField cellVolume
        (
            IOobject
            (
                "cellVolume",
                this->mesh_.time().timeName(),
                this->mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            this->mesh_,
            dimVolume,
            zeroGradientFvPatchScalarField::typeName
        );

        cellVolume.internalField() = this->mesh_.V();
        cellVolume.correctBoundaryConditions();

        volVectorField const& cellCenter = mesh_.C();
        surfaceVectorField const& faceCenter = mesh_.Cf();

        LimiterFunctionType limitFunction;

        // Compute limiter values
        forAll(owner, faceI)
        {
            label const own = owner[faceI];
            label const nei = neighbour[faceI];

            vector const deltaRLeft  = faceCenter[faceI] - cellCenter[own];
            vector const deltaRRight = faceCenter[faceI] - cellCenter[nei];

	    vector phiOwnerLimiter = limitFunction.limiter
            (
                cellVolume[own],
                this->phiMaxValue_[own]-this->phi_[own],
                this->phiMinValue_[own]-this->phi_[own],
                gradPhi[own],
                gradPhi[own],
                deltaRLeft
	    );

            vector phiNeighbourLimiter = limitFunction.limiter
            (
                cellVolume[nei],
                this->phiMaxValue_[nei]-this->phi_[nei],
                this->phiMinValue_[nei]-this->phi_[nei],
                gradPhi[nei],
                gradPhi[nei],
                deltaRRight
            );
		
            // find minimal limiter value in each cell
            this->phiLimiter_[own] =
                min(this->phiLimiter_[own], phiOwnerLimiter);

            this->phiLimiter_[nei] =
                min(this->phiLimiter_[nei], phiNeighbourLimiter);
        }

        this->phiLimiter_.correctBoundaryConditions();
    }
};

}
#endif
