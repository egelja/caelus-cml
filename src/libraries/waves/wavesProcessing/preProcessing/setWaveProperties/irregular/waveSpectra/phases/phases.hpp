/*---------------------------------------------------------------------------*\
Copyright  Niels Gjøl Jacobsen, Technical University of Denmark.
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
    CML::phases

Description
    An abstract base class for setting the phases for irregular wave trains.

SourceFiles
    phases.cpp

Author
    Niels Gjøl Jacobsen, Deltares.


\*---------------------------------------------------------------------------*/

#ifndef phases_HPP
#define phases_HPP

#include "fvMesh.hpp"
#include "volFields.hpp"
#include "IOdictionary.hpp"

#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

#include "setWaveProperties.hpp"

#include "uniformDimensionedFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
/*---------------------------------------------------------------------------*\
                       Class phases Declaration
\*---------------------------------------------------------------------------*/

class phases
{

protected:

    // Protected data
        const Time& rT_;

        dictionary& dict_;

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        phases(const phases&);

        //- Disallow default bitwise assignment
        void operator=(const phases&);

public:

    //- Runtime type information
    TypeName("phases");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            phases,
            phases,
            (
                const Time& rT,
                dictionary& dict
            ),
            (rT, dict)
        );

    // Constructors

        //- Construct from components
        phases
        (
            const Time&,
            dictionary&
        );

    // Selectors

        //- Return a reference to the selected turbulence model
        static autoPtr<phases> New
        (
            const Time&,
            dictionary&
        );

    // Destructor

        virtual ~phases();

    // Member functions

        virtual scalar phase(const scalar&, const vector&) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
