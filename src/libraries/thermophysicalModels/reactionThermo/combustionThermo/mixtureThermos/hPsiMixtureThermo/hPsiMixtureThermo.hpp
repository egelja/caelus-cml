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
    CML::hPsiMixtureThermo

Description
    CML::hPsiMixtureThermo

SourceFiles
    hPsiMixtureThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef hPsiMixtureThermo_H
#define hPsiMixtureThermo_H

#include "hCombustionThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class hPsiMixtureThermo Declaration
\*---------------------------------------------------------------------------*/

template<class MixtureType>
class hPsiMixtureThermo
:
    public hCombustionThermo,
    public MixtureType
{
    // Private Member Functions

        void calculate();

        //- Construct as copy (not implemented)
        hPsiMixtureThermo(const hPsiMixtureThermo<MixtureType>&);


public:

    //- Runtime type information
    TypeName("hPsiMixtureThermo");


    // Constructors

        //- Construct from mesh
        hPsiMixtureThermo(const fvMesh&);


    //- Destructor
    virtual ~hPsiMixtureThermo();


    // Member functions

        //- Return the compostion of the multi-component mixture
        virtual basicMultiComponentMixture& composition()
        {
            return *this;
        }

        //- Return the compostion of the multi-component mixture
        virtual const basicMultiComponentMixture& composition() const
        {
            return *this;
        }

        //- Update properties
        virtual void correct();


        // Fields derived from thermodynamic state variables

            //- Chemical enthalpy [J/kg]
            virtual tmp<volScalarField> hc() const;

            //- Enthalpy for cell-set [J/kg]
            virtual tmp<scalarField> h
            (
                const scalarField& T,
                const labelList& cells
            ) const;

            //- Enthalpy for patch [J/kg]
            virtual tmp<scalarField> h
            (
                const scalarField& T,
                const label patchi
            ) const;

            //- Heat capacity at constant pressure for patch [J/kg/K]
            virtual tmp<scalarField> Cp
            (
                const scalarField& T,
                const label patchi
            ) const;

            //- Heat capacity at constant pressure [J/kg/K]
            virtual tmp<volScalarField> Cp() const;


        //- Read thermophysicalProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "fvMesh.hpp"
#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class MixtureType>
void CML::hPsiMixtureThermo<MixtureType>::calculate()
{
    const scalarField& hCells = h_.internalField();
    const scalarField& pCells = p_.internalField();

    scalarField& TCells = T_.internalField();
    scalarField& psiCells = psi_.internalField();
    scalarField& muCells = mu_.internalField();
    scalarField& alphaCells = alpha_.internalField();

    forAll(TCells, celli)
    {
        const typename MixtureType::thermoType& mixture =
            this->cellMixture(celli);

        TCells[celli] = mixture.TH(hCells[celli], TCells[celli]);
        psiCells[celli] = mixture.psi(pCells[celli], TCells[celli]);

        muCells[celli] = mixture.mu(TCells[celli]);
        alphaCells[celli] = mixture.alpha(TCells[celli]);
    }

    forAll(T_.boundaryField(), patchi)
    {
        fvPatchScalarField& pp = p_.boundaryField()[patchi];
        fvPatchScalarField& pT = T_.boundaryField()[patchi];
        fvPatchScalarField& ppsi = psi_.boundaryField()[patchi];

        fvPatchScalarField& ph = h_.boundaryField()[patchi];

        fvPatchScalarField& pmu_ = mu_.boundaryField()[patchi];
        fvPatchScalarField& palpha_ = alpha_.boundaryField()[patchi];

        if (pT.fixesValue())
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture =
                    this->patchFaceMixture(patchi, facei);

                ph[facei] = mixture.H(pT[facei]);

                ppsi[facei] = mixture.psi(pp[facei], pT[facei]);
                pmu_[facei] = mixture.mu(pT[facei]);
                palpha_[facei] = mixture.alpha(pT[facei]);
            }
        }
        else
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture =
                    this->patchFaceMixture(patchi, facei);

                pT[facei] = mixture.TH(ph[facei], pT[facei]);

                ppsi[facei] = mixture.psi(pp[facei], pT[facei]);
                pmu_[facei] = mixture.mu(pT[facei]);
                palpha_[facei] = mixture.alpha(pT[facei]);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class MixtureType>
CML::hPsiMixtureThermo<MixtureType>::hPsiMixtureThermo(const fvMesh& mesh)
:
    hCombustionThermo(mesh),
    MixtureType(*this, mesh)
{
    scalarField& hCells = h_.internalField();
    const scalarField& TCells = T_.internalField();

    forAll(hCells, celli)
    {
        hCells[celli] = this->cellMixture(celli).H(TCells[celli]);
    }

    forAll(h_.boundaryField(), patchi)
    {
        h_.boundaryField()[patchi] == h(T_.boundaryField()[patchi], patchi);
    }

    hBoundaryCorrection(h_);

    calculate();

    // Switch on saving old time
    psi_.oldTime();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class MixtureType>
CML::hPsiMixtureThermo<MixtureType>::~hPsiMixtureThermo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class MixtureType>
void CML::hPsiMixtureThermo<MixtureType>::correct()
{
    if (debug)
    {
        Info<< "entering hPsiMixtureThermo<MixtureType>::correct()" << endl;
    }

    // force the saving of the old-time values
    psi_.oldTime();

    calculate();

    if (debug)
    {
        Info<< "exiting hPsiMixtureThermo<MixtureType>::correct()" << endl;
    }
}


template<class MixtureType>
CML::tmp<CML::volScalarField>
CML::hPsiMixtureThermo<MixtureType>::hc() const
{
    const fvMesh& mesh = T_.mesh();

    tmp<volScalarField> thc
    (
        new volScalarField
        (
            IOobject
            (
                "hc",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            h_.dimensions()
        )
    );

    volScalarField& hcf = thc();
    scalarField& hcCells = hcf.internalField();

    forAll(hcCells, celli)
    {
        hcCells[celli] = this->cellMixture(celli).Hc();
    }

    forAll(hcf.boundaryField(), patchi)
    {
        scalarField& hcp = hcf.boundaryField()[patchi];

        forAll(hcp, facei)
        {
            hcp[facei] = this->patchFaceMixture(patchi, facei).Hc();
        }
    }

    return thc;
}


template<class MixtureType>
CML::tmp<CML::scalarField>
CML::hPsiMixtureThermo<MixtureType>::h
(
    const scalarField& T,
    const labelList& cells
) const
{
    tmp<scalarField> th(new scalarField(T.size()));
    scalarField& h = th();

    forAll(T, celli)
    {
        h[celli] = this->cellMixture(cells[celli]).H(T[celli]);
    }

    return th;
}


template<class MixtureType>
CML::tmp<CML::scalarField>
CML::hPsiMixtureThermo<MixtureType>::h
(
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> th(new scalarField(T.size()));
    scalarField& h = th();

    forAll(T, facei)
    {
        h[facei] = this->patchFaceMixture(patchi, facei).H(T[facei]);
    }

    return th;
}


template<class MixtureType>
CML::tmp<CML::scalarField>
CML::hPsiMixtureThermo<MixtureType>::Cp
(
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> tCp(new scalarField(T.size()));

    scalarField& cp = tCp();

    forAll(T, facei)
    {
        cp[facei] = this->patchFaceMixture(patchi, facei).Cp(T[facei]);
    }

    return tCp;
}


template<class MixtureType>
CML::tmp<CML::volScalarField>
CML::hPsiMixtureThermo<MixtureType>::Cp() const
{
    const fvMesh& mesh = T_.mesh();

    tmp<volScalarField> tCp
    (
        new volScalarField
        (
            IOobject
            (
                "Cp",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimEnergy/dimMass/dimTemperature
        )
    );

    volScalarField& cp = tCp();

    scalarField& cpCells = cp.internalField();
    const scalarField& TCells = T_.internalField();

    forAll(TCells, celli)
    {
        cpCells[celli] = this->cellMixture(celli).Cp(TCells[celli]);
    }

    forAll(T_.boundaryField(), patchi)
    {
        cp.boundaryField()[patchi] = Cp(T_.boundaryField()[patchi], patchi);
    }

    return tCp;
}


template<class MixtureType>
bool CML::hPsiMixtureThermo<MixtureType>::read()
{
    if (hCombustionThermo::read())
    {
        MixtureType::read(*this);
        return true;
    }
    else
    {
        return false;
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
