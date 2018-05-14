/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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
    CML::fv::meanVelocityForce

Description
    Calculates and applies the force necessary to maintain the specified mean
    velocity.

    Note: Currently only handles kinematic pressure (incompressible solvers).

    \heading Source usage
    Example usage:
    \verbatim
    meanVelocityForceCoeffs
    {
        fieldNames      (U);                    // Name of velocity field
        Ubar            (10.0 0 0);             // Desired mean velocity
        relaxation      0.2;                    // Optional relaxation factor
    }
    \endverbatim

SourceFiles
    meanVelocityForce.cpp

\*---------------------------------------------------------------------------*/

#ifndef meanVelocityForce_HPP
#define meanVelocityForce_HPP

#include "autoPtr.hpp"
#include "topoSetSource.hpp"
#include "cellSet.hpp"
#include "fvMesh.hpp"
#include "volFields.hpp"
#include "fvOption.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace fv
{

/*---------------------------------------------------------------------------*\
               Class meanVelocityForce Declaration
\*---------------------------------------------------------------------------*/

class meanVelocityForce
:
    public option
{
protected:

    //- Average velocity
    vector Ubar_;

    //- Pressure gradient before correction
    scalar gradP0_;

    //- Change in pressure gradient
    scalar dGradP_;

    //- Flow direction
    vector flowDir_;

    //- Relaxation factor
    scalar relaxation_;

    //- Matrix 1/A coefficients field pointer
    autoPtr<volScalarField> rAPtr_;

    // Protected Member Functions

    //- Calculate and return the magnitude of the mean velocity
    //  averaged over the selected cellSet
    virtual scalar magUbarAve(const volVectorField& U) const;

    //- Write the pressure gradient to file (for restarts etc)
    void writeProps(const scalar gradP) const;

    //- Correct driving force for a constant mass flow rate
    void update(fvMatrix<vector>& eqn);

private:

    //- Disallow default bitwise copy construct
    meanVelocityForce(const meanVelocityForce&);

    //- Disallow default bitwise assignment
    void operator=(const meanVelocityForce&);

public:

    //- Runtime type information
    TypeName("meanVelocityForce");

    //- Construct from explicit source name and mesh
    meanVelocityForce
    (
        const word& sourceName,
        const word& modelType,
        const dictionary& dict,
        const fvMesh& mesh
    );

    //- Destructor
    virtual ~meanVelocityForce()
    {}

    // Member Functions

    //- Correct the pressure gradient
    virtual void correct(volVectorField& U);

    //- Add explicit contribution to momentum equation
    virtual void addSup
    (
        fvMatrix<vector>& eqn,
        const label fieldI
    );

    //- Add explicit contribution to compressible momentum equation
    virtual void addSup
    (
        const volScalarField& rho,
        fvMatrix<vector>& eqn,
        const label fieldI
    );

    //- Set 1/A coefficient
    virtual void setValue
    (
        fvMatrix<vector>& eqn,
        const label fieldI
    );

    //- Write the source properties
    virtual void writeData(Ostream&) const;

    //- Read source dictionary
    virtual bool read(const dictionary& dict);
};


} // End namespace fv
} // End namespace CML
#endif