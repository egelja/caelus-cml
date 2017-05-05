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
    CML::hPsiThermo

Description
    Enthalpy for a mixture based on compressibility

SourceFiles
    hPsiThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef hPsiThermo_H
#define hPsiThermo_H

#include "basicPsiThermo.hpp"
#include "basicMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class hPsiThermo Declaration
\*---------------------------------------------------------------------------*/

template<class MixtureType>
class hPsiThermo
:
    public basicPsiThermo,
    public MixtureType
{
    // Private data

        //- Enthalpy field
        volScalarField h_;


    // Private Member Functions

        //- Calculate the thermo variables
        void calculate();

        //- Construct as copy (not implemented)
        hPsiThermo(const hPsiThermo<MixtureType>&);


public:

    //- Runtime type information
    TypeName("hPsiThermo");


    // Constructors

        //- Construct from mesh
        hPsiThermo(const fvMesh&);


    //- Destructor
    virtual ~hPsiThermo();


    // Member functions

        //- Return the compostion of the mixture
        virtual basicMixture& composition()
        {
            return *this;
        }

        //- Return the compostion of the mixture
        virtual const basicMixture& composition() const
        {
            return *this;
        }

        //- Update properties
        virtual void correct();


        // Access to thermodynamic state variables

            //- Enthalpy [J/kg]
            //  Non-const access allowed for transport equations
            virtual volScalarField& h()
            {
                return h_;
            }

            //- Enthalpy [J/kg]
            virtual const volScalarField& h() const
            {
                return h_;
            }


        // Fields derived from thermodynamic state variables

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

            //- Heat capacity at constant volume for patch [J/kg/K]
            virtual tmp<scalarField> Cv
            (
                const scalarField& T,
                const label patchi
            ) const;

            //- Heat capacity at constant volume [J/kg/K]
            virtual tmp<volScalarField> Cv() const;


        //- Read thermophysicalProperties dictionary
        virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class MixtureType>
void CML::hPsiThermo<MixtureType>::calculate()
{
    const scalarField& hCells = h_.internalField();
    const scalarField& pCells = this->p_.internalField();

    scalarField& TCells = this->T_.internalField();
    scalarField& psiCells = this->psi_.internalField();
    scalarField& muCells = this->mu_.internalField();
    scalarField& alphaCells = this->alpha_.internalField();

    forAll(TCells, celli)
    {
        const typename MixtureType::thermoType& mixture_ =
            this->cellMixture(celli);

        TCells[celli] = mixture_.TH(hCells[celli], TCells[celli]);
        psiCells[celli] = mixture_.psi(pCells[celli], TCells[celli]);

        muCells[celli] = mixture_.mu(TCells[celli]);
        alphaCells[celli] = mixture_.alpha(TCells[celli]);
    }

    forAll(T_.boundaryField(), patchi)
    {
        fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
        fvPatchScalarField& ppsi = this->psi_.boundaryField()[patchi];

        fvPatchScalarField& ph = h_.boundaryField()[patchi];

        fvPatchScalarField& pmu = this->mu_.boundaryField()[patchi];
        fvPatchScalarField& palpha = this->alpha_.boundaryField()[patchi];

        if (pT.fixesValue())
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture_ =
                    this->patchFaceMixture(patchi, facei);

                ph[facei] = mixture_.H(pT[facei]);

                ppsi[facei] = mixture_.psi(pp[facei], pT[facei]);
                pmu[facei] = mixture_.mu(pT[facei]);
                palpha[facei] = mixture_.alpha(pT[facei]);
            }
        }
        else
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture_ =
                    this->patchFaceMixture(patchi, facei);

                pT[facei] = mixture_.TH(ph[facei], pT[facei]);

                ppsi[facei] = mixture_.psi(pp[facei], pT[facei]);
                pmu[facei] = mixture_.mu(pT[facei]);
                palpha[facei] = mixture_.alpha(pT[facei]);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class MixtureType>
CML::hPsiThermo<MixtureType>::hPsiThermo(const fvMesh& mesh)
:
    basicPsiThermo(mesh),
    MixtureType(*this, mesh),

    h_
    (
        IOobject
        (
            "h",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass,
        this->hBoundaryTypes()
    )
{
    scalarField& hCells = h_.internalField();
    const scalarField& TCells = this->T_.internalField();

    forAll(hCells, celli)
    {
        hCells[celli] = this->cellMixture(celli).H(TCells[celli]);
    }

    forAll(h_.boundaryField(), patchi)
    {
        h_.boundaryField()[patchi] ==
            h(this->T_.boundaryField()[patchi], patchi);
    }

    hBoundaryCorrection(h_);

    calculate();

    // Switch on saving old time
    this->psi_.oldTime();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class MixtureType>
CML::hPsiThermo<MixtureType>::~hPsiThermo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class MixtureType>
void CML::hPsiThermo<MixtureType>::correct()
{
    if (debug)
    {
        Info<< "entering hPsiThermo<MixtureType>::correct()" << endl;
    }

    // force the saving of the old-time values
    this->psi_.oldTime();

    calculate();

    if (debug)
    {
        Info<< "exiting hPsiThermo<MixtureType>::correct()" << endl;
    }
}


template<class MixtureType>
CML::tmp<CML::scalarField> CML::hPsiThermo<MixtureType>::h
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
CML::tmp<CML::scalarField> CML::hPsiThermo<MixtureType>::h
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
CML::tmp<CML::scalarField> CML::hPsiThermo<MixtureType>::Cp
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
CML::tmp<CML::volScalarField> CML::hPsiThermo<MixtureType>::Cp() const
{
    const fvMesh& mesh = this->T_.mesh();

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

    forAll(this->T_, celli)
    {
        cp[celli] = this->cellMixture(celli).Cp(this->T_[celli]);
    }

    forAll(this->T_.boundaryField(), patchi)
    {
        const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
        fvPatchScalarField& pCp = cp.boundaryField()[patchi];

        forAll(pT, facei)
        {
            pCp[facei] = this->patchFaceMixture(patchi, facei).Cp(pT[facei]);
        }
    }

    return tCp;
}


template<class MixtureType>
CML::tmp<CML::scalarField> CML::hPsiThermo<MixtureType>::Cv
(
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> tCv(new scalarField(T.size()));
    scalarField& cv = tCv();

    forAll(T, facei)
    {
        cv[facei] = this->patchFaceMixture(patchi, facei).Cv(T[facei]);
    }

    return tCv;
}


template<class MixtureType>
CML::tmp<CML::volScalarField> CML::hPsiThermo<MixtureType>::Cv() const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volScalarField> tCv
    (
        new volScalarField
        (
            IOobject
            (
                "Cv",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            mesh,
            dimEnergy/dimMass/dimTemperature
        )
    );

    volScalarField& cv = tCv();

    forAll(this->T_, celli)
    {
        cv[celli] = this->cellMixture(celli).Cv(this->T_[celli]);
    }

    forAll(this->T_.boundaryField(), patchi)
    {
        cv.boundaryField()[patchi] =
            Cv(this->T_.boundaryField()[patchi], patchi);
    }

    return tCv;
}


template<class MixtureType>
bool CML::hPsiThermo<MixtureType>::read()
{
    if (basicPsiThermo::read())
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
