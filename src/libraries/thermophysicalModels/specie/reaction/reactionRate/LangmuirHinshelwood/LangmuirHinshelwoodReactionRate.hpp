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
    CML::LangmuirHinshelwoodReactionRate

Description
    Langmuir-Hinshelwood reaction rate for gaseous reactions on surfaces.

    Reference:
    \verbatim
        Hinshelwood, C.N. (1940).
        The Kinetics of Chemical Change.
        Oxford Clarendon Press
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef LangmuirHinshelwoodReactionRate_HPP
#define LangmuirHinshelwoodReactionRate_HPP

#include "scalarField.hpp"
#include "typeInfo.hpp"
#include "FixedList.hpp"
#include "Tuple2.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class LangmuirHinshelwoodReactionRate;

Ostream& operator<<(Ostream&, const LangmuirHinshelwoodReactionRate&);


/*---------------------------------------------------------------------------*\
               Class LangmuirHinshelwoodReactionRate Declaration
\*---------------------------------------------------------------------------*/

class LangmuirHinshelwoodReactionRate
{

    //- List of species present in reaction system
    const speciesTable& species_;

    static const label n_ = 3;
    scalar A_[n_];
    scalar Ta_[n_];

    //- Index of specie "A"
    label a_;

    //- Index of specie "B"
    label b_;


public:


    //- Construct from dictionary
    inline LangmuirHinshelwoodReactionRate
    (
        const speciesTable& st,
        const dictionary& dict
    )
    :
        species_(st),
        a_(st["A"]),
        b_(st["B"])
    {
        // Read (A, Ta) pairs
        FixedList<Tuple2<scalar, scalar>, n_> coeffs(dict.lookup("coeffs"));

        forAll(coeffs, i)
        {
            A_[i] = coeffs[i].first();
            Ta_[i] = coeffs[i].second();
        }
    }


    // Member Functions

    //- Return the type name
    static word type()
    {
        return "LangmuirHinshelwood";
    }

    inline scalar operator()
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        return A_[0]*exp(-Ta_[0]/T)/
        (
            T*sqr(1 + A_[1]*exp(-Ta_[1]/T)*c[a_] + A_[2]*exp(-Ta_[2]/T)*c[b_])
        );
    }

    inline scalar ddT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        const scalar den =
        (
            T*sqr(1 + A_[1]*exp(-Ta_[1]/T)*c[a_] + A_[2]*exp(-Ta_[2]/T)*c[b_])
        );
        const scalar rate = A_[0]*exp(-Ta_[0]/T)/den;

        const scalar derivDen =
        (
            sqr(1 + A_[1]*exp(-Ta_[1]/T)*c[a_] + A_[2]*exp(-Ta_[2]/T)*c[b_])
          + 2*T*(1 + A_[1]*exp(-Ta_[1]/T)*c[a_] + A_[2]*exp(-Ta_[2]/T)*c[b_])
           *(
                A_[1]*exp(-Ta_[1]/T)*c[a_]*Ta_[1]/sqr(T)
              + A_[2]*exp(-Ta_[2]/T)*c[b_]*Ta_[2]/sqr(T)
            )
        );

        return rate*(Ta_[0]/sqr(T) - derivDen/den);
    }

    //- Third-body efficiencies (beta = 1-alpha)
    //  non-empty only for third-body reactions
    //  with enhanced molecularity (alpha != 1)
    inline const List<Tuple2<label, scalar> >& beta() const
    {
        return NullSingletonRef<List<Tuple2<label, scalar> > >();
    }

    //- Species concentration derivative of the pressure dependent term
    inline void dcidc
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarField& dcidc
    ) const
    {}

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
        os.writeKeyword("A") << species_[a_] << token::END_STATEMENT << nl;
        os.writeKeyword("B") << species_[b_] << token::END_STATEMENT << nl;

        FixedList<Tuple2<scalar, scalar>, n_> coeffs;

        forAll(coeffs, i)
        {
            coeffs[i].first() = A_[i];
            coeffs[i].second() = Ta_[i];
        }

        os.writeKeyword("coeffs") << coeffs << nl;
    }


    // Ostream Operator

    inline friend Ostream& operator<<
    (
        Ostream& os,
        const LangmuirHinshelwoodReactionRate& lhrr
    )
    {
        lhrr.write(os);
        return os;
    }

};


} // End namespace CML


#endif
