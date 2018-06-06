/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2017 OpenFOAM Foundation
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
    CML::fv::limitVelocity

Description
    Limits the maximum velocity magnitude to the specified \c max value.

Usage
    Example usage:
    \verbatim
    limitU
    {
        type            limitVelocity;
        active          yes;

        selectionMode   all;
        max             100;
    }
    \endverbatim

SourceFiles
    limitVelocity.cpp

\*---------------------------------------------------------------------------*/

#ifndef limitVelocity_HPP
#define limitVelocity_HPP

#include "fvOption.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fv
{

/*---------------------------------------------------------------------------*\
                 Class limitVelocity Declaration
\*---------------------------------------------------------------------------*/

class limitVelocity
:
    public option
{

protected:

    // Protected data

        //- Velocity field name, default = U
        word UName_;

        //- Maximum velocity magnitude
        scalar max_;


private:

    // Private Member Functions

        //- Disallow default bitwise copy construct
        limitVelocity(const limitVelocity&);

        //- Disallow default bitwise assignment
        void operator=(const limitVelocity&);


public:

    //- Runtime type information
    TypeName("limitVelocity");


    // Constructors

        //- Construct from components
        limitVelocity
        (
            const word& name,
            const word& modelType,
            const dictionary& dict,
            const fvMesh& mesh
        );


    //- Destructor
    virtual ~limitVelocity()
    {}


    // Member Functions

        //- Read dictionary
        virtual bool read(const dictionary& dict);

        //- Correct the velocity field
        virtual void correct(volVectorField& U);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace fv
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //