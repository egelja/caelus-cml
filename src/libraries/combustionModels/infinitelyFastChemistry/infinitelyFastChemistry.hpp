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
    CML::combustionModels::infinitelyFastChemistry

Description
    Simple infinitely fast chemistry combustion model based on the principle
    mixed is burnt. Additional parameter C is used to distribute the heat
    release rate.in time


\*---------------------------------------------------------------------------*/

#ifndef infinitelyFastChemistry_HPP
#define infinitelyFastChemistry_HPP

#include "singleStepCombustion.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                  Class infinitelyFastChemistry Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo, class ThermoType>
class infinitelyFastChemistry
:
    public singleStepCombustion<ReactionThermo, ThermoType>
{

    //- Model constant
    scalar C_;


    // Private Member Functions

    //- Disallow copy construct
    infinitelyFastChemistry(const infinitelyFastChemistry&);

    //- Disallow default bitwise assignment
    void operator=(const infinitelyFastChemistry&);


public:

    //- Runtime type information
    TypeName("infinitelyFastChemistry");


    //- Construct from components
    infinitelyFastChemistry
    (
        const word& modelType,
        ReactionThermo& thermo,
        const compressible::turbulenceModel& turb,
        const word& combustionProperties
    );


    //- Destructor
    virtual ~infinitelyFastChemistry()
    {}


    // Member Functions

    //- Correct combustion rate
    virtual void correct();

    //- Update properties
    virtual bool read();

};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
infinitelyFastChemistry<ReactionThermo, ThermoType>::infinitelyFastChemistry
(
    const word& modelType,
    ReactionThermo& thermo,
    const compressible::turbulenceModel& turb,
    const word& combustionProperties
)
:
    singleStepCombustion<ReactionThermo, ThermoType>
    (
        modelType,
        thermo,
        turb,
        combustionProperties
    ),
    C_(readScalar(this->coeffs().lookup("C")))
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
void infinitelyFastChemistry<ReactionThermo, ThermoType>::correct()
{
    this->wFuel_ ==
        dimensionedScalar("zero", dimMass/pow3(dimLength)/dimTime, 0.0);

    this->singleMixturePtr_->fresCorrect();

    const label fuelI = this->singleMixturePtr_->fuelIndex();

    const volScalarField& YFuel = this->thermo().composition().Y()[fuelI];

    const dimensionedScalar s = this->singleMixturePtr_->s();

    if (this->thermo().composition().contains("O2"))
    {
        const volScalarField& YO2 = this->thermo().composition().Y("O2");

        this->wFuel_ ==
            this->rho()/(this->mesh().time().deltaT()*C_)
           *min(YFuel, YO2/s.value());
    }
}


template<class ReactionThermo, class ThermoType>
bool infinitelyFastChemistry<ReactionThermo, ThermoType>::read()
{
    if (singleStepCombustion<ReactionThermo, ThermoType>::read())
    {
        this->coeffs().lookup("C") >> C_ ;
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
