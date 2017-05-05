/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License

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
    CML::combustionModels::PaSR

Description
    Simple infinitely fast chemistry combustion model based on the principle
    mixed is burnt. Additional parameter C is used to distribute the heat
    release rate.in time


\*---------------------------------------------------------------------------*/

#ifndef PaSR_H
#define PaSR_H
#include "fvmSup.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                            Class PaSR Declaration
\*---------------------------------------------------------------------------*/

template<class CombThermoType>
class PaSR
:
    public CombThermoType
{
    // Private data

        //- Mixing constant
        dimensionedScalar Cmix_;

        //- Turbulent reaction switch
        Switch turbulentReaction_;

        //- Mixing parameter
        volScalarField kappa_;

        //- Use reaction rate
        bool useReactionRate_;


    // Private Member Functions

        //- Return the chemical time scale
        tmp<volScalarField> tc() const;

        //- Disallow copy construct
        PaSR(const PaSR&);

        //- Disallow default bitwise assignment
        void operator=(const PaSR&);


public:

    //- Runtime type information
    TypeName("PaSR");


    // Constructors

        //- Construct from components
        PaSR
        (
            const word& modelType,
            const fvMesh& mesh
        );


    //- Destructor
    virtual ~PaSR();


    // Member Functions

        // Evolution

            //- Correct combustion rate
            virtual void correct();

            //- Fuel consumption rate matrix.
            virtual tmp<fvScalarMatrix> R(const volScalarField& Y) const;

            //- Heat release rate calculated from fuel consumption rate matrix
            virtual tmp<volScalarField> dQ() const;

            //-  Return source for enthalpy equation [kg/m/s3]
            virtual tmp<volScalarField> Sh() const;

    // I-O

            //- Update properties from given dictionary
            virtual bool read();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace combustionModels
} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CombThermoType>
CML::combustionModels::PaSR<CombThermoType>::PaSR
(
    const word& modelType,
    const fvMesh& mesh
)
:
    CombThermoType(modelType, mesh),
    Cmix_(this->coeffs().lookup("Cmix")),
    turbulentReaction_(this->coeffs().lookup("turbulentReaction")),
    kappa_
    (
        IOobject
        (
            "PaSR::kappa",
            mesh.time().timeName(),
            mesh,
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        mesh,
        dimensionedScalar("kappa", dimless, 0.0)
    ),
    useReactionRate_(this->coeffs().lookupOrDefault("useReactionRate", false))
{}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

template<class CombThermoType>
CML::combustionModels::PaSR<CombThermoType>::~PaSR()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


template<class CombThermoType>
CML::tmp<CML::volScalarField>
CML::combustionModels::PaSR<CombThermoType>::tc() const
{
    return this->pChemistry_->tc();
}


template<class CombThermoType>
void CML::combustionModels::PaSR<CombThermoType>::correct()
{
    if (this->active())
    {
        if (!useReactionRate_)
        {
            this->pChemistry_->solve
            (
                this->mesh().time().value()-this->mesh().time().deltaTValue(),
                this->mesh().time().deltaTValue()
            );
        }
        else
        {
            this->pChemistry_->calculate();
        }

        if (turbulentReaction_)
        {
            tmp<volScalarField> trho(this->rho());
            const volScalarField& rho = trho();
            tmp<volScalarField> tepsilon(this->turbulence().epsilon());
            const volScalarField& epsilon = tepsilon();
            tmp<volScalarField> tmuEff(this->turbulence().muEff());
            const volScalarField& muEff = tmuEff();
            tmp<volScalarField> ttc(tc());
            const volScalarField& tc = ttc();

            forAll(epsilon, i)
            {
                scalar tk =
                    Cmix_.value()*CML::sqrt(muEff[i]/rho[i]/(epsilon[i] + SMALL));

                if (tk > SMALL)
                {
                    kappa_[i] = tc[i]/(tc[i] + tk);
                }
                else
                {
                    // Return to laminar combustion
                    kappa_[i] = 1.0;
                }
            }
        }
        else
        {
            kappa_ = 1.0;
        }
    }
}


template<class CombThermoType>
CML::tmp<CML::fvScalarMatrix>
CML::combustionModels::PaSR<CombThermoType>::R(const volScalarField& Y) const
{

    tmp<fvScalarMatrix> tSu
    (
        new fvScalarMatrix(Y, dimMass/dimTime)
    );

    fvScalarMatrix& Su = tSu();

    if (this->active())
    {
        const label specieI = this->thermo().composition().species()[Y.name()];

        Su += kappa_*this->pChemistry_->RR(specieI);
    }

    return tSu;
}


template<class CombThermoType>
CML::tmp<CML::volScalarField>
CML::combustionModels::PaSR<CombThermoType>::dQ() const
{
    tmp<volScalarField> tdQ
    (
        new volScalarField
        (
            IOobject
            (
                "dQ",
                this->mesh().time().timeName(),
                this->mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->mesh(),
            dimensionedScalar("dQ", dimEnergy/dimTime, 0.0),
            zeroGradientFvPatchScalarField::typeName
        )
    );

    if (this->active())
    {
        volScalarField& dQ = tdQ();
        dQ = kappa_*this->pChemistry_->dQ();
    }

    return tdQ;
}


template<class CombThermoType>
CML::tmp<CML::volScalarField>
CML::combustionModels::PaSR<CombThermoType>::Sh() const
{
    tmp<volScalarField> tSh
    (
        new volScalarField
        (
            IOobject
            (
                "Sh",
                this->mesh().time().timeName(),
                this->mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->mesh(),
            dimensionedScalar("zero", dimEnergy/dimTime/dimVolume, 0.0),
            zeroGradientFvPatchScalarField::typeName
        )
    );

    if (this->active())
    {
        scalarField& Sh = tSh();
        Sh = kappa_*this->pChemistry_->Sh();
    }

    return tSh;
}


template<class CombThermoType>
bool CML::combustionModels::PaSR<CombThermoType>::read()
{
    if (CombThermoType::read())
    {
        this->coeffs().lookup("Cmix") >> Cmix_;
        this->coeffs().lookup("turbulentReaction") >> turbulentReaction_;
        this->coeffs().lookup("useReactionRate") >> useReactionRate_;
        return true;
    }
    else
    {
        return false;
    }
}


#endif

// ************************************************************************* //
