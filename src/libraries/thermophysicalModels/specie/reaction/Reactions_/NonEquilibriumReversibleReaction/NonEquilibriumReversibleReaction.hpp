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
    CML::NonEquilibriumReversibleReaction

Description
    Simple extension of Reaction to handle reversible reactions using
    equilibrium thermodynamics.


\*---------------------------------------------------------------------------*/

#ifndef NonEquilibriumReversibleReaction_HPP
#define NonEquilibriumReversibleReaction_HPP

#include "Reaction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
             Class NonEquilibriumReversibleReaction Declaration
\*---------------------------------------------------------------------------*/

template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
class NonEquilibriumReversibleReaction
:
    public ReactionType<ReactionThermo>
{

    ReactionRate fk_;
    ReactionRate rk_;


    //- Disallow default bitwise assignment
    void operator=
    (
        const NonEquilibriumReversibleReaction
            <ReactionType, ReactionThermo, ReactionRate>&
    );


public:

    //- Runtime type information
    TypeName("nonEquilibriumReversible");


    //- Construct from components
    NonEquilibriumReversibleReaction
    (
        const ReactionType<ReactionThermo>& reaction,
        const ReactionRate& forwardReactionRate,
        const ReactionRate& reverseReactionRate
    );

    //- Construct as copy given new speciesTable
    NonEquilibriumReversibleReaction
    (
        const NonEquilibriumReversibleReaction
            <ReactionType, ReactionThermo, ReactionRate>&,
        const speciesTable& species
    );

    //- Construct from dictionary
    NonEquilibriumReversibleReaction
    (
        const speciesTable& species,
        const HashPtrTable<ReactionThermo>& thermoDatabase,
        const dictionary& dict
    );

    //- Construct and return a clone
    virtual autoPtr<ReactionType<ReactionThermo> > clone() const
    {
        return autoPtr<ReactionType<ReactionThermo> >
        (
            new NonEquilibriumReversibleReaction
                <ReactionType, ReactionThermo, ReactionRate>(*this)
        );
    }

    //- Construct and return a clone with new speciesTable
    virtual autoPtr<ReactionType<ReactionThermo> > clone
    (
        const speciesTable& species
    ) const
    {
        return autoPtr<ReactionType<ReactionThermo> >
        (
            new NonEquilibriumReversibleReaction
                <ReactionType, ReactionThermo, ReactionRate>
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
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;

    //- Reverse rate constant from the given formard rate constant
    virtual scalar kr
    (
        const scalar kfwd,
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;

    //- Reverse rate constant.
    //  Note this evaluates the forward rate constant and divides by the
    //  equilibrium constant
    virtual scalar kr
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;


    // ReversibleReaction Jacobian functions

    //- Temperature derivative of forward rate
    virtual scalar dkfdT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;

    //- Temperature derivative of backward rate
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
    virtual const List<Tuple2<label, scalar> >& beta() const;

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
CML::NonEquilibriumReversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::
NonEquilibriumReversibleReaction
(
    const ReactionType<ReactionThermo>& reaction,
    const ReactionRate& forwardReactionRate,
    const ReactionRate& reverseReactionRate
)
:
    ReactionType<ReactionThermo>(reaction),
    fk_(forwardReactionRate),
    rk_(reverseReactionRate)
{}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::NonEquilibriumReversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::
NonEquilibriumReversibleReaction
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    const dictionary& dict
)
:
    ReactionType<ReactionThermo>(species, thermoDatabase, dict),
    fk_(species, dict.subDict("forward")),
    rk_(species, dict.subDict("reverse"))
{}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::NonEquilibriumReversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::
NonEquilibriumReversibleReaction
(
    const NonEquilibriumReversibleReaction
    <
        ReactionType,
        ReactionThermo,
        ReactionRate
    >& nerr,
    const speciesTable& species
)
:
    ReactionType<ReactionThermo>(nerr, species),
    fk_(nerr.fk_),
    rk_(nerr.rk_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar
CML::NonEquilibriumReversibleReaction
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
    return fk_(p, T, c);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar
CML::NonEquilibriumReversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::kr
(
    const scalar,
    const scalar p,
    const scalar T,
    const scalarField& c
) const
{
    return rk_(p, T, c);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar
CML::NonEquilibriumReversibleReaction
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
    return rk_(p, T, c);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar
CML::NonEquilibriumReversibleReaction
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
    return fk_.ddT(p, T, c);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar
CML::NonEquilibriumReversibleReaction
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
    return rk_.ddT(p, T, c);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
const CML::List<CML::Tuple2<CML::label, CML::scalar> >&
CML::NonEquilibriumReversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::beta() const
{
    return fk_.beta();
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
void CML::NonEquilibriumReversibleReaction
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
    fk_.dcidc(p, T, c, dcidc);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar CML::NonEquilibriumReversibleReaction
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
    return fk_.dcidT(p, T, c);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
void CML::NonEquilibriumReversibleReaction
<
    ReactionType,
    ReactionThermo,
    ReactionRate
>::write
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


#endif
