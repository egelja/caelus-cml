/*---------------------------------------------------------------------------*\
Copyright (C) 2018 OpenFOAM Foundation
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
    CML::MichaelisMentenReactionRate

Description
    Michaelis-Menten reaction rate for enzymatic reactions.

    Reference:
    \verbatim
        Michaelis, L., & Menten, M. L. (1913).
        Die Kinetik der InwertinWirkung.
        Biochem., (49), 333-369.
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef MichaelisMentenReactionRate_HPP
#define MichaelisMentenReactionRate_HPP

#include "Reaction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class MichaelisMentenReactionRate;

Ostream& operator<<(Ostream&, const MichaelisMentenReactionRate&);


/*---------------------------------------------------------------------------*\
               Class MichaelisMentenReactionRate Declaration
\*---------------------------------------------------------------------------*/

class MichaelisMentenReactionRate
{

    //- List of species present in reaction system
    const speciesTable& species_;

    //- The maximum reaction rate at saturating substrate concentration
    scalar Vmax_;

    //- The Michaelis constant
    //  the substrate concentration at which the reaction rate is half Vmax_
    scalar Km_;

    //- The substrate specie index
    label s_;

    List<Tuple2<label, scalar> > beta_;


public:


    //- Construct from dictionary
    inline MichaelisMentenReactionRate
    (
        const speciesTable& st,
        const dictionary& dict
    )
    :
        species_(st),
        Vmax_(readScalar(dict.lookup("Vmax"))),
        Km_(readScalar(dict.lookup("Km"))),
        s_(st[dict.lookup("S")])
    {
        beta_.append(Tuple2<label, scalar>(s_, 1.0));
    }


    // Member Functions

    //- Return the type name
    static word type()
    {
        return "MichaelisMenten";
    }

    inline scalar operator()
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        return Vmax_/(Km_ + c[s_]);
    }

    inline scalar ddT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        return 0;
    }

    //- Third-body efficiencies (beta = 1-alpha)
    //  non-empty only for third-body reactions
    //  with enhanced molecularity (alpha != 1)
    inline const List<Tuple2<label, scalar> >& beta() const
    {
        return beta_;
    }

    //- Species concentration derivative of the pressure dependent term
    inline void dcidc
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarField& dcidc
    ) const
    {
        dcidc[0] = -1.0/(Km_ + c[s_]);
    }

    //- Temperature derivative of the pressure dependent term
    inline scalar dcidT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        return 0;
    }

    //- Write to stream
    inline void write(Ostream& os) const
    {
        os.writeKeyword("Vmax") << Vmax_ << token::END_STATEMENT << nl;
        os.writeKeyword("Km") << Km_ << token::END_STATEMENT << nl;
        os.writeKeyword("S") << species_[s_] << token::END_STATEMENT << nl;
    }


    // Ostream Operator

    inline friend Ostream& operator<<
    (
        Ostream& os,
        const MichaelisMentenReactionRate& mmrr
    )
    {
        mmrr.write(os);
        return os;
    }
};


} // End namespace CML


#endif
