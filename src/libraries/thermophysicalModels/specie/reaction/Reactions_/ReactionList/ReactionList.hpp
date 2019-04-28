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
    CML::ReactionList

Description
    List of templated reactions

SourceFiles
    ReactionList.cpp

\*---------------------------------------------------------------------------*/

#ifndef ReactionList_HPP
#define ReactionList_HPP

#include "PtrList.hpp"
#include "SLPtrList.hpp"
#include "speciesTable.hpp"
#include "HashPtrTable.hpp"
#include "Reaction.hpp"
#include "fileName.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class ReactionList Declaration
\*---------------------------------------------------------------------------*/

template<class ThermoType>
class ReactionList
:
    public SLPtrList<Reaction<ThermoType> >
{

    //- Reference to the table of species
    const speciesTable& species_;

    //- Reference to the thermo database
    const HashPtrTable<ThermoType>& thermoDb_;

    //- The dictionary used for construction
    const dictionary dict_;


    //- Disallow default bitwise assignment
    void operator=(const ReactionList&);


public:


    //- Construct null
    ReactionList
    (
        const speciesTable& species,
        const HashPtrTable<ThermoType>& thermoDatabase
    );

    //- Construct from dictionary
    ReactionList
    (
        const speciesTable& species,
        const HashPtrTable<ThermoType>& thermoDatabase,
        const dictionary& dict
    );

    //- Construct copy
    ReactionList(const ReactionList<ThermoType>& reactions);


    //- Destructor
    ~ReactionList();


    // Public Member Functions

    //- Read reactions from dictionary
    bool readReactionDict();

    //- Write
    void write(Ostream& os) const;

};


} // End namespace CML

#include "IFstream.hpp"
#include "SLPtrList.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ThermoType>
CML::ReactionList<ThermoType>::ReactionList
(
    const speciesTable& species,
    const HashPtrTable<ThermoType>& thermoDb
)
:
    SLPtrList<Reaction<ThermoType> >(),
    species_(species),
    thermoDb_(thermoDb),
    dict_(dictionary::null)
{}


template<class ThermoType>
CML::ReactionList<ThermoType>::ReactionList
(
    const speciesTable& species,
    const HashPtrTable<ThermoType>& thermoDb,
    const dictionary& dict
)
:
    SLPtrList<Reaction<ThermoType> >(),
    species_(species),
    thermoDb_(thermoDb),
    dict_(dict)
{
    readReactionDict();
}


template<class ThermoType>
CML::ReactionList<ThermoType>::ReactionList(const ReactionList& reactions)
:
    SLPtrList<Reaction<ThermoType> >(reactions),
    species_(reactions.species_),
    thermoDb_(reactions.thermoDb_),
    dict_(reactions.dict_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class ThermoType>
CML::ReactionList<ThermoType>::~ReactionList()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ThermoType>
bool CML::ReactionList<ThermoType>::readReactionDict()
{
    const dictionary& reactions(dict_.subDict("reactions"));

    // Set general temperature limits from the dictionary
    Reaction<ThermoType>::TlowDefault =
        dict_.lookupOrDefault<scalar>("Tlow", 0);

    Reaction<ThermoType>::ThighDefault =
        dict_.lookupOrDefault<scalar>("Thigh", GREAT);

    forAllConstIter(dictionary, reactions, iter)
    {
        const word reactionName = iter().keyword();

        this->append
        (
            Reaction<ThermoType>::New
            (
                species_,
                thermoDb_,
                reactions.subDict(reactionName)
            ).ptr()
        );
    }

    return true;
}


template<class ThermoType>
void CML::ReactionList<ThermoType>::write(Ostream& os) const
{
    os  << "reactions" << nl;
    os  << token::BEGIN_BLOCK << incrIndent << nl;

    forAllConstIter(typename SLPtrList<Reaction<ThermoType> >, *this, iter)
    {
        const Reaction<ThermoType>& r = iter();
        os  << indent << r.name() << nl
            << indent << token::BEGIN_BLOCK << incrIndent << nl;
        os.writeKeyword("type") << r.type() << token::END_STATEMENT << nl;
        r.write(os);
        os  << decrIndent << indent << token::END_BLOCK << nl;
    }

    os << decrIndent << token::END_BLOCK << nl;
}


#endif
