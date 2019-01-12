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
    CML::heheuReactionThermo

Description


\*---------------------------------------------------------------------------*/

#ifndef heheuPsiThermo_HPP
#define heheuPsiThermo_HPP

#include "heThermo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class heheuPsiThermo Declaration
\*---------------------------------------------------------------------------*/

template<class BasicPsiThermo, class MixtureType>
class heheuPsiThermo
:
    public heThermo<BasicPsiThermo, MixtureType>
{

    volScalarField Tu_;
    volScalarField heu_;


    void calculate();

    //- Construct as copy (not implemented)
    heheuPsiThermo
    (
        const heheuPsiThermo<BasicPsiThermo, MixtureType>&
    );


public:

    //- Runtime type information
    TypeName("heheuPsiThermo");


    //- Construct from mesh and phase name
    heheuPsiThermo
    (
        const fvMesh&,
        const word& phaseName
    );


    //- Destructor
    virtual ~heheuPsiThermo()
    {}


    // Member functions

    //- Update properties
    virtual void correct();

    // Access to thermodynamic state variables.

    //- Unburnt gas enthalpy [J/kg]
    //  Non-const access allowed for transport equations
    virtual volScalarField& heu()
    {
                return heu_;
    }

    //- Unburnt gas enthalpy [J/kg]
    virtual const volScalarField& heu() const
    {
        return heu_;
    }

    //- Unburnt gas temperature [K]
    virtual const volScalarField& Tu() const
    {
        return Tu_;
    }


    // Fields derived from thermodynamic state variables

    //- Unburnt gas enthalpy for cell-set [J/kg]
    virtual tmp<scalarField> heu
    (
        const scalarField& p,
        const scalarField& T,
        const labelList& cells
    ) const;

    //- Unburnt gas enthalpy for patch [J/kg]
    virtual tmp<scalarField> heu
    (
        const scalarField& p,
        const scalarField& T,
        const label patchi
    ) const;


    //- Burnt gas temperature [K]
    virtual tmp<volScalarField> Tb() const;

    //- Unburnt gas compressibility [s^2/m^2]
    virtual tmp<volScalarField> psiu() const;

    //- Burnt gas compressibility [s^2/m^2]
    virtual tmp<volScalarField> psib() const;


    // Access to transport variables

    //- Dynamic viscosity of unburnt gas [kg/ms]
    virtual tmp<volScalarField> muu() const;

    //- Dynamic viscosity of burnt gas [kg/ms]
    virtual tmp<volScalarField> mub() const;
};


} // End namespace CML


#include "fvMesh.hpp"
#include "fixedValueFvPatchFields.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class BasicPsiThermo, class MixtureType>
void CML::heheuPsiThermo<BasicPsiThermo, MixtureType>::calculate()
{
    const scalarField& hCells = this->he_.internalField();
    const scalarField& heuCells = this->heu_.internalField();
    const scalarField& pCells = this->p_.internalField();

    scalarField& TCells = this->T_.internalField();
    scalarField& TuCells = this->Tu_.internalField();
    scalarField& psiCells = this->psi_.internalField();
    scalarField& muCells = this->mu_.internalField();
    scalarField& alphaCells = this->alpha_.internalField();

    forAll(TCells, celli)
    {
        const typename MixtureType::thermoType& mixture_ =
            this->cellMixture(celli);

        TCells[celli] = mixture_.THE
        (
            hCells[celli],
            pCells[celli],
            TCells[celli]
        );

        psiCells[celli] = mixture_.psi(pCells[celli], TCells[celli]);

        muCells[celli] = mixture_.mu(pCells[celli], TCells[celli]);
        alphaCells[celli] = mixture_.alphah(pCells[celli], TCells[celli]);

        TuCells[celli] = this->cellReactants(celli).THE
        (
            heuCells[celli],
            pCells[celli],
            TuCells[celli]
        );
    }

    volScalarField::GeometricBoundaryField& pBf =
        this->p_.boundaryField();

    volScalarField::GeometricBoundaryField& TBf =
        this->T_.boundaryField();

    volScalarField::GeometricBoundaryField& TuBf =
        this->Tu_.boundaryField();

    volScalarField::GeometricBoundaryField& psiBf =
        this->psi_.boundaryField();

    volScalarField::GeometricBoundaryField& heBf =
        this->he().boundaryField();

    volScalarField::GeometricBoundaryField& heuBf =
        this->heu().boundaryField();

    volScalarField::GeometricBoundaryField& muBf =
        this->mu_.boundaryField();

    volScalarField::GeometricBoundaryField& alphaBf =
        this->alpha_.boundaryField();

    forAll(this->T_.boundaryField(), patchi)
    {
        fvPatchScalarField& pp = pBf[patchi];
        fvPatchScalarField& pT = TBf[patchi];
        fvPatchScalarField& pTu = TuBf[patchi];
        fvPatchScalarField& ppsi = psiBf[patchi];
        fvPatchScalarField& phe = heBf[patchi];
        fvPatchScalarField& pheu = heuBf[patchi];
        fvPatchScalarField& pmu = muBf[patchi];
        fvPatchScalarField& palpha = alphaBf[patchi];

        if (pT.fixesValue())
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture_ =
                    this->patchFaceMixture(patchi, facei);

                phe[facei] = mixture_.HE(pp[facei], pT[facei]);

                ppsi[facei] = mixture_.psi(pp[facei], pT[facei]);
                pmu[facei] = mixture_.mu(pp[facei], pT[facei]);
                palpha[facei] = mixture_.alphah(pp[facei], pT[facei]);
            }
        }
        else
        {
            forAll(pT, facei)
            {
                const typename MixtureType::thermoType& mixture_ =
                    this->patchFaceMixture(patchi, facei);

                pT[facei] = mixture_.THE(phe[facei], pp[facei], pT[facei]);

                ppsi[facei] = mixture_.psi(pp[facei], pT[facei]);
                pmu[facei] = mixture_.mu(pp[facei], pT[facei]);
                palpha[facei] = mixture_.alphah(pp[facei], pT[facei]);

                pTu[facei] =
                    this->patchFaceReactants(patchi, facei)
                   .THE(pheu[facei], pp[facei], pTu[facei]);
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class BasicPsiThermo, class MixtureType>
CML::heheuPsiThermo<BasicPsiThermo, MixtureType>::heheuPsiThermo
(
    const fvMesh& mesh,
    const word& phaseName
)
:
    heThermo<psiuReactionThermo, MixtureType>(mesh, phaseName),
    Tu_
    (
        IOobject
        (
            "Tu",
            mesh.time().timeName(),
            mesh,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh
    ),

    heu_
    (
        IOobject
        (
            MixtureType::thermoType::heName() + 'u',
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionSet(0, 2, -2, 0, 0),
        this->heuBoundaryTypes()
    )
{
    scalarField& heuCells = this->heu_.internalField();
    const scalarField& pCells = this->p_.internalField();
    const scalarField& TuCells = this->Tu_.internalField();

    forAll(heuCells, celli)
    {
        heuCells[celli] = this->cellReactants(celli).HE
        (
            pCells[celli],
            TuCells[celli]
        );
    }

    volScalarField::GeometricBoundaryField& heuBf = heu_.boundaryField();

    forAll(heuBf, patchi)
    {
        fvPatchScalarField& pheu = heuBf[patchi];
        const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        const fvPatchScalarField& pTu = this->Tu_.boundaryField()[patchi];

        forAll(pheu, facei)
        {
            pheu[facei] = this->patchFaceReactants(patchi, facei).HE
            (
                pp[facei],
                pTu[facei]
            );
        }
    }

    this->heuBoundaryCorrection(this->heu_);

    calculate();
    this->psi_.oldTime();   // Switch on saving old time
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class BasicPsiThermo, class MixtureType>
void CML::heheuPsiThermo<BasicPsiThermo, MixtureType>::correct()
{
    if (debug)
    {
        InfoInFunction << endl;
    }

    // force the saving of the old-time values
    this->psi_.oldTime();

    calculate();

    if (debug)
    {
        Info<< "    Finished" << endl;
    }
}


template<class BasicPsiThermo, class MixtureType>
CML::tmp<CML::scalarField>
CML::heheuPsiThermo<BasicPsiThermo, MixtureType>::heu
(
    const scalarField& p,
    const scalarField& Tu,
    const labelList& cells
) const
{
    tmp<scalarField> theu(new scalarField(Tu.size()));
    scalarField& heu = theu();

    forAll(Tu, celli)
    {
        heu[celli] = this->cellReactants(cells[celli]).HE(p[celli], Tu[celli]);
    }

    return theu;
}


template<class BasicPsiThermo, class MixtureType>
CML::tmp<CML::scalarField>
CML::heheuPsiThermo<BasicPsiThermo, MixtureType>::heu
(
    const scalarField& p,
    const scalarField& Tu,
    const label patchi
) const
{
    tmp<scalarField> theu(new scalarField(Tu.size()));
    scalarField& heu = theu();

    forAll(Tu, facei)
    {
        heu[facei] =
            this->patchFaceReactants(patchi, facei).HE(p[facei], Tu[facei]);
    }

    return theu;
}


template<class BasicPsiThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heheuPsiThermo<BasicPsiThermo, MixtureType>::Tb() const
{
    tmp<volScalarField> tTb
    (
        new volScalarField
        (
            IOobject
            (
                "Tb",
                this->T_.time().timeName(),
                this->T_.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->T_
        )
    );

    volScalarField& Tb_ = tTb();
    scalarField& TbCells = Tb_.internalField();
    const scalarField& pCells = this->p_.internalField();
    const scalarField& TCells = this->T_.internalField();
    const scalarField& hCells = this->he_.internalField();

    forAll(TbCells, celli)
    {
        TbCells[celli] = this->cellProducts(celli).THE
        (
            hCells[celli],
            pCells[celli],
            TCells[celli]
        );
    }

    volScalarField::GeometricBoundaryField& TbBf = Tb_.boundaryField();

    forAll(TbBf, patchi)
    {
        fvPatchScalarField& pTb = TbBf[patchi];

        const fvPatchScalarField& ph = this->he_.boundaryField()[patchi];
        const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        const fvPatchScalarField& pT = this->T_.boundaryField()[patchi];

        forAll(pTb, facei)
        {
            pTb[facei] =
                this->patchFaceProducts(patchi, facei)
               .THE(ph[facei], pp[facei], pT[facei]);
        }
    }

    return tTb;
}


template<class BasicPsiThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heheuPsiThermo<BasicPsiThermo, MixtureType>::psiu() const
{
    tmp<volScalarField> tpsiu
    (
        new volScalarField
        (
            IOobject
            (
                "psiu",
                this->psi_.time().timeName(),
                this->psi_.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->psi_.mesh(),
            this->psi_.dimensions()
        )
    );

    volScalarField& psiu = tpsiu();
    scalarField& psiuCells = psiu.internalField();
    const scalarField& TuCells = this->Tu_.internalField();
    const scalarField& pCells = this->p_.internalField();

    forAll(psiuCells, celli)
    {
        psiuCells[celli] =
            this->cellReactants(celli).psi(pCells[celli], TuCells[celli]);
    }

    volScalarField::GeometricBoundaryField& psiuBf = psiu.boundaryField();

    forAll(psiuBf, patchi)
    {
        fvPatchScalarField& ppsiu = psiuBf[patchi];

        const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        const fvPatchScalarField& pTu = this->Tu_.boundaryField()[patchi];

        forAll(ppsiu, facei)
        {
            ppsiu[facei] =
                this->
                patchFaceReactants(patchi, facei).psi(pp[facei], pTu[facei]);
        }
    }

    return tpsiu;
}


template<class BasicPsiThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heheuPsiThermo<BasicPsiThermo, MixtureType>::psib() const
{
    tmp<volScalarField> tpsib
    (
        new volScalarField
        (
            IOobject
            (
                "psib",
                this->psi_.time().timeName(),
                this->psi_.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->psi_.mesh(),
            this->psi_.dimensions()
        )
    );

    volScalarField& psib = tpsib();
    scalarField& psibCells = psib.internalField();
    const volScalarField Tb_(Tb());
    const scalarField& TbCells = Tb_.internalField();
    const scalarField& pCells = this->p_.internalField();

    forAll(psibCells, celli)
    {
        psibCells[celli] =
            this->cellReactants(celli).psi(pCells[celli], TbCells[celli]);
    }

    volScalarField::GeometricBoundaryField& psibBf = psib.boundaryField();

    forAll(psibBf, patchi)
    {
        fvPatchScalarField& ppsib = psibBf[patchi];

        const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        const fvPatchScalarField& pTb = Tb_.boundaryField()[patchi];

        forAll(ppsib, facei)
        {
            ppsib[facei] =
                this->patchFaceReactants
                (patchi, facei).psi(pp[facei], pTb[facei]);
        }
    }

    return tpsib;
}


template<class BasicPsiThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heheuPsiThermo<BasicPsiThermo, MixtureType>::muu() const
{
    tmp<volScalarField> tmuu
    (
        new volScalarField
        (
            IOobject
            (
                "muu",
                this->T_.time().timeName(),
                this->T_.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->T_.mesh(),
            dimensionSet(1, -1, -1, 0, 0)
        )
    );

    volScalarField& muu_ = tmuu();
    scalarField& muuCells = muu_.internalField();
    const scalarField& pCells = this->p_.internalField();
    const scalarField& TuCells = this->Tu_.internalField();

    forAll(muuCells, celli)
    {
        muuCells[celli] = this->cellReactants(celli).mu
        (
            pCells[celli],
            TuCells[celli]
        );
    }

    volScalarField::GeometricBoundaryField& muuBf = muu_.boundaryField();

    forAll(muuBf, patchi)
    {
        fvPatchScalarField& pMuu = muuBf[patchi];
        const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        const fvPatchScalarField& pTu = this->Tu_.boundaryField()[patchi];

        forAll(pMuu, facei)
        {
            pMuu[facei] = this->patchFaceReactants(patchi, facei).mu
            (
                pp[facei],
                pTu[facei]
            );
        }
    }

    return tmuu;
}


template<class BasicPsiThermo, class MixtureType>
CML::tmp<CML::volScalarField>
CML::heheuPsiThermo<BasicPsiThermo, MixtureType>::mub() const
{
    tmp<volScalarField> tmub
    (
        new volScalarField
        (
            IOobject
            (
                "mub",
                this->T_.time().timeName(),
                this->T_.db(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->T_.mesh(),
            dimensionSet(1, -1, -1, 0, 0)
        )
    );

    volScalarField& mub_ = tmub();
    scalarField& mubCells = mub_.internalField();
    const volScalarField Tb_(Tb());
    const scalarField& pCells = this->p_.internalField();
    const scalarField& TbCells = Tb_.internalField();

    forAll(mubCells, celli)
    {
        mubCells[celli] = this->cellProducts(celli).mu
        (
            pCells[celli],
            TbCells[celli]
        );
    }

    volScalarField::GeometricBoundaryField& mubBf = mub_.boundaryField();

    forAll(mubBf, patchi)
    {
        fvPatchScalarField& pMub = mubBf[patchi];
        const fvPatchScalarField& pp = this->p_.boundaryField()[patchi];
        const fvPatchScalarField& pTb = Tb_.boundaryField()[patchi];

        forAll(pMub, facei)
        {
            pMub[facei] = this->patchFaceProducts(patchi, facei).mu
            (
                pp[facei],
                pTb[facei]
            );
        }
    }

    return tmub;
}


#endif
