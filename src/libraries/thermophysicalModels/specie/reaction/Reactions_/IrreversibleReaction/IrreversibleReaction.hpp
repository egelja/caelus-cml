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
    CML::IrreversibleReaction

Description
    Simple extension of Reaction to handle irreversible reactions

SourceFiles
    IrreversibleReaction.cpp

\*---------------------------------------------------------------------------*/

#ifndef IrreversibleReaction_HPP
#define IrreversibleReaction_HPP

#include "Reaction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                    Class IrreversibleReaction Declaration
\*---------------------------------------------------------------------------*/

template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
class IrreversibleReaction
:
    public ReactionType<ReactionThermo>
{

    ReactionRate k_;


    //- Disallow default bitwise assignment
    void operator=
    (
        const IrreversibleReaction
        <
            ReactionType,
            ReactionThermo,
            ReactionRate
        >&
    );


public:

    //- Runtime type information
    TypeName("irreversible");


    //- Construct from components
    IrreversibleReaction
    (
        const ReactionType<ReactionThermo>& reaction,
        const ReactionRate& reactionRate
    );

    //- Construct as copy given new speciesTable
    IrreversibleReaction
    (
        const IrreversibleReaction
        <
            ReactionType,
            ReactionThermo,
            ReactionRate
        >&,
        const speciesTable& species
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
            new IrreversibleReaction
            <
                ReactionType,
                ReactionThermo,
                ReactionRate
            >(*this)
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
            new IrreversibleReaction
            <
                ReactionType,
                ReactionThermo,
                ReactionRate
            >
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
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;

    //- Reverse rate constant from the given forward rate constant
    //  Returns 0
    virtual scalar kr
    (
        const scalar kfwd,
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;

    //- Reverse rate constant
    //  Returns 0
    virtual scalar kr
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;


    // IrreversibleReaction Jacobian functions

    //- Temperature derivative of forward rate
    virtual scalar dkfdT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;

    //- Temperature derivative of reverse rate
    //  Returns 0
    virtual scalar dkrdT
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        const scalar dkfdT,
        const scalar kr
    ) const;

    //- Third-body efficiencies (beta = 1-alpha)
    //  non-empty only for third-body reactions
    //  with enhanced molecularity (alpha != 1)
    virtual const List<Tuple2<label, scalar>>& beta() const;

    //- Species concentration derivative of the pressure dependent term
    //  By default this value is 1 as it multiplies the third-body term
    virtual void dcidc
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarField& dcidc
    ) const;

    //- Temperature derivative of the pressure dependent term
    //  By default this value is 0 since ddT of molecularity is approx.0
    virtual scalar dcidT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;


    //- Write
    virtual void write(Ostream&) const;

};


} // End namespace CML

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::IrreversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
IrreversibleReaction
(
    const ReactionType<ReactionThermo>& reaction,
    const ReactionRate& k
)
:
    ReactionType<ReactionThermo>(reaction),
    k_(k)
{}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::IrreversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
IrreversibleReaction
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    const dictionary& dict
)
:
    ReactionType<ReactionThermo>(species, thermoDatabase, dict),
    k_(species, dict)
{}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::IrreversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
IrreversibleReaction
(
    const IrreversibleReaction<ReactionType, ReactionThermo, ReactionRate>& irr,
    const speciesTable& species
)
:
    ReactionType<ReactionThermo>(irr, species),
    k_(irr.k_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar CML::IrreversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::kf
(
    const scalar p,
    const scalar T,
    const scalarField& c
) const
{
    return k_(p, T, c);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar CML::IrreversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::kr
(
    const scalar kfwd,
    const scalar p,
    const scalar T,
    const scalarField& c
) const
{
    return 0;
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar CML::IrreversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::kr
(
    const scalar p,
    const scalar T,
    const scalarField& c
) const
{
    return 0;
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar CML::IrreversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::dkfdT
(
    const scalar p,
    const scalar T,
    const scalarField& c
) const
{
    return k_.ddT(p, T, c);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar CML::IrreversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::dkrdT
(
    const scalar p,
    const scalar T,
    const scalarField& c,
    const scalar dkfdT,
    const scalar kr
) const
{
    return 0;
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
const CML::List<CML::Tuple2<CML::label, CML::scalar>>&
CML::IrreversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::beta() const
{
    return k_.beta();
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
void CML::IrreversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::dcidc
(
    const scalar p,
    const scalar T,
    const scalarField& c,
    scalarField& dcidc
) const
{
    k_.dcidc(p, T, c, dcidc);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar CML::IrreversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::dcidT
(
    const scalar p,
    const scalar T,
    const scalarField& c
) const
{
    return k_.dcidT(p, T, c);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
void CML::IrreversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
write
(
    Ostream& os
) const
{
    ReactionType<ReactionThermo>::write(os);
    k_.write(os);
}


#endif
