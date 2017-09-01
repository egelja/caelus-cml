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
    CML::rawAlphaProbes

Description
    Reads, concatenates and output raw alpha1 probe data.

SourceFiles
    rawAlphaProbes.cpp

Author
    Bjarne Jensen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef rawAlphaProbes_HPP
#define rawAlphaProbes_HPP

#include "postProcessingWaves.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class waveTheory Declaration
\*---------------------------------------------------------------------------*/

class rawAlphaProbes
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
        rawAlphaProbes(const rawAlphaProbes&);

        //- Disallow default bitwise assignment
        void operator=(const rawAlphaProbes&);

    // Private member data

public:

    //- Runtime type information
    TypeName("rawAlphaProbes");

    // Constructors

        //- Construct from components
        rawAlphaProbes
        (
            const Time&,
            const dictionary&,
            const word&
        );


    // Destructor

        virtual ~rawAlphaProbes();

    // Member Functions
        virtual void evaluate();

        void readAlphaProbesData
        (
            List<std::pair<scalar, label> >& timeLabel,
            scalarField& x,
            scalarField& y,
            scalarField& z,
            List<scalarField>& alphas
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
