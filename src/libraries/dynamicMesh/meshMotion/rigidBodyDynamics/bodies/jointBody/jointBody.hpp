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
    CML::jointBody

Description

SourceFiles
    jointBodyI.hpp
    jointBody.cpp

\*---------------------------------------------------------------------------*/

#ifndef RBD_jointBody_HPP
#define RBD_jointBody_HPP

#include "masslessBody.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace RBD
{

/*---------------------------------------------------------------------------*\
                         Class jointBody Declaration
\*---------------------------------------------------------------------------*/

class jointBody
:
    public masslessBody
{

public:

    //- Runtime type information
    TypeName("jointBody");


    // Constructors

        //- Construct a joint body
        inline jointBody();

        //- Construct a named joint body
        inline jointBody(const word& name);

        //- Construct from dictionary
        inline jointBody
        (
            const word& name,
            const dictionary& dict
        );

        //- Return clone of this jointBody
        virtual autoPtr<rigidBody> clone() const;


    //- Destructor
    virtual ~jointBody();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace RBD
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "jointBodyI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
