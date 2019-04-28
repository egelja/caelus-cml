/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright (C) 1991-2008 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is based on CAELUS.

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
    CML::unitCylinderSearchableSurface

Description
    Searching on sphere

SourceFiles
    unitCylinderSearchableSurface.cpp

Contributors/Copyright:
    2009, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef unitCylinderSearchableSurface_H
#define unitCylinderSearchableSurface_H

#include "searchableSurface.hpp"
#include "treeBoundBox.hpp"
#include "plane.hpp"

#include "swakSurfaceTypes.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

/*---------------------------------------------------------------------------*\
                           Class unitCylinderSearchableSurface Declaration
\*---------------------------------------------------------------------------*/

class unitCylinderSearchableSurface
:
    public searchableSurface
{
private:

    // Private Member Data

    plane top_;
    plane bottom_;

    //- Names of regions
    mutable wordList regions_;


    // Private Member Functions
    
        //- Inherit findNearest from searchableSurface
        using searchableSurface::findNearest;
    
        //- Find nearest point on sphere.
        pointIndexHit findNearest
        (
            const point& sample,
            const scalar nearestDistSqr
        ) const;

        //- Find intersections with cylindee
        void findLineAll
        (
            const point& start,
            const point& end,
            List<pointIndexHit>& hit
        ) const;


        //- Disallow default bitwise copy construct
        unitCylinderSearchableSurface(const unitCylinderSearchableSurface&);

        //- Disallow default bitwise assignment
        void operator=(const unitCylinderSearchableSurface&);


public:

    //- Runtime type information
    TypeName("unitCylinder");


    // Constructors

        //- Construct from components
        unitCylinderSearchableSurface(const IOobject& io, const point&, const scalar radius);

        //- Construct from dictionary (used by searchableSurface)
        unitCylinderSearchableSurface
        (
            const IOobject& io,
            const dictionary& dict
        );

    // Destructor

        virtual ~unitCylinderSearchableSurface();


    // Member Functions

        virtual label size() const { return 3; }

        virtual tmp<pointField> coordinates() const;

        virtual tmp<pointField> points() const;

        virtual bool overlaps(const boundBox &) const;

        virtual const wordList& regions() const;

        //- Whether supports volume type below
        virtual bool hasVolumeType() const
        {
            return true;
        }


        // Multiple point queries.

            virtual void findNearest
            (
                const pointField& sample,
                const scalarField& nearestDistSqr,
                List<pointIndexHit>&
            ) const;

            virtual void findLine
            (
                const pointField& start,
                const pointField& end,
                List<pointIndexHit>&
            ) const;

            virtual void findLineAny
            (
                const pointField& start,
                const pointField& end,
                List<pointIndexHit>&
            ) const;

            //- Get all intersections in order from start to end.
            virtual void findLineAll
            (
                const pointField& start,
                const pointField& end,
                List<List<pointIndexHit> >&
            ) const;

            //- From a set of points and indices get the region
            virtual void getRegion
            (
                const List<pointIndexHit>&,
                labelList& region
            ) const;

            //- From a set of points and indices get the normal
            virtual void getNormal
            (
                const List<pointIndexHit>&,
                vectorField& normal
            ) const;

            //- Determine type (inside/outside/mixed) for point. unknown if
            //  cannot be determined (e.g. non-manifold surface)
            virtual void getVolumeType
            (
                const pointField&,
                List<volumeType>&
            ) const;

            virtual void boundingSpheres
            (
                pointField& centres,
                scalarField& radiusSqr
            ) const;

        // regIOobject implementation

            bool writeData(Ostream&) const
            {
                NotImplemented;
                return false;
            }

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
