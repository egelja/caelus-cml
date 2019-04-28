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
    CML::chemistryReader

Description
    Abstract class for reading chemistry

SourceFiles
    chemistryReader.cpp

\*---------------------------------------------------------------------------*/

#ifndef chemistryReader_HPP
#define chemistryReader_HPP

#include "typeInfo.hpp"
#include "specieElement.hpp"
#include "Reaction.hpp"
#include "ReactionList.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

typedef HashTable<List<specieElement> > speciesCompositionTable;


/*---------------------------------------------------------------------------*\
                      Class chemistryReader Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class chemistryReader
{

    //- Disallow default bitwise copy construct
    chemistryReader(const chemistryReader&);

    //- Disallow default bitwise assignment
    void operator=(const chemistryReader&);


public:

    //- Runtime type information
    TypeName("chemistryReader");

    //- The type of thermo package the reader was instantiated for
    typedef ThermoType thermoType;


    //- Construct null
    chemistryReader()
    {}


    // Declare run-time constructor selection table
    declareRunTimeSelectionTable
    (
        autoPtr,
        chemistryReader,
        dictionary,
        (
            const dictionary& thermoDict,
            speciesTable& species
        ),
        (thermoDict, species)
    );


    //- Select constructed from dictionary
    static autoPtr<chemistryReader> New
    (
        const dictionary& thermoDict,
        speciesTable& species
    );


    //- Destructor
    virtual ~chemistryReader()
    {}


    // Member Functions

    //- Return access to the list of species
    virtual const speciesTable& species() const = 0;

    //- Table of species composition
    virtual const speciesCompositionTable& specieComposition() const = 0;

    //- Return access to the thermo packages
    virtual const HashPtrTable<ThermoType>& speciesThermo() const = 0;

    //- Return access to the list of reactions
    virtual const ReactionList<ThermoType>& reactions() const = 0;

};


} // End namespace CML


template<class ThermoType>
CML::autoPtr<CML::chemistryReader<ThermoType> >
CML::chemistryReader<ThermoType>::New
(
    const dictionary& thermoDict,
    speciesTable& species
)
{
    // Let the chemistry reader type default to CHEMKIN
    // for backward compatibility
    word chemistryReaderTypeName("chemkinReader");

    // otherwise use the specified reader
    thermoDict.readIfPresent("chemistryReader", chemistryReaderTypeName);

    Info<< "Selecting chemistryReader " << chemistryReaderTypeName << endl;

    typename dictionaryConstructorTable::iterator cstrIter =
        dictionaryConstructorTablePtr_->find(chemistryReaderTypeName);

    if (cstrIter == dictionaryConstructorTablePtr_->end())
    {
        FatalErrorInFunction
            << "Unknown chemistryReader type "
            << chemistryReaderTypeName << nl << nl
            << "Valid chemistryReader types are:" << nl
            << dictionaryConstructorTablePtr_->sortedToc()
            << exit(FatalError);
    }

    return autoPtr<chemistryReader<ThermoType> >
    (
        cstrIter()(thermoDict, species)
    );
}


#define makeChemistryReader(Thermo)                                            \
    defineTemplateTypeNameAndDebug(chemistryReader<Thermo>, 0);                \
    defineTemplateRunTimeSelectionTable(chemistryReader<Thermo>, dictionary)


#define makeChemistryReaderType(Reader, Thermo)                                \
    defineNamedTemplateTypeNameAndDebug(Reader<Thermo>, 0);                    \
    chemistryReader<Thermo>::adddictionaryConstructorToTable<Reader<Thermo> >  \
        add##Reader##Thermo##ConstructorToTable_


// For non-templated chemistry readers
#define addChemistryReaderType(Reader, Thermo)                                 \
    defineTypeNameAndDebug(Reader, 0);                                         \
    chemistryReader<Thermo>::adddictionaryConstructorToTable<Reader>           \
        add##Reader##Thermo##ConstructorToTable_


// For templated chemistry readers
#define addTemplateChemistryReaderType(Reader, Thermo)                         \
    defineNamedTemplateTypeNameAndDebug(Reader, 0);                            \
    chemistryReader<Thermo>::adddictionaryConstructorToTable<Reader>           \
        add##Reader##Thermo##ConstructorToTable_


#endif
