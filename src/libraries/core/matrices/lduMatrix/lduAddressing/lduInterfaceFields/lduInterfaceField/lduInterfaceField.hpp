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
    CML::lduInterfaceField

Description
    An abstract base class for implicitly-coupled interface fields
    e.g. processor and cyclic patch fields.

SourceFiles
    lduInterfaceField.cpp

\*---------------------------------------------------------------------------*/

#ifndef lduInterfaceField_H
#define lduInterfaceField_H

#include "lduInterface.hpp"
#include "primitiveFieldsFwd.hpp"
#include "Pstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class lduMatrix;

/*---------------------------------------------------------------------------*\
                     Class lduInterfaceField Declaration
\*---------------------------------------------------------------------------*/

class lduInterfaceField
{
    // Private data

        //- Reference to the coupled patch this field is defined for
        const lduInterface& interface_;


    // Private Member Functions

        //- Disallow default bitwise copy construct
        lduInterfaceField(const lduInterfaceField&);

        //- Disallow default bitwise assignment
        void operator=(const lduInterfaceField&);


public:

    //- Runtime type information
    TypeName("lduInterfaceField");


    // Constructors

        //- Construct given coupled patch
        lduInterfaceField(const lduInterface& patch)
        :
            interface_(patch)
        {}


    //- Destructor
    virtual ~lduInterfaceField();


    // Member Functions

        // Access

            //- Return the interface
            const lduInterface& interface() const
            {
                return interface_;
            }

            //- Return the interface type
            virtual const word& interfaceFieldType() const
            {
                return type();
            }


        // Coupled interface matrix update

            //- Initialise neighbour matrix update
            virtual void initInterfaceMatrixUpdate
            (
                const scalarField&,
                scalarField&,
                const lduMatrix&,
                const scalarField&,
                const direction,
                const Pstream::commsTypes commsType
            ) const
            {}

            //- Update result field based on interface functionality
            virtual void updateInterfaceMatrix
            (
                const scalarField&,
                scalarField&,
                const lduMatrix&,
                const scalarField&,
                const direction,
                const Pstream::commsTypes commsType
            ) const = 0;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
