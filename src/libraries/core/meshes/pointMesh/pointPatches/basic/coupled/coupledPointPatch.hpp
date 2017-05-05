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
    CML::coupledPointPatch

Description
    coupled patch for post-processing.  Used as the base class for processor
    and cyclic pointPatches

SourceFiles
    coupledPointPatch.cpp

\*---------------------------------------------------------------------------*/

#ifndef coupledPointPatch_H
#define coupledPointPatch_H

#include "coupledPolyPatch.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class pointBoundaryMesh;

/*---------------------------------------------------------------------------*\
                      Class coupledPointPatch Declaration
\*---------------------------------------------------------------------------*/

class coupledPointPatch
{
    // Private Member Functions

        //- Disallow default construct as copy
        coupledPointPatch(const coupledPointPatch&);

        //- Disallow default assignment
        void operator=(const coupledPointPatch&);


protected:

    // Protected Member Functions

        //- Initialise the calculation of the patch geometry
        virtual void initGeometry(PstreamBuffers&) = 0;

        //- Calculate the patch geometry
        virtual void calcGeometry(PstreamBuffers&) = 0;

        //- Initialise the patches for moving points
        virtual void initMovePoints(PstreamBuffers&, const pointField&) = 0;

        //- Correct patches after moving points
        virtual void movePoints(PstreamBuffers&, const pointField&) = 0;

        //- Initialise the update of the patch topology
        virtual void initUpdateMesh(PstreamBuffers&) = 0;

        //- Update of the patch topology
        virtual void updateMesh(PstreamBuffers&) = 0;


public:

    //- Runtime type information
    TypeName(coupledPolyPatch::typeName_());


    // Constructors

        //- Construct from components
        coupledPointPatch(const pointBoundaryMesh& bm);


    //- Destructor
    virtual ~coupledPointPatch();


    // Member Functions

        // Access

            //- Return true because this patch is coupled
            virtual bool coupled() const
            {
                return true;
            }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
