/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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
    CML::singleStepReactingMixture

Description
    Single step reacting mixture

SourceFiles
    singleStepReactingMixture.cpp

\*---------------------------------------------------------------------------*/

#ifndef singleStepReactingMixture_HPP
#define singleStepReactingMixture_HPP

#include "chemistryReader.hpp"
#include "reactingMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class singleStepReactingMixture Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class singleStepReactingMixture
:
    public reactingMixture<ThermoType>
{

protected:

    //- Stoichiometric air-fuel mass ratio
    dimensionedScalar stoicRatio_;

    //- Stoichiometric oxygen-fuel mass ratio
    dimensionedScalar s_;

    //- Heat of combustion [J/Kg]
    dimensionedScalar qFuel_;

    //- Stoichiometric coefficient for the reaction.
    scalarList specieStoichCoeffs_;

    //- Mass concentrations at stoichiometric mixture for fres.
    scalarList Yprod0_;

    //- List of components residual
    PtrList<volScalarField> fres_;

    //- Inert specie index
    label inertIndex_;

    //- Fuel specie index
    label fuelIndex_;

    //- List to indicate if specie is produced/consumed
    List<int> specieProd_;


    //- Calculate qFuel
    void calculateqFuel();

    //- Calculate air/fuel and oxygen/fuel ratio
    void massAndAirStoichRatios();

    //- Calculate maximum products at stoichiometric mixture
    void calculateMaxProducts();


private:

    //- Disallow default bitwise copy construct
    singleStepReactingMixture(const singleStepReactingMixture&);

    //- Disallow default bitwise assignment
    void operator=(const singleStepReactingMixture&);


public:

    //- The type of thermo package this mixture is instantiated for
    typedef ThermoType thermoType;


    //- Construct from dictionary, mesh and phase name
    singleStepReactingMixture
    (
        const dictionary&,
        const fvMesh&,
        const word&
    );


    //- Destructor
    virtual ~singleStepReactingMixture()
    {}


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "singleStepReactingMixture<" + ThermoType::typeName() + '>';
    }

    //- Calculates the residual for all components
    void fresCorrect();


    // Access functions

    //- Return the stoichiometric air-fuel mass ratio
    inline const dimensionedScalar stoicRatio() const
    {
        return stoicRatio_;
    }

    //- Return the Stoichiometric oxygen-fuel mass ratio
    inline const dimensionedScalar s() const
    {
        return s_;
    }

    //- Return the heat of combustion [J/Kg]
    inline const dimensionedScalar qFuel() const
    {
        return qFuel_;
    }

    //- Return the stoichiometric coefficient for the reaction
    inline const List<scalar>& specieStoichCoeffs() const
    {
        return specieStoichCoeffs_;
    }

    //- Return the list of components residual
    inline tmp<volScalarField> fres(const label index) const
    {
        return fres_[index];
    }

    //- Return the inert specie index
    inline label inertIndex() const
    {
        return inertIndex_;
    }

    //- Return the fuel specie index
    inline label fuelIndex() const
    {
        return fuelIndex_;
    }

    //- Return the list to indicate if specie is produced/consumed
    inline const List<int>& specieProd() const
    {
        return specieProd_;
    }

    //- Return the list of products mass concentrations
    inline const scalarList& Yprod0() const
    {
        return Yprod0_;
    }


    // I-O

    //- Read dictionary
    void read(const dictionary&);

};


} // End namespace CML


#include "fvMesh.hpp"

// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class ThermoType>
void CML::singleStepReactingMixture<ThermoType>::calculateqFuel()
{
    const Reaction<ThermoType>& reaction = this->operator[](0);
    const  scalar Wu = this->speciesData()[fuelIndex_].W();

    forAll(reaction.lhs(), i)
    {
        const label speciei = reaction.lhs()[i].index;
        const scalar stoichCoeff = reaction.lhs()[i].stoichCoeff;
        specieStoichCoeffs_[speciei] = -stoichCoeff;
        qFuel_.value() += this->speciesData()[speciei].hc()*stoichCoeff/Wu;
    }

    forAll(reaction.rhs(), i)
    {
        const label speciei = reaction.rhs()[i].index;
        const scalar stoichCoeff = reaction.rhs()[i].stoichCoeff;
        specieStoichCoeffs_[speciei] = stoichCoeff;
        qFuel_.value() -= this->speciesData()[speciei].hc()*stoichCoeff/Wu;
        specieProd_[speciei] = -1;
    }

    Info << "Fuel heat of combustion :" << qFuel_.value() << endl;
}


template<class ThermoType>
void CML::singleStepReactingMixture<ThermoType>::massAndAirStoichRatios()
{
    const label O2Index = this->species()["O2"];
    const scalar Wu = this->speciesData()[fuelIndex_].W();

    stoicRatio_ =
       (this->speciesData()[inertIndex_].W()
      * specieStoichCoeffs_[inertIndex_]
      + this->speciesData()[O2Index].W()
      * mag(specieStoichCoeffs_[O2Index]))
      / (Wu*mag(specieStoichCoeffs_[fuelIndex_]));

    s_ =
        (this->speciesData()[O2Index].W()
      * mag(specieStoichCoeffs_[O2Index]))
      / (Wu*mag(specieStoichCoeffs_[fuelIndex_]));

    Info << "stoichiometric air-fuel ratio :" << stoicRatio_.value() << endl;

    Info << "stoichiometric oxygen-fuel ratio :" << s_.value() << endl;
}


template<class ThermoType>
void CML::singleStepReactingMixture<ThermoType>::calculateMaxProducts()
{
    const Reaction<ThermoType>& reaction = this->operator[](0);

    scalar Wm = 0.0;
    scalar totalMol = 0.0;
    forAll(reaction.rhs(), i)
    {
        label speciei = reaction.rhs()[i].index;
        totalMol += mag(specieStoichCoeffs_[speciei]);
    }

    scalarList Xi(reaction.rhs().size());

    forAll(reaction.rhs(), i)
    {
        const label speciei = reaction.rhs()[i].index;
        Xi[i] = mag(specieStoichCoeffs_[speciei])/totalMol;

        Wm += Xi[i]*this->speciesData()[speciei].W();
    }

    forAll(reaction.rhs(), i)
    {
        const label speciei = reaction.rhs()[i].index;
        Yprod0_[speciei] =  this->speciesData()[speciei].W()/Wm*Xi[i];
    }

    Info << "Maximum products mass concentrations:" << nl;
    forAll(Yprod0_, i)
    {
        if (Yprod0_[i] > 0)
        {
            Info<< "    " << this->species()[i] << ": " << Yprod0_[i] << nl;
        }
    }

    // Normalize the stoichiometric coeff to mass
    forAll(specieStoichCoeffs_, i)
    {
        specieStoichCoeffs_[i] =
            specieStoichCoeffs_[i]
          * this->speciesData()[i].W()
          / (this->speciesData()[fuelIndex_].W()
          * mag(specieStoichCoeffs_[fuelIndex_]));
    }
}


template<class ThermoType>
void CML::singleStepReactingMixture<ThermoType>::fresCorrect()
{
    const Reaction<ThermoType>& reaction = this->operator[](0);

    label O2Index = this->species()["O2"];
    const volScalarField& YFuel = this->Y()[fuelIndex_];
    const volScalarField& YO2 = this->Y()[O2Index];

    // reactants
    forAll(reaction.lhs(), i)
    {
        const label speciei = reaction.lhs()[i].index;
        if (speciei == fuelIndex_)
        {
            fres_[speciei] =  max(YFuel - YO2/s_, scalar(0));
        }
        else if (speciei == O2Index)
        {
            fres_[speciei] =  max(YO2 - YFuel*s_, scalar(0));
        }
    }


    // products
    forAll(reaction.rhs(), i)
    {
        const label speciei = reaction.rhs()[i].index;
        if (speciei != inertIndex_)
        {
            forAll(fres_[speciei], celli)
            {
                if (fres_[fuelIndex_][celli] > 0.0)
                {
                    // rich mixture
                    fres_[speciei][celli] =
                        Yprod0_[speciei]
                      * (1.0 + YO2[celli]/s_.value() - YFuel[celli]);
                }
                else
                {
                    // lean mixture
                    fres_[speciei][celli] =
                        Yprod0_[speciei]
                      * (
                            1.0
                          - YO2[celli]/s_.value()*stoicRatio_.value()
                          + YFuel[celli]*stoicRatio_.value()
                        );
                }
            }
        }
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
CML::singleStepReactingMixture<ThermoType>::singleStepReactingMixture
(
    const dictionary& thermoDict,
    const fvMesh& mesh,
    const word& phaseName
)
:
    reactingMixture<ThermoType>(thermoDict, mesh, phaseName),
    stoicRatio_(dimensionedScalar("stoicRatio", dimless, 0.0)),
    s_(dimensionedScalar("s", dimless, 0.0)),
    qFuel_(dimensionedScalar("qFuel", sqr(dimVelocity), 0.0)),
    specieStoichCoeffs_(this->species_.size(), 0.0),
    Yprod0_(this->species_.size(), 0.0),
    fres_(Yprod0_.size()),
    inertIndex_(this->species()[thermoDict.lookup("inertSpecie")]),
    fuelIndex_(this->species()[thermoDict.lookup("fuel")]),
    specieProd_(Yprod0_.size(), 1)
{
    if (this->size() == 1)
    {
        forAll(fres_, fresI)
        {
            IOobject header
            (
                "fres_" + this->species()[fresI],
                mesh.time().timeName(),
                mesh,
                IOobject::NO_READ,
                IOobject::NO_WRITE
            );

            fres_.set
            (
                fresI,
                new volScalarField
                (
                    header,
                    mesh,
                    dimensionedScalar("fres" + name(fresI), dimless, 0.0)
                )
            );
        }

        calculateqFuel();

        massAndAirStoichRatios();

        calculateMaxProducts();

        autoPtr<chemistryReader<ThermoType> >::clear();
    }
    else
    {
        FatalErrorInFunction
            << "Only one reaction required for single step reaction"
            << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
void CML::singleStepReactingMixture<ThermoType>::read
(
    const dictionary& thermoDict
)
{}


#endif
