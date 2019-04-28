/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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
    Dummy combustion model for 'no combustion'


\*---------------------------------------------------------------------------*/

#ifndef noCombustion_HPP
#define noCombustion_HPP

#include "ThermoCombustion.hpp"
#include "fvmSup.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                        Class noCombustion Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class noCombustion
:
    public ThermoCombustion<ReactionThermo>
{

    //- Disallow copy construct
    noCombustion(const noCombustion&);

    //- Disallow default bitwise assignment
    void operator=(const noCombustion&);


public:

    //- Runtime type information
    TypeName("none");


    //- Construct from components
    noCombustion
    (
        const word& modelType,
        ReactionThermo& thermo,
        const compressible::turbulenceModel& turb,
        const word& combustionProperties
    );


    //- Destructor
    virtual ~noCombustion()
    {}


    // Member Functions

    //- Correct combustion rate
    virtual void correct();

    //- Fuel consumption rate matrix
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
CML::combustionModels::noCombustion<ReactionThermo>::noCombustion
(
    const word& modelType,
    ReactionThermo& thermo,
    const compressible::turbulenceModel& turb,
    const word& combustionProperties
)
:
    ThermoCombustion<ReactionThermo>(modelType, thermo, turb)
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ReactionThermo>
void CML::combustionModels::noCombustion<ReactionThermo>::correct()
{}


template<class ReactionThermo>
CML::tmp<CML::fvScalarMatrix>
CML::combustionModels::noCombustion<ReactionThermo>::R
(
    volScalarField& Y
) const
{
    tmp<fvScalarMatrix> tSu
    (
        new fvScalarMatrix(Y, dimMass/dimTime)
    );

    return tSu;
}


template<class ReactionThermo>
CML::tmp<CML::volScalarField>
CML::combustionModels::noCombustion<ReactionThermo>::Qdot() const
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
            this->mesh(),
            dimensionedScalar("Qdot", dimEnergy/dimVolume/dimTime, 0.0)
        )
    );
}


template<class ReactionThermo>
bool CML::combustionModels::noCombustion<ReactionThermo>::read()
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


#endif
