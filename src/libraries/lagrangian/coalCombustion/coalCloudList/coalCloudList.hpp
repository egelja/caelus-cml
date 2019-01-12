/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2016 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#ifndef coalCloudList_HPP
#define coalCloudList_HPP

#include "coalCloud.hpp"
#include "volFieldsFwd.hpp"
#include "fvMatricesFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class coalCloudList Declaration
\*---------------------------------------------------------------------------*/

class coalCloudList
:
    public PtrList<coalCloud>
{
private:

    //- Reference to the mesh
    const fvMesh& mesh_;


public:

    // Constructor

        coalCloudList
        (
            const volScalarField& rho,
            const volVectorField& U,
            const dimensionedVector& g,
            const SLGThermo& slgThermo
        );


    // Member Functions

        // Evolution

            //- Evolve the cloud collection
            void evolve();


        // Source terms

            //- Return const reference to momentum source
            inline tmp<volVectorField::Internal> UTrans() const;

            //- Return tmp momentum source term
            inline tmp<fvVectorMatrix> SU(volVectorField& U) const;

            //- Sensible enthalpy transfer [J/kg]
            inline tmp<volScalarField::Internal> hsTrans() const;

            //- Return sensible enthalpy source term [J/kg/m3/s]
            inline tmp<fvScalarMatrix> Sh(volScalarField& hs) const;


            //- Return mass source term for specie i - specie eqn
            inline tmp<fvScalarMatrix> SYi
            (
                const label i,
                volScalarField& Yi
            ) const;

            //- Return total mass transfer [kg/m3]
            inline tmp<volScalarField::Internal> rhoTrans() const;

            //- Return tmp total mass source for carrier phase
            //  - fully explicit
            inline tmp<volScalarField::Internal> Srho() const;

            //- Return tmp total mass source for carrier phase specie i
            //  - fully explicit
            inline tmp<volScalarField::Internal> Srho
            (
                const label i
            ) const;

            //- Return total mass source term [kg/m3/s]
            inline tmp<fvScalarMatrix> Srho(volScalarField& rho) const;
};


} // End namespace CML


#include "fvMatrices.hpp"
#include "volFields.hpp"
#include "DimensionedField.hpp"

CML::tmp<CML::DimensionedField<CML::vector, CML::volMesh> >
CML::coalCloudList::UTrans() const
{
    tmp<volVectorField::Internal> tfld
    (
        new volVectorField::Internal
        (
            IOobject
            (
                "UTransEff",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedVector("zero", dimMass*dimVelocity, Zero)
        )
    );

    volVectorField::Internal& fld = tfld.ref();

    forAll(*this, i)
    {
        fld += operator[](i).UTrans();
    }

    return tfld;
}


CML::tmp<CML::fvVectorMatrix> CML::coalCloudList::SU
(
    volVectorField& U
) const
{
    tmp<fvVectorMatrix> tfvm(new fvVectorMatrix(U, dimForce));
    fvVectorMatrix& fvm = tfvm.ref();

    forAll(*this, i)
    {
        fvm += operator[](i).SU(U);
    }

    return tfvm;
}


CML::tmp<CML::DimensionedField<CML::scalar, CML::volMesh> >
CML::coalCloudList::hsTrans() const
{
    tmp<volScalarField::Internal> tfld
    (
        new volScalarField::Internal
        (
            IOobject
            (
                "hsTransEff",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("zero", dimEnergy, 0.0)
        )
    );

    volScalarField::Internal& fld = tfld.ref();

    forAll(*this, i)
    {
        fld += operator[](i).hsTrans();
    }

    return tfld;
}


CML::tmp<CML::fvScalarMatrix> CML::coalCloudList::Sh
(
    volScalarField& hs
) const
{
    tmp<fvScalarMatrix> tfvm(new fvScalarMatrix(hs, dimEnergy/dimTime));
    fvScalarMatrix& fvm = tfvm.ref();

    forAll(*this, i)
    {
        fvm += operator[](i).Sh(hs);
    }

    return tfvm;
}


CML::tmp<CML::fvScalarMatrix> CML::coalCloudList::SYi
(
    const label ii,
    volScalarField& Yi
) const
{
    tmp<fvScalarMatrix> tfvm(new fvScalarMatrix(Yi, dimMass/dimTime));
    fvScalarMatrix& fvm = tfvm.ref();

    forAll(*this, i)
    {
        fvm += operator[](i).SYi(ii, Yi);
    }

    return tfvm;
}


CML::tmp<CML::DimensionedField<CML::scalar, CML::volMesh> >
CML::coalCloudList::rhoTrans() const
{
    tmp<volScalarField::Internal> tfld
    (
        new volScalarField::Internal
        (
            IOobject
            (
                "rhoTransEff",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("zero", dimMass, 0.0)
        )
    );

    volScalarField::Internal& fld = tfld.ref();

    forAll(*this, i)
    {
        forAll(operator[](i).rhoTrans(), j)
        {
            fld += operator[](i).rhoTrans()[j];
        }
    }

    return tfld;
}



CML::tmp<CML::DimensionedField<CML::scalar, CML::volMesh> >
CML::coalCloudList::Srho() const
{
    tmp<volScalarField::Internal> tfld
    (
        new volScalarField::Internal
        (
            IOobject
            (
                "rhoTransEff",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("zero", dimDensity/dimTime, 0.0)
        )
    );

    volScalarField::Internal& fld = tfld.ref();

    forAll(*this, i)
    {
        fld += operator[](i).Srho();
    }

    return tfld;
}


CML::tmp<CML::DimensionedField<CML::scalar, CML::volMesh> >
CML::coalCloudList::Srho
(
    const label i
) const
{
    tmp<volScalarField::Internal> tfld
    (
        new volScalarField::Internal
        (
            IOobject
            (
                "rhoTransEff",
                mesh_.time().timeName(),
                mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh_,
            dimensionedScalar("zero", dimDensity/dimTime, 0.0)
        )
    );

    volScalarField::Internal& fld = tfld.ref();

    forAll(*this, j)
    {
        fld += operator[](j).Srho(i);
    }

    return tfld;
}


CML::tmp<CML::fvScalarMatrix> CML::coalCloudList::Srho
(
    volScalarField& rho
) const
{
    tmp<fvScalarMatrix> tfvm(new fvScalarMatrix(rho, dimMass/dimTime));
    fvScalarMatrix& fvm = tfvm.ref();

    forAll(*this, i)
    {
        fvm += operator[](i).Srho(rho);
    }

    return tfvm;
}


#endif
