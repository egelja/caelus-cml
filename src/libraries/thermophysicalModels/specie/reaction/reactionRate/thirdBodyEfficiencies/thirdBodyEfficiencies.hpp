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
    CML::thirdBodyEfficiencies

Description
    Third body efficiencies

SourceFiles
    thirdBodyEfficienciesI.hpp

\*---------------------------------------------------------------------------*/

#ifndef thirdBodyEfficiencies_H
#define thirdBodyEfficiencies_H

#include "scalarList.hpp"
#include "speciesTable.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class thirdBodyEfficiencies;
Ostream& operator<<(Ostream&, const thirdBodyEfficiencies&);


/*---------------------------------------------------------------------------*\
                  Class thirdBodyEfficiencies Declaration
\*---------------------------------------------------------------------------*/

class thirdBodyEfficiencies
:
    public scalarList
{
    // Private data

        const speciesTable& species_;


public:

    // Constructors

        //- Construct from components
        inline thirdBodyEfficiencies
        (
            const speciesTable& species,
            const scalarList& efficiencies
        );

        //- Construct from Istream
        inline thirdBodyEfficiencies
        (
            const speciesTable& species,
            Istream& is
        );

        //- Construct from dictionary
        inline thirdBodyEfficiencies
        (
            const speciesTable& species,
            const dictionary& dict
        );


    // Member functions

        //- Calculate and return M, the concentration of the third-bodies
        inline scalar M(const scalarList& c) const;

        //- Write to stream
        inline void write(Ostream& os) const;



    // Ostream Operator

        friend Ostream& operator<<(Ostream&, const thirdBodyEfficiencies&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "thirdBodyEfficienciesI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
