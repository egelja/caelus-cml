/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2013-2015 H. Jasak
Copyright (C) 2017-2018 Applied CCM Pty Ltd
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
    CML::fv::blockGaussGrad

Description
    Basic second-order block gradient scheme using face-interpolation
    and Gauss' theorem.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

SourceFiles
    blockGaussGrad.cpp

\*---------------------------------------------------------------------------*/

#ifndef blockGaussGrad_HPP
#define blockGaussGrad_HPP

#include "blockGradScheme.hpp"
#include "surfaceInterpolationScheme.hpp"
#include "linear.hpp"


namespace CML
{
namespace fv
{

template<class Type>
class blockGaussGrad
:
    public fv::blockGradScheme<Type>
{

    tmp<surfaceInterpolationScheme<Type> > tinterpScheme_;

    //- Disallow default bitwise copy construct
    blockGaussGrad(const blockGaussGrad&);

    //- Disallow default bitwise assignment
    void operator=(const blockGaussGrad&);


public:

    //- Runtime type information
    TypeName("Gauss");

    //- Construct from mesh
    blockGaussGrad(const fvMesh& mesh)
    :
        blockGradScheme<Type>(mesh),
        tinterpScheme_(new linear<Type>(mesh))
    {}

    //- Construct from mesh and Istream
    blockGaussGrad(const fvMesh& mesh, Istream& is)
    :
        blockGradScheme<Type>(mesh),
        tinterpScheme_(nullptr)
    {
        if (is.eof())
        {
            tinterpScheme_ =
                tmp<surfaceInterpolationScheme<Type> >
                (
                    new linear<Type>(mesh)
                );
        }
        else
        {
            tinterpScheme_ =
                tmp<surfaceInterpolationScheme<Type> >
                (
                    surfaceInterpolationScheme<Type>::New(mesh, is)
                );
        }
    }


    // Member Functions
    //- Return the BlockLduSystem corresponding to the implicit grad
    // discretization. For block coupled systems.
    tmp<BlockLduSystem<vector, typename outerProduct<vector, Type>::type> >
    fvmGrad
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    ) const;

};


} // End namespace fv
} // End namespace CML

#include "scalarBlockGaussGrad.hpp"


namespace CML
{
namespace fv
{

template<class Type>
tmp<BlockLduSystem<vector, typename outerProduct<vector, Type>::type> >
blockGaussGrad<Type>::fvmGrad
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
