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
    CML::solidArrheniusReactionRate

Description
    Arrhenius reaction rate for solids

SourceFiles
    solidArrheniusReactionRateI.hpp

\*---------------------------------------------------------------------------*/

#ifndef solidArrheniusReactionRate_HPP
#define solidArrheniusReactionRate_HPP

#include "scalarField.hpp"
#include "typeInfo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class solidArrheniusReactionRate;

Ostream& operator<<(Ostream&, const solidArrheniusReactionRate&);


/*---------------------------------------------------------------------------*\
                       Class solidArrheniusReactionRate Declaration
\*---------------------------------------------------------------------------*/

class solidArrheniusReactionRate
{

    scalar A_;
    scalar Ta_;
    scalar Tcrit_;


public:

    //- Construct from components
    inline solidArrheniusReactionRate
    (
        const scalar A,
        const scalar Ta,
        const scalar Tcrit
    )
    :
        A_(A),
        Ta_(Ta),
        Tcrit_(Tcrit)
    {}



    //- Construct from dictionary
    inline solidArrheniusReactionRate
    (
        const speciesTable& species,
        const dictionary& dict
    )
    :
        A_(readScalar(dict.lookup("A"))),
        Ta_(readScalar(dict.lookup("Ta"))),
        Tcrit_(readScalar(dict.lookup("Tcrit")))
    {}


    //- Destructor
    virtual ~solidArrheniusReactionRate()
    {}


    // Member Functions

    //- Return the type name
    static word type()
    {
        return "Arrhenius";
    }

    inline scalar operator()
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        if (T < Tcrit_)
        {
            return 0;
        }
        else
        {
            return A_*exp(-Ta_/T);
        }
    }

    inline scalar ddT
    (
        const scalar p,
        const scalar T,
        const scalarField& c
    ) const
    {
        if (T < Tcrit_)
        {
            return 0;
        }
        else
        {
            return A_*exp(-Ta_/T)*Ta_/sqr(T);
        }
    }

    //- Third-body efficiencies (beta = 1-alpha)
    //  non-empty only for third-body reactions
    //  with enhanced molecularity (alpha != 1)
    inline const List<Tuple2<label, scalar> >& beta() const
    {
        return NullSingletonRef<List<Tuple2<label, scalar> > >();
    }

    //- Species concentration derivative of the pressure dependent term
    //  By default this value is 1 as it multiplies the third-body term
    inline void dcidc
    (
        const scalar p,
        const scalar T,
        const scalarField& c,
        scalarField& dcidc
    ) const
    {}

    //- Temperature derivative of the pressure dependent term
    //  By default this value is 0 since ddT of molecularity is approx.0
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
        os.writeKeyword("A") << A_ << token::END_STATEMENT << nl;
        os.writeKeyword("Ta") << Ta_ << token::END_STATEMENT << nl;
        os.writeKeyword("Tcrit") << Tcrit_ << token::END_STATEMENT << nl;
    }


    // Ostream Operator

    inline friend Ostream& operator<<
    (
        Ostream& os,
        const solidArrheniusReactionRate& arr
    )
    {
        arr.write(os);
        return os;
    }

};


} // End namespace CML


#endif
