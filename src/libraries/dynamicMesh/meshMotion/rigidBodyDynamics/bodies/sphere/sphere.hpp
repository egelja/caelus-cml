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
    CML::sphere

Description
    Specialization of rigidBody to construct a sphere given the mass and radius.

SourceFiles
    sphereI.hpp
    sphere.cpp

\*---------------------------------------------------------------------------*/

#ifndef RBD_sphere_HPP
#define RBD_sphere_HPP

#include "rigidBody.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{

/*---------------------------------------------------------------------------*\
                         Class sphere Declaration
\*---------------------------------------------------------------------------*/

class sphere
:
    public rigidBody
{
    // Private member data

        //- Radius
        scalar r_;


    // Private member functions

        // Calculate and return the inertia tensor
        inline symmTensor I(const scalar m, const scalar r) const;


public:

    //- Runtime type information
    TypeName("sphere");


    // Constructors

        //- Construct from name, mass, centre of mass and radius
        inline sphere
        (
            const word& name,
            const scalar m,
            const vector& c,
            const scalar r
        );

        //- Construct from dictionary
        inline sphere
        (
            const word& name,
            const dictionary& dict
        );

        //- Return clone of this sphere
        virtual autoPtr<rigidBody> clone() const;


    //- Destructor
    virtual ~sphere();


    // Member Functions

        //- Return the radius of the sphere
        inline scalar r() const;

        //- Write
        virtual void write(Ostream&) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RBD
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "sphereI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
