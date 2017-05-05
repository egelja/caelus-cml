/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 Applied CCM
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
    CML::CoarsePBiCG

Description
    Diagonal incomplete LU preconditioned BiCG solver derived from the general
    preconditioned BiCG solver PBiCG but with the choice of preconditioner
    pre-selected. This solver is intended for the solution of the coarsest 
    level set of equation in GAMG solver

SourceFiles
    CoarsePBiCG.cpp

\*---------------------------------------------------------------------------*/

#ifndef CoarsePBiCG_H
#define CoarsePBiCG_H

#include "PBiCG.hpp"

namespace CML
{

class CoarsePBiCG
:
    public PBiCG
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        CoarsePBiCG(const CoarsePBiCG&);

        //- Disallow default bitwise assignment
        void operator=(const CoarsePBiCG&);

public:

    //- Return the dictionary constructed from the components.
    //  Needed for backward compatibility
    static dictionary solverDict
    (
        const scalar tol,
        const scalar relTol
    );

    //- Return the dictionary constructed from the old-style data-stream.
    //  Needed for backward compatibility
    static dictionary solverDict(Istream&);


    //- Runtime type information
    TypeName("CoarsePBiCG");


    // Constructors

        //- Construct from matrix components and solver data stream
        CoarsePBiCG
        (
            const word& fieldName,
            const lduMatrix& matrix,
            const FieldField<Field, scalar>& interfaceBouCoeffs,
            const FieldField<Field, scalar>& interfaceIntCoeffs,
            const lduInterfaceFieldPtrsList& interfaces,
            const dictionary& solverControls
        );


        //- Construct from matrix components and tolerances
        CoarsePBiCG
        (
            const word& fieldName,
            const lduMatrix& matrix,
            const FieldField<Field, scalar>& interfaceBouCoeffs,
            const FieldField<Field, scalar>& interfaceIntCoeffs,
            const lduInterfaceFieldPtrsList& interfaces,
            const scalar tolerance,
            const scalar relTol = 0.0
        );

    //- Destructor
    virtual ~CoarsePBiCG()
    {}

};


}

#endif

