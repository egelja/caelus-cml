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
    CML::trapz

Description
    Perform a trapezoidal integration of the data set.

SourceFiles
    trapz.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef trapz_HPP
#define trapz_HPP

#include "postProcessingWaves.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class trapz
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
            const scalarField&
        );

        void writeVector
        (
            const vectorField&
        );

        //- Disallow default bitwise copy construct
        trapz(const trapz&);

        //- Disallow default bitwise assignment
        void operator=(const trapz&);

    // Private member data

public:

    //- Runtime type information
    TypeName("trapz");

    // Constructors

        //- Construct from components
        trapz
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~trapz();

    // Member Functions
        virtual void evaluate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
