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
    CML::NonEquilibriumReversibleReaction

Description
    Simple extension of Reaction to handle reversible reactions using
    equilibrium thermodynamics.


\*---------------------------------------------------------------------------*/

#ifndef NonEquilibriumReversibleReaction_H
#define NonEquilibriumReversibleReaction_H

#include "Reaction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
             Class NonEquilibriumReversibleReaction Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo, class ReactionRate>
class NonEquilibriumReversibleReaction
:
    public Reaction<ReactionThermo>
{
    // Private data

        ReactionRate fk_;
        ReactionRate rk_;


    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=
        (
            const NonEquilibriumReversibleReaction
                <ReactionThermo, ReactionRate>&
        );


public:

    //- Runtime type information
    TypeName("nonEquilibriumReversible");


    // Constructors

        //- Construct from components
        NonEquilibriumReversibleReaction
        (
            const Reaction<ReactionThermo>& reaction,
            const ReactionRate& forwardReactionRate,
            const ReactionRate& reverseReactionRate
        );

        //- Construct as copy given new speciesTable
        NonEquilibriumReversibleReaction
        (
            const NonEquilibriumReversibleReaction
                <ReactionThermo, ReactionRate>&,
            const speciesTable& species
        );

        //- Construct from Istream
        NonEquilibriumReversibleReaction
        (
            const speciesTable& species,
            const HashPtrTable<ReactionThermo>& thermoDatabase,
            Istream& is
        );

        //- Construct from dictionary
        NonEquilibriumReversibleReaction
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
                new NonEquilibriumReversibleReaction
                    <ReactionThermo, ReactionRate>(*this)
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
                new NonEquilibriumReversibleReaction
                    <ReactionThermo, ReactionRate>
                    (*this, species)
            );
        }


    //- Destructor
    virtual ~NonEquilibriumReversibleReaction()
    {}


    // Member Functions

        // NonEquilibriumReversibleReaction rate coefficients

            //- Forward rate constant
            virtual scalar kf
            (
                const scalar T,
                const scalar p,
                const scalarField& c
            ) const;

            //- Reverse rate constant from the given formard rate constant
            virtual scalar kr
            (
                const scalar kfwd,
                const scalar T,
                const scalar p,
                const scalarField& c
            ) const;

            //- Reverse rate constant.
            //  Note this evaluates the forward rate constant and divides by the
            //  equilibrium constant
            virtual scalar kr
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
CML::NonEquilibriumReversibleReaction<ReactionThermo, ReactionRate>::
NonEquilibriumReversibleReaction
(
    const Reaction<ReactionThermo>& reaction,
    const ReactionRate& forwardReactionRate,
    const ReactionRate& reverseReactionRate
)
:
    Reaction<ReactionThermo>(reaction),
    fk_(forwardReactionRate),
    rk_(reverseReactionRate)
{}



template<class ReactionThermo, class ReactionRate>
CML::NonEquilibriumReversibleReaction<ReactionThermo, ReactionRate>::
NonEquilibriumReversibleReaction
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    Istream& is
)
:
    Reaction<ReactionThermo>(species, thermoDatabase, is),
    fk_(species, is),
    rk_(species, is)
{}


template<class ReactionThermo, class ReactionRate>
CML::NonEquilibriumReversibleReaction<ReactionThermo, ReactionRate>::
NonEquilibriumReversibleReaction
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    const dictionary& dict
)
:
    Reaction<ReactionThermo>(species, thermoDatabase, dict),
    fk_(species, dict.subDict("forward")),
    rk_(species, dict.subDict("reverse"))
{}


template<class ReactionThermo, class ReactionRate>
CML::NonEquilibriumReversibleReaction<ReactionThermo, ReactionRate>::
NonEquilibriumReversibleReaction
(
    const NonEquilibriumReversibleReaction<ReactionThermo, ReactionRate>& nerr,
    const speciesTable& species
)
:
    Reaction<ReactionThermo>(nerr, species),
    fk_(nerr.fk_),
    rk_(nerr.rk_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ReactionThermo, class ReactionRate>
CML::scalar
CML::NonEquilibriumReversibleReaction<ReactionThermo, ReactionRate>::kf
(
    const scalar T,
    const scalar p,
    const scalarField& c
) const
{
    return fk_(T, p, c);
}


template<class ReactionThermo, class ReactionRate>
CML::scalar
CML::NonEquilibriumReversibleReaction<ReactionThermo, ReactionRate>::kr
(
    const scalar,
    const scalar T,
    const scalar p,
    const scalarField& c
) const
{
    return rk_(T, p, c);
}


template<class ReactionThermo, class ReactionRate>
CML::scalar
CML::NonEquilibriumReversibleReaction<ReactionThermo, ReactionRate>::kr
(
    const scalar T,
    const scalar p,
    const scalarField& c
) const
{
    return rk_(T, p, c);
}


template<class ReactionThermo, class ReactionRate>
void CML::NonEquilibriumReversibleReaction<ReactionThermo, ReactionRate>::write
(
    Ostream& os
) const
{
    Reaction<ReactionThermo>::write(os);

    os  << indent << "forward" << nl;
    os  << indent << token::BEGIN_BLOCK << nl;
    os  << incrIndent;
    fk_.write(os);
    os  << decrIndent;
    os  << indent << token::END_BLOCK << nl;

    os  << indent << "reverse" << nl;
    os  << indent << token::BEGIN_BLOCK << nl;
    os  << incrIndent;
    rk_.write(os);
    os  << decrIndent;
    os  << indent << token::END_BLOCK << nl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
