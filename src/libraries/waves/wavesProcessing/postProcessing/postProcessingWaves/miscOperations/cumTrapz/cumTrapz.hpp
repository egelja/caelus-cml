/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

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
    CML::cumTrapz

Description
    Perform a cumulative trapezoidal integration of the data set.

SourceFiles
    cumTrapz.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef cumTrapz_HPP
#define cumTrapz_HPP

#include "postProcessingWaves.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class cumTrapz
:
    public postProcessingWaves
{

protected:

    // Protected data
        IOdictionary dataDict_;

        labelList indices_;

    // Protected member functions

private:

    // Private Member Functions
        void evaluateScalar();

        void evaluateVector();

        void writeScalar
        (
            const List<scalarField>&
        );

        void writeVector
        (
            const List<vectorField>&
        );

        //- Disallow default bitwise copy construct
        cumTrapz(const cumTrapz&);

        //- Disallow default bitwise assignment
        void operator=(const cumTrapz&);

    // Private member data

public:

    //- Runtime type information
    TypeName("cumTrapz");

    // Constructors

        //- Construct from components
        cumTrapz
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~cumTrapz();

    // Member Functions
        virtual void evaluate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
