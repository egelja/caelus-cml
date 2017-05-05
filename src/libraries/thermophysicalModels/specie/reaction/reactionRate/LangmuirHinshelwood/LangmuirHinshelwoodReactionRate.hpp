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
    CML::LangmuirHinshelwoodReactionRate

Description
    Power series reaction rate.

SourceFiles
    LangmuirHinshelwoodReactionRateI.hpp

\*---------------------------------------------------------------------------*/

#ifndef LangmuirHinshelwoodReactionRate_H
#define LangmuirHinshelwoodReactionRate_H

#include "scalarField.hpp"
#include "typeInfo.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
               Class LangmuirHinshelwoodReactionRate Declaration
\*---------------------------------------------------------------------------*/

class LangmuirHinshelwoodReactionRate
{
    // Private data

        static const label n_ = 5;
        scalar A_[n_];
        scalar Ta_[n_];

        label co_;
        label c3h6_;
        label no_;


public:

    // Constructors

        //- Construct from components
        inline LangmuirHinshelwoodReactionRate
        (
            const scalar A[],
            const scalar Ta[],
            const label co,
            const label c3h6,
            const label no
        );

        //- Construct from Istream
        inline LangmuirHinshelwoodReactionRate
        (
            const speciesTable& species,
            Istream& is
        );

        //- Construct from dictionary
        inline LangmuirHinshelwoodReactionRate
        (
            const speciesTable& species,
            const dictionary& dict
        );


    // Member Functions

        //- Return the type name
        static word type()
        {
            return "LangmuirHinshelwood";
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
            const LangmuirHinshelwoodReactionRate&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "LangmuirHinshelwoodReactionRateI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
