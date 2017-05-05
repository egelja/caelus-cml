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

    CML::CoarsePCG

Description

    Preconditioned CG solver derived from the general preconditioned CG 
    solver PCG but with the choice of preconditioner pre-selected. 
    This solver is intendend for the solution of the corsest
    level set of equations in GAMG solver.

SourceFiles

    CoarsePCG.cpp

Modifictions 

    Aleksandar Jemcov

\*---------------------------------------------------------------------------*/

#ifndef CoarsePCG_HPP
#define CoarsePCG_HPP

#include "PCG.hpp"

namespace CML
{

class CoarsePCG : public PCG
{
    // Private Member Functions

    //- Disallow default bitwise copy construct
    CoarsePCG(CoarsePCG const&);

    //- Disallow default bitwise assignment
    void operator=(CoarsePCG const&);

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
    TypeName("CoarsePCG");

    // Constructors

    //- Construct from matrix components and solver data stream
    CoarsePCG
    (
        word const& fieldName,
        lduMatrix const& matrix,
        FieldField<Field, scalar> const& interfaceBouCoeffs,
        FieldField<Field, scalar> const& interfaceIntCoeffs,
        lduInterfaceFieldPtrsList const& interfaces,
        dictionary const& solverControls
    );

    //- Construct from matrix components and tolerances
    CoarsePCG
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
    virtual ~CoarsePCG() {}

};


} // End namespace CML

#endif


