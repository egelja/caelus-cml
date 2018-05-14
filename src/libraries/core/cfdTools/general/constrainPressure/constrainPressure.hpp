/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::constrainPressure

Description

SourceFiles
    constrainPressure.cpp

\*---------------------------------------------------------------------------*/

#ifndef constrainPressure_HPP
#define constrainPressure_HPP

#include "volFieldsFwd.hpp"
#include "surfaceFieldsFwd.hpp"
#include "IOMRFZoneList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*---------------------------------------------------------------------------*\
                           Class NullMRF Declaration
\*---------------------------------------------------------------------------*/

class NullMRF
{
public:

    //- Construct null
    NullMRF()
        {}

    //- Return the argument unchanged
    template<class Type>
    inline const Type& relative(const Type& U) const
    {
        return U;
    }

    //- Return the argument unchanged
    template<class Type>
    inline const Type& relative(const Type& U, const label patchi) const
    {
        return U;
    }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

template<class RhoType, class RAUType, class MRFType>
void constrainPressure
(
    volScalarField& p,
    const RhoType& rho,
    const volVectorField& U,
    const surfaceScalarField& phiHbyA,
    const RAUType& rhorAU,
    const MRFType& MRF
);

template<class RAUType>
void constrainPressure
(
    volScalarField& p,
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& phiHbyA,
    const RAUType& rhorAU
);

template<class RAUType, class MRFType>
void constrainPressure
(
    volScalarField& p,
    const volVectorField& U,
    const surfaceScalarField& phiHbyA,
    const RAUType& rAU,
    const MRFType& MRF
);

template<class RAUType>
void constrainPressure
(
    volScalarField& p,
    const volVectorField& U,
    const surfaceScalarField& phiHbyA,
    const RAUType& rAU
);


} // End namespace CML

#include "volFields.hpp"
#include "surfaceFields.hpp"
#include "geometricOneField.hpp"
#include "fixedFluxPressureFvPatchScalarField.hpp"


template<class RhoType, class RAUType, class MRFType>
void CML::constrainPressure
(
    volScalarField& p,
    const RhoType& rho,
    const volVectorField& U,
    const surfaceScalarField& phiHbyA,
    const RAUType& rhorAU,
    const MRFType& MRF
)
{
    const fvMesh& mesh = p.mesh();

    volScalarField::GeometricBoundaryField& pBf = p.boundaryField();

    const volVectorField::GeometricBoundaryField& UBf = U.boundaryField();
    const surfaceScalarField::GeometricBoundaryField& phiHbyABf =
        phiHbyA.boundaryField();
    const typename RAUType::GeometricBoundaryField& rhorAUBf =
        rhorAU.boundaryField();
    const surfaceVectorField::GeometricBoundaryField& SfBf =
        mesh.Sf().boundaryField();
    const surfaceScalarField::GeometricBoundaryField& magSfBf =
        mesh.magSf().boundaryField();

    forAll(pBf, patchi)
    {
        if (isA<fixedFluxPressureFvPatchScalarField>(pBf[patchi]))
        {
            refCast<fixedFluxPressureFvPatchScalarField>
            (
                pBf[patchi]
            ).updateCoeffs
            (
                (
                    phiHbyABf[patchi]
                  - rho.boundaryField()[patchi]
                   *MRF.relative(SfBf[patchi] & UBf[patchi], patchi)
                )
               /(magSfBf[patchi]*rhorAUBf[patchi])
            );
        }
    }
}


template<class RAUType>
void CML::constrainPressure
(
    volScalarField& p,
    const volScalarField& rho,
    const volVectorField& U,
    const surfaceScalarField& phiHbyA,
    const RAUType& rAU
)
{
    constrainPressure(p, rho, U, phiHbyA, rAU, NullMRF());
}


template<class RAUType, class MRFType>
void CML::constrainPressure
(
    volScalarField& p,
    const volVectorField& U,
    const surfaceScalarField& phiHbyA,
    const RAUType& rAU,
    const MRFType& MRF
)
{
    constrainPressure(p, geometricOneField(), U, phiHbyA, rAU, MRF);
}


template<class RAUType>
void CML::constrainPressure
(
    volScalarField& p,
    const volVectorField& U,
    const surfaceScalarField& phiHbyA,
    const RAUType& rAU
)
{
    constrainPressure(p, U, phiHbyA, rAU, NullMRF());
}


#endif
