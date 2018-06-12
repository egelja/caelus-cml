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
    CML::numericalBeaches::numericalBeachEmpty

Description
    A dummy numerical beach to be used, if a relaxation zone should not include
    artificial viscosity contributions in the momentum equation

SourceFiles
    numericalBeachEmpty.cpp

Author
    Niels Gjøl Jacobsen, Technical University of Denmark.

\*---------------------------------------------------------------------------*/

#ifndef numericalBeachEmpty_HPP
#define numericalBeachEmpty_HPP

#include "numericalBeach.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace numericalBeaches
{

/*---------------------------------------------------------------------------*\
                     Class numericalBeachEmpty Declaration
\*---------------------------------------------------------------------------*/

class numericalBeachEmpty
:
    public numericalBeach
{

protected:


public:

    //- Runtime type information
    TypeName("numericalBeachEmpty");

    // Constructors

        //- from components
        numericalBeachEmpty
        (
            const word& ,
            const fvMesh& mesh_
        );


    // Destructor

        ~numericalBeachEmpty()
        {}


    // Member Functions

        virtual void correct
        (
            const labelList&,
            const scalarField&,
            volScalarField&
        );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace numericalBeaches
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
