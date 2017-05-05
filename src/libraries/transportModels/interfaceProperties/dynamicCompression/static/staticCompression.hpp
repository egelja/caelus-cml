/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2017 Applied CCM
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
    CML::staticCompression

Description
    No dynamic compression model - returns constant value of cAlpha

Author
    Darrin Stephens

SourceFiles
    staticCompression.cpp

\*---------------------------------------------------------------------------*/

#ifndef staticCompression_HPP
#define staticCompression_HPP

#include "dynCompressionModel.hpp"
#include "volFields.hpp"
#include "surfaceFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class staticCompression Declaration
\*---------------------------------------------------------------------------*/

class staticCompression
:
    public dynCompressionModel
{
    // Private data

        //- Interface compression on faces
        surfaceScalarField cAlphaf_;

        //- Interface compression in cells
        volScalarField cAlpha_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        staticCompression(const staticCompression&);

        //- Disallow default bitwise assignment
        void operator=(const staticCompression&);


public:

    //- Runtime type information
    TypeName("none");

    // Constructors

        //- Construct from components
        staticCompression
        (
            const fvMesh& mesh,
            const volVectorField& U,
            const surfaceScalarField& phi,
            const volScalarField& alpha1
        );


    // Destructor

        virtual ~staticCompression()
        {}


    // Member Functions

        //- Return the interface compression coefficient at face centres
        const surfaceScalarField& cAlphaf() const
        {
            return cAlphaf_;
        }

        //- Return the interface compression coefficient at cell centres
        const volScalarField& cAlpha() const
        {
            return cAlpha_;
        }

        //- Correct the interface compression coefficient
        void correct()
        {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
