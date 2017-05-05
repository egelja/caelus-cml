/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014 - 2016 Applied CCM
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

    Preconditioned BiCG solver derived from the general preconditioned 
    BiCG solver PBiCG but with the choice of preconditioner
    pre-selected. This solver is intended for the solution of the 
    coarsest level set of equation in GAMG solver

SourceFiles

    CoarsePBiCG.cpp

Modifications

    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef CoarsePBiCG_H
#define CoarsePBiCG_H

#include "PBiCG.hpp"

namespace CML
{

class CoarsePBiCG : public PBiCG
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    CoarsePBiCG(CoarsePBiCG const&);

    //- Disallow default bitwise assignment
    void operator=(CoarsePBiCG const&);

public:

    //- Return the dictionary constructed from the components.
    //  Needed for backward compatibility
    static dictionary solverDict
    (
        scalar const tol,
        scalar const relTol
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
        word const& fieldName,
        lduMatrix const& matrix,
        FieldField<Field, scalar> const& interfaceBouCoeffs,
        FieldField<Field, scalar> const& interfaceIntCoeffs,
        lduInterfaceFieldPtrsList const& interfaces,
        dictionary const& solverControls
    );

    //- Construct from matrix components and tolerances
    CoarsePBiCG
    (
        word const& fieldName,
        lduMatrix const& matrix,
        FieldField<Field, scalar> const& interfaceBouCoeffs,
        FieldField<Field, scalar> const& interfaceIntCoeffs,
        lduInterfaceFieldPtrsList const& interfaces,
        scalar const tolerance,
        scalar const relTol = 0.0
    );

    //- Destructor
    virtual ~CoarsePBiCG() {}

};


}

#endif

