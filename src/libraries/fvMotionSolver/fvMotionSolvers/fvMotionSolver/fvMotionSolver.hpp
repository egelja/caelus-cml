/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of CAELUS.

    CAELUS is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CAELUS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with CAELUS.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::fvMotionSolver

Description
    Virtual base class for finite volume mesh motion solvers.

    The boundary motion is set as a boundary condition on the motion velocity
    variable motionU.

SourceFiles
    fvMotionSolver.cpp

\*---------------------------------------------------------------------------*/

#ifndef fvMotionSolver_H
#define fvMotionSolver_H

#include "motionSolver.hpp"
#include "pointFieldsFwd.hpp"
#include "fixedValuePointPatchFields.hpp"
#include "cellMotionFvPatchFields.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class fvMesh;
class mapPolyMesh;

/*---------------------------------------------------------------------------*\
                           Class fvMotionSolver Declaration
\*---------------------------------------------------------------------------*/

class fvMotionSolver
:
    public motionSolver
{

protected:

    // Protected data

        //- The fvMesh to be moved
        const fvMesh& fvMesh_;


    // Protected Member Functions

        //- Create the corresponding patch types for cellMotion from those
        //  of the given pointMotion
        template<class Type>
        wordList cellMotionBoundaryTypes
        (
            const typename GeometricField<Type, pointPatchField, pointMesh>::
            GeometricBoundaryField& pmUbf
        ) const;


public:

    //- Runtime type information
    TypeName("fvMotionSolver");


    // Constructors

        //- Construct from polyMesh
        fvMotionSolver(const polyMesh& mesh);


    //- Destructor
    virtual ~fvMotionSolver();


    // Member Functions

        //- Return reference to the fvMesh to be moved
        const fvMesh& mesh() const
        {
            return fvMesh_;
        }

        //- Return point location obtained from the current motion field
        virtual tmp<pointField> curPoints() const = 0;

        //- Solve for motion
        virtual void solve() = 0;

        //- Update the pointMesh corresponding to the new points
        void movePoints(const pointField&);

        //- Update the mesh corresponding to given map
        virtual void updateMesh(const mapPolyMesh&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML


// * * * * * * * * * * * * Protected Member Functions  * * * * * * * * * * * //

template<class Type>
CML::wordList CML::fvMotionSolver::cellMotionBoundaryTypes
(
    const typename GeometricField<Type, pointPatchField, pointMesh>::
    GeometricBoundaryField& pmUbf
) const
{
    wordList cmUbf = pmUbf.types();

    // Remove global patches from the end of the list
    cmUbf.setSize(fvMesh_.boundary().size());

    forAll(cmUbf, patchi)
    {
        if (isA<fixedValuePointPatchField<Type> >(pmUbf[patchi]))
        {
            cmUbf[patchi] = cellMotionFvPatchField<Type>::typeName;
        }

        if (debug)
        {
            Pout<< "Patch:" << fvMesh_.boundary()[patchi].patch().name()
                << " pointType:" << pmUbf.types()[patchi]
                << " cellType:" << cmUbf[patchi] << endl;
        }
    }

    return cmUbf;
}


#endif

// ************************************************************************* //
