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
    CML::heThermo

Description
    Enthalpy/Internal energy for a mixture

SourceFiles
    heThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef heThermo_HPP
#define heThermo_HPP

#include "basicMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class heThermo Declaration
\*---------------------------------------------------------------------------*/

template<class BasicThermo, class MixtureType>
class heThermo
:
    public BasicThermo,
    public MixtureType
{
protected:

    //- Energy field
    volScalarField he_;

    //- Correct the enthalpy/internal energy field boundaries
    void heBoundaryCorrection(volScalarField& he);


private:

    //- Construct as copy (not implemented)
    heThermo(const heThermo<BasicThermo, MixtureType>&);

    //- Initialize heThermo
    void init();


public:

    //- Construct from mesh
    heThermo
    (
        const fvMesh&,
        const word& phaseName
    );

    //- Construct from mesh and dictionary
    heThermo
    (
        const fvMesh&,
        const dictionary&,
        const word& phaseName
    );


    //- Destructor
    virtual ~heThermo();


    // Member functions

    //- Return the compostion of the mixture
    virtual typename MixtureType::basicMixtureType&
    composition()
    {
        return *this;
    }

    //- Return the compostion of the mixture
    virtual const typename MixtureType::basicMixtureType&
    composition() const
    {
        return *this;
    }

    //- Return the name of the thermo physics
    virtual word thermoName() const
    {
        return MixtureType::thermoType::typeName();
    }

    //- Return true if the equation of state is incompressible
    //  i.e. rho != f(p)
    virtual bool incompressible() const
    {
        return MixtureType::thermoType::incompressible;
    }

    //- Return true if the equation of state is isochoric
    //  i.e. rho = const
    virtual bool isochoric() const
    {
        return MixtureType::thermoType::isochoric;
    }


    // Access to thermodynamic state variables

    //- Enthalpy/Internal energy [J/kg]
    //  Non-const access allowed for transport equations
    virtual volScalarField& he()
    {
        return he_;
    }

    //- Enthalpy/Internal energy [J/kg]
    virtual const volScalarField& he() const
    {
        return he_;
    }


    // Fields derived from thermodynamic state variables

    //- Enthalpy/Internal energy
    //  for given pressure and temperature [J/kg]
    virtual tmp<volScalarField> he
    (
        const volScalarField& p,
        const volScalarField& T
    ) const;

    //- Enthalpy/Internal energy for cell-set [J/kg]
    virtual tmp<scalarField> he
    (
        const scalarField& p,
        const scalarField& T,
        const labelList& cells
    ) const;

    //- Enthalpy/Internal energy for patch [J/kg]
    virtual tmp<scalarField> he
    (
        const scalarField& p,
        const scalarField& T,
        const label patchi
    ) const;

    //- Chemical enthalpy [J/kg]
    virtual tmp<volScalarField> hc() const;

    //- Temperature from enthalpy/internal energy for cell-set
    virtual tmp<scalarField> THE
    (
        const scalarField& he,
        const scalarField& p,
        const scalarField& T0,      // starting temperature
        const labelList& cells
    ) const;

    //- Temperature from enthalpy/internal energy for patch
    virtual tmp<scalarField> THE
    (
       const scalarField& he,
       const scalarField& p,
       const scalarField& T0,      // starting temperature
       const label patchi
    ) const;

    //- Heat capacity at constant pressure for patch [J/kg/K]
    virtual tmp<scalarField> Cp
    (
        const scalarField& p,
        const scalarField& T,
        const label patchi
    ) const;

    //- Heat capacity at constant pressure [J/kg/K]
    virtual tmp<volScalarField> Cp() const;

    //- Heat capacity at constant volume for patch [J/kg/K]
    virtual tmp<scalarField> Cv
    (
        const scalarField& p,
        const scalarField& T,
        const label patchi
    ) const;

    //- Heat capacity at constant volume [J/kg/K]
    virtual tmp<volScalarField> Cv() const;

    //- Gamma = Cp/Cv []
    virtual tmp<volScalarField> gamma() const;

    //- Gamma = Cp/Cv for patch []
    virtual tmp<scalarField> gamma
    (
        const scalarField& p,
        const scalarField& T,
        const label patchi
    ) const;

    //- Heat capacity at constant pressure/volume for patch [J/kg/K]
    virtual tmp<scalarField> Cpv
    (
        const scalarField& p,
        const scalarField& T,
        const label patchi
    ) const;

    //- Heat capacity at constant pressure/volume [J/kg/K]
    virtual tmp<volScalarField> Cpv() const;

    //- Heat capacity ratio []
    virtual tmp<volScalarField> CpByCpv() const;

    //- Heat capacity ratio for patch []
    virtual tmp<scalarField> CpByCpv
    (
        const scalarField& p,
        const scalarField& T,
        const label patchi
    ) const;

    //- Molecular weight [kg/kmol]
    virtual tmp<volScalarField> W() const;

    //- Molecular weight for patch [kg/kmol]
    virtual tmp<scalarField> W(const label patchi) const;


    // Fields derived from transport state variables

    //- Thermal diffusivity for temperature of mixture [J/m/s/K]
    virtual tmp<volScalarField> kappa() const;

    //- Thermal diffusivity for temperature
    //  of mixture for patch [J/m/s/K]
    virtual tmp<scalarField> kappa
    (
        const label patchi
    ) const;

    //- Effective thermal diffusivity for temperature
    //  of mixture [J/m/s/K]
    virtual tmp<volScalarField> kappaEff(const volScalarField&) const;

    //- Effective thermal diffusivity for temperature
    //  of mixture for patch [J/m/s/K]
    virtual tmp<scalarField> kappaEff
    (
        const scalarField& alphat,
        const label patchi
    ) const;

    //- Effective thermal diffusivity of mixture [kg/m/s]
    virtual tmp<volScalarField> alphaEff
    (
        const volScalarField& alphat
    ) const;

    //- Effective thermal diffusivity of mixture for patch [kg/m/s]
    virtual tmp<scalarField> alphaEff
    (
        const scalarField& alphat,
        const label patchi
    ) const;


    //- Read thermophysical properties dictionary
    virtual bool read();

};


} // End namespace CML


#include "gradientEnergyFvPatchScalarField.hpp"
#include "mixedEnergyFvPatchScalarField.hpp"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class BasicThermo, class MixtureType>
void CML::heThermo<BasicThermo, MixtureType>::
heBoundaryCorrection(volScalarField& h)
{
    volScalarField::GeometricBoundaryField& hBf = h.boundaryField();

    forAll(hBf, patchi)
    {
        if (isA<gradientEnergyFvPatchScalarField>(hBf[patchi]))
        {
            refCast<gradientEnergyFvPatchScalarField>(hBf[patchi]).gradient()
                = hBf[patchi].fvPatchField::snGrad();
        }
        else if (isA<mixedEnergyFvPatchScalarField>(hBf[patchi]))
        {
            refCast<mixedEnergyFvPatchScalarField>(hBf[patchi]).refGrad()
                = hBf[patchi].fvPatchField::snGrad();
        }
    }
}


template<class BasicThermo, class MixtureType>
void CML::heThermo<BasicThermo, MixtureType>::init()
{
    scalarField& heCells = he_.internalField();
    const scalarField& pCells = this->p_.internalField();
    const scalarField& TCells = this->T_.internalField();

    forAll(heCells, celli)
    {
        heCells[celli] =
            this->cellMixture(celli).HE(pCells[celli], TCells[celli]);
    }

    volScalarField::GeometricBoundaryField& heBf = he_.boundaryField();

    forAll(heBf, patchi)
    {
        heBf[patchi] == he
        (
            this->p_.boundaryField()[patchi],
            this->T_.boundaryField()[patchi],
            patchi
        );
    }

    this->heBoundaryCorrection(he_);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


template<class BasicThermo, class MixtureType>
CML::heThermo<BasicThermo, MixtureType>::heThermo
(
    const fvMesh& mesh,
    const word& phaseName
)
:
    BasicThermo(mesh, phaseName),
    MixtureType(*this, mesh, phaseName),

    he_
    (
        IOobject
        (
            BasicThermo::phasePropertyName
            (
                MixtureType::thermoType::heName()
            ),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass,
        this->heBoundaryTypes(),
        this->heBoundaryBaseTypes()
    )
{
    init();
}


template<class BasicThermo, class MixtureType>
CML::heThermo<BasicThermo, MixtureType>::heThermo
(
    const fvMesh& mesh,
    const dictionary& dict,
    const word& phaseName
)
:
    BasicThermo(mesh, dict, phaseName),
    MixtureType(*this, mesh, phaseName),

    he_
    (
        IOobject
        (
            BasicThermo::phasePropertyName
            (
                MixtureType::thermoType::heName()
            ),
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimEnergy/dimMass,
        this->heBoundaryTypes(),
        this->heBoundaryBaseTypes()
    )
{
    init();
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class BasicThermo, class MixtureType>
CML::heThermo<BasicThermo, MixtureType>::~heThermo()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class BasicThermo, class MixtureType>
CML::tmp<CML::volScalarField> CML::heThermo<BasicThermo, MixtureType>::he
(
    const volScalarField& p,
    const volScalarField& T
) const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volScalarField> the
    (
        new volScalarField
        (
            IOobject
            (
                "he",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh,
            he_.dimensions()
        )
    );

    volScalarField& he = the();
    scalarField& heCells = he.internalField();
    const scalarField& pCells = p.internalField();
    const scalarField& TCells = T.internalField();

    forAll(heCells, celli)
    {
        heCells[celli] =
            this->cellMixture(celli).HE(pCells[celli], TCells[celli]);
    }

    volScalarField::GeometricBoundaryField& heBf = he.boundaryField();

    forAll(heBf, patchi)
    {
        scalarField& hep = heBf[patchi];
        const scalarField& pp = p.boundaryField()[patchi];
        const scalarField& Tp = T.boundaryField()[patchi];

        forAll(hep, facei)
        {
            hep[facei] =
                this->patchFaceMixture(patchi, facei).HE(pp[facei], Tp[facei]);
        }
    }

    return the;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField> CML::heThermo<BasicThermo, MixtureType>::he
(
    const scalarField& p,
    const scalarField& T,
    const labelList& cells
) const
{
    tmp<scalarField> the(new scalarField(T.size()));
    scalarField& he = the();

    forAll(T, celli)
    {
        he[celli] = this->cellMixture(cells[celli]).HE(p[celli], T[celli]);
    }

    return the;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField> CML::heThermo<BasicThermo, MixtureType>::he
(
    const scalarField& p,
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> the(new scalarField(T.size()));
    scalarField& he = the();

    forAll(T, facei)
    {
        he[facei] =
            this->patchFaceMixture(patchi, facei).HE(p[facei], T[facei]);
    }

    return the;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heThermo<BasicThermo, MixtureType>::hc() const
{
    const fvMesh& mesh = this->T_.mesh();

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
                IOobject::NO_WRITE,
                false
            ),
            mesh,
            he_.dimensions()
        )
    );

    volScalarField& hcf = thc();
    scalarField& hcCells = hcf.internalField();

    forAll(hcCells, celli)
    {
        hcCells[celli] = this->cellMixture(celli).Hc();
    }

    volScalarField::GeometricBoundaryField& hcfBf = hcf.boundaryField();

    forAll(hcfBf, patchi)
    {
        scalarField& hcp = hcfBf[patchi];

        forAll(hcp, facei)
        {
            hcp[facei] = this->patchFaceMixture(patchi, facei).Hc();
        }
    }

    return thc;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField> CML::heThermo<BasicThermo, MixtureType>::Cp
(
    const scalarField& p,
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> tCp(new scalarField(T.size()));
    scalarField& cp = tCp();

    forAll(T, facei)
    {
        cp[facei] =
            this->patchFaceMixture(patchi, facei).Cp(p[facei], T[facei]);
    }

    return tCp;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heThermo<BasicThermo, MixtureType>::Cp() const
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
                IOobject::NO_WRITE,
                false
            ),
            mesh,
            dimEnergy/dimMass/dimTemperature
        )
    );

    volScalarField& cp = tCp();

    forAll(this->T_, celli)
    {
        cp[celli] =
            this->cellMixture(celli).Cp(this->p_[celli], this->T_[celli]);
    }

    volScalarField::GeometricBoundaryField& cpBf = cp.boundaryField();

    forAll(cpBf, patchi)
    {
        const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
        fvPatchScalarField& pCp = cpBf[patchi];

        forAll(pT, facei)
        {
            pCp[facei] =
                this->patchFaceMixture(patchi, facei).Cp(pp[facei], pT[facei]);
        }
    }

    return tCp;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField>
CML::heThermo<BasicThermo, MixtureType>::Cv
(
    const scalarField& p,
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> tCv(new scalarField(T.size()));
    scalarField& cv = tCv();

    forAll(T, facei)
    {
        cv[facei] =
            this->patchFaceMixture(patchi, facei).Cv(p[facei], T[facei]);
    }

    return tCv;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heThermo<BasicThermo, MixtureType>::Cv() const
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
                IOobject::NO_WRITE,
                false
            ),
            mesh,
            dimEnergy/dimMass/dimTemperature
        )
    );

    volScalarField& cv = tCv();

    forAll(this->T_, celli)
    {
        cv[celli] =
            this->cellMixture(celli).Cv(this->p_[celli], this->T_[celli]);
    }

    volScalarField::GeometricBoundaryField& cvBf = cv.boundaryField();

    forAll(cvBf, patchi)
    {
        cvBf[patchi] = Cv
        (
            this->p_.boundaryField()[patchi],
            this->T_.boundaryField()[patchi],
            patchi
        );
    }

    return tCv;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField> CML::heThermo<BasicThermo, MixtureType>::gamma
(
    const scalarField& p,
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> tgamma(new scalarField(T.size()));
    scalarField& gamma = tgamma();

    forAll(T, facei)
    {
        gamma[facei] =
            this->patchFaceMixture(patchi, facei).gamma(p[facei], T[facei]);
    }

    return tgamma;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heThermo<BasicThermo, MixtureType>::gamma() const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volScalarField> tgamma
    (
        new volScalarField
        (
            IOobject
            (
                "gamma",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh,
            dimless
        )
    );

    volScalarField& gamma = tgamma();

    forAll(this->T_, celli)
    {
        gamma[celli] =
            this->cellMixture(celli).gamma(this->p_[celli], this->T_[celli]);
    }

    volScalarField::GeometricBoundaryField& gammaBf = gamma.boundaryField();

    forAll(gammaBf, patchi)
    {
        const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
        fvPatchScalarField& pgamma = gammaBf[patchi];

        forAll(pT, facei)
        {
            pgamma[facei] = this->patchFaceMixture(patchi, facei).gamma
            (
                pp[facei],
                pT[facei]
            );
        }
    }

    return tgamma;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField> CML::heThermo<BasicThermo, MixtureType>::Cpv
(
    const scalarField& p,
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> tCpv(new scalarField(T.size()));
    scalarField& Cpv = tCpv();

    forAll(T, facei)
    {
        Cpv[facei] =
            this->patchFaceMixture(patchi, facei).Cpv(p[facei], T[facei]);
    }

    return tCpv;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heThermo<BasicThermo, MixtureType>::Cpv() const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volScalarField> tCpv
    (
        new volScalarField
        (
            IOobject
            (
                "Cpv",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh,
            dimEnergy/dimMass/dimTemperature
        )
    );

    volScalarField& Cpv = tCpv();

    forAll(this->T_, celli)
    {
        Cpv[celli] =
            this->cellMixture(celli).Cpv(this->p_[celli], this->T_[celli]);
    }

    volScalarField::GeometricBoundaryField& CpvBf = Cpv.boundaryField();

    forAll(CpvBf, patchi)
    {
        const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
        fvPatchScalarField& pCpv = CpvBf[patchi];

        forAll(pT, facei)
        {
            pCpv[facei] =
                this->patchFaceMixture(patchi, facei).Cpv(pp[facei], pT[facei]);
        }
    }

    return tCpv;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField> CML::heThermo<BasicThermo, MixtureType>::CpByCpv
(
    const scalarField& p,
    const scalarField& T,
    const label patchi
) const
{
    tmp<scalarField> tCpByCpv(new scalarField(T.size()));
    scalarField& CpByCpv = tCpByCpv();

    forAll(T, facei)
    {
        CpByCpv[facei] =
            this->patchFaceMixture(patchi, facei).CpByCpv(p[facei], T[facei]);
    }

    return tCpByCpv;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heThermo<BasicThermo, MixtureType>::CpByCpv() const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volScalarField> tCpByCpv
    (
        new volScalarField
        (
            IOobject
            (
                "CpByCpv",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh,
            dimless
        )
    );

    volScalarField& CpByCpv = tCpByCpv();

    forAll(this->T_, celli)
    {
        CpByCpv[celli] = this->cellMixture(celli).CpByCpv
        (
            this->p_[celli],
            this->T_[celli]
        );
    }

    volScalarField::GeometricBoundaryField& CpByCpvBf =
        CpByCpv.boundaryField();

    forAll(CpByCpvBf, patchi)
    {
        const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];
        fvPatchScalarField& pCpByCpv = CpByCpvBf[patchi];

        forAll(pT, facei)
        {
            pCpByCpv[facei] = this->patchFaceMixture(patchi, facei).CpByCpv
            (
                pp[facei],
                pT[facei]
            );
        }
    }

    return tCpByCpv;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField> CML::heThermo<BasicThermo, MixtureType>::THE
(
    const scalarField& h,
    const scalarField& p,
    const scalarField& T0,
    const labelList& cells
) const
{
    tmp<scalarField> tT(new scalarField(h.size()));
    scalarField& T = tT();

    forAll(h, celli)
    {
        T[celli] =
            this->cellMixture(cells[celli]).THE(h[celli], p[celli], T0[celli]);
    }

    return tT;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField> CML::heThermo<BasicThermo, MixtureType>::THE
(
    const scalarField& h,
    const scalarField& p,
    const scalarField& T0,
    const label patchi
) const
{

    tmp<scalarField> tT(new scalarField(h.size()));
    scalarField& T = tT();
    forAll(h, facei)
    {
        T[facei] = this->patchFaceMixture
        (
            patchi,
            facei
        ).THE(h[facei], p[facei], T0[facei]);
    }

    return tT;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::volScalarField> CML::heThermo<BasicThermo, MixtureType>::W
(
) const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<volScalarField> tW
    (
        new volScalarField
        (
            IOobject
            (
                "W",
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            mesh,
            dimMass/dimMoles
        )
    );

    volScalarField& W = tW();
    scalarField& WCells = W.internalField();

    forAll(WCells, celli)
    {
        WCells[celli] = this->cellMixture(celli).W();
    }

    volScalarField::GeometricBoundaryField& WBf = W.boundaryField();

    forAll(WBf, patchi)
    {
        scalarField& Wp = WBf[patchi];
        forAll(Wp, facei)
        {
            Wp[facei] = this->patchFaceMixture(patchi, facei).W();
        }
    }

    return tW;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField> CML::heThermo<BasicThermo, MixtureType>::W
(
    const label patchi
) const
{
    const fvMesh& mesh = this->T_.mesh();

    tmp<scalarField> tW(new scalarField(mesh.boundaryMesh()[patchi].size()));
    scalarField& W = tW();
    forAll(W, facei)
    {
        W[facei] = this->patchFaceMixture(patchi, facei).W();
    }

    return tW;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heThermo<BasicThermo, MixtureType>::kappa() const
{
    tmp<CML::volScalarField> kappa(Cp()*this->alpha_);
    kappa().rename("kappa");
    return kappa;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField> CML::heThermo<BasicThermo, MixtureType>::kappa
(
    const label patchi
) const
{
    return
        Cp
        (
            this->p_.boundaryField()[patchi],
            this->T_.boundaryField()[patchi],
            patchi
        )*this->alpha_.boundaryField()[patchi];
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heThermo<BasicThermo, MixtureType>::kappaEff
(
    const volScalarField& alphat
) const
{
    tmp<CML::volScalarField> kappaEff(Cp()*(this->alpha_ + alphat));
    kappaEff().rename("kappaEff");
    return kappaEff;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField>
CML::heThermo<BasicThermo, MixtureType>::kappaEff
(
    const scalarField& alphat,
    const label patchi
) const
{
    return
        Cp
        (
            this->p_.boundaryField()[patchi],
            this->T_.boundaryField()[patchi],
            patchi
        )
       *(
           this->alpha_.boundaryField()[patchi]
         + alphat
        );
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heThermo<BasicThermo, MixtureType>::alphaEff
(
    const volScalarField& alphat
) const
{
    tmp<CML::volScalarField> alphaEff(this->CpByCpv()*(this->alpha_ + alphat));
    alphaEff().rename("alphaEff");
    return alphaEff;
}


template<class BasicThermo, class MixtureType>
CML::tmp<CML::scalarField>
CML::heThermo<BasicThermo, MixtureType>::alphaEff
(
    const scalarField& alphat,
    const label patchi
) const
{
    return
    this->CpByCpv
    (
        this->p_.boundaryField()[patchi],
        this->T_.boundaryField()[patchi],
        patchi
    )
   *(
        this->alpha_.boundaryField()[patchi]
      + alphat
    );
}


template<class BasicThermo, class MixtureType>
bool CML::heThermo<BasicThermo, MixtureType>::read()
{
    if (BasicThermo::read())
    {
        MixtureType::read(*this);
        return true;
    }
    else
    {
        return false;
    }
}


#endif
