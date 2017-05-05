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
    CML::solidArrheniusReactionRate

Description
    Arrhenius reaction rate for solids

SourceFiles
    solidArrheniusReactionRateI.hpp

\*---------------------------------------------------------------------------*/

#ifndef solidArrheniusReactionRate_H
#define solidArrheniusReactionRate_H

#include "scalarField.hpp"
#include "typeInfo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class solidArrheniusReactionRate Declaration
\*---------------------------------------------------------------------------*/

class solidArrheniusReactionRate
{
    // Private data

        scalar A_;
        scalar Ta_;
        scalar Tcrit_;


public:

    // Constructors

        //- Construct from components
        inline solidArrheniusReactionRate
        (
            const scalar A,
            const scalar Ta,
            const scalar Tcrit
        );

        //- Construct from Istream
        inline solidArrheniusReactionRate
        (
            const speciesTable& species,
            Istream& is
        );


    // Member Functions

        //- Return the type name
        static word type()
        {
            return "SolidArrhenius";
        }

        inline scalar operator()
        (
            const scalar T,
            const scalar p,
            const scalarField& c
        ) const;


    // Ostream Operator

        inline friend Ostream& operator<<
        (
            Ostream&,
            const solidArrheniusReactionRate&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "solidArrheniusReactionRateI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
