/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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
    CML::procLduInterface

Description
    CML::procLduInterface

SourceFiles
    procLduInterface.cpp

\*---------------------------------------------------------------------------*/

#ifndef procLduInterface_H
#define procLduInterface_H

#include "labelList.hpp"
#include "scalarField.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class lduInterfaceField;

// Forward declaration of friend functions and operators

class procLduInterface;

Ostream& operator<<(Ostream&, const procLduInterface&);


/*---------------------------------------------------------------------------*\
                           Class procLduInterface Declaration
\*---------------------------------------------------------------------------*/

class procLduInterface
{
    // Private data

        labelList faceCells_;
        scalarField coeffs_;
        label myProcNo_;
        label neighbProcNo_;
        label tag_;


    // Private Member Functions

        //- Dissallow construction as copy
        procLduInterface(const procLduInterface&);


public:

    friend class LUscalarMatrix;


    // Constructors

        procLduInterface
        (
            const lduInterfaceField& interface,
            const scalarField& coeffs
        );

        procLduInterface(Istream& is);

        autoPtr<procLduInterface> clone()
        {
            NotImplemented;
            return autoPtr<procLduInterface>(nullptr);
        }

        static autoPtr<procLduInterface> New(Istream& is)
        {
            return autoPtr<procLduInterface>(new procLduInterface(is));
        }


    // Ostream operator

        friend Ostream& operator<<(Ostream&, const procLduInterface&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
