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
    CML::combustionModels::PaSR

Description
    Partially stirred reactor turbulent combustion model.

    This model calculates a finite rate, based on both turbulence and chemistry
    time scales.  Depending on mesh resolution, the Cmix parameter can be used
    to scale the turbulence mixing time scale.


\*---------------------------------------------------------------------------*/

#ifndef PaSR_HPP
#define PaSR_HPP

#include "../laminar/laminar.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                            Class PaSR Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class PaSR
:
    public laminar<ReactionThermo>
{

    //- Mixing constant
    scalar Cmix_;

    //- Mixing parameter
    volScalarField kappa_;


    // Private Member Functions

    //- Disallow copy construct
    PaSR(const PaSR&);

    //- Disallow default bitwise assignment
    void operator=(const PaSR&);


public:

    //- Runtime type information
    TypeName("PaSR");


    //- Construct from components
    PaSR
    (
        const word& modelType,
        ReactionThermo& thermo,
        const compressible::turbulenceModel& turb,
        const word& combustionProperties
    );


    //- Destructor
    virtual ~PaSR()
    {}


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
CML::combustionModels::PaSR<ReactionThermo>::PaSR
(
    const word& modelType,
    ReactionThermo& thermo,
    const compressible::turbulenceModel& turb,
    const word& combustionProperties
)
:
    laminar<ReactionThermo>(modelType, thermo, turb, combustionProperties),
    Cmix_(readScalar(this->coeffs().lookup("Cmix"))),
    kappa_
    (
        IOobject
        (
            thermo.phasePropertyName(typeName + ":kappa"),
            this->mesh().time().timeName(),
            this->mesh(),
            IOobject::NO_READ,
            IOobject::AUTO_WRITE
        ),
        this->mesh(),
        dimensionedScalar("kappa", dimless, 0)
    )
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ReactionThermo>
void CML::combustionModels::PaSR<ReactionThermo>::correct()
{
    laminar<ReactionThermo>::correct();

    tmp<volScalarField> tepsilon(this->turbulence().epsilon());
    const volScalarField& epsilon = tepsilon();
    tmp<volScalarField> tmuEff(this->turbulence().muEff());
    const volScalarField& muEff = tmuEff();
    tmp<volScalarField> ttc(this->tc());
    const volScalarField& tc = ttc();

    tmp<volScalarField> trho(this->rho());
    const volScalarField& rho = trho();

    forAll(epsilon, i)
    {
        const scalar tk =
            Cmix_*sqrt(max(muEff[i]/rho[i]/(epsilon[i] + SMALL), 0));

        if (tk > SMALL)
        {
            kappa_[i] = tc[i]/(tc[i] + tk);
        }
        else
        {
            kappa_[i] = 1.0;
        }
    }
}


template<class ReactionThermo>
CML::tmp<CML::fvScalarMatrix>
CML::combustionModels::PaSR<ReactionThermo>::R(volScalarField& Y) const
{
    return kappa_*laminar<ReactionThermo>::R(Y);
}


template<class ReactionThermo>
CML::tmp<CML::volScalarField>
CML::combustionModels::PaSR<ReactionThermo>::Qdot() const
{
    return tmp<volScalarField>
    (
        new volScalarField
        (
            this->thermo().phasePropertyName(typeName + ":Qdot"),
            kappa_*laminar<ReactionThermo>::Qdot()
        )
    );
}


template<class ReactionThermo>
bool CML::combustionModels::PaSR<ReactionThermo>::read()
{
    if (laminar<ReactionThermo>::read())
    {
        this->coeffs().lookup("Cmix") >> Cmix_;
        return true;
    }
    else
    {
        return false;
    }
}


#endif
