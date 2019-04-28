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
    CML::inhomogeneousMixture


\*---------------------------------------------------------------------------*/

#ifndef inhomogeneousMixture_HPP
#define inhomogeneousMixture_HPP

#include "basicCombustionMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class inhomogeneousMixture Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class inhomogeneousMixture
:
    public basicCombustionMixture
{

    static const int nSpecies_ = 2;
    static const char* specieNames_[2];

    dimensionedScalar stoicRatio_;

    ThermoType fuel_;
    ThermoType oxidant_;
    ThermoType products_;

    mutable ThermoType mixture_;

    //- Mixture fraction
    volScalarField& ft_;

    //- Regress variable
    volScalarField& b_;

    //- Construct as copy (not implemented)
    inhomogeneousMixture(const inhomogeneousMixture<ThermoType>&);


public:

    //- The type of thermodynamics this mixture is instantiated for
    typedef ThermoType thermoType;


    //- Construct from dictionary, mesh and phase name
    inhomogeneousMixture(const dictionary&, const fvMesh&, const word&);


    //- Destructor
    virtual ~inhomogeneousMixture()
    {}


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "inhomogeneousMixture<" + ThermoType::typeName() + '>';
    }

    const dimensionedScalar& stoicRatio() const
    {
        return stoicRatio_;
    }

    const ThermoType& mixture(const scalar, const scalar) const;

    const ThermoType& cellMixture(const label celli) const
    {
        return mixture(ft_[celli], b_[celli]);
    }

    const ThermoType& patchFaceMixture
    (
        const label patchi,
        const label facei
    ) const
    {
        return mixture
        (
            ft_.boundaryField()[patchi][facei],
            b_.boundaryField()[patchi][facei]
        );
    }

    const ThermoType& cellReactants(const label celli) const
    {
        return mixture(ft_[celli], 1);
    }

    const ThermoType& patchFaceReactants
    (
        const label patchi,
        const label facei
    ) const
    {
        return mixture
        (
            ft_.boundaryField()[patchi][facei],
            1
        );
    }

    const ThermoType& cellProducts(const label celli) const
    {
        return mixture(ft_[celli], 0);
    }

    const ThermoType& patchFaceProducts
    (
        const label patchi,
        const label facei
    ) const
    {
        return mixture
        (
            ft_.boundaryField()[patchi][facei],
            0
        );
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
const char* CML::inhomogeneousMixture<ThermoType>::specieNames_[2] =
{
    "ft",
    "b"
};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
CML::inhomogeneousMixture<ThermoType>::inhomogeneousMixture
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

    stoicRatio_(thermoDict.lookup("stoichiometricAirFuelMassRatio")),

    fuel_(thermoDict.subDict("fuel")),
    oxidant_(thermoDict.subDict("oxidant")),
    products_(thermoDict.subDict("burntProducts")),

    mixture_("mixture", fuel_),

    ft_(Y("ft")),
    b_(Y("b"))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
const ThermoType& CML::inhomogeneousMixture<ThermoType>::mixture
(
    const scalar ft,
    const scalar b
) const
{
    if (ft < 0.0001)
    {
        return oxidant_;
    }
    else
    {
        scalar fu = b*ft + (1.0 - b)*fres(ft, stoicRatio().value());
        scalar ox = 1 - ft - (ft - fu)*stoicRatio().value();
        scalar pr = 1 - fu - ox;

        mixture_ = fu*fuel_;
        mixture_ += ox*oxidant_;
        mixture_ += pr*products_;

        return mixture_;
    }
}


template<class ThermoType>
void CML::inhomogeneousMixture<ThermoType>::read(const dictionary& thermoDict)
{
    stoicRatio_ = thermoDict.lookup("stoichiometricAirFuelMassRatio");

    fuel_ = ThermoType(thermoDict.subDict("fuel"));
    oxidant_ = ThermoType(thermoDict.subDict("oxidant"));
    products_ = ThermoType(thermoDict.subDict("burntProducts"));
}


template<class ThermoType>
const ThermoType& CML::inhomogeneousMixture<ThermoType>::getLocalThermo
(
    const label speciei
) const
{
    if (speciei == 0)
    {
        return fuel_;
    }
    else if (speciei == 1)
    {
        return oxidant_;
    }
    else if (speciei == 2)
    {
        return products_;
    }
    else
    {
        FatalErrorInFunction
            << "Unknown specie index " << speciei << ". Valid indices are 0..2"
            << abort(FatalError);

        return fuel_;
    }
}


#endif
