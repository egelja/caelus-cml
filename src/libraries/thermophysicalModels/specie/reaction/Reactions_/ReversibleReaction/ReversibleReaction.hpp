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
    CML::ReversibleReaction

Description
    Simple extension of Reaction to handle reversible reactions using
    equilibrium thermodynamics.

SourceFiles
    ReversibleReaction.cpp

\*---------------------------------------------------------------------------*/

#ifndef ReversibleReaction_HPP
#define ReversibleReaction_HPP

#include "Reaction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class ReversibleReaction Declaration
\*---------------------------------------------------------------------------*/

template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
class ReversibleReaction
:
    public ReactionType<ReactionThermo>
{

    ReactionRate k_;


    //- Disallow default bitwise assignment
    void operator=
    (
        const ReversibleReaction
        <
            ReactionType,
            ReactionThermo,
            ReactionRate
        >&
    );


public:

    //- Runtime type information
    TypeName("reversible");


    //- Construct from components
    ReversibleReaction
    (
        const ReactionType<ReactionThermo>& reaction,
        const ReactionRate& k
    );

    //- Construct as copy given new speciesTable
    ReversibleReaction
    (
        const ReversibleReaction
        <
            ReactionType,
            ReactionThermo,
            ReactionRate
        >&,
        const speciesTable& species
    );

    //- Construct from dictionary
    ReversibleReaction
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
            new ReversibleReaction
            <
                ReactionType,
                ReactionThermo,
                ReactionRate
            >(*this)
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
            new ReversibleReaction
            <ReactionType, ReactionThermo, ReactionRate>
            (
                *this,
                species
            )
        );
    }


    //- Destructor
    virtual ~ReversibleReaction()
    {}


    // Member Functions

    // ReversibleReaction rate coefficients

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
    //  Note this evaluates the forward rate constant and divides by
    //  the equilibrium constant
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
CML::ReversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
ReversibleReaction
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
CML::ReversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
ReversibleReaction
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
CML::ReversibleReaction<ReactionType, ReactionThermo, ReactionRate>::
ReversibleReaction
(
    const ReversibleReaction<ReactionType, ReactionThermo, ReactionRate>& rr,
    const speciesTable& species
)
:
    ReactionType<ReactionThermo>(rr, species),
    k_(rr.k_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar CML::ReversibleReaction
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
CML::scalar CML::ReversibleReaction
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
    return kfwd/max(this->Kc(p, T), ROOTSMALL);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar CML::ReversibleReaction
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
    return kr(kf(p, T, c), p, T, c);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
CML::scalar CML::ReversibleReaction
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
CML::scalar CML::ReversibleReaction
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
    scalar Kc = max(this->Kc(p, T), ROOTSMALL);

    return dkfdT/Kc - kr*this->dKcdTbyKc(p, T);
}


template
<
    template<class> class ReactionType,
    class ReactionThermo,
    class ReactionRate
>
const CML::List<CML::Tuple2<CML::label, CML::scalar> >&
CML::ReversibleReaction
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
void CML::ReversibleReaction
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
CML::scalar CML::ReversibleReaction
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
void CML::ReversibleReaction
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
    k_.write(os);
}


#endif
