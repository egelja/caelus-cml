/*---------------------------------------------------------------------------*\
Copyright (C) 2015 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::fv::patchMeanVelocityForce

Description
    Calculates and applies the force necessary to maintain the specified mean
    velocity averaged over the specified patch.

    Note: Currently only handles kinematic pressure (incompressible solvers).

    \heading Source usage
    Example usage:
    \verbatim
    patchMeanVelocityForceCoeffs
    {
        fieldNames      (U);                    // Name of velocity field
        patch           inlet;                  // Name of the patch
        Ubar            (10.0 0 0);             // Desired mean velocity
        relaxation      0.2;                    // Optional relaxation factor
    }
    \endverbatim

SourceFiles
    patchMeanVelocityForce.cpp

\*---------------------------------------------------------------------------*/

#ifndef patchMeanVelocityForce_HPP
#define patchMeanVelocityForce_HPP

#include "meanVelocityForce.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fv
{

/*---------------------------------------------------------------------------*\
               Class patchMeanVelocityForce Declaration
\*---------------------------------------------------------------------------*/

class patchMeanVelocityForce
:
    public meanVelocityForce
{
protected:

    //- Patch name
    word patch_;

    //- Patch index
    label patchi_;


    // Protected Member Functions

    //- Calculate and return the magnitude of the mean velocity
    //  averaged over the specified patch
    virtual scalar magUbarAve(const volVectorField& U) const;

private:

    // Private Member Functions

    //- Disallow default bitwise copy construct
    patchMeanVelocityForce(const patchMeanVelocityForce&);

    //- Disallow default bitwise assignment
    void operator=(const patchMeanVelocityForce&);

public:

    //- Runtime type information
    TypeName("patchMeanVelocityForce");

    //- Construct from explicit source name and mesh
    patchMeanVelocityForce
    (
        const word& sourceName,
        const word& modelType,
        const dictionary& dict,
        const fvMesh& mesh
    );

};


} // End namespace fv
} // End namespace CML
#endif
