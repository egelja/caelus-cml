/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::heSolidThermo

Description
    Energy for a solid mixture

SourceFiles
    heSolidThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef heSolidThermo_HPP
#define heSolidThermo_HPP

#include "heThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class heSolidThermo Declaration
\*---------------------------------------------------------------------------*/

template<class BasicSolidThermo, class MixtureType>
class heSolidThermo
:
    public heThermo<BasicSolidThermo, MixtureType>
{

    //- Calculate the thermo variables
    void calculate();

    //- Construct as copy (not implemented)
    heSolidThermo(const heSolidThermo<BasicSolidThermo, MixtureType>&);


public:

    //- Runtime type information
    TypeName("heSolidThermo");


    //- Construct from mesh and phase name
    heSolidThermo
    (
        const fvMesh&,
        const word& phaseName
    );

    //- Construct from mesh, dictionary and phase name
    heSolidThermo
    (
        const fvMesh&,
        const dictionary&,
        const word& phaseName
    );


    //- Destructor
    virtual ~heSolidThermo()
    {}


    // Member functions

    //- Update properties
    virtual void correct();


    // Derived thermal properties

    //- Anisotropic thermal conductivity [W/m/K]
    virtual tmp<volVectorField> Kappa() const;

    //- Return true if thermal conductivity is isotropic
    virtual bool isotropic() const
    {
        return MixtureType::thermoType::isotropic;
    }


    // Per patch calculation

    //- Anisotropic thermal conductivity [W/m/K]
    virtual tmp<vectorField> Kappa(const label patchi) const;

};


} // End namespace CML


#include "volFields.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class BasicSolidThermo, class MixtureType>
void CML::heSolidThermo<BasicSolidThermo, MixtureType>::calculate()
{
    scalarField& TCells = this->T_.internalField();

    const scalarField& hCells = this->he_.internalField();
    const scalarField& pCells = this->p_.internalField();
    scalarField& rhoCells = this->rho_.internalField();
    scalarField& alphaCells = this->alpha_.internalField();

    forAll(TCells, celli)
    {
        const typename MixtureType::thermoType& mixture_ =
            this->cellMixture(celli);

        const typename MixtureType::thermoType& volMixture_ =
            this->cellVolMixture(pCells[celli], TCells[celli], celli);

        TCells[celli] = mixture_.THE
        (
            hCells[celli],
            pCells[celli],
            TCells[celli]
        );

        rhoCells[celli] = volMixture_.rho(pCells[celli], TCells[celli]);

        alphaCells[celli] =
            volMixture_.kappa(pCells[celli], TCells[celli])
            /
            mixture_.Cpv(pCells[celli], TCells[celli]);
    }

    volScalarField::GeometricBoundaryField& pBf =
        this->p_.boundaryField();

    volScalarField::GeometricBoundaryField& TBf =
        this->T_.boundaryField();

    volScalarField::GeometricBoundaryField& rhoBf =
        this->rho_.boundaryField();

    volScalarField::GeometricBoundaryField& heBf =
        this->he().boundaryField();

    volScalarField::GeometricBoundaryField& alphaBf =
        this->alpha_.boundaryField();

    forAll(this->T_.boundaryField(), patchi)
    {
        fvPatchScalarField& pp = pBf[patchi];
        fvPatchScalarField& pT = TBf[patchi];
        fvPatchScalarField& prho = rhoBf[patchi];
        fvPatchScalarField& phe = heBf[patchi];
        fvPatchScalarField& palpha = alphaBf[patchi];

        if (pT.fixesValue())
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture_ =
                    this->patchFaceMixture(patchi, facei);

                const typename MixtureType::thermoType& volMixture_ =
                    this->patchFaceVolMixture
                    (
                        pp[facei],
                        pT[facei],
                        patchi,
                        facei
                    );


                phe[facei] = mixture_.HE(pp[facei], pT[facei]);
                prho[facei] = volMixture_.rho(pp[facei], pT[facei]);

                palpha[facei] =
                    volMixture_.kappa(pp[facei], pT[facei])
                  / mixture_.Cpv(pp[facei], pT[facei]);
            }
        }
        else
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture_ =
                    this->patchFaceMixture(patchi, facei);

                const typename MixtureType::thermoType& volMixture_ =
                    this->patchFaceVolMixture
                    (
                        pp[facei],
                        pT[facei],
                        patchi,
                        facei
                    );

                pT[facei] = mixture_.THE(phe[facei], pp[facei] ,pT[facei]);
                prho[facei] = volMixture_.rho(pp[facei], pT[facei]);

                palpha[facei] =
                    volMixture_.kappa(pp[facei], pT[facei])
                  / mixture_.Cpv(pp[facei], pT[facei]);
            }
        }
    }

    this->alpha_.correctBoundaryConditions();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class BasicSolidThermo, class MixtureType>
CML::heSolidThermo<BasicSolidThermo, MixtureType>::
heSolidThermo
(
    const fvMesh& mesh,
    const word& phaseName
)
:
    heThermo<BasicSolidThermo, MixtureType>(mesh, phaseName)
{
    calculate();
}


template<class BasicSolidThermo, class MixtureType>
CML::heSolidThermo<BasicSolidThermo, MixtureType>::
heSolidThermo
(
    const fvMesh& mesh,
    const dictionary& dict,
    const word& phaseName
)
:
    heThermo<BasicSolidThermo, MixtureType>(mesh, dict, phaseName)
{
    calculate();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class BasicSolidThermo, class MixtureType>
void CML::heSolidThermo<BasicSolidThermo, MixtureType>::correct()
{
    if (debug)
    {
        InfoInFunction << endl;
    }

    calculate();

    if (debug)
    {
        Info<< "    Finished" << endl;
    }
}


template<class BasicSolidThermo, class MixtureType>
CML::tmp<CML::volVectorField>
CML::heSolidThermo<BasicSolidThermo, MixtureType>::Kappa() const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volVectorField> tKappa
    (
        new volVectorField
        (
            IOobject
            (
                "Kappa",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimEnergy/dimTime/dimLength/dimTemperature
        )
    );

    volVectorField& Kappa = tKappa();
    vectorField& KappaCells = Kappa.internalField();
    const scalarField& TCells = this->T_.internalField();
    const scalarField& pCells = this->p_.internalField();

    forAll(KappaCells, celli)
    {
        Kappa[celli] =
            this->cellVolMixture
            (
                pCells[celli],
                TCells[celli],
                celli
            ).Kappa(pCells[celli], TCells[celli]);
    }

    volVectorField::GeometricBoundaryField& KappaBf = Kappa.boundaryField();

    forAll(KappaBf, patchi)
    {
        vectorField& Kappap = KappaBf[patchi];
        const scalarField& pT = this->T_.boundaryField()[patchi];
        const scalarField& pp = this->p_.boundaryField()[patchi];

        forAll(Kappap, facei)
        {
            Kappap[facei] =
                this->patchFaceVolMixture
                (
                    pp[facei],
                    pT[facei],
                    patchi,
                    facei
                ).Kappa(pp[facei], pT[facei]);
        }
    }

    return tKappa;
}


template<class BasicSolidThermo, class MixtureType>
CML::tmp<CML::vectorField>
CML::heSolidThermo<BasicSolidThermo, MixtureType>::Kappa
(
    const label patchi
) const
{
    const scalarField& pp = this->p_.boundaryField()[patchi];
    const scalarField& Tp = this->T_.boundaryField()[patchi];
    tmp<vectorField> tKappa(new vectorField(pp.size()));

    vectorField& Kappap = tKappa();

    forAll(Tp, facei)
    {
        Kappap[facei] =
            this->patchFaceVolMixture
            (
                pp[facei],
                Tp[facei],
                patchi,
                facei
            ).Kappa(pp[facei], Tp[facei]);
    }

    return tKappa;
}


#endif
