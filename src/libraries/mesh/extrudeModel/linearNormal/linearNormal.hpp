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
    CML::extrudeModels::linearNormal

Description
    Extrudes by transforming points normal to the surface by a given distance

\*---------------------------------------------------------------------------*/

#ifndef linearNormal_H
#define linearNormal_H

#include "point.hpp"
#include "extrudeModel.hpp"
#include "scalarList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace extrudeModels
{

/*---------------------------------------------------------------------------*\
                    Class linearNormal Declaration
\*---------------------------------------------------------------------------*/

class linearNormal
:
    public extrudeModel
{
    // Private data

        //- Layer thickness
        scalar thickness_;

        //- First cell thickness
        scalar firstCellThickness_;

        //- Layer cell distibution
        scalarList layerPoints_;



public:

    //- Runtime type information
    TypeName("linearNormal");

    // Constructors

        //- Construct from dictionary
        linearNormal(const dictionary& dict);


    //- Destructor
    virtual ~linearNormal();


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
