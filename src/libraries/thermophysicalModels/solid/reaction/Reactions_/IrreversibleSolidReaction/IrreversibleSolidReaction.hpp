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
    CML::IrreversibleSolidReaction

Description
    Simple extension of Reaction to handle reversible reactions

SourceFiles
    IrreversibleSolidReaction.cpp

\*---------------------------------------------------------------------------*/

#ifndef IrreversibleSolidReaction_H
#define IrreversibleSolidReaction_H

#include "solidReaction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class IrreversibleSolidReaction Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionRate>
class IrreversibleSolidReaction
:
    public solidReaction
{
    // Private data

        // Reaction rate
        ReactionRate k_;

        // Reaction order
        scalar nReact_;


    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=
        (
            const IrreversibleSolidReaction<ReactionRate>&
        );


public:

    //- Runtime type information
    TypeName("irreversible");


    // Constructors

        //- Construct from components
        IrreversibleSolidReaction
        (
            const solidReaction& reaction,
            const ReactionRate& reactionRate,
            const scalar nReact
        );


        //- Construct from Istream
        IrreversibleSolidReaction
        (
            const speciesTable& components,
            Istream& is,
            const speciesTable& pyrolysisGases
        );


    //- Destructor
    virtual ~IrreversibleSolidReaction()
    {}


    // Member Functions

        // IrreversibleSolidReaction rate coefficients

            //- Forward rate constant
            virtual scalar kf
            (
                const scalar T,
                const scalar p,
                const scalarField& c
            ) const;


            //- Reaction order
            virtual scalar nReact() const;


        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionRate>
CML::IrreversibleSolidReaction<ReactionRate>::IrreversibleSolidReaction
(
    const solidReaction& reaction,
    const ReactionRate& k,
    const scalar nReact
)
:
    solidReaction(reaction),
    k_(k),
    nReact_(nReact)
{}


template<class ReactionRate>
CML::IrreversibleSolidReaction<ReactionRate>::IrreversibleSolidReaction
(
    const speciesTable& components,
    Istream& is,
    const speciesTable& pyrolysisGases
)
:
    solidReaction(components, is, pyrolysisGases),
    k_(components, is),
    nReact_(readScalar(is))
{
    is.readEnd("solidArrheniusReactionRate(Istream&)");
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ReactionRate>
CML::scalar CML::IrreversibleSolidReaction<ReactionRate>::kf
(
    const scalar T,
    const scalar p,
    const scalarField& c
) const
{
    return k_(T, p, c);
}


template<class ReactionRate>
CML::scalar CML::IrreversibleSolidReaction<ReactionRate>::nReact() const
{
    return nReact_;
}


template<class ReactionRate>
void CML::IrreversibleSolidReaction<ReactionRate>::write
(
    Ostream& os
) const
{
    solidReaction::write(os);
    os  << token::SPACE << "Reaction order: " << nReact_ << nl << k_;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
