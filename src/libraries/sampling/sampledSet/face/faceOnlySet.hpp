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
    CML::faceOnlySet

Description

SourceFiles
    faceOnlySet.cpp

\*---------------------------------------------------------------------------*/

#ifndef faceOnlySet_H
#define faceOnlySet_H

#include "sampledSet.hpp"
#include "DynamicList.hpp"
#include "passiveParticleCloud.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class faceOnlySet Declaration
\*---------------------------------------------------------------------------*/

class faceOnlySet
:
    public sampledSet
{
    // Private data

        //- Starting point
        point start_;

        //- End point
        point end_;


    // Private Member Functions

        //- Samples from startTrackPt/CellI. Updates particle/samplePt/sampleI
        //  and puts
        //  samples in the DynamicLists. Returns false if end of all samples
        //  reached
        bool trackToBoundary
        (
            passiveParticleCloud& particleCloud,
            passiveParticle& singleParticle,
            DynamicList<point>& samplingPts,
            DynamicList<label>& samplingCells,
            DynamicList<label>& samplingFaces,
            DynamicList<scalar>& samplingCurve
        ) const;

        //- Samples from start_ to end_. samplingSegments contains segmentNo
        //  for each sample.
        void calcSamples
        (
            DynamicList<point>& samplingPts,
            DynamicList<label>& samplingCells,
            DynamicList<label>& samplingFaces,
            DynamicList<label>& samplingSegments,
            DynamicList<scalar>& samplingCurveDist
        ) const;

        //- Uses calcSamples to obtain samples. Copies them into *this.
        void genSamples();


public:

    //- Runtime type information
    TypeName("face");

    // Constructors

        //- Construct from components
        faceOnlySet
        (
            const word& name,
            const polyMesh& mesh,
            const meshSearch& searchEngine,
            const word& axis,
            const point& start,
            const point& end
        );

        //- Construct from dictionary
        faceOnlySet
        (
            const word& name,
            const polyMesh& mesh,
            const meshSearch& searchEngine,
            const dictionary& dict
        );


    //- Destructor
    virtual ~faceOnlySet();


    // Member Functions

        const point& start() const
        {
            return start_;
        }

        const point& end() const
        {
            return end_;
        }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
