/*---------------------------------------------------------------------------*\
Copyright (C) 2004-6 H. Jasak All rights reserved
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus. If not, see <http://www.gnu.org/licenses/>.

Class
    BlockCoeff

Description
    Template specialisation for the terminal class.  It is designed to avoid
    endless expansion of tensor order by excluding block coupling at the
    terminal type level.

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*---------------------------------------------------------------------------*/

#ifndef sphericalTensorBlockCoeff_HPP
#define sphericalTensorBlockCoeff_HPP

#include "BlockCoeff.hpp"
#include "DecoupledBlockCoeff.hpp"
#include "sphericalTensor.hpp"


namespace CML
{

/*---------------------------------------------------------------------------*\
                          Class BlockCoeff Declaration
\*---------------------------------------------------------------------------*/

template<>
class BlockCoeff<sphericalTensor>
:
    public DecoupledBlockCoeff<sphericalTensor>
{
public:

    //- Construct null
    explicit BlockCoeff();

    //- Construct as copy
    BlockCoeff(const BlockCoeff<sphericalTensor>&);

    //- Construct from Istream
    BlockCoeff(Istream&);
};


} // End namespace CML
#endif
