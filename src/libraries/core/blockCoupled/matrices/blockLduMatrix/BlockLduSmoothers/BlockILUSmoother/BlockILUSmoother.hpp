/*---------------------------------------------------------------------------*\
Copyright (C) 2004-6 H. Jasak
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
    BlockILUSmoother

Description
    ILU smoother

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

\*---------------------------------------------------------------------------*/

#ifndef BlockILUSmoother_HPP
#define BlockILUSmoother_HPP

#include "BlockLduSmoother.hpp"
#include "BlockCholeskyPrecon.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                       Class BlockILUSmoother Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockILUSmoother
:
    public BlockLduSmoother<Type>
{

    //- Cholesky preconditioner
    BlockCholeskyPrecon<Type> precon_;

    //- Correction array
    mutable Field<Type> xCorr_;

    //- Residual array
    mutable Field<Type> residual_;

    //- Disallow default bitwise copy construct
    BlockILUSmoother(const BlockILUSmoother&);

    //- Disallow default bitwise assignment
    void operator=(const BlockILUSmoother&);


public:

    //- Runtime type information
    TypeName("ILU");

    //- Construct from components
    BlockILUSmoother
    (
        const BlockLduMatrix<Type>& matrix,
        const dictionary& dict
    )
    :
        BlockLduSmoother<Type>(matrix),
        precon_(matrix),
        xCorr_(matrix.lduAddr().size()),
        residual_(matrix.lduAddr().size())
    {}

    virtual ~BlockILUSmoother()
    {}

    // Member Functions
    //- Execute smoothing
    virtual void smooth
    (
        Field<Type>& x,
        const Field<Type>& b,
        const label nSweeps
    ) const
    {
        for (label sweep = 0; sweep < nSweeps; sweep++)
        {
            // Calculate residual
            this-> matrix_.Amul(residual_, x);

            // residual = b - Ax
            forAll (b, i)
            {
                residual_[i] = b[i] - residual_[i];
            }

            precon_.precondition(xCorr_, residual_);

            // Add correction to x
            x += xCorr_;
        }
    }
};


} // End namespace CML
#endif
