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
    CML::homogeneousMixture

Description
    CML::homogeneousMixture

SourceFiles
    homogeneousMixture.cpp

\*---------------------------------------------------------------------------*/

#ifndef homogeneousMixture_HPP
#define homogeneousMixture_HPP

#include "basicCombustionMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class homogeneousMixture Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class homogeneousMixture
:
    public basicCombustionMixture
{

    static const int nSpecies_ = 1;
    static const char* specieNames_[1];

    ThermoType reactants_;
    ThermoType products_;

    mutable ThermoType mixture_;

    //- Construct as copy (not implemented)
    homogeneousMixture(const homogeneousMixture<ThermoType>&);

    //- Regress variable
    volScalarField& b_;


public:

    //- The type of thermodynamics this mixture is instantiated for
    typedef ThermoType thermoType;


    //- Construct from dictionary, mesh and phase name
    homogeneousMixture(const dictionary&, const fvMesh&, const word&);


    //- Destructor
    virtual ~homogeneousMixture()
    {}


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "homogeneousMixture<" + ThermoType::typeName() + '>';
    }

    const ThermoType& mixture(const scalar) const;

    const ThermoType& cellMixture(const label celli) const
    {
        return mixture(b_[celli]);
    }

    const ThermoType& patchFaceMixture
    (
        const label patchi,
        const label facei
    ) const
    {
        return mixture(b_.boundaryField()[patchi][facei]);
    }

    const ThermoType& cellReactants(const label) const
    {
        return reactants_;
    }

    const ThermoType& patchFaceReactants(const label, const label) const
    {
        return reactants_;
    }

    const ThermoType& cellProducts(const label) const
    {
        return products_;
    }

    const ThermoType& patchFaceProducts(const label, const label) const
    {
        return products_;
    }

    //- Read dictionary
    void read(const dictionary&);

    //- Return thermo based on index
    const ThermoType& getLocalThermo(const label speciei) const;

};


} // End namespace CML

#include "fvMesh.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

template<class ThermoType>
const char* CML::homogeneousMixture<ThermoType>::specieNames_[1] = {"b"};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
CML::homogeneousMixture<ThermoType>::homogeneousMixture
(
    const dictionary& thermoDict,
    const fvMesh& mesh,
    const word& phaseName
)
:
    basicCombustionMixture
    (
        thermoDict,
        speciesTable(nSpecies_, specieNames_),
        mesh,
        phaseName
    ),

    reactants_(thermoDict.subDict("reactants")),
    products_(thermoDict.subDict("products")),
    mixture_("mixture", reactants_),
    b_(Y("b"))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
const ThermoType& CML::homogeneousMixture<ThermoType>::mixture
(
    const scalar b
) const
{
    if (b > 0.999)
    {
        return reactants_;
    }
    else if (b < 0.001)
    {
        return products_;
    }
    else
    {
        mixture_ = b*reactants_;
        mixture_ += (1 - b)*products_;

        return mixture_;
    }
}


template<class ThermoType>
void CML::homogeneousMixture<ThermoType>::read(const dictionary& thermoDict)
{
    reactants_ = ThermoType(thermoDict.subDict("reactants"));
    products_ = ThermoType(thermoDict.subDict("products"));
}


template<class ThermoType>
const ThermoType& CML::homogeneousMixture<ThermoType>::getLocalThermo
(
    const label speciei
) const
{
    if (speciei == 0)
    {
        return reactants_;
    }
    else if (speciei == 1)
    {
        return products_;
    }
    else
    {
        FatalErrorInFunction
            << "Unknown specie index " << speciei << ". Valid indices are 0..1"
            << abort(FatalError);

        return reactants_;
    }
}


#endif
