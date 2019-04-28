/*---------------------------------------------------------------------------*\
Copyright (C) 2004-6 H. Jasak
Copyright (C) 2018 Applied CCM Pty Ltd
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
    BlockDiagonalPrecon

Description
    Diagonal preconditioning

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved.

\*---------------------------------------------------------------------------*/

#ifndef BlockDiagonalPrecon_HPP
#define BlockDiagonalPrecon_HPP

#include "BlockLduPrecon.hpp"
#include "error.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                      Class BlockDiagonalPrecon Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockDiagonalPrecon
:
    public BlockLduPrecon<Type>
{

    //- Disallow default bitwise copy construct
    BlockDiagonalPrecon(const BlockDiagonalPrecon&);

    //- Disallow default bitwise assignment
    void operator=(const BlockDiagonalPrecon&);


public:

    //- Runtime type information
    TypeName("diagonal");

    //- Construct from components
    BlockDiagonalPrecon
    (
        const BlockLduMatrix<Type>& matrix,
        const dictionary& dict
    )
    :
        BlockLduPrecon<Type>(matrix)
    {}

    virtual ~BlockDiagonalPrecon()
    {}

    // Member Functions
    //- Execute preconditioning
    virtual void precondition
    (
        Field<Type>& x,
        const Field<Type>& b
    ) const;

    //- Execute preconditioning on a transposed matrix
    virtual void preconditionT
    (
        Field<Type>& xT,
        const Field<Type>& bT
    ) const
    {
        return precondition(xT, bT);
    }
};

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void BlockDiagonalPrecon<Type>::precondition
(
    Field<Type>& x,
    const Field<Type>& b
) const
{
    typedef CoeffField<Type> TypeCoeffField;

    typedef typename TypeCoeffField::scalarTypeField scalarTypeField;
    typedef typename TypeCoeffField::linearTypeField linearTypeField;
    typedef typename TypeCoeffField::squareTypeField squareTypeField;

    const TypeCoeffField& diag = this->matrix_.diag();

    if (diag.activeType() == blockCoeffBase::SCALAR)
    {
        const scalarTypeField& activeDiag = diag.asScalar();

        forAll (x, i)
        {
            x[i] = b[i]/activeDiag[i];
        }
    }
    else if (diag.activeType() == blockCoeffBase::LINEAR)
    {
        const linearTypeField& activeDiag = diag.asLinear();

        forAll (x, i)
        {
            x[i] = cmptDivide(b[i], activeDiag[i]);
        }
    }
    else if (diag.activeType() == blockCoeffBase::SQUARE)
    {
        const squareTypeField& activeDiag = diag.asSquare();

        forAll (x, i)
        {
            x[i] = (b[i] & inv(activeDiag[i]));
        }
    }
    else
    {
        FatalErrorInFunction
            << "Problem with coefficient type morphing."
            << abort(FatalError);
    }
}


} // End namespace CML
#endif
