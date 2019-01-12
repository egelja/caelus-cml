/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2018 OpenFOAM Foundation
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
    CML::tetrahedron

Description
    A tetrahedron primitive.

    Ordering of edges needs to be the same for a tetrahedron
    class, a tetrahedron cell shape model and a tetCell.


\*---------------------------------------------------------------------------*/

#ifndef tetrahedron_HPP
#define tetrahedron_HPP

#include "point.hpp"
#include "primitiveFieldsFwd.hpp"
#include "pointHit.hpp"
#include "Random.hpp"
#include "FixedList.hpp"
#include "UList.hpp"
#include "triPointRef.hpp"
#include "boundBox.hpp"
#include "barycentric.hpp"

namespace CML
{

class Istream;
class Ostream;

// Forward declaration of friend functions and operators

template<class Point, class PointRef> class tetrahedron;

template<class Point, class PointRef>
inline Istream& operator>>
(
    Istream&,
    tetrahedron<Point, PointRef>&
);

template<class Point, class PointRef>
inline Ostream& operator<<
(
    Ostream&,
    const tetrahedron<Point, PointRef>&
);

/*---------------------------------------------------------------------------*\
                           class tetrahedron Declaration
\*---------------------------------------------------------------------------*/

template<class Point, class PointRef>
class tetrahedron
{
private:

    // Private data

        PointRef a_, b_, c_, d_;


public:

    // Member constants

        enum
        {
            nVertices = 4,  // Number of vertices in tetrahedron
            nEdges = 6      // Number of edges in tetrahedron
        };


    // Constructors

        //- Construct from points
        inline tetrahedron
        (
            const Point& a,
            const Point& b,
            const Point& c,
            const Point& d
        );

        //- Construct from four points in the list of points
        inline tetrahedron
        (
            const UList<Point>&,
            const FixedList<label, 4>& indices
        );

        //- Construct from Istream
        inline tetrahedron(Istream&);


    // Member Functions

        // Access

            //- Return vertices
            inline const Point& a() const;

            inline const Point& b() const;

            inline const Point& c() const;

            inline const Point& d() const;

            //- Return i-th face
            inline triPointRef tri(const label facei) const;

        // Properties

            //- Return face normal
            inline vector Sa() const;

            inline vector Sb() const;

            inline vector Sc() const;

            inline vector Sd() const;

            //- Return centre (centroid)
            inline Point centre() const;

            //- Return volume
            inline scalar mag() const;

            //- Return circum-centre
            inline Point circumCentre() const;

            //- Return circum-radius
            inline scalar circumRadius() const;

            //- Return quality: Ratio of tetrahedron and circum-sphere
            //  volume, scaled so that a regular tetrahedron has a
            //  quality of 1
            inline scalar quality() const;

            //- Return a random point in the tetrahedron from a
            //  uniform distribution
            inline Point randomPoint(Random& rndGen) const;

            //- Calculate the point from the given barycentric coordinates.
            inline Point barycentricToPoint(const barycentric& bary) const;

            //- Calculate the barycentric coordinates from the given point
            inline barycentric pointToBarycentric(const point& pt) const;

            //- Calculate the barycentric coordinates from the given point.
            //  Returns the determinant.
            inline scalar pointToBarycentric
            (
                const point& pt,
                barycentric& bary
            ) const;

            //- Return nearest point to p on tetrahedron. Is p itself
            //  if inside.
            inline pointHit nearestPoint(const point& p) const;

            //- Return true if point is inside tetrahedron
            inline bool inside(const point& pt) const;

            //- Return (min)containment sphere, i.e. the smallest sphere with
            //  all points inside. Returns pointHit with:
            //  - hit         : if sphere is equal to circumsphere
            //                  (biggest sphere)
            //  - point       : centre of sphere
            //  - distance    : radius of sphere
            //  - eligiblemiss: false
            // Tol (small compared to 1, e.g. 1e-9) is used to determine
            // whether point is inside: mag(pt - ctr) < (1+tol)*radius.
            pointHit containmentSphere(const scalar tol) const;

            //- Fill buffer with shape function products
            void gradNiSquared(scalarField& buffer) const;

            void gradNiDotGradNj(scalarField& buffer) const;

            void gradNiGradNi(tensorField& buffer) const;

            void gradNiGradNj(tensorField& buffer) const;

            //- Calculate the bounding box
            boundBox bounds() const;


    // IOstream operators

        friend Istream& operator>> <Point, PointRef>
        (
            Istream&,
            tetrahedron&
        );

        friend Ostream& operator<< <Point, PointRef>
        (
            Ostream&,
            const tetrahedron&
        );
};


} // End namespace CML


#include "tetrahedronI.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "triPointRef.hpp"
#include "scalarField.hpp"

template<class Point, class PointRef>
CML::pointHit CML::tetrahedron<Point, PointRef>::containmentSphere
(
    const scalar tol
) const
{
    // (Probably very inefficient) minimum containment sphere calculation.
    // From http://www.imr.sandia.gov/papers/imr11/shewchuk2.pdf:
    // Sphere ctr is smallest one of
    // - tet circumcentre
    // - triangle circumcentre
    // - edge mids

    const scalar fac = 1 + tol;

    // Halve order of tolerance for comparisons of sqr.
    const scalar facSqr = CML::sqrt(fac);


    // 1. Circumcentre itself.

    pointHit pHit(circumCentre());
    pHit.setHit();
    scalar minRadiusSqr = magSqr(pHit.rawPoint() - a_);


    // 2. Try circumcentre of tet triangles. Create circumcircle for triFace and
    // check if 4th point is inside.

    {
        point ctr = triPointRef(a_, b_, c_).circumCentre();
        scalar radiusSqr = magSqr(ctr - a_);

        if
        (
            radiusSqr < minRadiusSqr
         && CML::magSqr(d_-ctr) <= facSqr*radiusSqr
        )
        {
            pHit.setMiss(false);
            pHit.setPoint(ctr);
            minRadiusSqr = radiusSqr;
        }
    }
    {
        point ctr = triPointRef(a_, b_, d_).circumCentre();
        scalar radiusSqr = magSqr(ctr - a_);

        if
        (
            radiusSqr < minRadiusSqr
         && CML::magSqr(c_-ctr) <= facSqr*radiusSqr
        )
        {
            pHit.setMiss(false);
            pHit.setPoint(ctr);
            minRadiusSqr = radiusSqr;
        }
    }
    {
        point ctr = triPointRef(a_, c_, d_).circumCentre();
        scalar radiusSqr = magSqr(ctr - a_);

        if
        (
            radiusSqr < minRadiusSqr
         && CML::magSqr(b_-ctr) <= facSqr*radiusSqr
        )
        {
            pHit.setMiss(false);
            pHit.setPoint(ctr);
            minRadiusSqr = radiusSqr;
        }
    }
    {
        point ctr = triPointRef(b_, c_, d_).circumCentre();
        scalar radiusSqr = magSqr(ctr - b_);

        if
        (
            radiusSqr < minRadiusSqr
         && CML::magSqr(a_-ctr) <= facSqr*radiusSqr
        )
        {
            pHit.setMiss(false);
            pHit.setPoint(ctr);
            minRadiusSqr = radiusSqr;
        }
    }


    // 3. Try midpoints of edges

    // mid of edge A-B
    {
        point ctr = 0.5*(a_ + b_);
        scalar radiusSqr = magSqr(a_ - ctr);
        scalar testRadSrq = facSqr*radiusSqr;

        if
        (
            radiusSqr < minRadiusSqr
         && magSqr(c_-ctr) <= testRadSrq
         && magSqr(d_-ctr) <= testRadSrq)
        {
            pHit.setMiss(false);
            pHit.setPoint(ctr);
            minRadiusSqr = radiusSqr;
        }
    }

    // mid of edge A-C
    {
        point ctr = 0.5*(a_ + c_);
        scalar radiusSqr = magSqr(a_ - ctr);
        scalar testRadSrq = facSqr*radiusSqr;

        if
        (
            radiusSqr < minRadiusSqr
         && magSqr(b_-ctr) <= testRadSrq
         && magSqr(d_-ctr) <= testRadSrq
        )
        {
            pHit.setMiss(false);
            pHit.setPoint(ctr);
            minRadiusSqr = radiusSqr;
        }
    }

    // mid of edge A-D
    {
        point ctr = 0.5*(a_ + d_);
        scalar radiusSqr = magSqr(a_ - ctr);
        scalar testRadSrq = facSqr*radiusSqr;

        if
        (
            radiusSqr < minRadiusSqr
         && magSqr(b_-ctr) <= testRadSrq
         && magSqr(c_-ctr) <= testRadSrq
        )
        {
            pHit.setMiss(false);
            pHit.setPoint(ctr);
            minRadiusSqr = radiusSqr;
        }
    }

    // mid of edge B-C
    {
        point ctr = 0.5*(b_ + c_);
        scalar radiusSqr = magSqr(b_ - ctr);
        scalar testRadSrq = facSqr*radiusSqr;

        if
        (
            radiusSqr < minRadiusSqr
         && magSqr(a_-ctr) <= testRadSrq
         && magSqr(d_-ctr) <= testRadSrq
        )
        {
            pHit.setMiss(false);
            pHit.setPoint(ctr);
            minRadiusSqr = radiusSqr;
        }
    }

    // mid of edge B-D
    {
        point ctr = 0.5*(b_ + d_);
        scalar radiusSqr = magSqr(b_ - ctr);
        scalar testRadSrq = facSqr*radiusSqr;

        if
        (
            radiusSqr < minRadiusSqr
         && magSqr(a_-ctr) <= testRadSrq
         && magSqr(c_-ctr) <= testRadSrq)
        {
            pHit.setMiss(false);
            pHit.setPoint(ctr);
            minRadiusSqr = radiusSqr;
        }
    }

    // mid of edge C-D
    {
        point ctr = 0.5*(c_ + d_);
        scalar radiusSqr = magSqr(c_ - ctr);
        scalar testRadSrq = facSqr*radiusSqr;

        if
        (
            radiusSqr < minRadiusSqr
         && magSqr(a_-ctr) <= testRadSrq
         && magSqr(b_-ctr) <= testRadSrq
        )
        {
            pHit.setMiss(false);
            pHit.setPoint(ctr);
            minRadiusSqr = radiusSqr;
        }
    }


    pHit.setDistance(sqrt(minRadiusSqr));

    return pHit;
}


template<class Point, class PointRef>
void CML::tetrahedron<Point, PointRef>::gradNiSquared
(
    scalarField& buffer
) const
{
    // Change of sign between face area vector and gradient
    // does not matter because of square

    // Warning: Added a mag to produce positive coefficients even if
    // the tetrahedron is twisted inside out.  This is pretty
    // dangerous, but essential for mesh motion.
    scalar magVol = CML::mag(mag());

    buffer[0] = (1.0/9.0)*magSqr(Sa())/magVol;
    buffer[1] = (1.0/9.0)*magSqr(Sb())/magVol;
    buffer[2] = (1.0/9.0)*magSqr(Sc())/magVol;
    buffer[3] = (1.0/9.0)*magSqr(Sd())/magVol;
}


template<class Point, class PointRef>
void CML::tetrahedron<Point, PointRef>::gradNiDotGradNj
(
    scalarField& buffer
) const
{
    // Warning. Ordering of edges needs to be the same for a tetrahedron
    // class, a tetrahedron cell shape model and a tetCell

    // Warning: Added a mag to produce positive coefficients even if
    // the tetrahedron is twisted inside out.  This is pretty
    // dangerous, but essential for mesh motion.

    // Double change of sign between face area vector and gradient

    scalar magVol = CML::mag(mag());
    vector sa = Sa();
    vector sb = Sb();
    vector sc = Sc();
    vector sd = Sd();

    buffer[0] = (1.0/9.0)*(sa & sb)/magVol;
    buffer[1] = (1.0/9.0)*(sa & sc)/magVol;
    buffer[2] = (1.0/9.0)*(sa & sd)/magVol;
    buffer[3] = (1.0/9.0)*(sd & sb)/magVol;
    buffer[4] = (1.0/9.0)*(sb & sc)/magVol;
    buffer[5] = (1.0/9.0)*(sd & sc)/magVol;
}


template<class Point, class PointRef>
void CML::tetrahedron<Point, PointRef>::gradNiGradNi
(
    tensorField& buffer
) const
{
    // Change of sign between face area vector and gradient
    // does not matter because of square

    scalar magVol = CML::mag(mag());

    buffer[0] = (1.0/9.0)*sqr(Sa())/magVol;
    buffer[1] = (1.0/9.0)*sqr(Sb())/magVol;
    buffer[2] = (1.0/9.0)*sqr(Sc())/magVol;
    buffer[3] = (1.0/9.0)*sqr(Sd())/magVol;
}


template<class Point, class PointRef>
void CML::tetrahedron<Point, PointRef>::gradNiGradNj
(
    tensorField& buffer
) const
{
    // Warning. Ordering of edges needs to be the same for a tetrahedron
    // class, a tetrahedron cell shape model and a tetCell

    // Double change of sign between face area vector and gradient

    scalar magVol = CML::mag(mag());
    vector sa = Sa();
    vector sb = Sb();
    vector sc = Sc();
    vector sd = Sd();

    buffer[0] = (1.0/9.0)*(sa * sb)/magVol;
    buffer[1] = (1.0/9.0)*(sa * sc)/magVol;
    buffer[2] = (1.0/9.0)*(sa * sd)/magVol;
    buffer[3] = (1.0/9.0)*(sd * sb)/magVol;
    buffer[4] = (1.0/9.0)*(sb * sc)/magVol;
    buffer[5] = (1.0/9.0)*(sd * sc)/magVol;
}


template<class Point, class PointRef>
CML::boundBox CML::tetrahedron<Point, PointRef>::bounds() const
{
    return
        boundBox
        (
            min(a(), min(b(), min(c(), d()))),
            max(a(), max(b(), max(c(), d())))
        );
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
