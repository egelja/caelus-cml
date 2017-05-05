/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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
    CML::combustionModels::noCombustion

Description


\*---------------------------------------------------------------------------*/

#ifndef noCombustion_H
#define noCombustion_H

#include "fvmSup.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                            Class noCombustion Declaration
\*---------------------------------------------------------------------------*/

template<class CombThermoType>
class noCombustion
:
    public CombThermoType
{

    //- Disallow copy construct
    noCombustion(const noCombustion&);

    //- Disallow default bitwise assignment
    void operator=(const noCombustion&);


public:

    //- Runtime type information
    TypeName("noCombustion");


    // Constructors

        //- Construct from components
        noCombustion
        (
            const word& modelType,
            const fvMesh& mesh
        );


    //- Destructor
    virtual ~noCombustion();


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
CML::combustionModels::noCombustion<CombThermoType>::noCombustion
(
    const word& modelType,
    const fvMesh& mesh
)
:
    CombThermoType(modelType, mesh)
{}


// * * * * * * * * * * * * * * * * Destructors * * * * * * * * * * * * * * * //

template<class CombThermoType>
CML::combustionModels::noCombustion<CombThermoType>::~noCombustion()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class CombThermoType>
void CML::combustionModels::noCombustion<CombThermoType>::correct()
{
//  Do Nothing
}


template<class CombThermoType>
CML::tmp<CML::fvScalarMatrix>
CML::combustionModels::noCombustion<CombThermoType>::R
(
    const volScalarField& Y
) const
{
    tmp<fvScalarMatrix> tSu
    (
        new fvScalarMatrix(Y, dimMass/dimTime)
    );

    return tSu;
}


template<class CombThermoType>
CML::tmp<CML::volScalarField>
CML::combustionModels::noCombustion<CombThermoType>::dQ() const
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

    return tdQ;
}


template<class CombThermoType>
CML::tmp<CML::volScalarField>
CML::combustionModels::noCombustion<CombThermoType>::Sh() const
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

    return tSh;
}


template<class CombThermoType>
bool CML::combustionModels::noCombustion<CombThermoType>::read()
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


#endif

// ************************************************************************* //
