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
    CML::rawSurfaceElevation

Description
    Reads, concatenates and output raw surface elevation data.

SourceFiles
    rawSurfaceElevation.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef rawSurfaceElevation_HPP
#define rawSurfaceElevation_HPP

#include "postProcessingWaves.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class rawSurfaceElevation
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
            List<scalarField>&,
            label
        );

        void writeRawData
        (
            const List<std::pair<scalar, label> >&,
            const scalarField&,
            const scalarField&,
            const scalarField&,
            const List<scalarField>&
        );

        //- Disallow default bitwise copy construct
        rawSurfaceElevation(const rawSurfaceElevation&);

        //- Disallow default bitwise assignment
        void operator=(const rawSurfaceElevation&);

    // Private member data

public:

    //- Runtime type information
    TypeName("rawSurfaceElevation");

    // Constructors

        //- Construct from components
        rawSurfaceElevation
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~rawSurfaceElevation();

    // Member Functions
        virtual void evaluate();

        void readSurfaceElevationData
        (
            List<std::pair<scalar, label> >& timeLabel,
            scalarField& x,
            scalarField& y,
            scalarField& z,
            List<scalarField>& etas
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
