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
    CML::combustionModels::singleStepCombustion

Description
    Base class for combustion models using singleStepReactingMixture.


\*---------------------------------------------------------------------------*/

#ifndef singleStepCombustion_H
#define singleStepCombustion_H

#include "singleStepReactingMixture.hpp"
#include "fvmSup.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                  Class singleStepCombustion Declaration
\*---------------------------------------------------------------------------*/

template<class CombThermoType, class ThermoType>
class singleStepCombustion
:
    public CombThermoType
{

protected:

    // Protected data

        //- Reference to singleStepReactingMixture mixture
        singleStepReactingMixture<ThermoType>& singleMixture_;

        //- Fuel consumption rate
        volScalarField wFuel_;


private:

    // Private Member Functions

        //- Disallow copy construct
        singleStepCombustion(const singleStepCombustion&);

        //- Disallow default bitwise assignment
        void operator=(const singleStepCombustion&);


public:


    // Constructors

        //- Construct from components
        singleStepCombustion
        (
            const word& modelType, const fvMesh& mesh
        );


    //- Destructor
    virtual ~singleStepCombustion();


    // Member Functions

        // Evolution


            //- Fuel consumption rate matrix
            virtual tmp<fvScalarMatrix> R(const volScalarField& Y) const;

            //- Heat release rate calculated from fuel consumption rate matrix
            virtual tmp<volScalarField> dQ() const;

            //- Sensible enthalpy source term
            virtual tmp<volScalarField> Sh() const;


    // I-O

            //- Update properties from given dictionary
            virtual bool read();
};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CombThermoType, class ThermoType>
singleStepCombustion<CombThermoType, ThermoType>
::singleStepCombustion
(
    const word& modelType, const fvMesh& mesh
)
:
    CombThermoType(modelType, mesh),
    singleMixture_
    (
        dynamic_cast<singleStepReactingMixture<ThermoType>&>(this->thermo())
    ),
    wFuel_
    (
        IOobject
        (
            "wFuel",
            this->mesh().time().timeName(),
            this->mesh(),
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        this->mesh(),
        dimensionedScalar("zero", dimMass/pow3(dimLength)/dimTime, 0.0)
    )
{}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

template<class CombThermoType, class ThermoType>
singleStepCombustion<CombThermoType, ThermoType>
::~singleStepCombustion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //


template<class CombThermoType, class ThermoType>
CML::tmp<CML::fvScalarMatrix>
singleStepCombustion<CombThermoType, ThermoType>::R
(
    const volScalarField& Y
) const
{
    const label specieI = this->thermo_->composition().species()[Y.name()];

    const volScalarField wSpecie
    (
        wFuel_*singleMixture_.specieStoichCoeffs()[specieI]
    );

    return wSpecie + fvm::Sp(0.0*wSpecie, Y);
}


template<class CombThermoType, class ThermoType>
CML::tmp<CML::volScalarField>
singleStepCombustion< CombThermoType, ThermoType>::Sh() const
{
    const label fuelI = singleMixture_.fuelIndex();
    const volScalarField& YFuel = this->thermo_->composition().Y(fuelI);

    return -singleMixture_.qFuel()*(R(YFuel) & YFuel);
}


template<class CombThermoType, class ThermoType>
CML::tmp<CML::volScalarField>
singleStepCombustion< CombThermoType, ThermoType>::dQ() const
{
    tmp<volScalarField> tdQ
    (
        new volScalarField
        (
            IOobject
            (
                "dQ",
                this->mesh_.time().timeName(),
                this->mesh_,
                IOobject::NO_READ,
                IOobject::NO_WRITE,
                false
            ),
            this->mesh_,
            dimensionedScalar("dQ", dimEnergy/dimTime, 0.0),
            zeroGradientFvPatchScalarField::typeName
        )
    );

    if (this->active())
    {
        volScalarField& dQ = tdQ();
        dQ.dimensionedInternalField() = this->mesh().V()*Sh()();
    }
    return tdQ;
}


template<class CombThermoType, class ThermoType>
bool singleStepCombustion< CombThermoType, ThermoType>::read()
{
    if (CombThermoType::read())
    {
        return true;
    }
    else
    {
        return false;
    }
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace combustionModels
} // End namespace CML


#endif

// ************************************************************************* //
