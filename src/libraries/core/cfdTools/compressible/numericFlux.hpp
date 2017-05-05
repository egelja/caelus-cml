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

Description
    Numeric flux class for approxiamte Riemann solvers

Author
    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/
#ifndef numericFlux_HPP
#define numericFlux_HPP

#include "fvCFD.hpp"
#include "basicThermo.hpp"
#include "mdLimiter.hpp"

namespace CML
{

template <typename Flux, typename Limiter>
class numericFlux : public Flux
{

    fvMesh const& mesh_;

    volScalarField const& p_;

    volVectorField const& U_;

    volScalarField const& T_;

    basicThermo& thermophysicalModel_;

    surfaceScalarField rhoFlux_;

    surfaceVectorField rhoUFlux_;

    surfaceScalarField rhoEFlux_;

    surfaceVectorField dotX_;

    volVectorField gradp_;

    volTensorField gradU_;

    volVectorField gradT_;

    numericFlux(const numericFlux&);

    void operator=(const numericFlux&);

public:

    numericFlux
    (
        volScalarField const& p,
        volVectorField const& U,
        volScalarField const& T,
        basicThermo& thermo
    );

    fvMesh const& mesh() const
    {
        return this->mesh_;
    }


    surfaceScalarField const & rhoFlux() const
    {
        return this->rhoFlux_;
    }

    surfaceVectorField const& rhoUFlux() const
    {
        return this->rhoUFlux_;
    }

    surfaceScalarField const& rhoEFlux() const
    {
        return this->rhoEFlux_;
    }

    surfaceVectorField const& dotX() const
    {
        return this->dotX_;
    }

    surfaceVectorField& dotX()
    {
        return this->dotX_;
    }

    volVectorField const& gradp() const
    {
        return this->gradp_;
    }

    volTensorField const& gradU() const
    {
        return this->gradU_;
    }

    const volVectorField& gradT() const
    {
        return this->gradT_;
    }

    void update();
};


}

template <typename Flux, typename Limiter>
CML::numericFlux<Flux,Limiter>::numericFlux
(
    volScalarField const& p,
    volVectorField const& U,
    volScalarField const& T,
    basicThermo& thermophysicalModel
) : 
    mesh_(p.mesh()),
    p_(p),
    U_(U),
    T_(T),
    thermophysicalModel_(thermophysicalModel),
    rhoFlux_
    (
        IOobject
        (
            "rhoFlux",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        (linearInterpolate(p/(( thermophysicalModel_.Cp()
        - thermophysicalModel_.Cv() )*T)*U) & mesh_.Sf())
    ),
    rhoUFlux_
    (
        IOobject
        (
            "rhoUFlux",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        rhoFlux_*linearInterpolate(U)
    ),
    rhoEFlux_
    (
        IOobject
        (
            "rhoEFlux",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        rhoFlux_*linearInterpolate(thermophysicalModel_.Cv()*T+0.5*magSqr(U))
    ),
    dotX_
    (
        IOobject
        (
            "dotX",
            mesh_.time().timeName(),
            mesh_,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh_,
        dimensionedVector("zero", dimVelocity, vector::zero)
    ),
    gradp_(fvc::grad(this->p_)),
    gradU_(fvc::grad(this->U_)),
    gradT_(fvc::grad(this->T_))
{}

template <typename Flux, typename Limiter>
void CML::numericFlux<Flux,Limiter>::update()
{
    unallocLabelList const& owner = this->mesh_.owner();
    unallocLabelList const& neighbour = this->mesh_.neighbour();

    surfaceVectorField const& Sf = this->mesh_.Sf();
    surfaceScalarField const& magSf = this->mesh_.magSf();

    volScalarField const Cv = (this->thermophysicalModel_.Cv())();
    volScalarField const R =
        ( this->thermophysicalModel_.Cp() - this->thermophysicalModel_.Cv() )();

    this->gradp_ = fvc::grad(this->p_);
    this->gradU_ = fvc::grad(this->U_);
    this->gradT_ = fvc::grad(this->T_);

    const volVectorField& cellCenter = this->mesh_.C();
    const surfaceVectorField& faceCenter = this->mesh_.Cf();

    MDLimiter<Limiter,volScalarField> scalarPLimiter
    (
        this->mesh_,
        this->p_
    );

    MDLimiter<Limiter,volScalarField> scalarTLimiter
    (
        this->mesh_,
        this->T_
    );

    MDLimiter<Limiter,volVectorField> vectorULimiter
    (
        this->mesh_,
        this->U_
    );

    scalarPLimiter.limitField(this->gradp_);
    scalarTLimiter.limitField(this->gradT_);
    vectorULimiter.limitField(this->gradU_);

    volScalarField const& pLimiter = scalarPLimiter.getPhiLimiter();
    volVectorField const& ULimiter = vectorULimiter.getPhiLimiter();
    volScalarField const& TLimiter = scalarTLimiter.getPhiLimiter();

    forAll(owner, faceI)
    {
        label own = owner[faceI];
        label nei = neighbour[faceI];

        vector deltaRLeft  = faceCenter[faceI] - cellCenter[own];
        vector deltaRRight = faceCenter[faceI] - cellCenter[nei];

        Flux::evaluateFlux
        (
            this->rhoFlux_[faceI],
            this->rhoUFlux_[faceI],
            this->rhoEFlux_[faceI],
            this->p_[own] + pLimiter[own] * (deltaRLeft  & this->gradp_[own]),
            this->p_[nei] + pLimiter[nei] * (deltaRRight & this->gradp_[nei]),
            this->U_[own] + cmptMultiply(ULimiter[own], (deltaRLeft  & this->gradU_[own])),
            this->U_[nei] + cmptMultiply(ULimiter[nei], (deltaRRight & this->gradU_[nei])),
            this->T_[own] + TLimiter[own] * (deltaRLeft  & this->gradT_[own]),
            this->T_[nei] + TLimiter[nei] * (deltaRRight & this->gradT_[nei]),
            R[own],        
            R[nei],        
            Cv[own],       
            Cv[nei],       
            Sf[faceI],      
            magSf[faceI],   
            this->dotX_[faceI]    
        );
    }

    forAll(T_.boundaryField(), patchi)
    {
        fvsPatchScalarField& pRhoFlux = this->rhoFlux_.boundaryField()[patchi];
        fvsPatchVectorField& pRhoUFlux = this->rhoUFlux_.boundaryField()[patchi];
        fvsPatchScalarField& pRhoEFlux = this->rhoEFlux_.boundaryField()[patchi];

        fvPatchScalarField const& pp = this->p_.boundaryField()[patchi];
        fvPatchVectorField const& pU = this->U_.boundaryField()[patchi];
        fvPatchScalarField const& pT = this->T_.boundaryField()[patchi];

        fvPatchScalarField const& pCv = Cv.boundaryField()[patchi];
        fvPatchScalarField const& pR = R.boundaryField()[patchi];

        fvsPatchVectorField const& pSf = Sf.boundaryField()[patchi];
        fvsPatchScalarField const& pMagSf = magSf.boundaryField()[patchi];
        fvsPatchVectorField const& pDotX = dotX_.boundaryField()[patchi];

        if (pp.coupled())
        {

            scalarField const ppLeft  = (pp.patchInternalField())();
            scalarField const ppRight = (pp.patchNeighbourField())();

            vectorField const pULeft  = (pU.patchInternalField())();
            vectorField const pURight = (pU.patchNeighbourField())();

            scalarField const pTLeft  = (pT.patchInternalField())();
            scalarField const pTRight = (pT.patchNeighbourField())();

            forAll(pp, facei)
            {
                Flux::evaluateFlux
                (
                    pRhoFlux[facei],
                    pRhoUFlux[facei],
                    pRhoEFlux[facei],
                    ppLeft[facei],    
                    ppRight[facei],   
                    pULeft[facei],    
                    pURight[facei],   
                    pTLeft[facei],    
                    pTRight[facei],   
                    pR[facei],        
                    pR[facei],        
                    pCv[facei],       
                    pCv[facei],       
                    pSf[facei],       
                    pMagSf[facei],    
                    pDotX[facei]      
                );
            }
        }
        else
        {
            forAll(pp, facei)
            {
                Flux::evaluateFlux
                (
                    pRhoFlux[facei],
                    pRhoUFlux[facei],
                    pRhoEFlux[facei],
                    pp[facei],        
                    pp[facei],        
                    pU[facei],        
                    pU[facei],        
                    pT[facei],        
                    pT[facei],        
                    pR[facei],        
                    pR[facei],        
                    pCv[facei],       
                    pCv[facei],       
                    pSf[facei],       
                    pMagSf[facei],    
                    pDotX[facei]      
                );
            }
        }
    }
}


#endif

