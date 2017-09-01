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
    CML::reflectionAnalysis2DLS

Description
    Performs a reflection analysis for a regular signal. It is based on a
    least-squares approach and the wave number is given by the user.

SourceFiles
    reflectionAnalysis2DLS.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef reflectionAnalysis2DLS_HPP
#define reflectionAnalysis2DLS_HPP

#include "postProcessingWaves.hpp"

#include "spectralMethodsLeastSquaresBased.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class reflectionAnalysis2DLS
:
    public postProcessingWaves
{

protected:

    // Protected data
        IOdictionary dataDict_;

        labelList indices_;

        label N_;

        scalar period_;

        vector waveNumber_;

        pointField x_;

    // Protected member functions

private:

    // Private Member Functions
        void write
        (
            const scalarField&,
            const scalarField&
        );

        //- Disallow default bitwise copy construct
        reflectionAnalysis2DLS(const reflectionAnalysis2DLS&);

        //- Disallow default bitwise assignment
        void operator=(const reflectionAnalysis2DLS&);

    // Private member data

public:

    //- Runtime type information
    TypeName("reflectionAnalysis2DLS");

    // Constructors

        //- Construct from components
        reflectionAnalysis2DLS
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~reflectionAnalysis2DLS();

    // Member Functions
        virtual void evaluate();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
