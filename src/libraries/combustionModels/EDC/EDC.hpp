/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
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
    CML::combustionModels::EDC

Description
    Eddy Dissipation Concept (EDC) turbulent combustion model.

    This model considers that the reaction occurs in the regions of the flow
    where the dissipation of turbulence kinetic energy takes place (fine
    structures). The mass fraction of the fine structures and the mean residence
    time are provided by an energy cascade model.

    There are many versions and developments of the EDC model, 4 of which are
    currently supported in this implementation: v1981, v1996, v2005 and
    v2016.  The model variant is selected using the optional \c version entry in
    the \c EDCCoeffs dictionary, \eg

    \verbatim
        EDCCoeffs
        {
            version v2016;
        }
    \endverbatim

    The default version is \c v2005 if the \c version entry is not specified.

    Model versions and references:
    \verbatim
        Version v2005:

            Cgamma = 2.1377
            Ctau = 0.4083
            kappa = gammaL^exp1 / (1 - gammaL^exp2),

            where exp1 = 2, and exp2 = 2.

            Magnussen, B. F. (2005, June).
            The Eddy Dissipation Concept -
            A Bridge Between Science and Technology.
            In ECCOMAS thematic conference on computational combustion
            (pp. 21-24).

        Version v1981:

            Changes coefficients exp1 = 3 and exp2 = 3

            Magnussen, B. (1981, January).
            On the structure of turbulence and a generalized
            eddy dissipation concept for chemical reaction in turbulent flow.
            In 19th Aerospace Sciences Meeting (p. 42).

        Version v1996:

            Changes coefficients exp1 = 2 and exp2 = 3

            Gran, I. R., & Magnussen, B. F. (1996).
            A numerical study of a bluff-body stabilized diffusion flame.
            Part 2. Influence of combustion modeling and finite-rate chemistry.
            Combustion Science and Technology, 119(1-6), 191-217.

        Version v2016:

            Use local constants computed from the turbulent Da and Re numbers.

            Parente, A., Malik, M. R., Contino, F., Cuoci, A., & Dally, B. B.
            (2016).
            Extension of the Eddy Dissipation Concept for
            turbulence/chemistry interactions to MILD combustion.
            Fuel, 163, 98-111.
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef EDC_HPP
#define EDC_HPP

#include "../laminar/laminar.hpp"
#include "NamedEnum.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

//- EDC model versions
enum class EDCversions
{
    v1981,
    v1996,
    v2005,
    v2016
};

extern const NamedEnum<EDCversions, 4> EDCversionNames;
extern const EDCversions EDCdefaultVersion;

const scalar EDCexp1[] = {3, 2, 2, 2};
const scalar EDCexp2[] = {3, 3, 2, 2};

/*---------------------------------------------------------------------------*\
                            Class EDC Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class EDC
:
    public laminar<ReactionThermo>
{

    //- The selected model version
    EDCversions version_;

    scalar C1_;
    scalar C2_;
    scalar Cgamma_;
    scalar Ctau_;
    scalar exp1_;
    scalar exp2_;

    //- Mixing parameter
    volScalarField kappa_;


    // Private Member Functions

    //- Disallow copy construct
    EDC(const EDC&);

    //- Disallow default bitwise assignment
    void operator=(const EDC&);


public:

    //- Runtime type information
    TypeName("EDC");


    //- Construct from components
    EDC
    (
        const word& modelType,
        ReactionThermo& type,
        const compressible::turbulenceModel& turb,
        const word& combustionProperties
    );


    //- Destructor
    virtual ~EDC();


    // Member Functions

    //- Correct combustion rate
    virtual void correct();

    //- Fuel consumption rate matrix.
    virtual tmp<fvScalarMatrix> R(volScalarField& Y) const;

    //- Heat release rate [kg/m/s3]
    virtual tmp<volScalarField> Qdot() const;

    //- Update properties from given dictionary
    virtual bool read();

};


} // End namespace combustionModels
} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::combustionModels::EDC<ReactionThermo>::EDC
(
    const word& modelType,
    ReactionThermo& thermo,
    const compressible::turbulenceModel& turb,
    const word& combustionProperties
)
:
    laminar<ReactionThermo>(modelType, thermo, turb, combustionProperties),
    version_
    (
        EDCversionNames
        [
            this->coeffs().lookupOrDefault
            (
                "version",
                word(EDCversionNames[EDCdefaultVersion])
            )
        ]
    ),
    C1_(this->coeffs().lookupOrDefault("C1", 0.05774)),
    C2_(this->coeffs().lookupOrDefault("C2", 0.5)),
    Cgamma_(this->coeffs().lookupOrDefault("Cgamma", 2.1377)),
    Ctau_(this->coeffs().lookupOrDefault("Ctau", 0.4083)),
    exp1_(this->coeffs().lookupOrDefault("exp1", EDCexp1[int(version_)])),
    exp2_(this->coeffs().lookupOrDefault("exp2", EDCexp2[int(version_)])),
    kappa_
    (
        IOobject
        (
            this->thermo().phasePropertyName(typeName + ":kappa"),
            this->mesh().time().timeName(),
            this->mesh(),
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        this->mesh(),
        dimensionedScalar("kappa", dimless, 0)
    )
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::combustionModels::EDC<ReactionThermo>::~EDC()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ReactionThermo>
void CML::combustionModels::EDC<ReactionThermo>::correct()
{
    tmp<volScalarField> tepsilon(this->turbulence().epsilon());
    const volScalarField& epsilon = tepsilon();

    tmp<volScalarField> tmu(this->turbulence().mu());
    const volScalarField& mu = tmu();

    tmp<volScalarField> tk(this->turbulence().k());
    const volScalarField& k = tk();

    tmp<volScalarField> trho(this->rho());
    const volScalarField& rho = trho();

    scalarField tauStar(epsilon.size(), 0);

    if (version_ == EDCversions::v2016)
    {
        tmp<volScalarField> ttc(this->chemistryPtr_->tc());
        const volScalarField& tc = ttc();

        forAll(tauStar, i)
        {
            const scalar nu = mu[i]/(rho[i] + SMALL);

            const scalar Da =
                max(min(sqrt(nu/(epsilon[i] + SMALL))/tc[i], 10), 1e-10);

            const scalar ReT = sqr(k[i])/(nu*epsilon[i] + SMALL);
            const scalar CtauI = min(C1_/(Da*sqrt(ReT + 1)), 2.1377);

            const scalar CgammaI =
                max(min(C2_*sqrt(Da*(ReT + 1)), 5), 0.4082);

            const scalar gammaL =
                CgammaI*pow025(nu*epsilon[i]/(sqr(k[i]) + SMALL));

            tauStar[i] = CtauI*sqrt(nu/(epsilon[i] + SMALL));

            if (gammaL >= 1)
            {
                kappa_[i] = 1;
            }
            else
            {
                kappa_[i] =
                    max
                    (
                        min
                        (
                            pow(gammaL, exp1_)/(1 - pow(gammaL, exp2_)),
                            1
                        ),
                        0
                    );
            }
        }
    }
    else
    {
        forAll(tauStar, i)
        {
            const scalar nu = mu[i]/(rho[i] + SMALL);
            const scalar gammaL =
                Cgamma_*pow025(nu*epsilon[i]/(sqr(k[i]) + SMALL));

            tauStar[i] = Ctau_*sqrt(nu/(epsilon[i] + SMALL));
            if (gammaL >= 1)
            {
                kappa_[i] = 1;
            }
            else
            {
                kappa_[i] =
                    max
                    (
                        min
                        (
                            pow(gammaL, exp1_)/(1 - pow(gammaL, exp2_)),
                            1
                        ),
                        0
                    );
            }
        }
    }

    this->chemistryPtr_->solve(tauStar);
}


template<class ReactionThermo>
CML::tmp<CML::fvScalarMatrix>
CML::combustionModels::EDC<ReactionThermo>::R(volScalarField& Y) const
{
    return kappa_*laminar<ReactionThermo>::R(Y);
}


template<class ReactionThermo>
CML::tmp<CML::volScalarField>
CML::combustionModels::EDC<ReactionThermo>::Qdot() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            IOobject
            (
                this->thermo().phasePropertyName(typeName + ":Qdot"),
                this->mesh().time().timeName(),
                this->mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            kappa_*this->chemistryPtr_->Qdot()
        )
    );
}


template<class ReactionThermo>
bool CML::combustionModels::EDC<ReactionThermo>::read()
{
    if (laminar<ReactionThermo>::read())
    {
        version_ =
        (
            EDCversionNames
            [
                this->coeffs().lookupOrDefault
                (
                    "version",
                    word(EDCversionNames[EDCdefaultVersion])
                )
            ]
        );
        C1_ = this->coeffs().lookupOrDefault("C1", 0.05774);
        C2_ = this->coeffs().lookupOrDefault("C2", 0.5);
        Cgamma_ = this->coeffs().lookupOrDefault("Cgamma", 2.1377);
        Ctau_ = this->coeffs().lookupOrDefault("Ctau", 0.4083);
        exp1_ = this->coeffs().lookupOrDefault("exp1", EDCexp1[int(version_)]);
        exp2_ = this->coeffs().lookupOrDefault("exp2", EDCexp2[int(version_)]);

        return true;
    }
    else
    {
        return false;
    }
}


#endif
