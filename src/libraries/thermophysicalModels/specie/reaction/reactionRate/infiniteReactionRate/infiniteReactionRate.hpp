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
    CML::infiniteReactionRate

Description
    infinite reaction rate.


\*---------------------------------------------------------------------------*/

#ifndef infiniteReactionRate_HPP
#define infiniteReactionRate_HPP

#include "scalarField.hpp"
#include "typeInfo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class infiniteReactionRate;

Ostream& operator<<(Ostream&, const infiniteReactionRate&);


/*---------------------------------------------------------------------------*\
                    Class infiniteReactionRate Declaration
\*---------------------------------------------------------------------------*/

class infiniteReactionRate
{
public:


    //- Null constructor
    inline infiniteReactionRate
    ()
    {}

    //- Construct from dictionary
    inline infiniteReactionRate
    (
        const speciesTable& species,
        const dictionary& dict
    )
    {}


    // Member Functions

    //- Return the type name
    static word type()
    {
        return "infinite";
    }

    inline scalar operator()
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        return (1);
    }

    inline scalar ddT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        return (0);
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
    {}


    // Ostream Operator

    inline friend Ostream& operator<<
    (
        Ostream& os,
        const infiniteReactionRate& rr
    )
    {
        rr.write(os);
        return os;
    }
};


} // End namespace CML


#endif
