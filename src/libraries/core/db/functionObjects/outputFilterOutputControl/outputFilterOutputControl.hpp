/*---------------------------------------------------------------------------* \
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
    CML::outputFilterOutputControl

Description
    An output control for function objects.
    The default is time-step execution at every interval.

SourceFiles
    outputFilterOutputControl.cpp

\*---------------------------------------------------------------------------*/

#ifndef outputFilterOutputControl_H
#define outputFilterOutputControl_H

#include "dictionary.hpp"
#include "Time.hpp"
#include "NamedEnum.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                  Class outputFilterOutputControl Declaration
\*---------------------------------------------------------------------------*/

class outputFilterOutputControl
{
public:

    //- The output control options
    enum outputControls
    {
        ocTimeStep,   /*!< execution is coupled to the time-step */
        ocOutputTime  /*!< execution is coupled to the output-time */
    };


private:

    // Private data

        //- Time object
        const Time& time_;

        //- String representation of outputControls enums
        static const NamedEnum<outputControls, 2> outputControlNames_;

        //- Type of output
        outputControls outputControl_;

        //- The execution interval (in time steps) when using \c timeStep mode,
        //  a value <= 1 means execute at every time step
        label outputInterval_;


    // Private Member Functions

        //- Disallow default bitwise copy construct and assignment
        outputFilterOutputControl(const outputFilterOutputControl&);
        void operator=(const outputFilterOutputControl&);


public:

    // Constructors

        //- Construct from Time object and dictionary
        outputFilterOutputControl(const Time&, const dictionary&);


    //- Destructor
    ~outputFilterOutputControl();


    // Member Functions

        //- Read from dictionary
        void read(const dictionary&);

        //- Return const access to the Time object
        const Time& time() const
        {
            return time_;
        }

        //- Flag to indicate whether to output
        bool output() const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
