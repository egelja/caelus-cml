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
    CML::caelusChemistryReader

Description
    Chemistry reader for CAELUS format

SourceFiles
    caelusChemistryReader.cpp

\*---------------------------------------------------------------------------*/

#ifndef caelusChemistryReader_H
#define caelusChemistryReader_H

#include "chemistryReader.hpp"
#include "fileName.hpp"
#include "typeInfo.hpp"
#include "HashPtrTable.hpp"
#include "labelList.hpp"
#include "speciesTable.hpp"
#include "atomicWeights.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class caelusChemistry Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class caelusChemistryReader
:
    public chemistryReader<ThermoType>
{
        //- Chemistry/reactions dictionary
        dictionary chemDict_;

        //- Thermo properties dictionary
        dictionary thermoDict_;

        //- Table of the thermodynamic data given in the caelusChemistry file
        HashPtrTable<ThermoType> speciesThermo_;

        //- Table of species
        speciesTable& speciesTable_;

        //- List of the reactions
        ReactionList<ThermoType> reactions_;


    // Private Member Functions

        //- Set the species list
        speciesTable& setSpecies(const dictionary& dict, speciesTable& species);

        //- Disallow default bitwise copy construct
        caelusChemistryReader(const caelusChemistryReader&);

        //- Disallow default bitwise assignment
        void operator=(const caelusChemistryReader&);


public:

    //- Runtime type information
    TypeName("caelusChemistryReader");


    // Constructors

        //- Construct from caelusChemistry and thermodynamics file names
        caelusChemistryReader
        (
            const fileName& reactionsFileName,
            speciesTable& species,
            const fileName& thermoFileName
        );

        //- Construct by getting the caelusChemistry and thermodynamics file names
        //  from dictionary
        caelusChemistryReader
        (
            const dictionary& thermoDict,
            speciesTable& species
        );


    //- Destructor
    virtual ~caelusChemistryReader()
    {}


    // Member functions

        //- Table of species
        const speciesTable& species() const
        {
            return speciesTable_;
        }

        //- Table of the thermodynamic data given in the caelusChemistry file
        const HashPtrTable<ThermoType>& speciesThermo() const
        {
            return speciesThermo_;
        }

        //- List of the reactions
        const ReactionList<ThermoType>& reactions() const
        {
            return reactions_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

#include "IFstream.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class ThermoType>
CML::speciesTable& CML::caelusChemistryReader<ThermoType>::setSpecies
(
    const dictionary& dict,
    speciesTable& species
)
{
    wordList s(dict.lookup("species"));
    species.transfer(s);
    return species;
}


// * * * * * * * * * * * * * * * * Constructor * * * * * * * * * * * * * * * //

template<class ThermoType>
CML::caelusChemistryReader<ThermoType>::caelusChemistryReader
(
    const fileName& reactionsFileName,
    speciesTable& species,
    const fileName& thermoFileName
)
:
    chemistryReader<ThermoType>(),
    chemDict_
    (
        IFstream
        (
            fileName(reactionsFileName).expand()
        )()
    ),
    thermoDict_
    (
        IFstream
        (
            fileName(thermoFileName).expand()
        )()
    ),
    speciesTable_(setSpecies(chemDict_, species)),
    speciesThermo_(thermoDict_),
    reactions_(speciesTable_, speciesThermo_, chemDict_)
{}


template<class ThermoType>
CML::caelusChemistryReader<ThermoType>::caelusChemistryReader
(
    const dictionary& thermoDict,
    speciesTable& species
)
:
    chemistryReader<ThermoType>(),
    chemDict_
    (
        IFstream
        (
            fileName(thermoDict.lookup("caelusChemistryFile")).expand()
        )()
    ),
    thermoDict_
    (
        IFstream
        (
            fileName(thermoDict.lookup("caelusChemistryThermoFile")).expand()
        )()
    ),
    speciesThermo_(thermoDict_),
    speciesTable_(setSpecies(chemDict_, species)),
    reactions_(speciesTable_, speciesThermo_, chemDict_)
{}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
