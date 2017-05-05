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
    CML::IrreversibleReaction

Description
    Simple extension of Reaction to handle reversible reactions using
    equilibrium thermodynamics.

SourceFiles
    IrreversibleReaction.cpp

\*---------------------------------------------------------------------------*/

#ifndef IrreversibleReaction_H
#define IrreversibleReaction_H

#include "Reaction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class IrreversibleReaction Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo, class ReactionRate>
class IrreversibleReaction
:
    public Reaction<ReactionThermo>
{
    // Private data

        ReactionRate k_;


    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=
        (
            const IrreversibleReaction<ReactionThermo, ReactionRate>&
        );


public:

    //- Runtime type information
    TypeName("irreversible");


    // Constructors

        //- Construct from components
        IrreversibleReaction
        (
            const Reaction<ReactionThermo>& reaction,
            const ReactionRate& reactionRate
        );

        //- Construct as copy given new speciesTable
        IrreversibleReaction
        (
            const IrreversibleReaction<ReactionThermo, ReactionRate>&,
            const speciesTable& species
        );

        //- Construct from Istream
        IrreversibleReaction
        (
            const speciesTable& species,
            const HashPtrTable<ReactionThermo>& thermoDatabase,
            Istream& is
        );

        //- Construct from dictionary
        IrreversibleReaction
        (
            const speciesTable& species,
            const HashPtrTable<ReactionThermo>& thermoDatabase,
            const dictionary& dict
        );

        //- Construct and return a clone
        virtual autoPtr<Reaction<ReactionThermo> > clone() const
        {
            return autoPtr<Reaction<ReactionThermo> >
            (
                new IrreversibleReaction<ReactionThermo, ReactionRate>(*this)
            );
        }

        //- Construct and return a clone with new speciesTable
        virtual autoPtr<Reaction<ReactionThermo> > clone
        (
            const speciesTable& species
        ) const
        {
            return autoPtr<Reaction<ReactionThermo> >
            (
                new IrreversibleReaction<ReactionThermo, ReactionRate>
                (
                    *this,
                    species
                )
            );
        }


    //- Destructor
    virtual ~IrreversibleReaction()
    {}


    // Member Functions

        // IrreversibleReaction rate coefficients

            //- Forward rate constant
            virtual scalar kf
            (
                const scalar T,
                const scalar p,
                const scalarField& c
            ) const;


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo, class ReactionRate>
CML::IrreversibleReaction<ReactionThermo, ReactionRate>::IrreversibleReaction
(
    const Reaction<ReactionThermo>& reaction,
    const ReactionRate& k
)
:
    Reaction<ReactionThermo>(reaction),
    k_(k)
{}


template<class ReactionThermo, class ReactionRate>
CML::IrreversibleReaction<ReactionThermo, ReactionRate>::IrreversibleReaction
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    Istream& is
)
:
    Reaction<ReactionThermo>(species, thermoDatabase, is),
    k_(species, is)
{}


template<class ReactionThermo, class ReactionRate>
CML::IrreversibleReaction<ReactionThermo, ReactionRate>::IrreversibleReaction
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    const dictionary& dict
)
:
    Reaction<ReactionThermo>(species, thermoDatabase, dict),
    k_(species, dict)
{}


template<class ReactionThermo, class ReactionRate>
CML::IrreversibleReaction<ReactionThermo, ReactionRate>::IrreversibleReaction
(
    const IrreversibleReaction<ReactionThermo, ReactionRate>& irr,
    const speciesTable& species
)
:
    Reaction<ReactionThermo>(irr, species),
    k_(irr.k_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ReactionThermo, class ReactionRate>
CML::scalar CML::IrreversibleReaction<ReactionThermo, ReactionRate>::kf
(
    const scalar T,
    const scalar p,
    const scalarField& c
) const
{
    return k_(T, p, c);
}


template<class ReactionThermo, class ReactionRate>
void CML::IrreversibleReaction<ReactionThermo, ReactionRate>::write
(
    Ostream& os
) const
{
    Reaction<ReactionThermo>::write(os);
    k_.write(os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
