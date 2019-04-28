/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
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
    CML::radiation::mixtureFractionSoot

Description
    This soot model is purely an state model. The amount of soot produced is
    determined by a single step chemistry as :

        nuf Fuel + nuOx Ox = nuP P + nuSoot soot

    nuSoot is prescribed by the user.

    The single step chemistry used is read from the combustion.
    The soot is not considered into the thermodynamics of the system and it
    is not considered as an extra specie in the solver.

    The spacial distribution is given by the normalization of the first product
    on the rhs of the reaction by default or it can be added as input.

    The input dictionary reads like in the radiationProperties dictionary:

    sootModel mixtureFractionSoot<gasHThermoPhysics>;

    mixtureFractionSootCoeffs
    {
        nuSoot              0.015;
        Wsoot               12;
        mappingField        P;
    }

SourceFiles
    mixtureFractionSoot.cpp

\*---------------------------------------------------------------------------*/

#ifndef mixtureFractionSoot_HPP
#define mixtureFractionSoot_HPP

#include "interpolationLookUpTable.hpp"
#include "sootModel.hpp"
#include "HashTable.hpp"

#include "fluidThermo.hpp"
#include "singleStepReactingMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

namespace radiation
{

/*---------------------------------------------------------------------------*\
                    Class mixtureFractionSoot Declaration
\*---------------------------------------------------------------------------*/
template<class ThermoType>
class mixtureFractionSoot
:
    public sootModel
{

    //- Check mixture in thermo
    static const singleStepReactingMixture<ThermoType>& checkThermo
    (
        const fluidThermo&
    );


    //- Soot mass fraction
    volScalarField soot_;

    //- Soot model dictionary
    dictionary coeffsDict_;

    //- Soot yield
    scalar nuSoot_;

    //- Soot molecular weight
    scalar Wsoot_;

    //- Maximum soot mass concentration at stoichiometric
    scalar sootMax_;

    //- Name of the field mapping the soot
    word mappingFieldName_;

    //- Maximum value of the map field
    scalar mapFieldMax_;

    //- Thermo package
    const fluidThermo& thermo_;

    //- Auto Ptr to singleStepReactingMixture
    const singleStepReactingMixture<ThermoType>& mixture_;


public:

    //- Runtime type information
    TypeName("mixtureFractionSoot");


    //- Construct from components
    mixtureFractionSoot
    (
        const dictionary& dict,
        const fvMesh& mesh,
        const word& modelType
    );


    //- Destructor
    virtual ~mixtureFractionSoot()
    {}


    // Member Functions

    // Edit

    //- Main update/correction routine
    virtual void correct();


    // Access

    //- Return Ysoot
    const volScalarField& soot() const
    {
        return soot_;
    }

};


} // End namespace CML
} // End namespace radiation


// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


template<class ThermoType>
const CML::singleStepReactingMixture<ThermoType>&
CML::radiation::mixtureFractionSoot<ThermoType>::checkThermo
(
    const fluidThermo& thermo
)
{
    if (isA<singleStepReactingMixture<ThermoType> >(thermo))
    {
        return dynamic_cast<const singleStepReactingMixture<ThermoType>& >
        (
            thermo
        );
    }
    else
    {
        FatalErrorInFunction
            << "Inconsistent thermo package for " << thermo.type()
            << "Please select a thermo package based on "
            << "singleStepReactingMixture" << exit(FatalError);

        return dynamic_cast<const singleStepReactingMixture<ThermoType>& >
        (
            thermo
        );
    }

}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
CML::radiation::mixtureFractionSoot<ThermoType>::mixtureFractionSoot
(
    const dictionary& dict,
    const fvMesh& mesh,
    const word& modelType
)
:
    sootModel(dict, mesh, modelType),
    soot_
    (
        IOobject
        (
            "soot",
            mesh_.time().timeName(),
            mesh_,
            IOobject::MUST_READ,
            IOobject::AUTO_WRITE
        ),
        mesh_
    ),
    coeffsDict_(dict.subOrEmptyDict(modelType + "Coeffs")),
    nuSoot_(readScalar(coeffsDict_.lookup("nuSoot"))),
    Wsoot_(readScalar(coeffsDict_.lookup("Wsoot"))),
    sootMax_(-1),
    mappingFieldName_
    (
        coeffsDict_.lookupOrDefault<word>("mappingField", "none")
    ),
    mapFieldMax_(1),
    thermo_(mesh.lookupObject<fluidThermo>(basicThermo::dictName)),
    mixture_(checkThermo(thermo_))
{
    const Reaction<ThermoType>& reaction = mixture_.operator[](0);

    const scalarList& specieStoichCoeffs(mixture_.specieStoichCoeffs());

    scalar totalMol = 0.0;
    forAll(reaction.rhs(), i)
    {
        label speciei = reaction.rhs()[i].index;
        totalMol += mag(specieStoichCoeffs[speciei]);
    }

    totalMol += nuSoot_;

    scalarList Xi(reaction.rhs().size());

    scalar Wm = 0.0;
    forAll(reaction.rhs(), i)
    {
        const label speciei = reaction.rhs()[i].index;
        Xi[i] = mag(specieStoichCoeffs[speciei])/totalMol;
        Wm += Xi[i]*mixture_.speciesData()[speciei].W();
    }

    const scalar XSoot = nuSoot_/totalMol;
    Wm += XSoot*Wsoot_;

    sootMax_ = XSoot*Wsoot_/Wm;

    Info << "Maximum soot mass concentrations: " << sootMax_ << nl;

    if (mappingFieldName_ == "none")
    {
        const label index = reaction.rhs()[0].index;
        mappingFieldName_ = mixture_.Y(index).name();
    }

    const label mapFieldIndex = mixture_.species()[mappingFieldName_];

    mapFieldMax_ = mixture_.Yprod0()[mapFieldIndex];

}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
void CML::radiation::mixtureFractionSoot<ThermoType>::correct()
{
    const volScalarField& mapField =
        mesh_.lookupObject<volScalarField>(mappingFieldName_);

    soot_ = sootMax_*(mapField/mapFieldMax_);
}

#endif
