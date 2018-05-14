/*---------------------------------------------------------------------------*\
Copyright (C) 2010 I. Clifford
Copyright (C) 2013 K. Jareteg
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    BlockLduInterfaceField

Description
    Abstract base class for interface fields with block coefficients

    This file was obtained from Ivor Clifford at the OpenFOAM Workshop
    in PennState, USA, 2011.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Ivor Clifford, ivor.clifford@gmail.com
    Klas Jareteg

\*---------------------------------------------------------------------------*/

#ifndef BlockLduInterfaceField_HPP
#define BlockLduInterfaceField_HPP

#include "lduInterfaceField.hpp"


namespace CML
{

template<class Type>
class BlockLduMatrix;

template<class Type>
class CoeffField;

/*---------------------------------------------------------------------------*\
                    Class BlockLduInterfaceField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockLduInterfaceField
:
    public lduInterfaceField
{

    //- Disallow default bitwise copy construct
    BlockLduInterfaceField(const BlockLduInterfaceField&);

    //- Disallow default bitwise assignment
    void operator=(const BlockLduInterfaceField&);


public:

    //- Runtime Type information
    TypeName("BlockLduInterfaceField");


    //- Construct given coupled patch
    BlockLduInterfaceField(const lduInterface& patch)
    :
        lduInterfaceField(patch)
    {}

    virtual ~BlockLduInterfaceField()
    {}

    // Member Functions
    // Coupled interface matrix update, scalar matrices
    // Used to override base class lduInterfaceField virtual functions

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
    ) const
    {}

   //KRJ: 2013-02-08: Must be virtual!
   // - Initialise neighbour matrix update
    virtual void initInterfaceMatrixUpdate
    (
        Field<Type>&,
        const Field<Type>&,
        const BlockLduMatrix<Type>&,
        const CoeffField<Type>&,
        const Pstream::commsTypes commsType
    ) const = 0;

    //- Update result field based on interface functionality
    virtual void updateInterfaceMatrix
    (
        Field<Type>&,
        const Field<Type>&,
        const BlockLduMatrix<Type>&,
        const CoeffField<Type>&,
        const Pstream::commsTypes commsType
    ) const = 0;

};


} // End namespace CML
#endif
