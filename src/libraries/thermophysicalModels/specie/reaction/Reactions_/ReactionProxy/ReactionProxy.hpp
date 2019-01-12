/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::ReactionProxy

Description
    Proxy version of Reaction which provides dummy implementations of the
    abstract virtual functions.

    Used for read-construction and format conversion.


\*---------------------------------------------------------------------------*/

#ifndef ReactionProxy_HPP
#define ReactionProxy_HPP

#include "Reaction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                        Class ReactionProxy Declaration
\*---------------------------------------------------------------------------*/

template<class ReactionThermo>
class ReactionProxy
:
    public Reaction<ReactionThermo>
{

public:


    //- Construct from components
    ReactionProxy
    (
        const speciesTable& species,
        const List<typename Reaction<ReactionThermo>::specieCoeffs>& lhs,
        const List<typename Reaction<ReactionThermo>::specieCoeffs>& rhs,
        const HashPtrTable<ReactionThermo>& thermoDatabase
    );

    //- Construct as copy given new speciesTable
    ReactionProxy
    (
        const Reaction<ReactionThermo>&,
        const speciesTable& species
    );

    //- Construct from dictionary
    ReactionProxy
    (
        const speciesTable& species,
        const HashPtrTable<ReactionThermo>& thermoDatabase,
        const dictionary& dict
    );

    //- Construct and return a clone
    virtual autoPtr<Reaction<ReactionThermo> > clone() const;

    //- Construct and return a clone with new speciesTable
    virtual autoPtr<Reaction<ReactionThermo> > clone
    (
        const speciesTable& species
    ) const;


    //- Destructor
    virtual ~ReactionProxy()
    {}


    // Member Functions
    // Reaction rate coefficients

    //- Forward rate constant
    virtual scalar kf
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;

    //- Reverse rate constant from the given forward rate constant
    virtual scalar kr
    (
        const scalar kfwd,
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;

    //- Reverse rate constant
    virtual scalar kr
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;


    // Jacobian coefficients

    //- Temperature derivative of forward rate
    virtual scalar dkfdT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;

    //- Temperature derivative of reverse rate
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
    virtual void dcidc
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarField& dcidc
    ) const;

    //- Temperature derivative of the pressure dependent term
    virtual scalar dcidT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const;
};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::ReactionProxy<ReactionThermo>::ReactionProxy
(
    const speciesTable& species,
    const List<typename Reaction<ReactionThermo>::specieCoeffs>& lhs,
    const List<typename Reaction<ReactionThermo>::specieCoeffs>& rhs,
    const HashPtrTable<ReactionThermo>& thermoDatabase
)
:
    Reaction<ReactionThermo>
    (
        species,
        lhs,
        rhs,
        thermoDatabase
    )
{}


template<class ReactionThermo>
CML::ReactionProxy<ReactionThermo>::ReactionProxy
(
    const Reaction<ReactionThermo>& r,
    const speciesTable& species
)
:
    Reaction<ReactionThermo>
    (
        r,
        species
    )
{}


template<class ReactionThermo>
CML::ReactionProxy<ReactionThermo>::ReactionProxy
(
    const speciesTable& species,
    const HashPtrTable<ReactionThermo>& thermoDatabase,
    const dictionary& dict
)
:
    Reaction<ReactionThermo>
    (
        species,
        thermoDatabase,
        dict
    )
{}


template<class ReactionThermo>
CML::autoPtr<CML::Reaction<ReactionThermo> >
CML::ReactionProxy<ReactionThermo>::clone() const
{
    NotImplemented;
    return autoPtr<CML::Reaction<ReactionThermo> >();
}


template<class ReactionThermo>
CML::autoPtr<CML::Reaction<ReactionThermo> >
CML::ReactionProxy<ReactionThermo>::clone
(
    const speciesTable& species
) const
{
    NotImplemented;
    return autoPtr<Reaction<ReactionThermo> >();
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class ReactionThermo>
CML::scalar CML::ReactionProxy<ReactionThermo>::kf
(
    const scalar p,
    const scalar T,
    const scalarField& c
) const
{
    NotImplemented;
    return 0;
}


template<class ReactionThermo>
CML::scalar CML::ReactionProxy<ReactionThermo>::kr
(
    const scalar kfwd,
    const scalar p,
    const scalar T,
    const scalarField& c
) const
{
    NotImplemented;
    return 0;
}


template<class ReactionThermo>
CML::scalar CML::ReactionProxy<ReactionThermo>::kr
(
    const scalar p,
    const scalar T,
    const scalarField& c
) const
{
    NotImplemented;
    return 0;
}


template<class ReactionThermo>
CML::scalar CML::ReactionProxy<ReactionThermo>::dkfdT
(
    const scalar p,
    const scalar T,
    const scalarField& c
) const
{
    NotImplemented;
    return 0;
}


template<class ReactionThermo>
CML::scalar CML::ReactionProxy<ReactionThermo>::dkrdT
(
    const scalar p,
    const scalar T,
    const scalarField& c,
    const scalar dkfdT,
    const scalar kr
) const
{
    NotImplemented;
    return 0;
}


template<class ReactionThermo>
const CML::List<CML::Tuple2<CML::label, CML::scalar> >&
CML::ReactionProxy<ReactionThermo>::beta() const
{
    NotImplemented;
    return NullSingletonRef<List<Tuple2<label, scalar> > >();
}


template<class ReactionThermo>
void CML::ReactionProxy<ReactionThermo>::dcidc
(
    const scalar p,
    const scalar T,
    const scalarField& c,
    scalarField& dcidc
) const
{
    NotImplemented;
}


template<class ReactionThermo>
CML::scalar CML::ReactionProxy<ReactionThermo>::dcidT
(
    const scalar p,
    const scalar T,
    const scalarField& c
) const
{
    NotImplemented;
    return 0;
}


#endif
