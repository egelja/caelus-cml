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
    CML::lduInterface

Description
    An abstract base class for implicitly-coupled interfaces
    e.g. processor and cyclic patches.

SourceFiles
    lduInterface.cpp

\*---------------------------------------------------------------------------*/

#ifndef lduInterface_H
#define lduInterface_H

#include "labelField.hpp"
#include "typeInfo.hpp"
#include "Pstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

/*---------------------------------------------------------------------------*\
                     Class lduInterface Declaration
\*---------------------------------------------------------------------------*/

class lduInterface
{
    // Private Member Functions

        //- Disallow default bitwise copy construct
        lduInterface(const lduInterface&);

        //- Disallow default bitwise assignment
        void operator=(const lduInterface&);


public:

    //- Runtime type information
    TypeName("lduInterface");


    // Constructors

        //- Construct null
        lduInterface()
        {}


    //- Destructor
    virtual ~lduInterface();


    // Member Functions

        // Access

            //- Return faceCell addressing
            virtual const labelUList& faceCells() const = 0;


        // Interface transfer functions

            //- Return the values of the given internal data adjacent to
            //  the interface as a field
            virtual tmp<labelField> interfaceInternalField
            (
                const labelUList& internalData
            ) const = 0;

            //- Initialise transfer of internal field adjacent to the interface
            virtual void initInternalFieldTransfer
            (
                const Pstream::commsTypes commsType,
                const labelUList& iF
            ) const
            {}

            //- Transfer and return internal field adjacent to the interface
            virtual tmp<labelField> internalFieldTransfer
            (
                const Pstream::commsTypes commsType,
                const labelUList& iF
            ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
