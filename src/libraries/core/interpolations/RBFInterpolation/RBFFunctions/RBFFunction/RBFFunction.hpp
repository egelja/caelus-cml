/*---------------------------------------------------------------------------*\
Copyright 2009 TU Delft
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Class
    RBFFunction

Description
    Radial basis function virtual base class

Author
    Frank Bos, TU Delft.

SourceFiles
    RBFFunction.cpp
    newRBFFunction.cpp

\*---------------------------------------------------------------------------*/

#ifndef RBFFunction_HPP
#define RBFFunction_HPP

#include "typeInfo.hpp"
#include "runTimeSelectionTables.hpp"
#include "tmp.hpp"
#include "autoPtr.hpp"
#include "primitiveFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class RBFFunction Declaration
\*---------------------------------------------------------------------------*/

class RBFFunction
{
    // Private Member Functions

        //- Disallow copy construct
        RBFFunction(const RBFFunction&);

        //- Disallow default bitwise assignment
        void operator=(const RBFFunction&);


public:

    //- Runtime type information
    TypeName("RBFFunction");


    // Declare run-time constructor selection table

        declareRunTimeSelectionTable
        (
            autoPtr,
            RBFFunction,
            dictionary,
            (
                const dictionary& dict
            ),
            (dict)
        );


    // Selectors

        //- Return a pointer to the selected RBF function
        static autoPtr<RBFFunction> New
        (
            const word& type,
            const dictionary& dict
        );


    // Constructors

        //- Construct null
        RBFFunction()
        {}

        //- Create and return a clone
        virtual autoPtr<RBFFunction> clone() const = 0;


    // Destructor

        virtual ~RBFFunction()
        {}


    // Member Functions

        //- Return RBF weights
        virtual tmp<scalarField> weights
        (
            const vectorField& points,
            const vector& controlPoint
        ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
