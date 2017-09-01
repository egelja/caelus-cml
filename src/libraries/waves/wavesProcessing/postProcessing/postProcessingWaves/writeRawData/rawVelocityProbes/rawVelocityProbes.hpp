/*---------------------------------------------------------------------------*\
Copyright Niels Gjøl Jacobsen, Technical University of Denmark.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

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
    CML::rawVelocityProbes

Description
    Reads, concatenates and output the raw velocity signals.

SourceFiles
    rawVelocityProbes.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef rawVelocityProbes_HPP
#define rawVelocityProbes_HPP

#include "postProcessingWaves.hpp"
#include "coordinateRotation.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class rawVelocityProbes
:
    public postProcessingWaves
{

protected:

    // Protected data
        word inputDir_;

        wordList timeDirs_;

        Switch removeDuplicate_;

        tensor R_;

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
            const scalarField&,
            const scalarField&,
            const scalarField&,
            const List<vectorField>&
        );

        //- Disallow default bitwise copy construct
        rawVelocityProbes(const rawVelocityProbes&);

        //- Disallow default bitwise assignment
        void operator=(const rawVelocityProbes&);

    // Private member data

public:

    //- Runtime type information
    TypeName("rawVelocityProbes");

    // Constructors

        //- Construct from components
        rawVelocityProbes
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~rawVelocityProbes();

    // Member Functions
        virtual void evaluate();

        void readVelocityProbeData
        (
            List<std::pair<scalar, label> >& timeLabel,
            scalarField& x,
            scalarField& y,
            scalarField& z,
            List<vectorField>& etas
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
