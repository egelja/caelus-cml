/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
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
    CML::singleComponentMixture

Description
    Single component mixture

SourceFiles
    singleComponentMixture.cpp

\*---------------------------------------------------------------------------*/

#ifndef singleComponentMixture_HPP
#define singleComponentMixture_HPP

#include "basicSpecieMixture.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class singleComponentMixture Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class singleComponentMixture
:
    public basicSpecieMixture
{

    //- Thermo model
    ThermoType thermo_;


public:

    //- The type of thermodynamics this mixture is instantiated for
    typedef ThermoType thermoType;


    //- Construct from dictionary, mesh and phase name
    singleComponentMixture(const dictionary&, const fvMesh&, const word&);


    //- Destructor
    virtual ~singleComponentMixture();


    // Member Functions

    //- Return the instantiated type name
    //  Note: This defines the same name as pureMixture so that
    //  non-reacting and reacting solvers are compatible
    static word typeName()
    {
        return "pureMixture<" + ThermoType::typeName() + '>';
    }

    //- Get the mixture for the given cell
    const ThermoType& cellMixture(const label celli) const
    {
        return thermo_;
    }

    //- Get the mixture for the given patch face
    const ThermoType& patchFaceMixture
    (
        const label patchi,
        const label facei
    ) const
    {
        return thermo_;
    }

    //- Get the volumetric mixture for the given cell
    const ThermoType& cellVolMixture
    (
        const scalar p,
        const scalar T,
        const label celli
    )
    {
        return thermo_;
    }

    //- Get the volumetric mixture for the given patch face
    const ThermoType& patchFaceVolMixture
    (
        const scalar p,
        const scalar T,
        const label patchi,
        const label facei
    ) const
    {
        return thermo_;
    }

    //- Read dictionary
    void read(const dictionary&);

    //- Return thermo based on index
    inline const ThermoType& getLocalThermo(const label speciei) const
    {
        #ifdef FULLDEBUG
        if (speciei != 0)
        {
            FatalErrorInFunction
                << "Specie index must be zero for a single component "
                << "mixture" << exit(FatalError);
        }
        #endif
        return thermo_;
    }

};


} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
CML::singleComponentMixture<ThermoType>::singleComponentMixture
(
    const dictionary& thermoDict,
    const fvMesh& mesh,
    const word& phaseName
)
:
    basicSpecieMixture(thermoDict, wordList(), mesh, phaseName),
    thermo_(thermoDict.subDict("mixture"))
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ThermoType>
CML::singleComponentMixture<ThermoType>::~singleComponentMixture()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class ThermoType>
void CML::singleComponentMixture<ThermoType>::read
(
    const dictionary& thermoDict
)
{
    thermo_ = ThermoType(thermoDict.subDict("mixture"));
}


#endif
