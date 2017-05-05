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
    CML::interpolateSolid

Description
    Helping class for T-interpolated solid thermo.

SourceFiles
    interpolateSolid.cpp

\*---------------------------------------------------------------------------*/

#ifndef interpolateSolid_H
#define interpolateSolid_H
#include "volFields.hpp"
#include "dictionary.hpp"
#include "interpolateXY.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                     Class interpolateSolid Declaration
\*---------------------------------------------------------------------------*/

class interpolateSolid
{
protected:

    // Protected data

        Field<scalar> TValues_;

        Field<scalar> rhoValues_;

        Field<scalar> cpValues_;

        Field<scalar> HfValues_;

        Field<scalar> emissivityValues_;

        Field<scalar> kappaValues_;

        Field<scalar> sigmaSValues_;


public:

    // Constructors

        //- Construct from mesh
        interpolateSolid(const dictionary&);


    //- Destructor
    virtual ~interpolateSolid();


    // Member Functions

        // I-O

            //- Read the interpolateSolid properties
            bool read(const dictionary& dict);

            //- Write the interpolateSolid properties
            bool writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
