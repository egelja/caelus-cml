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
    CML::pointDistributions

Description
    Abstract base class for distribution of points on/inside geometric objects.
    This is used for both the definition of wave gauges and point probes.

SourceFiles
    pointDistributions.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef pointDistributions_HPP
#define pointDistributions_HPP

#include "fvMesh.hpp"
#include "fvCFD.hpp"
#include "volFields.hpp"
#include "IOdictionary.hpp"

#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

#include "OFstream.hpp"

#include <fstream>

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class porosityCoefficient Declaration
\*---------------------------------------------------------------------------*/

class pointDistributions
{

protected:

    // Protected data
        const fvMesh& mesh_;
        const dictionary& pointDict_;

    // Protected member functions

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        pointDistributions(const pointDistributions&);

        //- Disallow default bitwise assignment
        void operator=(const pointDistributions&);

public:

    //- Runtime type information
    TypeName("pointDistributions");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            pointDistributions,
            pointDistributions,
            (
                   const fvMesh& mesh,
                const dictionary& dict
            ),
            (mesh, dict)
        );

    // Constructors

        //- Construct from components
        pointDistributions
        (
            const fvMesh&,
            const dictionary&
        );

    // Selectors

        //- Return a reference to the selected turbulence model
        static autoPtr<pointDistributions> New
        (
            const fvMesh&,
            const dictionary&
        );

    // Destructor

        virtual ~pointDistributions() {};

    // Member Functions

        // Access
        virtual pointField evaluate() = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
