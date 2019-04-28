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
    CML::pureMixture

Description
    CML::pureMixture

SourceFiles
    pureMixture.cpp

\*---------------------------------------------------------------------------*/

#ifndef pureMixture_HPP
#define pureMixture_HPP

#include "basicMixture.hpp"
#include "fvMesh.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class pureMixture Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class pureMixture
:
    public basicMixture
{

    ThermoType mixture_;

    //- Construct as copy (not implemented)
    pureMixture(const pureMixture<ThermoType>&);


public:

    //- The type of thermodynamics this mixture is instantiated for
    typedef ThermoType thermoType;


    //- Construct from dictionary, mesh and phase name
    pureMixture(const dictionary&, const fvMesh&, const word&);


    // Member functions

    //- Return the instantiated type name
    static word typeName()
    {
        return "pureMixture<" + ThermoType::typeName() + '>';
    }

    const ThermoType& mixture() const
    {
        return mixture_;
    }

    const ThermoType& cellMixture(const label) const
    {
        return mixture_;
    }

    const ThermoType& patchFaceMixture
    (
        const label,
        const label
    ) const
    {
        return mixture_;
    }

    const ThermoType& cellVolMixture
    (
        const scalar,
        const scalar,
        const label
    ) const
    {
        return mixture_;
    }

    const ThermoType& patchFaceVolMixture
    (
        const scalar,
        const scalar,
        const label,
        const label
    ) const
    {
        return mixture_;
    }

    //- Read dictionary
    void read(const dictionary&);

};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
CML::pureMixture<ThermoType>::pureMixture
(
    const dictionary& thermoDict,
    const fvMesh& mesh,
    const word& phaseName
)
:
    basicMixture(thermoDict, mesh, phaseName),
    mixture_(thermoDict.subDict("mixture"))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
void CML::pureMixture<ThermoType>::read(const dictionary& thermoDict)
{
    mixture_ = ThermoType(thermoDict.subDict("mixture"));
}

#endif
