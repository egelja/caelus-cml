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
    CML::hsPsiMixtureThermo

Description
    CML::hsPsiMixtureThermo

SourceFiles
    hsPsiMixtureThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef hsPsiMixtureThermo_H
#define hsPsiMixtureThermo_H

#include "hsCombustionThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class hsPsiMixtureThermo Declaration
\*---------------------------------------------------------------------------*/

template<class MixtureType>
class hsPsiMixtureThermo
:
    public hsCombustionThermo,
    public MixtureType
{
    // Private Member Functions

        void calculate();

        //- Construct as copy (not implemented)
        hsPsiMixtureThermo(const hsPsiMixtureThermo<MixtureType>&);


public:

    //- Runtime type information
    TypeName("hsPsiMixtureThermo");


    // Constructors

        //- Construct from mesh
        hsPsiMixtureThermo(const fvMesh&);


    //- Destructor
    virtual ~hsPsiMixtureThermo();


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

            //- Sensible enthalpy for cell-set [J/kg]
            virtual tmp<scalarField> hs
            (
                const scalarField& T,
                const labelList& cells
            ) const;

            //- Sensible enthalpy for patch [J/kg]
            virtual tmp<scalarField> hs
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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "fvMesh.hpp"
#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class MixtureType>
CML::hsPsiMixtureThermo<MixtureType>::hsPsiMixtureThermo(const fvMesh& mesh)
:
    hsCombustionThermo(mesh),
    MixtureType(*this, mesh)
{
    scalarField& hCells = hs_.internalField();
    const scalarField& TCells = T_.internalField();

    forAll(hCells, celli)
    {
        hCells[celli] = this->cellMixture(celli).Hs(TCells[celli]);
    }

    forAll(hs_.boundaryField(), patchi)
    {
        hs_.boundaryField()[patchi] == hs(T_.boundaryField()[patchi], patchi);
    }

    hBoundaryCorrection(hs_);

    calculate();
    psi_.oldTime();   // Switch on saving old time
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class MixtureType>
CML::hsPsiMixtureThermo<MixtureType>::~hsPsiMixtureThermo()
{}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class MixtureType>
void CML::hsPsiMixtureThermo<MixtureType>::calculate()
{
    const scalarField& hsCells = hs_.internalField();
    const scalarField& pCells = p_.internalField();

    scalarField& TCells = T_.internalField();
    scalarField& psiCells = psi_.internalField();
    scalarField& muCells = mu_.internalField();
    scalarField& alphaCells = alpha_.internalField();

    forAll(TCells, celli)
    {
        const typename MixtureType::thermoType& mixture_ =
            this->cellMixture(celli);

        TCells[celli] = mixture_.THs(hsCells[celli], TCells[celli]);
        psiCells[celli] = mixture_.psi(pCells[celli], TCells[celli]);

        muCells[celli] = mixture_.mu(TCells[celli]);
        alphaCells[celli] = mixture_.alpha(TCells[celli]);
    }

    forAll(T_.boundaryField(), patchi)
    {
        fvPatchScalarField& pp = p_.boundaryField()[patchi];
        fvPatchScalarField& pT = T_.boundaryField()[patchi];
        fvPatchScalarField& ppsi = psi_.boundaryField()[patchi];

        fvPatchScalarField& phs = hs_.boundaryField()[patchi];

        fvPatchScalarField& pmu_ = mu_.boundaryField()[patchi];
        fvPatchScalarField& palpha_ = alpha_.boundaryField()[patchi];

        if (pT.fixesValue())
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture_ =
                    this->patchFaceMixture(patchi, facei);

                phs[facei] = mixture_.Hs(pT[facei]);

                ppsi[facei] = mixture_.psi(pp[facei], pT[facei]);
                pmu_[facei] = mixture_.mu(pT[facei]);
                palpha_[facei] = mixture_.alpha(pT[facei]);
            }
        }
        else
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture_ =
                    this->patchFaceMixture(patchi, facei);

                pT[facei] = mixture_.THs(phs[facei], pT[facei]);

                ppsi[facei] = mixture_.psi(pp[facei], pT[facei]);
                pmu_[facei] = mixture_.mu(pT[facei]);
                palpha_[facei] = mixture_.alpha(pT[facei]);
            }
        }
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class MixtureType>
void CML::hsPsiMixtureThermo<MixtureType>::correct()
{
    if (debug)
    {
        Info<< "entering hMixtureThermo<MixtureType>::correct()" << endl;
    }

    // force the saving of the old-time values
    psi_.oldTime();

    calculate();

    if (debug)
    {
        Info<< "exiting hMixtureThermo<MixtureType>::correct()" << endl;
    }
}

template<class MixtureType>
CML::tmp<CML::volScalarField>
CML::hsPsiMixtureThermo<MixtureType>::hc() const
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
            hs_.dimensions()
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
CML::hsPsiMixtureThermo<MixtureType>::hs
(
    const scalarField& T,
    const labelList& cells
) const
{
    tmp<scalarField> ths(new scalarField(T.size()));
    scalarField& hs = ths();

    forAll(T, celli)
    {
        hs[celli] = this->cellMixture(cells[celli]).Hs(T[celli]);
    }

    return ths;
}


template<class MixtureType>
CML::tmp<CML::scalarField>
CML::hsPsiMixtureThermo<MixtureType>::hs
(
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> ths(new scalarField(T.size()));
    scalarField& hs = ths();

    forAll(T, facei)
    {
        hs[facei] = this->patchFaceMixture(patchi, facei).Hs(T[facei]);
    }

    return ths;
}


template<class MixtureType>
CML::tmp<CML::scalarField>
CML::hsPsiMixtureThermo<MixtureType>::Cp
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
CML::hsPsiMixtureThermo<MixtureType>::Cp() const
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
bool CML::hsPsiMixtureThermo<MixtureType>::read()
{
    if (hsCombustionThermo::read())
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
