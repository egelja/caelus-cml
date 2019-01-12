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
    CML::solidReaction

Description

    Read solid reactions of the type S1 = S2 + G1


\*---------------------------------------------------------------------------*/

#ifndef SolidReaction_HPP
#define SolidReaction_HPP

#include "speciesTable.hpp"
#include "Reaction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators
template<class ReactionThermo>
class SolidReaction;

template<class ReactionThermo>
inline Ostream& operator<<(Ostream&, const SolidReaction<ReactionThermo>&);

/*---------------------------------------------------------------------------*\
                        Class SolidReaction Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class SolidReaction
:
    public Reaction<ReactionThermo>
{

private:

    // Private data

    typedef typename Reaction<ReactionThermo>::specieCoeffs specieCoeffs;

    //- List of gas species present in reaction system
    speciesTable pyrolisisGases_;

    //- Gas specie index for the left-hand-side of the reaction
    List<specieCoeffs> glhs_;

    //- Gas specie index for the right-hand-side of the reaction
    List<specieCoeffs> grhs_;


    // Private Member Functions


    //- Return string representation of reaction
    string solidReactionStr(OStringStream&) const;

    //- Return string representation of the left of the reaction
    void solidReactionStrLeft(OStringStream&) const;

    //- Return string representation of the right of the reaction
    void solidReactionStrRight(OStringStream&) const;

    //- Disallow default bitwise assignment
    void operator=(const SolidReaction&);


public:

    //- Runtime type information
    TypeName("SolidReaction");


    //- Construct from components
    SolidReaction
    (
        const Reaction<ReactionThermo>& reaction,
        const speciesTable& pyrolisisGases,
        const List<specieCoeffs>& glhs,
        const List<specieCoeffs>& grhs
    );

    //- Construct as copy given new speciesTable
    SolidReaction
    (
        const SolidReaction<ReactionThermo>&,
        const speciesTable& pyrolisisGases
    );

    //- Construct from dictionary
    SolidReaction
    (
        const speciesTable& species,
        const HashPtrTable<ReactionThermo>& thermoDatabase,
        const dictionary& dict
    );


    //- Destructor
    virtual ~SolidReaction()
    {}


    // Member Functions

    // Access

    //- Access to the gas components of the left hand side
    virtual const List<specieCoeffs>& grhs() const;

    //- Access to the gas components of the right hand side
    virtual const List<specieCoeffs>& glhs() const;

    //- Access to gas specie list
    virtual const speciesTable& gasSpecies() const;


    //- Write
    virtual void write(Ostream&) const;


    // Ostream Operator

    friend Ostream& operator<<
    (
        Ostream& os,
        const SolidReaction<ReactionThermo>& r
    )
    {
        OStringStream reaction;
        os << r.solidReactionStr(reaction)<< token::END_STATEMENT <<nl;
        return os;
    }
};


} // End namespace CML


#include "DynamicList.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::SolidReaction<ReactionThermo>::SolidReaction
(
    const Reaction<ReactionThermo>& reaction,
    const speciesTable& pyrolisisGases,
    const List<specieCoeffs>& glhs,
    const List<specieCoeffs>& grhs
)
:
    Reaction<ReactionThermo>(reaction),
    pyrolisisGases_(pyrolisisGases),
    glhs_(glhs),
    grhs_(grhs)
{}


template<class ReactionThermo>
CML::SolidReaction<ReactionThermo>::SolidReaction
(
    const SolidReaction<ReactionThermo>& r,
    const speciesTable& pyrolisisGases
)
:
    Reaction<ReactionThermo>(r),
    pyrolisisGases_(pyrolisisGases),
    glhs_(r.glhs_),
    grhs_(r.grhs_)
{}


template<class ReactionThermo>
CML::SolidReaction<ReactionThermo>::SolidReaction
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    const dictionary& dict
)
:
    Reaction<ReactionThermo>(species, thermoDatabase, dict),
    pyrolisisGases_(dict.parent().parent().lookup("gaseousSpecies")),
    glhs_(),
    grhs_()
{
    this->setLRhs
    (
        IStringStream(dict.lookup("reaction"))(),
        pyrolisisGases_,
        glhs_,
        grhs_
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ReactionThermo>
const CML::List<typename CML::SolidReaction<ReactionThermo>::specieCoeffs>&
CML::SolidReaction<ReactionThermo>::glhs() const
{
    return glhs_;
}


template<class ReactionThermo>
const CML::List<typename CML::Reaction<ReactionThermo>::specieCoeffs>&
CML::SolidReaction<ReactionThermo>::grhs() const
{
    return grhs_;
}


template<class ReactionThermo>
const CML::speciesTable& CML::SolidReaction<ReactionThermo>::
gasSpecies() const
{
    return pyrolisisGases_;
}


template<class ReactionThermo>
void CML::SolidReaction<ReactionThermo>::write(Ostream& os) const
{
    OStringStream reaction;
    os.writeKeyword("reaction") << solidReactionStr(reaction)
        << token::END_STATEMENT << nl;
}


template<class ReactionThermo>
CML::string CML::SolidReaction<ReactionThermo>::solidReactionStr
(
    OStringStream& reaction
) const
{
    this->reactionStrLeft(reaction);
    if (glhs().size() > 0)
    {
        reaction << " + ";
        solidReactionStrLeft(reaction);
    }
    reaction << " = ";
    this->reactionStrRight(reaction);
    if (grhs().size() > 0)
    {
        reaction << " + ";
        solidReactionStrRight(reaction);
    }
    return reaction.str();

}


template<class ReactionThermo>
void CML::SolidReaction<ReactionThermo>::solidReactionStrLeft
(
    OStringStream& reaction
) const
{
    for (label i = 0; i < glhs().size(); ++i)
    {
        if (i > 0)
        {
            reaction << " + ";
        }
        if (mag(glhs()[i].stoichCoeff - 1) > SMALL)
        {
            reaction << glhs()[i].stoichCoeff;
        }
        reaction << gasSpecies()[glhs()[i].index];
        if (mag(glhs()[i].exponent - glhs()[i].stoichCoeff) > SMALL)
        {
            reaction << "^" << glhs()[i].exponent;
        }
    }
}


template<class ReactionThermo>
void CML::SolidReaction<ReactionThermo>::solidReactionStrRight
(
    OStringStream& reaction
) const
{

    for (label i = 0; i < grhs().size(); ++i)
    {
        if (i > 0)
        {
            reaction << " + ";
        }
        if (mag(grhs()[i].stoichCoeff - 1) > SMALL)
        {
            reaction << grhs()[i].stoichCoeff;
        }
        reaction << gasSpecies()[grhs()[i].index];
        if (mag(grhs()[i].exponent - grhs()[i].stoichCoeff) > SMALL)
        {
            reaction << "^" << grhs()[i].exponent;
        }
    }
}

#endif
