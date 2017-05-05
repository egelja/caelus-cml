/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
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
    CML::RBD::joints::Pxyz

Description
    Prismatic joint for translation in the x/y/z directions.

    Reference:
    \verbatim
        Featherstone, R. (2008).
        Rigid body dynamics algorithms.
        Springer.
        Chapter 4.
    \endverbatim

SourceFiles
    Pxyz.cpp

\*---------------------------------------------------------------------------*/

#ifndef RBD_joints_Pxyz_HPP
#define RBD_joints_Pxyz_HPP

#include "joint.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{
namespace joints
{

/*---------------------------------------------------------------------------*\
                         Class Pxyz Declaration
\*---------------------------------------------------------------------------*/

class Pxyz
:
    public joint
{

public:

    //- Runtime type information
    TypeName("Pxyz");


    // Constructors

        //- Construct for given model
        Pxyz();

        //- Construct for given model from dictionary
        Pxyz(const dictionary& dict);

        //- Clone this joint
        virtual autoPtr<joint> clone() const;


    //- Destructor
    virtual ~Pxyz();


    // Member Functions

        //- Update the model state for this joint
        virtual void jcalc
        (
            joint::XSvc& J,
            const scalarField& q,
            const scalarField& qDot
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace joints
} // End namespace RBD
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
