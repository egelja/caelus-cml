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
    CML::GAMGPreconditioner

Description
    Geometric agglomerated algebraic multigrid preconditioner.

See Also
    GAMGSolver for more details.

SourceFiles
    GAMGPreconditioner.cpp

\*---------------------------------------------------------------------------*/

#ifndef GAMGPreconditioner_H
#define GAMGPreconditioner_H

#include "GAMGSolver.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class GAMGPreconditioner Declaration
\*---------------------------------------------------------------------------*/

class GAMGPreconditioner
:
    public GAMGSolver,
    public lduMatrix::preconditioner
{
protected:
    // Protected data

        //- Number of V-cycles to perform
        label nVcycles_;

        //- Read the control parameters from the controlDict_
        virtual void readControls();

public:

    //- Runtime type information
    TypeName("GAMG");


    // Constructors

        //- Construct from matrix components and preconditioner solver controls
        GAMGPreconditioner
        (
            const lduMatrix::solver&,
            const dictionary& solverControls
        );


    //- Destructor
    virtual ~GAMGPreconditioner();


    // Member Functions

        //- Return wA the preconditioned form of residual rA
        virtual void precondition
        (
            scalarField& wA,
            const scalarField& rA,
            const direction cmpt=0
        ) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
