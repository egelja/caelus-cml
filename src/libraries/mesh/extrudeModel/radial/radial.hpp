/*---------------------------------------------------------------------------*\
Copyright (C) 2013 OpenFOAM Foundation
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
    CML::extrudeModels::radial

Description

\*---------------------------------------------------------------------------*/

#ifndef radial_H
#define radial_H

#include "extrudeModel.hpp"
#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace extrudeModels
{

/*---------------------------------------------------------------------------*\
                           Class radial Declaration
\*---------------------------------------------------------------------------*/

class radial
:
    public extrudeModel
{
    // Private data

        autoPtr<DataEntry<scalar> > R_;


public:

    //- Runtime type information
    TypeName("radial");

    // Constructors

        //- Construct from dictionary
        radial(const dictionary& dict);


    //-Destructor
    virtual ~radial();


    // Member Operators

        point operator()
        (
            const point& surfacePoint,
            const vector& surfaceNormal,
            const label layer
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace extrudeModels
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
