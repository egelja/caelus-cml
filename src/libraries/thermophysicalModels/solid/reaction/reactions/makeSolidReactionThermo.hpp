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

InClass
    CML::makeSolidReactionThermo

Description
    Macros for instantiating reactions on given solid thermo packages

\*---------------------------------------------------------------------------*/

#ifndef makeReactionThermo_H
#define makeReactionThermo_H

#include "solidReaction.hpp"
#include "IrreversibleSolidReaction.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#define makeReaction(ReactionType, ReactionRate)                              \
                                                                              \
    typedef solidReaction Reaction;                                           \
                                                                              \
    typedef ReactionType<ReactionRate>                                        \
        ReactionType##ReactionRate;                                           \
                                                                              \
    template<>                                                                \
    const word ReactionType##ReactionRate::typeName                           \
    (                                                                         \
        ReactionType::typeName_()                                             \
      + ReactionRate::type()                                                  \
      + Reaction::typeName_()                                                 \
    );                                                                        \
                                                                              \
    addToRunTimeSelectionTable                                                \
    (                                                                         \
        Reaction,                                                             \
        ReactionType##ReactionRate,                                           \
        Istream                                                               \
    );


#define makeIRReactions(ReactionRate)                                         \
                                                                              \
    makeReaction(IrreversibleSolidReaction, ReactionRate)


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif


// ************************************************************************* //
