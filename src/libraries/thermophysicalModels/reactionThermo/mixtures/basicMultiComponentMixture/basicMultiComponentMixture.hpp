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
    CML::basicMultiComponentMixture

Description
    Multi-component mixture.

    Provides a list of mass fraction fields and helper functions to
    query mixture composition.

SourceFiles
    basicMultiComponentMixture.cpp

\*---------------------------------------------------------------------------*/

#ifndef basicMultiComponentMixture_HPP
#define basicMultiComponentMixture_HPP

#include "volFields.hpp"
#include "PtrList.hpp"
#include "basicMixture.hpp"
#include "speciesTable.hpp"
#include "typeInfo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                 Class basicMultiComponentMixture Declaration
\*---------------------------------------------------------------------------*/

class basicMultiComponentMixture
:
    public basicMixture
{

protected:

    //- Table of specie names
    speciesTable species_;

    //- List of specie active flags
    List<bool> active_;

    //- Species mass fractions
    PtrList<volScalarField> Y_;


public:

    //- Run time type information
    TypeName("basicMultiComponentMixture");


    //- The base class of the mixture
    typedef basicMultiComponentMixture basicMixtureType;


    //- Construct from dictionary, species names, mesh and phase name
    basicMultiComponentMixture
    (
        const dictionary&,
        const wordList& specieNames,
        const fvMesh&,
        const word&
    );


    //- Destructor
    virtual ~basicMultiComponentMixture()
    {}


    // Member functions

    //- Return the table of species
    inline const speciesTable& species() const
    {
        return species_;
    }

    //- Does the mixture include this specie?
    inline bool contains(const word& specieName) const
    {
        return species_.contains(specieName);
    }

    //- Return true for active species
    inline bool active(label speciei) const
    {
        return active_[speciei];
    }

    //- Return the bool list of active species
    inline const List<bool>& active() const
    {
        return active_;
    }

    //- Set speciei active
    inline void setActive(label speciei)
    {
        active_[speciei] = true;
    }

    //- Set speciei inactive
    inline void setInactive(label speciei)
    {
        active_[speciei] = false;
    }

    //- Return the mass-fraction fields
    inline PtrList<volScalarField>& Y()
    {
        return Y_;
    }

    //- Return the const mass-fraction fields
    inline const PtrList<volScalarField>& Y() const
    {
        return Y_;
    }

    //- Return the mass-fraction field for a specie given by index
    inline volScalarField& Y(const label i)
    {
        return Y_[i];
    }

    //- Return the const mass-fraction field for a specie given by index
    inline const volScalarField& Y(const label i) const
    {
        return Y_[i];
    }

    //- Return the mass-fraction field for a specie given by name
    inline volScalarField& Y(const word& specieName)
    {
        return Y_[species_[specieName]];
    }

    //- Return the const mass-fraction field for a specie given by name
    inline const volScalarField& Y(const word& specieName) const
    {
        return Y_[species_[specieName]];
    }

};


} // End namespace CML


#endif
