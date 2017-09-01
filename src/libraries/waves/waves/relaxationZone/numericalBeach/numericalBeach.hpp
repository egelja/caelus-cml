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
    CML::numericalBeaches::numericalBeach

Description
    An abstract base class for different types of numerical beaches

SourceFiles
    numericalBeach.cpp
    newNumericalBeach.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.


\*---------------------------------------------------------------------------*/

#ifndef numericalBeach_HPP
#define numericalBeach_HPP

#include "volFields.hpp"

#include "IOdictionary.hpp"
#include "autoPtr.hpp"
#include "runTimeSelectionTables.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace numericalBeaches
{

/*---------------------------------------------------------------------------*\
                      Class numericalBeach Declaration
\*---------------------------------------------------------------------------*/

class numericalBeach
:
    public IOdictionary
{

protected:

    // Protected data
        const fvMesh& mesh_;

        const dictionary coeffDict_;

        word beachType_;

private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        numericalBeach(const numericalBeach&);

        //- Disallow default bitwise assignment
        void operator=(const numericalBeach&);

public:

    //- Runtime type information
    TypeName("numericalBeach");

    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            numericalBeach,
            dictionary,
            (
                const word& subDictName, const fvMesh& mesh_
            ),
            (subDictName, mesh_)
        );

    // Constructors

        //- Construct from components
        numericalBeach
        (
            const word& type,
            const fvMesh& mesh_
        );


    // Selectors

        //- Return a reference to the selected turbulence model
        static autoPtr<numericalBeach> New
        (
            const word& subDictName,
            const fvMesh& mesh_
        );

    // Destructor

        virtual ~numericalBeach();


    // Member Functions

        virtual void correct
        (
            const labelList&,
            const scalarField&,
            volScalarField&
        ) = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace relaxationShapes
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
