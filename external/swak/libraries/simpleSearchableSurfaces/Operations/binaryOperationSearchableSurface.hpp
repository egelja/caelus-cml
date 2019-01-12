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
    CML::binaryOperationSearchableSurface

Description
    Searching on transformed surface

SourceFiles
    binaryOperationSearchableSurface.cpp

Contributors/Copyright:
    2009, 2013-2014 Bernhard F.W. Gschaider <bgschaid@ice-sf.at>

\*---------------------------------------------------------------------------*/

#ifndef binaryOperationSearchableSurface_H
#define binaryOperationSearchableSurface_H

#include "searchableSurface.hpp"

#include "swakSurfaceTypes.hpp"

#include "boundBox.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes

/*---------------------------------------------------------------------------*\
                           Class binaryOperationSearchableSurface Declaration
\*---------------------------------------------------------------------------*/

class binaryOperationSearchableSurface
:
    public searchableSurface
{
private:

    word aName_;

    word bName_;

    // Private Member Data

    autoPtr<searchableSurface> a_;

    autoPtr<searchableSurface> b_;

    label nrARegions_;

    label nrBRegions_;

    mutable wordList regions_;

    //- Disallow default bitwise copy construct
    binaryOperationSearchableSurface(const binaryOperationSearchableSurface&);

    //- Disallow default bitwise assignment
    void operator=(const binaryOperationSearchableSurface&);

    //- Calculate from the common index the index of region A
    label toIndexA(const label index) const;

    //- Calculate from the common index the index of region B
    label toIndexB(const label index) const;

protected:

    searchableSurface &a() { return a_(); }

    const searchableSurface &a() const { return a_(); }

    searchableSurface &b() { return b_(); }

    const searchableSurface &b() const { return b_(); }

    //- fitler from the separate hits on the two surfaces who stays in the combination
    virtual void filter
    (
        const point &start,
        const List<pointIndexHit>& hitsA,
        const List<pointIndexHit>& hitsB,
        List<pointIndexHit>& result
    ) const;

    enum hitWhom {
        NONE,
        HITSA,
        HITSB,
        BOTH
    };

    virtual bool decidePoint(
        const hitWhom who,
        const bool inA,
        const bool inB
    ) const = 0;

    //- find out who this hit belongs to
    void whose
    (
        const List<pointIndexHit>& hits,
        List<hitWhom> &whom
    ) const;

    //- make a list with the hits belonging to one surface
    void splitHits
    (
        const List<pointIndexHit>& hits,
        const List<hitWhom> &isA,
        List<pointIndexHit>& hitsA,
        List<pointIndexHit>& hitsB
    ) const;

    //- find out which hits are inside one of the objects
    void inside
    (
        const searchableSurface &s,
        const List<pointIndexHit>& hits,
        List<bool> &in
    ) const;

    //- convenience method
    void insideA
    (
        const List<pointIndexHit>& hits,
        List<bool> &in
    ) const {
        inside(
            a(),
            hits,
            in
        );
    }

    //- convenience method
    void insideB
    (
        const List<pointIndexHit>& hits,
        List<bool> &in
    ) const {
        inside(
            b(),
            hits,
            in
        );
    }

    void samePoint
    (
        const List<pointIndexHit>& hitA,
        const List<pointIndexHit>& hitB,
        List<bool> &samr
    ) const;

    //- Collect two lists into one with all relevant information
    void collectInfo(
        const point &start,
        const List<pointIndexHit>& hitsA,
        const List<pointIndexHit>& hitsB,
        List<pointIndexHit>& allHits,
        List<bool>& insideA,
        List<bool>& insideB,
        List<hitWhom>& who
    ) const;

    //- Check whether the normal for a point on surface A should be reverted
    virtual bool revertNormalA(const pointIndexHit&) const = 0;

    //- Check whether the normal for a point on surface B should be reverted
    virtual bool revertNormalB(const pointIndexHit&) const = 0;

public:

    //- Runtime type information
    TypeName("binaryOperationSearchableSurface");


    // Constructors

        //- Construct from dictionary (used by searchableSurface)
        binaryOperationSearchableSurface
        (
            const IOobject& io,
            const dictionary& dict
        );

    // Destructor

        virtual ~binaryOperationSearchableSurface();


    // Member Functions

        virtual const wordList& regions() const;

        virtual label size() const { return regions().size(); }

        virtual tmp<pointField> coordinates() const;

        virtual bool overlaps(const boundBox &) const;

        virtual tmp<pointField> points() const;

        //- Whether supports volume type below
        virtual bool hasVolumeType() const
        {
            return a().hasVolumeType() && b().hasVolumeType();
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
