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
    CML::rawOvertopping

Description
    Reads, concatenates and output raw overtopping data

SourceFiles
    rawOvertopping.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef rawOvertopping_HPP
#define rawOvertopping_HPP

#include "postProcessingWaves.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class rawOvertopping
:
    public postProcessingWaves
{

protected:

    // Protected data
        word inputDir_;

        wordList timeDirs_;

        scalar scaleFlux_;

        Switch removeDuplicate_;

    // Protected member functions

private:

    // Private Member Functions
        void resizeFields
        (
            List<std::pair<scalar, label> >&,
            List<vectorField>&,
            label
        );

        void writeRawData
        (
            const List<std::pair<scalar, label> >&,
            const wordList&,
            const List<vectorField>&
        );

        //- Disallow default bitwise copy construct
        rawOvertopping(const rawOvertopping&);

        //- Disallow default bitwise assignment
        void operator=(const rawOvertopping&);

    // Private member data

public:

    //- Runtime type information
    TypeName("rawOvertopping");

    // Constructors

        //- Construct from components
        rawOvertopping
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~rawOvertopping();

    // Member Functions
        virtual void evaluate();

        void readOvertoppingData
        (
            List<std::pair<scalar, label> >&,
            wordList&,
            List<vectorField>&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
