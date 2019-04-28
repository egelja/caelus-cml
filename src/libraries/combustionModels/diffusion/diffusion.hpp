/*---------------------------------------------------------------------------*\
Copyright (C) 2012-2018 OpenFOAM Foundation
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
    CML::combustionModels::diffusion

Description
    Simple diffusion-based combustion model based on the principle mixed is
    burnt. Additional parameter C is used to distribute the heat release rate
    in time.


\*---------------------------------------------------------------------------*/

#ifndef diffusion_HPP
#define diffusion_HPP

#include "singleStepCombustion.hpp"
#include "fvcGrad.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                         Class diffusion Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo, class ThermoType>
class diffusion
:
    public singleStepCombustion<ReactionThermo, ThermoType>
{

    //- Model constant
    scalar C_;

    //- Name of oxidant - default is "O2"
    word oxidantName_;


    // Private Member Functions

    //- Disallow copy construct
    diffusion(const diffusion&);

    //- Disallow default bitwise assignment
    void operator=(const diffusion&);


public:

    //- Runtime type information
    TypeName("diffusion");


    //- Construct from components
    diffusion
    (
        const word& modelType,
        ReactionThermo& thermo,
        const compressible::turbulenceModel& turb,
        const word& combustionProperties
    );


    //- Destructor
    virtual ~diffusion();


    // Member Functions

    //- Correct combustion rate
    virtual void correct();

    //- Update properties
    virtual bool read();

};


} // End namespace combustionModels
} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
CML::combustionModels::diffusion<ReactionThermo, ThermoType>::diffusion
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
    C_(readScalar(this->coeffs().lookup("C"))),
    oxidantName_(this->coeffs().template lookupOrDefault<word>("oxidant", "O2"))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
CML::combustionModels::diffusion<ReactionThermo, ThermoType>::~diffusion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ReactionThermo, class ThermoType>
void CML::combustionModels::diffusion<ReactionThermo, ThermoType>::correct()
{
    this->wFuel_ ==
        dimensionedScalar("zero", dimMass/pow3(dimLength)/dimTime, 0.0);

    this->singleMixturePtr_->fresCorrect();

    const label fuelI = this->singleMixturePtr_->fuelIndex();

    const volScalarField& YFuel = this->thermo().composition().Y()[fuelI];

    if (this->thermo().composition().contains(oxidantName_))
    {
        const volScalarField& YO2 =
            this->thermo().composition().Y(oxidantName_);

        this->wFuel_ ==
            C_*this->turbulence().muEff()
           *mag(fvc::grad(YFuel) & fvc::grad(YO2))
           *pos(YFuel)*pos(YO2);
    }
}


template<class ReactionThermo, class ThermoType>
bool CML::combustionModels::diffusion<ReactionThermo, ThermoType>::read()
{
    if (singleStepCombustion<ReactionThermo, ThermoType>::read())
    {
        this->coeffs().lookup("C") >> C_ ;
        this->coeffs().readIfPresent("oxidant", oxidantName_);
        return true;
    }
    else
    {
        return false;
    }
}


#endif
