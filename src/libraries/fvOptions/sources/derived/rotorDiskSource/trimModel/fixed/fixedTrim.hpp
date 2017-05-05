/*---------------------------------------------------------------------------*\
Copyright (C) 2012 OpenFOAM Foundation
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
    CML::fixedTrim

Description
    Fixed trim coefficients

SourceFiles
    fixedTrim.cpp

\*---------------------------------------------------------------------------*/

#ifndef fixedTrim_H
#define fixedTrim_H

#include "trimModel.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class fixedTrim Declaration
\*---------------------------------------------------------------------------*/

class fixedTrim
:
    public trimModel
{

protected:

    // Protected data

        //- Geometric angle of attack [rad]
        scalarField thetag_;


public:

    //- Run-time type information
    TypeName("fixedTrim");

    //- Constructor
    fixedTrim(const fv::rotorDiskSource& rotor, const dictionary& dict);

    //- Destructor
    virtual ~fixedTrim();


    // Member functions

        //- Read
        void read(const dictionary& dict);

        //- Return the geometric angle of attack [rad]
        virtual tmp<scalarField> thetag() const;

        //- Correct the model
        virtual void correct(const vectorField& U, vectorField& force);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
