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
    CML::solidBodyMotionFunctions::SDA

Description
    Ship design analysis (SDA) 3DoF motion function.

    Comprising sinusoidal roll (rotation about x), heave (z-translation)
    and sway (y-translation) motions with changing amplitude and phase.

See Also
    CML::solidBodyMotionFunctions::tabulated6DoFMotion

SourceFiles
    SDA.cpp

\*---------------------------------------------------------------------------*/

#ifndef SDA_H
#define SDA_H

#include "solidBodyMotionFunction.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace solidBodyMotionFunctions
{

/*---------------------------------------------------------------------------*\
                          Class SDA Declaration
\*---------------------------------------------------------------------------*/

class SDA
:
    public solidBodyMotionFunction
{
    // Private data

        //- Center of gravity
        vector CofG_;

        //- Model scale ratio
        scalar lamda_;

        //- Max roll amplitude [rad]
        scalar rollAmax_;

        //- Min roll amplitude [rad]
        scalar rollAmin_;

        //- Heave amplitude [m]
        scalar heaveA_;

        //- Sway amplitude [m]
        scalar swayA_;

        //- Damping Coefficient [-]
        scalar Q_;

        //- Time Period for liquid [sec]
        scalar Tp_;

        //- Natural Period of Ship [sec]
        scalar Tpn_;

        //- Reference time step [sec]
        scalar dTi_;

        //- Incr. in Tp/unit 'dTi'[-]
        scalar dTp_;


    // Private Member Functions

        //- Disallow copy construct
        SDA(const SDA&);

        //- Disallow default bitwise assignment
        void operator=(const SDA&);


public:

    //- Runtime type information
    TypeName("SDA");


    // Constructors

        //- Construct from components
        SDA
        (
            const dictionary& SBMFCoeffs,
            const Time& runTime,
            const bool incremental
        );


    //- Destructor
    virtual ~SDA();


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
