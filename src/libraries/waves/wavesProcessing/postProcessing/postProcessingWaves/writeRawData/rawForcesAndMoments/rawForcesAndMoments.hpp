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
    CML::rawForcesAndMoments

Description
    Reading the pressure forces from a set of time directories and output
    them in a concatenated manner.

    There is an assumption of one-patch per force file only.

SourceFiles
    rawForcesAndMoments.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef rawForcesAndMoments_HPP
#define rawForcesAndMoments_HPP

#include "postProcessingWaves.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class rawForcesAndMoments
:
    public postProcessingWaves
{

protected:

    // Protected data
        word inputDir_;

        wordList timeDirs_;

        Switch removeDuplicate_;

    // Protected member functions

private:

    // Private Member Functions
        void resizeFields
        (
            List<std::pair<scalar, label> >&,
            vectorField&,
            vectorField&,
            label
        );

        void writeRawData
        (
            const List<std::pair<scalar, label> >&,
            const vectorField&,
            const vectorField&
        );

        //- Disallow default bitwise copy construct
        rawForcesAndMoments(const rawForcesAndMoments&);

        //- Disallow default bitwise assignment
        void operator=(const rawForcesAndMoments&);

    // Private member data

public:

    //- Runtime type information
    TypeName("rawForcesAndMoments");

    // Constructors

        //- Construct from components
        rawForcesAndMoments
        (
            const Time&,
            const dictionary&,
            const word&
        );

    // Destructor

        virtual ~rawForcesAndMoments();

    // Member Functions
        virtual void evaluate();

        void readForceAndMomentData
        (
            List<std::pair<scalar, label> >&,
            vectorField&,
            vectorField&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
