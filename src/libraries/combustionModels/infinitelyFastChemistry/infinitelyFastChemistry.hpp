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
    CML::combustionModels::infinitelyFastChemistry

Description
    Simple infinitely fast chemistry combustion model based on the principle
    mixed is burnt. Additional parameter C is used to distribute the heat
    release rate.in time


\*---------------------------------------------------------------------------*/

#ifndef infinitelyFastChemistry_H
#define infinitelyFastChemistry_H
#include "singleStepCombustion.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                  Class infinitelyFastChemistry Declaration
\*---------------------------------------------------------------------------*/

template<class CombThermoType, class ThermoType>
class infinitelyFastChemistry
:
    public singleStepCombustion <CombThermoType, ThermoType>
{
    // Private data

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


    // Constructors

        //- Construct from components
        infinitelyFastChemistry
        (
            const word& modelType, const fvMesh& mesh
        );


    //- Destructor
    virtual ~infinitelyFastChemistry();


    // Member Functions

        // Evolution

            //- Correct combustion rate
            virtual void correct();


    // I-O

            //- Update properties
            virtual bool read();
};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class CombThermoType, class ThermoType>
infinitelyFastChemistry<CombThermoType, ThermoType>::infinitelyFastChemistry
(
    const word& modelType, const fvMesh& mesh
)
:
    singleStepCombustion<CombThermoType, ThermoType>(modelType, mesh),
    C_(readScalar(this->coeffs().lookup("C")))
{}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

template<class CombThermoType, class ThermoType>
infinitelyFastChemistry<CombThermoType, ThermoType>::~infinitelyFastChemistry()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class CombThermoType, class ThermoType>
void infinitelyFastChemistry<CombThermoType, ThermoType>::correct()
{
    this->wFuel_ ==
        dimensionedScalar("zero", dimMass/pow3(dimLength)/dimTime, 0.0);

    if (this->active())
    {
        this->singleMixture_.fresCorrect();

        const label fuelI = this->singleMixture_.fuelIndex();

        const volScalarField& YFuel = this->thermo_->composition().Y()[fuelI];

        const dimensionedScalar s = this->singleMixture_.s();

        if (this->thermo_->composition().contains("O2"))
        {
            const volScalarField& YO2 = this->thermo_->composition().Y("O2");

            this->wFuel_ ==
                 this->rho()/(this->mesh().time().deltaT()*C_)
                *min(YFuel, YO2/s.value());
        }
    }
}


template<class CombThermoType, class ThermoType>
bool infinitelyFastChemistry<CombThermoType, ThermoType>::read()
{
    if (singleStepCombustion<CombThermoType, ThermoType>::read())
    {
        this->coeffs().lookup("C") >> C_ ;
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
