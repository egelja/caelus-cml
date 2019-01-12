/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2014-2016 H. Jasak
Copyright (C) 2014 V. Vukcevic
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
    CML::fv::faceLimitedBlockGrad

Description
    faceLimitedGrad gradient scheme applied to a runTime selected base gradient
    scheme.

    The scalar limiter based on limiting the extrapolated face values
    between the face-neighbour cell values and is applied to all components
    of the gradient.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

SourceFiles
    faceLimitedBlockGrad.cpp

\*---------------------------------------------------------------------------*/

#ifndef faceLimitedBlockGrad_HPP
#define faceLimitedBlockGrad_HPP

#include "blockGradScheme.hpp"
#include "gradScheme.hpp"


namespace CML
{
namespace fv
{

template<class Type>
class faceLimitedBlockGrad
:
    public fv::blockGradScheme<Type>
{

    tmp<fv::blockGradScheme<Type> > basicBlockGradScheme_;
    tmp<fv::gradScheme<Type> >      basicGradScheme_;

    //- Limiter coefficient
    const scalar k_;

    inline void limitFace
    (
        scalar& limiter,
        const scalar maxDelta,
        const scalar minDelta,
        const scalar extrapolate
    ) const;

    //- Disallow default bitwise copy construct
    faceLimitedBlockGrad(const faceLimitedBlockGrad&);

    //- Disallow default bitwise assignment
    void operator=(const faceLimitedBlockGrad&);


public:

    //- RunTime type information
    TypeName("faceLimited");

    //- Construct from mesh and schemeData
    faceLimitedBlockGrad(const fvMesh& mesh, Istream& schemeData)
    :
        blockGradScheme<Type>(mesh),
        basicBlockGradScheme_(fv::blockGradScheme<Type>::New(mesh, schemeData)),
        basicGradScheme_(nullptr),
        k_(readScalar(schemeData))
    {
        // Rewind stream as we want to re-read the scheme for the basic
        // gradient. This ensures the scheme used to calculate the limiter
        // is the same as the block scheme.
        schemeData.rewind();

        // Advance token on stream as we don't need the first
        const word schemeName(schemeData);

        basicGradScheme_ = tmp<fv::gradScheme<Type> >
            (
               fv::gradScheme<Type>::New(mesh, schemeData)
            );

        // Advance token to end of stream
        const scalar dummy(readScalar(schemeData));

        if (fv::debug)
        {
            Info<<"Gradient scheme = "<<schemeName
                <<", limiting coefficient = "<<dummy<<endl;
        }

        if (k_ < 0 || k_ > 1)
        {
            FatalIOErrorInFunction(schemeData)
                << "coefficient = " << k_
                << " should be >= 0 and <= 1"
                << exit(FatalIOError);
        }
    }

    // Member Functions
    //- Return the BlockLduSystem corresponding to the implicit face
    // limited grad discretization. For block coupled systems.
    tmp
    <
        BlockLduSystem<vector, typename outerProduct<vector, Type>::type>
    > fvmGrad
    (
        const GeometricField<Type, fvPatchField, volMesh>&
    ) const;

};


} // End namespace fv
} // End namespace CML

#endif
