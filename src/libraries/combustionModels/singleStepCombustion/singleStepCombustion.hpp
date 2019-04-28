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
    CML::combustionModels::singleStepCombustion

Description
    Base class for combustion models using singleStepReactingMixture.


\*---------------------------------------------------------------------------*/

#ifndef singleStepCombustion_HPP
#define singleStepCombustion_HPP

#include "singleStepReactingMixture.hpp"
#include "ThermoCombustion.hpp"
#include "fvmSup.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                    Class singleStepCombustion Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo, class ThermoType>
class singleStepCombustion
:
    public ThermoCombustion<ReactionThermo>
{
    // Private Member Functions

    //- Disallow copy construct
    singleStepCombustion(const singleStepCombustion&);

    //- Disallow default bitwise assignment
    void operator=(const singleStepCombustion&);


protected:

    // Protected data

    //- Pointer to singleStepReactingMixture mixture
    singleStepReactingMixture<ThermoType>* singleMixturePtr_;

    //- Fuel consumption rate
    volScalarField wFuel_;

    //- Semi-implicit (true) or explicit (false) treatment
    bool semiImplicit_;


public:

    //- Construct from components
    singleStepCombustion
    (
        const word& modelType,
        ReactionThermo& thermo,
        const compressible::turbulenceModel& turb,
        const word& combustionProperties
    );


    //- Destructor
    virtual ~singleStepCombustion()
    {}


    // Member Functions

    //- Fuel consumption rate matrix
    virtual tmp<fvScalarMatrix> R(volScalarField& Y) const;

    //- Heat release rate [kg/m/s3]
    virtual tmp<volScalarField> Qdot() const;

    //- Update properties from given dictionary
    virtual bool read();

};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
singleStepCombustion<ReactionThermo, ThermoType>
::singleStepCombustion
(
    const word& modelType,
    ReactionThermo& thermo,
    const compressible::turbulenceModel& turb,
    const word& combustionProperties
)
:
    ThermoCombustion<ReactionThermo>(modelType, thermo, turb),
    singleMixturePtr_(nullptr),
    wFuel_
    (
        IOobject
        (
            this->thermo().phasePropertyName("wFuel"),
            this->mesh().time().timeName(),
            this->mesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        this->mesh(),
        dimensionedScalar("zero", dimMass/dimVolume/dimTime, 0.0)
    ),
    semiImplicit_(readBool(this->coeffs_.lookup("semiImplicit")))
{
    if (isA<singleStepReactingMixture<ThermoType>>(this->thermo()))
    {
        singleMixturePtr_ =
            &dynamic_cast<singleStepReactingMixture<ThermoType>&>
            (
                this->thermo()
            );
    }
    else
    {
        FatalErrorInFunction
            << "Inconsistent thermo package for " << this->type() << " model:\n"
            << "    " << this->thermo().type() << nl << nl
            << "Please select a thermo package based on "
            << "singleStepReactingMixture" << exit(FatalError);
    }

    if (semiImplicit_)
    {
        Info<< "Combustion mode: semi-implicit" << endl;
    }
    else
    {
        Info<< "Combustion mode: explicit" << endl;
    }
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
CML::tmp<CML::fvScalarMatrix> singleStepCombustion<ReactionThermo, ThermoType>::R
(
    volScalarField& Y
) const
{
    const label specieI =
        this->thermo().composition().species()[Y.member()];

    volScalarField wSpecie
    (
        wFuel_*singleMixturePtr_->specieStoichCoeffs()[specieI]
    );

    if (semiImplicit_)
    {
        const label fNorm = singleMixturePtr_->specieProd()[specieI];
        const volScalarField fres(singleMixturePtr_->fres(specieI));
        wSpecie /= max(fNorm*(Y - fres), scalar(1e-2));

        return -fNorm*wSpecie*fres + fNorm*fvm::Sp(wSpecie, Y);
    }
    else
    {
        return wSpecie + fvm::Sp(0.0*wSpecie, Y);
    }
}


template<class ReactionThermo, class ThermoType>
CML::tmp<CML::volScalarField>
singleStepCombustion<ReactionThermo, ThermoType>::Qdot() const
{
    const label fuelI = singleMixturePtr_->fuelIndex();
    volScalarField& YFuel =
        const_cast<volScalarField&>(this->thermo().composition().Y(fuelI));

    return -singleMixturePtr_->qFuel()*(R(YFuel) & YFuel);
}


template<class ReactionThermo, class ThermoType>
bool singleStepCombustion<ReactionThermo, ThermoType>::read()
{
    if (ThermoCombustion<ReactionThermo>::read())
    {
        return true;
    }
    else
    {
        return false;
    }
}


} // End namespace combustionModels
} // End namespace CML


#endif
