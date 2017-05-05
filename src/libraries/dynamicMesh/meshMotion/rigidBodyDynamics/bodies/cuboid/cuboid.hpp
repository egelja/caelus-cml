/*---------------------------------------------------------------------------*\3
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
    CML::cuboid

Description
    Specialization of rigidBody to construct a cuboid given the mass and
    lengths of the sides.

SourceFiles
    cuboidI.hpp
    cuboid.cpp

\*---------------------------------------------------------------------------*/

#ifndef RBD_cuboid_HPP
#define RBD_cuboid_HPP

#include "rigidBody.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{

/*---------------------------------------------------------------------------*\
                         Class cuboid Declaration
\*---------------------------------------------------------------------------*/

class cuboid
:
    public rigidBody
{
    // Private member data

        //- Lengths of the sides
        vector L_;


    // Private member functions

        // Calculate and return the inertia tensor
        inline symmTensor I(const scalar m, const vector& L) const;


public:

    //- Runtime type information
    TypeName("cuboid");


    // Constructors

        //- Construct from name, mass, centre of mass and lengths of the sides.
        inline cuboid
        (
            const word& name,
            const scalar m,
            const vector& c,
            const vector& L
        );

        //- Construct from dictionary
        inline cuboid
        (
            const word& name,
            const dictionary& dict
        );

        //- Return clone of this cuboid
        virtual autoPtr<rigidBody> clone() const;


    //- Destructor
    virtual ~cuboid();


    // Member Functions

        //- Return the lengths of the sides of the cuboid
        inline const vector& L() const;

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RBD
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "cuboidI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
