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
    BlockGaussSeidelSmoother

Description
    Symmetric Gauss-Seidel smoother with prescribed number of smoothing sweeps

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved.

\*---------------------------------------------------------------------------*/

#ifndef BlockGaussSeidelSmoother_HPP
#define BlockGaussSeidelSmoother_HPP

#include "BlockLduSmoother.hpp"
#include "BlockGaussSeidelPrecon.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                   Class BlockGaussSeidelSmoother Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockGaussSeidelSmoother
:
    public BlockLduSmoother<Type>
{

    //- Gauss-Seidel preconditioner
    BlockGaussSeidelPrecon<Type> gs_;

    //- Disallow default bitwise copy construct
    BlockGaussSeidelSmoother(const BlockGaussSeidelSmoother&);

    //- Disallow default bitwise assignment
    void operator=(const BlockGaussSeidelSmoother&);


public:

    //- Runtime type information
    TypeName("GaussSeidel");

    //- Construct from components
    BlockGaussSeidelSmoother
    (
        const BlockLduMatrix<Type>& matrix,
        const dictionary& dict
    )
    :
        BlockLduSmoother<Type>(matrix),
        gs_(matrix)
    {}

    virtual ~BlockGaussSeidelSmoother()
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
            gs_.precondition(x, b);
        }
    }
};


} // End namespace CML
#endif
