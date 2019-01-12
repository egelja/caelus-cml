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
    CML::multiComponentMixture

Description
    CML::multiComponentMixture


\*---------------------------------------------------------------------------*/

#ifndef multiComponentMixture_HPP
#define multiComponentMixture_HPP

#include "basicSpecieMixture.hpp"
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
    public basicSpecieMixture
{

    //- Species data
    PtrList<ThermoType> speciesData_;

    //- Temporary storage for the cell/face mixture thermo data
    mutable ThermoType mixture_;

    //- Temporary storage for the volume weighted
    //  cell/face mixture thermo data
    mutable ThermoType mixtureVol_;


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


    //- Construct from dictionary, specie names, thermo database,
    //  mesh and phase name
    multiComponentMixture
    (
        const dictionary&,
        const wordList& specieNames,
        const HashPtrTable<ThermoType>& thermoData,
        const fvMesh&,
        const word&
    );

    //- Construct from dictionary, mesh and phase name
    multiComponentMixture(const dictionary&, const fvMesh&, const word&);


    //- Destructor
    virtual ~multiComponentMixture()
    {}


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "multiComponentMixture<" + ThermoType::typeName() + '>';
    }

    const ThermoType& cellMixture(const label celli) const;

    const ThermoType& patchFaceMixture
    (
        const label patchi,
        const label facei
    ) const;

    const ThermoType& cellVolMixture
    (
        const scalar p,
        const scalar T,
        const label celli
    ) const;

    const ThermoType& patchFaceVolMixture
    (
        const scalar p,
        const scalar T,
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

    //- Return thermo based on index
    inline const ThermoType& getLocalThermo(const label speciei) const
    {
        return speciesData_[speciei];
    }

};


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
    // Multiplication by 1.0 changes Yt patches to "calculated"
    volScalarField Yt("Yt", 1.0*Y_[0]);

    for (label n=1; n<Y_.size(); n++)
    {
        Yt += Y_[n];
    }

    if (mag(max(Yt).value()) < ROOTVSMALL)
    {
        FatalErrorInFunction
            << "Sum of mass fractions is zero for species " << this->species()
            << exit(FatalError);
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
    const HashPtrTable<ThermoType>& thermoData,
    const fvMesh& mesh,
    const word& phaseName
)
:
    basicSpecieMixture(thermoDict, specieNames, mesh, phaseName),
    speciesData_(species_.size()),
    mixture_("mixture", *thermoData[specieNames[0]]),
    mixtureVol_("volMixture", *thermoData[specieNames[0]])
{
    forAll(species_, i)
    {
        speciesData_.set
        (
            i,
            new ThermoType(*thermoData[species_[i]])
        );
    }

    correctMassFractions();
}


template<class ThermoType>
CML::multiComponentMixture<ThermoType>::multiComponentMixture
(
    const dictionary& thermoDict,
    const fvMesh& mesh,
    const word& phaseName
)
:
    basicSpecieMixture
    (
        thermoDict,
        thermoDict.lookup("species"),
        mesh,
        phaseName
    ),
    speciesData_(species_.size()),
    mixture_("mixture", constructSpeciesData(thermoDict)),
    mixtureVol_("volMixture", speciesData_[0])
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
    mixture_ = Y_[0][celli]*speciesData_[0];

    for (label n=1; n<Y_.size(); n++)
    {
        mixture_ += Y_[n][celli]*speciesData_[n];
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
    mixture_ = Y_[0].boundaryField()[patchi][facei]*speciesData_[0];

    for (label n=1; n<Y_.size(); n++)
    {
        mixture_ += Y_[n].boundaryField()[patchi][facei]*speciesData_[n];
    }

    return mixture_;
}


template<class ThermoType>
const ThermoType& CML::multiComponentMixture<ThermoType>::cellVolMixture
(
    const scalar p,
    const scalar T,
    const label celli
) const
{
    scalar rhoInv = 0.0;
    forAll(speciesData_, i)
    {
        rhoInv += Y_[i][celli]/speciesData_[i].rho(p, T);
    }

    mixtureVol_ =
        Y_[0][celli]/speciesData_[0].rho(p, T)/rhoInv*speciesData_[0];

    for (label n=1; n<Y_.size(); n++)
    {
        mixtureVol_ +=
            Y_[n][celli]/speciesData_[n].rho(p, T)/rhoInv*speciesData_[n];
    }

    return mixtureVol_;
}


template<class ThermoType>
const ThermoType& CML::multiComponentMixture<ThermoType>::
patchFaceVolMixture
(
    const scalar p,
    const scalar T,
    const label patchi,
    const label facei
) const
{
    scalar rhoInv = 0.0;
    forAll(speciesData_, i)
    {
        rhoInv +=
            Y_[i].boundaryField()[patchi][facei]/speciesData_[i].rho(p, T);
    }

    mixtureVol_ =
        Y_[0].boundaryField()[patchi][facei]/speciesData_[0].rho(p, T)/rhoInv
      * speciesData_[0];

    for (label n=1; n<Y_.size(); n++)
    {
        mixtureVol_ +=
            Y_[n].boundaryField()[patchi][facei]/speciesData_[n].rho(p,T)
          / rhoInv*speciesData_[n];
    }

    return mixtureVol_;
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


#endif
