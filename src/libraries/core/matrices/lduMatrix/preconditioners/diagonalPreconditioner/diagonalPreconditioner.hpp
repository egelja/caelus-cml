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
    CML::diagonalPreconditioner

Description
    Diagonal preconditioner for both symmetric and asymmetric matrices.

    The reciprocal of the diagonal is calculated and stored for reuse
    because on most systems '*' is faster than '/'.

SourceFiles
    diagonalPreconditioner.cpp

\*---------------------------------------------------------------------------*/

#ifndef diagonalPreconditioner_H
#define diagonalPreconditioner_H

#include "lduMatrix.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class diagonalPreconditioner Declaration
\*---------------------------------------------------------------------------*/

class diagonalPreconditioner
:
    public lduMatrix::preconditioner
{
    // Private data

        //- The reciprocal diagonal
        scalarField rD;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        diagonalPreconditioner(const diagonalPreconditioner&);

        //- Disallow default bitwise assignment
        void operator=(const diagonalPreconditioner&);


public:

    //- Runtime type information
    TypeName("diagonal");


    // Constructors

        //- Construct from matrix components and preconditioner solver controls
        diagonalPreconditioner
        (
            const lduMatrix::solver&,
            const dictionary& solverControlsUnused
        );


    //- Destructor
    virtual ~diagonalPreconditioner()
    {}


    // Member Functions

        //- Return wA the preconditioned form of residual rA
        virtual void precondition
        (
            scalarField& wA,
            const scalarField& rA,
            const direction cmpt=0
        ) const;

        //- Return wT the transpose-matrix preconditioned form of residual rT.
        virtual void preconditionT
        (
            scalarField& wT,
            const scalarField& rT,
            const direction cmpt=0
        ) const
        {
            return precondition(wT, rT, cmpt);
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
