/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2014-2015 H. Jasak
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
    CML::fv::blockLeastSquaresGrad

Description
    Second-order block gradient scheme using least-squares.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.


SourceFiles
    blockLeastSquaresGrad.cpp

\*---------------------------------------------------------------------------*/

#ifndef blockLeastSquaresGrad_HPP
#define blockLeastSquaresGrad_HPP

#include "blockGradScheme.hpp"
#include "leastSquaresVectors.hpp"
#include "zeroGradientFvPatchFields.hpp"


namespace CML
{
namespace fv
{

template<class Type>
class blockLeastSquaresGrad
:
    public fv::blockGradScheme<Type>
{

    //- Disallow default bitwise copy construct
    blockLeastSquaresGrad(const blockLeastSquaresGrad&);

    //- Disallow default bitwise assignment
    void operator=(const blockLeastSquaresGrad&);


public:

    //- Runtime type information
    TypeName("leastSquares");

    //- Construct from mesh
    blockLeastSquaresGrad(const fvMesh& mesh)
    :
        blockGradScheme<Type>(mesh)
    {}

    //- Construct from Istream
    blockLeastSquaresGrad(const fvMesh& mesh, Istream&)
    :
        blockGradScheme<Type>(mesh)
    {}

    // Member Functions
    //- Return the BlockLduSystem corresponding to the implicit least
    // quares grad discretization. For block coupled systems.
    tmp<BlockLduSystem<vector, typename outerProduct<vector, Type>::type> >
    fvmGrad
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    ) const;
};


} // End namespace fv
} // End namespace CML


#include "scalarBlockLeastSquaresGrad.hpp"


namespace CML
{
namespace fv
{

template<class Type>
tmp
<
    BlockLduSystem<vector, typename outerProduct<vector, Type>::type>
> blockLeastSquaresGrad<Type>::fvmGrad
(
   const GeometricField<Type, fvPatchField, volMesh>& vf
) const
{
   FatalErrorInFunction
       << "Implicit block gradient operator defined only for scalar."
       << abort(FatalError);

   typedef typename outerProduct<vector, Type>::type GradType;

   tmp<BlockLduSystem<vector, GradType> > tbs
   (
       new BlockLduSystem<vector, GradType>(vf.mesh())
   );

   return tbs;
}


} // End namespace fv
} // End namespace CML
#endif
