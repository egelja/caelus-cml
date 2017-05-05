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
    CML::simpleGeomDecomp

Description

SourceFiles
    simpleGeomDecomp.cpp

\*---------------------------------------------------------------------------*/

#ifndef simpleGeomDecomp_H
#define simpleGeomDecomp_H

#include "geomDecomp.hpp"

namespace CML
{

/*---------------------------------------------------------------------------*\
                           Class simpleGeomDecomp Declaration
\*---------------------------------------------------------------------------*/

class simpleGeomDecomp
:
    public geomDecomp
{
    // Private Member Functions

        void assignToProcessorGroup(labelList&, const label) const;

        void assignToProcessorGroup
        (
            labelList& processorGroup,
            const label nProcGroup,
            const labelList& indices,
            const scalarField& weights,
            const scalar summedWeights
        ) const;

        labelList decomposeOneProc(const pointField& points) const;

        labelList decomposeOneProc
        (
            const pointField& points,
            const scalarField& weights
        ) const;

        //- Disallow default bitwise copy construct and assignment
        void operator=(const simpleGeomDecomp&);
        simpleGeomDecomp(const simpleGeomDecomp&);


public:

    //- Runtime type information
    TypeName("simple");


    // Constructors

        //- Construct given the decomposition dictionary
        simpleGeomDecomp(const dictionary& decompositionDict);


    //- Destructor
    virtual ~simpleGeomDecomp()
    {}


    // Member Functions

        virtual bool parallelAware() const
        {
            // simpleDecomp sends all points to the master which does
            // the decomposition.
            return true;
        }

        virtual labelList decompose(const pointField&);

        virtual labelList decompose(const pointField&, const scalarField&);

        virtual labelList decompose(const polyMesh&, const pointField& points)
        {
            return decompose(points);
        }

        virtual labelList decompose
        (
            const polyMesh&,
            const pointField& points,
            const scalarField& pointWeights
        )
        {
            return decompose(points, pointWeights);
        }

        //- Explicitly provided connectivity
        virtual labelList decompose
        (
            const labelListList& globalCellCells,
            const pointField& cc,
            const scalarField& cWeights
        )
        {
            return decompose(cc, cWeights);
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
