/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::bladeModel

Description
    Blade model class

SourceFiles
    bladeModel.cpp

\*---------------------------------------------------------------------------*/

#ifndef bladeModel_H
#define bladeModel_H

#include "List.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class bladeModel Declaration
\*---------------------------------------------------------------------------*/

class bladeModel
{

protected:

    // Protected data

        //- Corresponding profile name per section
        List<word> profileName_;

        //- Corresponding profile ID per section
        List<label> profileID_;

        //- Radius [m]
        List<scalar> radius_;

        //- Twist [deg] on input, converted to [rad]
        List<scalar> twist_;

        //- Chord [m]
        List<scalar> chord_;

        //- File name (optional)
        fileName fName_;


    // Protected Member Functions

        //- Return ture if file name is set
        bool readFromFile() const;

        //- Return the interpolation indices and gradient
        void interpolateWeights
        (
            const scalar& xIn,
            const List<scalar>& values,
            label& i1,
            label& i2,
            scalar& ddx
        ) const;


public:

    //- Constructor
    bladeModel(const dictionary& dict);


    //- Destructor
    virtual ~bladeModel();


    // Member functions

        // Access

            //- Return const access to the profile name list
            const List<word>& profileName() const;

            //- Return const access to the profile ID list
            const List<label>& profileID() const;

            //- Return const access to the radius list
            const List<scalar>& radius() const;

            //- Return const access to the twist list
            const List<scalar>& twist() const;

            //- Return const access to the chord list
            const List<scalar>& chord() const;


        // Edit

            //- Return non-const access to the profile ID list
            List<label>& profileID();


        // Evaluation

            //- Return the twist and chord for a given radius
            virtual void interpolate
            (
                const scalar radius,
                scalar& twist,
                scalar& chord,
                label& i1,
                label& i2,
                scalar& invDr
            ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
