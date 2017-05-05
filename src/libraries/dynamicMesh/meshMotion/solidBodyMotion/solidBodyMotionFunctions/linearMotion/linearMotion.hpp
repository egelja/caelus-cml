/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

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
    CML::solidBodyMotionFunctions::linearMotion

Description
    SolidBodyMotionFvMesh 6DoF motion function. Constant velocity displacement.

SourceFiles
    linearMotion.cpp

\*---------------------------------------------------------------------------*/

#ifndef linearMotion_H
#define linearMotion_H

#include "solidBodyMotionFunction.hpp"
#include "primitiveFields.hpp"
#include "point.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace solidBodyMotionFunctions
{

/*---------------------------------------------------------------------------*\
                          Class linearMotion Declaration
\*---------------------------------------------------------------------------*/

class linearMotion
:
    public solidBodyMotionFunction
{
    // Private data

        //- Linear velocity
        vector velocity_;


    // Private Member Functions

        //- Disallow copy construct
        linearMotion(const linearMotion&);

        //- Disallow default bitwise assignment
        void operator=(const linearMotion&);


public:

    //- Runtime type information
    TypeName("linearMotion");


    // Constructors

        //- Construct from components
        linearMotion
        (
            const dictionary& SBMFCoeffs,
            const Time& runTime,
            const bool incremental
        );


    //- Destructor
    virtual ~linearMotion();


    // Member Functions

        //- Return the solid-body motion transformation septernion
        virtual septernion transformation() const;

        //- Update properties from given dictionary
        virtual bool read(const dictionary& SBMFCoeffs);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace solidBodyMotionFunctions
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
