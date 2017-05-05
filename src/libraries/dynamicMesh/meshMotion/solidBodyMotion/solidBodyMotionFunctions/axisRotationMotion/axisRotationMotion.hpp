/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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
    CML::solidBodyMotionFunctions::axisRotationMotion

Description
    Constant velocity rotation around CoG. Similar to rotatingMotion but
    motion specified as rotation vector.

SourceFiles
    axisRotationMotion.cpp

\*---------------------------------------------------------------------------*/

#ifndef axisRotationMotion_H
#define axisRotationMotion_H

#include "solidBodyMotionFunction.hpp"
#include "primitiveFields.hpp"
#include "point.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace solidBodyMotionFunctions
{

/*---------------------------------------------------------------------------*\
                          Class axisRotationMotion Declaration
\*---------------------------------------------------------------------------*/

class axisRotationMotion
:
    public solidBodyMotionFunction
{
    // Private data

        //- Origin
        point origin_;

        //- Rotational velocity (deg/s)
        vector radialVelocity_;


    // Private Member Functions

        //- Disallow copy construct
        axisRotationMotion(const axisRotationMotion&);

        //- Disallow default bitwise assignment
        void operator=(const axisRotationMotion&);


public:

    //- Runtime type information
    TypeName("axisRotationMotion");


    // Constructors

        //- Construct from components
        axisRotationMotion
        (
            const dictionary& SBMFCoeffs,
            const Time& runTime,
            const bool incremental
        );


    //- Destructor
    virtual ~axisRotationMotion();


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
