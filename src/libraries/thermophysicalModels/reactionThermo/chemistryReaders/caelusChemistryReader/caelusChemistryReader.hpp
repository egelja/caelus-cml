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
    CML::caelusChemistryReader

Description
    Chemistry reader for CAELUS format

SourceFiles
    caelusChemistryReader.cpp

\*---------------------------------------------------------------------------*/

#ifndef caelusChemistryReader_HPP
#define caelusChemistryReader_HPP

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

    //- List of elements
    DynamicList<word> elementNames_;

    //- Element indices
    HashTable<label> elementIndices_;

    //- Table of species
    speciesTable& speciesTable_;

    //- Table of species composition
    speciesCompositionTable speciesComposition_;

    //- Table of the thermodynamic data given in the foamChemistry file
    HashPtrTable<ThermoType> speciesThermo_;

    //- List of the reactions
    ReactionList<ThermoType> reactions_;


    //- Set the species list
    speciesTable& setSpecies(const dictionary& dict, speciesTable& species);

    //- Read the species composition
    void readSpeciesComposition();

    //- Disallow default bitwise copy construct
    caelusChemistryReader(const caelusChemistryReader&);

    //- Disallow default bitwise assignment
    void operator=(const caelusChemistryReader&);


public:

    //- Runtime type information
    TypeName("caelusChemistryReader");


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

    //- List of elements
    const wordList& elementNames() const
    {
        return elementNames_;
    }

    //- Element indices
    const HashTable<label>& elementIndices() const
    {
        return elementIndices_;
    }

    //- Table of species
    const speciesTable& species() const
    {
        return speciesTable_;
    }

    //- Table of species composition
    const speciesCompositionTable& specieComposition() const
    {
        return speciesComposition_;
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


template<class ThermoType>
void CML::caelusChemistryReader<ThermoType>::readSpeciesComposition()
{
    if (!chemDict_.found("elements"))
    {
        Info<< "    elements not defined in " << chemDict_.name() << endl;
        return;
    }

    wordList e(chemDict_.lookup("elements"));
    label currentElementIndex(0);

    DynamicList<word> elementNames_;
    HashTable<label> elementIndices_;

    forAll(e, ei)
    {
        if (!elementIndices_.found(e[ei]))
        {
            elementIndices_.insert(e[ei], currentElementIndex++);
            elementNames_.append(e[ei]);
        }
        else
        {
            IOWarningInFunction(chemDict_)
                << "element " << e[ei] << " already in table." << endl;
        }
    }

    // Loop through all species in thermoDict to retrieve
    // the species composition
    forAll(speciesTable_, si)
    {
        if (thermoDict_.subDict(speciesTable_[si]).isDict("elements"))
        {
            dictionary currentElements
            (
                thermoDict_.subDict(speciesTable_[si]).subDict("elements")
            );

            wordList currentElementsName(currentElements.toc());
            List<specieElement> currentComposition(currentElementsName.size());

            forAll(currentElementsName, eni)
            {
                currentComposition[eni].name() = currentElementsName[eni];

                currentComposition[eni].nAtoms() =
                    currentElements.lookupOrDefault
                    (
                        currentElementsName[eni],
                        0
                    );
            }

            // Add current specie composition to the hash table
            speciesCompositionTable::iterator specieCompositionIter
            (
                speciesComposition_.find(speciesTable_[si])
            );

            if (specieCompositionIter != speciesComposition_.end())
            {
                speciesComposition_.erase(specieCompositionIter);
            }

            speciesComposition_.insert(speciesTable_[si], currentComposition);
        }
        else
        {
            FatalIOErrorInFunction(thermoDict_)
                << "Specie " << speciesTable_[si]
                << " does not contain element description."
                << exit(FatalIOError);
        }
    }
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
{
    readSpeciesComposition();
}


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
    speciesTable_(setSpecies(chemDict_, species)),
    speciesThermo_(thermoDict_),
    reactions_(speciesTable_, speciesThermo_, chemDict_)
{
    readSpeciesComposition();
}


#endif
