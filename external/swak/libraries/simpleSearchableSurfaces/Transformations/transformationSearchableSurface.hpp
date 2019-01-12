/*---------------------------------------------------------------------------*\
Copyright: ICE Stroemungsfoschungs GmbH
Copyright held by original author
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
    CML::transformationSearchableSurface

Description
    Searching on transformed surface

SourceFiles
    transformationSearchableSurface.cpp

Contributors/Copyright:
    2009, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef transformationSearchableSurface_H
#define transformationSearchableSurface_H

#include "searchableSurface.hpp"

#include "swakSurfaceTypes.hpp"

#include "boundBox.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

/*---------------------------------------------------------------------------*\
                           Class transformationSearchableSurface Declaration
\*---------------------------------------------------------------------------*/

class transformationSearchableSurface
:
    public searchableSurface
{
private:

    // Private Member Data

        autoPtr<IOobject> rename_;

        autoPtr<searchableSurface> delegate_;

        //- Disallow default bitwise copy construct
        transformationSearchableSurface(const transformationSearchableSurface&);

        //- Disallow default bitwise assignment
        void operator=(const transformationSearchableSurface&);

protected:

    searchableSurface &delegate() { return delegate_(); }

    const searchableSurface &delegate() const { return delegate_(); }

    // Do the transformation for a point
    virtual point transform(const point &) const =0;

    // Do the inverse transformation for a point
    virtual point inverseTransform(const point &) const =0;

    // Transform a hit to the real world
    pointIndexHit transform(const pointIndexHit &) const;

    // Transform a hit from the real world
    pointIndexHit inverseTransform(const pointIndexHit &) const;

public:

    //- Runtime type information
    TypeName("transformationSearchableSurface");


    // Constructors

        //- Construct from dictionary (used by searchableSurface)
        transformationSearchableSurface
        (
            const IOobject& io,
            const dictionary& dict
        );

    // Destructor

        virtual ~transformationSearchableSurface();


    // Member Functions

    virtual label size() const { return delegate().size(); }

        virtual tmp<pointField> coordinates() const;

        virtual tmp<pointField> points() const;

        virtual const wordList& regions() const;

        virtual bool overlaps(const boundBox &) const;

        //- Whether supports volume type below
        virtual bool hasVolumeType() const
        {
            return delegate().hasVolumeType();
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

            void getVolumeType
            (
                const pointField& points,
                List<volumeType>& volType
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
