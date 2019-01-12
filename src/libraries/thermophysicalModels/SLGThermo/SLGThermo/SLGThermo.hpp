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
    CML::SLGThermo

Description
    Thermo package for (S)olids (L)iquids and (G)ases
    Takes reference to thermo package, and provides:
    - carrier : components of thermo - access to elemental properties
    - liquids : liquid components - access  to elemental properties
    - solids  : solid components - access  to elemental properties

    If thermo is not a multi-component thermo package, carrier is nullptr.
    Similarly, if no liquids or solids are specified, their respective
    pointers will also be nullptr.

    Registered to the mesh so that it can be looked-up

SourceFiles
    SLGThermo.cpp

\*---------------------------------------------------------------------------*/

#ifndef SLGThermo_HPP
#define SLGThermo_HPP

#include "regIOobject.hpp"
#include "fluidThermo.hpp"
#include "basicSpecieMixture.hpp"
#include "liquidMixtureProperties.hpp"
#include "solidMixtureProperties.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class SLGThermo Declaration
\*---------------------------------------------------------------------------*/

class SLGThermo
:
    public regIOobject
{

    //- Thermo package
    fluidThermo& thermo_;

    //- Reference to the multi-component carrier phase thermo
    basicSpecieMixture* carrier_;

    //- Additional liquid properties data
    autoPtr<liquidMixtureProperties> liquids_;

    //- Additional solid properties data
    autoPtr<solidMixtureProperties> solids_;


public:

    //- Runtime type information
    TypeName("SLGThermo");


    //- Construct from mesh
    SLGThermo(const fvMesh& mesh, fluidThermo& thermo);


    //- Destructor
    virtual ~SLGThermo()
    {}


    // Member Functions

    //- Return reference to the thermo database
    const fluidThermo& thermo() const;

    //- Return reference to the gaseous components
    const basicSpecieMixture& carrier() const;

    //- Return reference to the global (additional) liquids
    const liquidMixtureProperties& liquids() const;

    //- Return reference to the global (additional) solids
    const solidMixtureProperties& solids() const;


    // Index retrieval

    //- Index of carrier component
    label carrierId
    (
        const word& cmptName,
        bool allowNotFound = false
    ) const;

    //- Index of liquid component
    label liquidId
    (
        const word& cmptName,
        bool allowNotFound = false
    ) const;

    //- Index of solid component
    label solidId
    (
        const word& cmptName,
        bool allowNotFound = false
    ) const;


    // Checks

    //- Thermo database has multi-component carrier flag
    bool hasMultiComponentCarrier() const;

    //- Thermo database has liquid components flag
    bool hasLiquids() const;

    //- Thermo database has solid components flag
    bool hasSolids() const;

    // IO

    bool writeData(CML::Ostream&) const
    {
        return true;
    }

};


} // End namespace CML


#endif
