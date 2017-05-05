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

Class
    CML::JanevReactionRate

Description
    Janev, Langer, Evans and Post reaction rate.

SourceFiles
    JanevReactionRateI.hpp

\*---------------------------------------------------------------------------*/

#ifndef JanevReactionRate_H
#define JanevReactionRate_H

#include "scalarField.hpp"
#include "typeInfo.hpp"
#include "FixedList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class JanevReactionRate Declaration
\*---------------------------------------------------------------------------*/

class JanevReactionRate
{
    // Private data

        scalar A_;
        scalar beta_;
        scalar Ta_;

        static const label nb_ = 9;
        FixedList<scalar, nb_> b_;


public:

    // Constructors

        //- Construct from components
        inline JanevReactionRate
        (
            const scalar A,
            const scalar beta,
            const scalar Ta,
            const FixedList<scalar, nb_> b
        );

        //- Construct from Istream
        inline JanevReactionRate
        (
            const speciesTable& species,
            Istream& is
        );

        //- Construct from dictionary
        inline JanevReactionRate
        (
            const speciesTable& species,
            const dictionary& dict
        );


    // Member Functions

        //- Return the type name
        static word type()
        {
            return "Janev";
        }

        inline scalar operator()
        (
            const scalar T,
            const scalar p,
            const scalarField& c
        ) const;

        //- Write to stream
        inline void write(Ostream& os) const;


    // Ostream Operator

        inline friend Ostream& operator<<
        (
            Ostream&,
            const JanevReactionRate&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "JanevReactionRateI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
