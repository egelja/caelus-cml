/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::multiComponentMixture

Description
    CML::multiComponentMixture

SourceFiles
    multiComponentMixture.cpp

\*---------------------------------------------------------------------------*/

#ifndef multiComponentMixture_H
#define multiComponentMixture_H

#include "basicMultiComponentMixture.hpp"
#include "HashPtrTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class multiComponentMixture Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class multiComponentMixture
:
    public basicMultiComponentMixture
{
    // Private data

        //- Species data
        PtrList<ThermoType> speciesData_;

        //- Temporary storage for the cell/face mixture thermo data
        mutable ThermoType mixture_;


    // Private Member Functions

        //- Construct the species data from the given dictionary and return the
        //  data for the first specie to initialise the mixture thermo data
        const ThermoType& constructSpeciesData(const dictionary& thermoDict);

        //- Correct the mass fractions to sum to 1
        void correctMassFractions();

        //- Construct as copy (not implemented)
        multiComponentMixture(const multiComponentMixture<ThermoType>&);


public:

    //- The type of thermodynamics this mixture is instantiated for
    typedef ThermoType thermoType;


    // Constructors

        //- Construct from dictionary, specie names, thermo database and mesh
        multiComponentMixture
        (
            const dictionary&,
            const wordList& specieNames,
            const HashPtrTable<ThermoType>& specieThermoData,
            const fvMesh&
        );

        //- Construct from dictionary and mesh
        multiComponentMixture(const dictionary&, const fvMesh&);


    //- Destructor
    virtual ~multiComponentMixture()
    {}


    // Member functions

        const ThermoType& cellMixture(const label celli) const;

        const ThermoType& patchFaceMixture
        (
            const label patchi,
            const label facei
        ) const;

        //- Return the raw specie thermodynamic data
        const PtrList<ThermoType>& speciesData() const
        {
            return speciesData_;
        }

        //- Read dictionary
        void read(const dictionary&);


        // Per specie properties

            //- Number of moles []
            virtual scalar nMoles(const label specieI) const;

            //- Molecular weight [kg/kmol]
            virtual scalar W(const label specieI) const;


        // Per specie thermo properties

            //- Heat capacity at constant pressure [J/(kg K)]
            virtual scalar Cp(const label specieI, const scalar T) const;

            //- Heat capacity at constant volume [J/(kg K)]
            virtual scalar Cv(const label specieI, const scalar T) const;

            //- Enthalpy [J/kg]
            virtual scalar H(const label specieI, const scalar T) const;

            //- Sensible enthalpy [J/kg]
            virtual scalar Hs(const label specieI, const scalar T) const;

            //- Chemical enthalpy [J/kg]
            virtual scalar Hc(const label specieI) const;

            //- Entropy [J/(kg K)]
            virtual scalar S(const label specieI, const scalar T) const;

            //- Internal energy [J/kg]
            virtual scalar E(const label specieI, const scalar T) const;

            //- Gibbs free energy [J/kg]
            virtual scalar G(const label specieI, const scalar T) const;

            //- Helmholtz free energy [J/kg]
            virtual scalar A(const label specieI, const scalar T) const;


        // Per specie transport properties

            //- Dynamic viscosity [kg/m/s]
            virtual scalar mu(const label specieI, const scalar T) const;

            //- Thermal conductivity [W/m/K]
            virtual scalar kappa(const label specieI, const scalar T) const;

            //- Thermal diffusivity [kg/m/s]
            virtual scalar alpha(const label specieI, const scalar T) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class ThermoType>
const ThermoType& CML::multiComponentMixture<ThermoType>::constructSpeciesData
(
    const dictionary& thermoDict
)
{
    forAll(species_, i)
    {
        speciesData_.set
        (
            i,
            new ThermoType(thermoDict.subDict(species_[i]))
        );
    }

    return speciesData_[0];
}


template<class ThermoType>
void CML::multiComponentMixture<ThermoType>::correctMassFractions()
{
    // It changes Yt patches to "calculated"
    volScalarField Yt("Yt", 1.0*Y_[0]);

    for (label n=1; n<Y_.size(); n++)
    {
        Yt += Y_[n];
    }

    forAll(Y_, n)
    {
        Y_[n] /= Yt;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
CML::multiComponentMixture<ThermoType>::multiComponentMixture
(
    const dictionary& thermoDict,
    const wordList& specieNames,
    const HashPtrTable<ThermoType>& specieThermoData,
    const fvMesh& mesh
)
:
    basicMultiComponentMixture(thermoDict, specieNames, mesh),
    speciesData_(species_.size()),
    mixture_("mixture", *specieThermoData[specieNames[0]])
{
    forAll(species_, i)
    {
        speciesData_.set
        (
            i,
            new ThermoType(*specieThermoData[species_[i]])
        );
    }

    correctMassFractions();
}


template<class ThermoType>
CML::multiComponentMixture<ThermoType>::multiComponentMixture
(
    const dictionary& thermoDict,
    const fvMesh& mesh
)
:
    basicMultiComponentMixture(thermoDict, thermoDict.lookup("species"), mesh),
    speciesData_(species_.size()),
    mixture_("mixture", constructSpeciesData(thermoDict))
{
    correctMassFractions();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
const ThermoType& CML::multiComponentMixture<ThermoType>::cellMixture
(
    const label celli
) const
{
    mixture_ = Y_[0][celli]/speciesData_[0].W()*speciesData_[0];

    for (label n=1; n<Y_.size(); n++)
    {
        mixture_ += Y_[n][celli]/speciesData_[n].W()*speciesData_[n];
    }

    return mixture_;
}


template<class ThermoType>
const ThermoType& CML::multiComponentMixture<ThermoType>::patchFaceMixture
(
    const label patchi,
    const label facei
) const
{
    mixture_ =
        Y_[0].boundaryField()[patchi][facei]
       /speciesData_[0].W()*speciesData_[0];

    for (label n=1; n<Y_.size(); n++)
    {
        mixture_ +=
            Y_[n].boundaryField()[patchi][facei]
           /speciesData_[n].W()*speciesData_[n];
    }

    return mixture_;
}


template<class ThermoType>
void CML::multiComponentMixture<ThermoType>::read
(
    const dictionary& thermoDict
)
{
    forAll(species_, i)
    {
        speciesData_[i] = ThermoType(thermoDict.subDict(species_[i]));
    }
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::nMoles
(
    const label specieI
) const
{
    return speciesData_[specieI].nMoles();
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::W
(
    const label specieI
) const
{
    return speciesData_[specieI].W();
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::Cp
(
    const label specieI,
    const scalar T
) const
{
    return speciesData_[specieI].Cp(T);
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::Cv
(
    const label specieI,
    const scalar T
) const
{
    return speciesData_[specieI].Cv(T);
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::H
(
    const label specieI,
    const scalar T
) const
{
    return speciesData_[specieI].H(T);
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::Hs
(
    const label specieI,
    const scalar T
) const
{
    return speciesData_[specieI].Hs(T);
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::Hc
(
    const label specieI
) const
{
    return speciesData_[specieI].Hc();
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::S
(
    const label specieI,
    const scalar T
) const
{
    return speciesData_[specieI].S(T);
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::E
(
    const label specieI,
    const scalar T
) const
{
    return speciesData_[specieI].E(T);
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::G
(
    const label specieI,
    const scalar T
) const
{
    return speciesData_[specieI].G(T);
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::A
(
    const label specieI,
    const scalar T
) const
{
    return speciesData_[specieI].A(T);
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::mu
(
    const label specieI,
    const scalar T
) const
{
    return speciesData_[specieI].mu(T);
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::kappa
(
    const label specieI,
    const scalar T
) const
{
    return speciesData_[specieI].kappa(T);
}


template<class ThermoType>
CML::scalar CML::multiComponentMixture<ThermoType>::alpha
(
    const label specieI,
    const scalar T
) const
{
    return speciesData_[specieI].alpha(T);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
