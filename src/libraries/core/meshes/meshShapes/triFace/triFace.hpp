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
    CML::triFace

Description
    A triangular face using a FixedList of labels corresponding to mesh
    vertices.

SeeAlso
    CML::face, CML::triangle

SourceFiles
    triFaceI.hpp
    triFaceTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef triFace_H
#define triFace_H

#include "FixedList.hpp"
#include "edgeList.hpp"
#include "pointHit.hpp"
#include "intersection.hpp"
#include "pointField.hpp"
#include "triPointRef.hpp"
#include "ListListOps.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class face;
class triFace;

inline bool operator==(const triFace&, const triFace&);
inline bool operator!=(const triFace&, const triFace&);


/*---------------------------------------------------------------------------*\
                          Class triFace Declaration
\*---------------------------------------------------------------------------*/

class triFace
:
    public FixedList<label, 3>
{

public:

    // Constructors

        //- Construct null
        inline triFace();

        //- Construct from three point labels
        inline triFace
        (
            const label a,
            const label b,
            const label c
        );

        //- Construct from a list of labels
        explicit inline triFace(const labelUList&);

        //- Construct from Istream
        inline triFace(Istream&);


    // Member Functions

        //- Collapse face by removing duplicate point labels
        //  return the collapsed size, set collapsed point labels to -1
        inline label collapse();

        //- Flip the face in-place.
        //  The starting points of the original and flipped face are identical.
        inline void flip();

        //- Return the points corresponding to this face
        inline pointField points(const pointField&) const;

        //- Return triangle as a face
        inline face triFaceFace() const;

        //- Return the triangle
        inline triPointRef tri(const pointField&) const;

        //- Return centre (centroid)
        inline point centre(const pointField&) const;

        //- Calculate average value at centroid of face
        template<class Type>
        Type average(const pointField&, const Field<Type>&) const;

        //- Return vector area
        inline vector area(const pointField&) const;

        //- Return scalar magnitude
        inline scalar mag(const pointField&) const;

        //- Return unit normal
        inline vector normal(const pointField&) const;

        //- Number of triangles after splitting
        inline label nTriangles() const;

        //- Return face with reverse direction
        //  The starting points of the original and reverse face are identical.
        inline triFace reverseFace() const;

        //- Return swept-volume
        inline scalar sweptVol
        (
            const pointField& oldPoints,
            const pointField& newPoints
        ) const;

        //- Return the inertia tensor, with optional reference
        //  point and density specification
        inline tensor inertia
        (
            const pointField&,
            const point& refPt = vector::zero,
            scalar density = 1.0
        ) const;

        //- Return point intersection with a ray starting at p,
        //  with direction q.
        inline pointHit ray
        (
            const point& p,
            const vector& q,
            const pointField& points,
            const intersection::algorithm = intersection::FULL_RAY,
            const intersection::direction dir = intersection::VECTOR
        ) const;

        //- Fast intersection with a ray.
        inline pointHit intersection
        (
            const point& p,
            const vector& q,
            const pointField& points,
            const intersection::algorithm alg,
            const scalar tol = 0.0
        ) const;

        inline pointHit intersection
        (
            const point& p,
            const vector& q,
            const point& ctr,
            const pointField& points,
            const intersection::algorithm alg,
            const scalar tol = 0.0
        ) const;

        //- Return nearest point to face
        inline pointHit nearestPoint
        (
            const point& p,
            const pointField& points
        ) const;


        //- Return nearest point to face and classify it:
        //  + near point (nearType=POINT, nearLabel=0, 1, 2)
        //  + near edge (nearType=EDGE, nearLabel=0, 1, 2)
        //    Note: edges are counted from starting vertex so
        //    e.g. edge n is from f[n] to f[0], where the face has n + 1
        //    points
        inline pointHit nearestPointClassify
        (
            const point& p,
            const pointField& points,
            label& nearType,
            label& nearLabel
        ) const;

        //- Return number of edges
        inline label nEdges() const;

        //- Return edges in face point ordering,
        //  i.e. edges()[0] is edge between [0] and [1]
        inline edgeList edges() const;

        //- Return n-th face edge
        inline edge faceEdge(const label n) const;

        //- Return the edge direction on the face
        //  Returns:
        //  - +1: forward (counter-clockwise) on the face
        //  - -1: reverse (clockwise) on the face
        //  -  0: edge not found on the face
        inline int edgeDirection(const edge&) const;

        //- Compare triFaces
        //  Returns:
        //  -  0: different
        //  - +1: identical
        //  - -1: same face, but different orientation
        static inline int compare(const triFace&, const triFace&);

    // Friend Operators

        inline friend bool operator==(const triFace&, const triFace&);
        inline friend bool operator!=(const triFace&, const triFace&);
};


//- Hash specialization for hashing triFace - a commutative hash value.
//  Hash incrementally.
template<>
inline unsigned Hash<triFace>::operator()(const triFace& t, unsigned seed) const
{
    // Fortunately we don't need this very often
    const uLabel t0(t[0]);
    const uLabel t1(t[1]);
    const uLabel t2(t[2]);

    const uLabel val = (t0*t1*t2 + t0+t1+t2);

    return Hash<uLabel>()(val, seed);
}


//- Hash specialization for hashing triFace - a commutative hash value.
template<>
inline unsigned Hash<triFace>::operator()(const triFace& t) const
{
    return Hash<triFace>::operator()(t, 0);
}


template<>
inline bool contiguous<triFace>()  {return true;}


//- Hash specialization to offset faces in ListListOps::combineOffset
template<>
class offsetOp<triFace>
{

public:

    inline triFace operator()
    (
        const triFace& x,
        const label offset
    ) const
    {
        triFace result(x);

        forAll(x, xI)
        {
            result[xI] = x[xI] + offset;
        }
        return result;
    }
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "IOstreams.hpp"
#include "face.hpp"
#include "triPointRef.hpp"
#include "Swap.hpp"

// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

inline int CML::triFace::compare(const triFace& a, const triFace& b)
{
    if
    (
        (a[0] == b[0] && a[1] == b[1] && a[2] == b[2])
     || (a[0] == b[1] && a[1] == b[2] && a[2] == b[0])
     || (a[0] == b[2] && a[1] == b[0] && a[2] == b[1])
    )
    {
        // identical
        return 1;
    }
    else if
    (
        (a[0] == b[2] && a[1] == b[1] && a[2] == b[0])
     || (a[0] == b[1] && a[1] == b[0] && a[2] == b[2])
     || (a[0] == b[0] && a[1] == b[2] && a[2] == b[1])
    )
    {
        // same face, but reversed orientation
        return -1;
    }
    else
    {
        return 0;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::triFace::triFace()
{}


inline CML::triFace::triFace
(
    const label a,
    const label b,
    const label c
)
{
    operator[](0) = a;
    operator[](1) = b;
    operator[](2) = c;
}


inline CML::triFace::triFace(const labelUList& lst)
:
    FixedList<label, 3>(lst)
{}


inline CML::triFace::triFace(Istream& is)
:
    FixedList<label, 3>(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::triFace::collapse()
{
    // we cannot resize a FixedList, so mark duplicates with '-1'
    // (the lower vertex is retained)
    // catch any '-1' (eg, if called twice)

    label n = 3;
    if (operator[](0) == operator[](1) || operator[](1) == -1)
    {
        operator[](1) = -1;
        n--;
    }
    else if (operator[](1) == operator[](2) || operator[](2) == -1)
    {
        operator[](2) = -1;
        n--;
    }
    if (operator[](0) == operator[](2))
    {
        operator[](2) = -1;
        n--;
    }

    return n;
}


inline void CML::triFace::flip()
{
    Swap(operator[](1), operator[](2));
}


inline CML::pointField CML::triFace::points(const pointField& points) const
{
    pointField p(3);

    p[0] = points[operator[](0)];
    p[1] = points[operator[](1)];
    p[2] = points[operator[](2)];

    return p;
}


inline CML::face CML::triFace::triFaceFace() const
{
    CML::face f(3);

    f[0] = operator[](0);
    f[1] = operator[](1);
    f[2] = operator[](2);

    return f;
}


inline CML::triPointRef CML::triFace::tri(const pointField& points) const
{
    return triPointRef
    (
        points[operator[](0)],
        points[operator[](1)],
        points[operator[](2)]
    );
}


inline CML::point CML::triFace::centre(const pointField& points) const
{
    return (1.0/3.0)*
    (
        points[operator[](0)]
      + points[operator[](1)]
      + points[operator[](2)]
    );
}


inline CML::vector CML::triFace::area(const pointField& points) const
{
    return 0.5*
    (
        (points[operator[](1)] - points[operator[](0)])
       ^(points[operator[](2)] - points[operator[](0)])
    );
}


inline CML::scalar CML::triFace::mag(const pointField& points) const
{
    return ::CML::mag(area(points));
}


inline CML::vector CML::triFace::normal(const pointField& points) const
{
    const vector a = area(points);
    const scalar maga = CML::mag(a);
    return maga > 0 ? a/maga : Zero;
}


inline CML::label CML::triFace::nTriangles() const
{
    return 1;
}


inline CML::triFace CML::triFace::reverseFace() const
{
    // The starting points of the original and reverse face are identical.
    return triFace(operator[](0), operator[](2), operator[](1));
}


inline CML::scalar CML::triFace::sweptVol
(
    const pointField& opts,
    const pointField& npts
) const
{
    return (1.0/6.0)*
    (
        (
            (npts[operator[](0)] - opts[operator[](0)])
          & (
                (opts[operator[](1)] - opts[operator[](0)])
              ^ (opts[operator[](2)] - opts[operator[](0)])
            )
        )
      + (
            (npts[operator[](1)] - opts[operator[](1)])
          & (
                (opts[operator[](2)] - opts[operator[](1)])
              ^ (npts[operator[](0)] - opts[operator[](1)])
            )
        )
      + (
            (opts[operator[](2)] - npts[operator[](2)])
          & (
                (npts[operator[](1)] - npts[operator[](2)])
              ^ (npts[operator[](0)] - npts[operator[](2)])
            )
        )
    );
}


CML::tensor CML::triFace::inertia
(
    const pointField& points,
    const point& refPt,
    scalar density
) const
{
    // a triangle, do a direct calculation
    return this->tri(points).inertia(refPt, density);
}


inline CML::pointHit CML::triFace::ray
(
    const point& p,
    const vector& q,
    const pointField& points,
    const intersection::algorithm alg,
    const intersection::direction dir
) const
{
    return this->tri(points).ray(p, q, alg, dir);
}



inline CML::pointHit CML::triFace::intersection
(
    const point& p,
    const vector& q,
    const pointField& points,
    const intersection::algorithm alg,
    const scalar tol
) const
{
    return this->tri(points).intersection(p, q, alg, tol);
}


inline CML::pointHit CML::triFace::intersection
(
    const point& p,
    const vector& q,
    const point& ctr,
    const pointField& points,
    const intersection::algorithm alg,
    const scalar tol
) const
{
    return intersection(p, q, points, alg, tol);
}


inline CML::pointHit CML::triFace::nearestPoint
(
    const point& p,
    const pointField& points
) const
{
    return this->tri(points).nearestPoint(p);
}


inline CML::pointHit CML::triFace::nearestPointClassify
(
    const point& p,
    const pointField& points,
    label& nearType,
    label& nearLabel
) const
{
    return this->tri(points).nearestPointClassify(p, nearType, nearLabel);
}


inline CML::label CML::triFace::nEdges() const
{
    return 3;
}


inline CML::edgeList CML::triFace::edges() const
{
    edgeList e(3);

    e[0].start() = operator[](0);
    e[0].end()   = operator[](1);

    e[1].start() = operator[](1);
    e[1].end()   = operator[](2);

    e[2].start() = operator[](2);
    e[2].end()   = operator[](0);

    return e;
}


inline CML::edge CML::triFace::faceEdge(const label n) const
{
    return edge(operator[](n), operator[](fcIndex(n)));
}


// return
//  - +1: forward (counter-clockwise) on the face
//  - -1: reverse (clockwise) on the face
//  -  0: edge not found on the face
inline int CML::triFace::edgeDirection(const edge& e) const
{
    if
    (
        (operator[](0) == e.start() && operator[](1) == e.end())
     || (operator[](1) == e.start() && operator[](2) == e.end())
     || (operator[](2) == e.start() && operator[](0) == e.end())
    )
    {
        return 1;
    }
    else if
    (
        (operator[](0) == e.end() && operator[](1) == e.start())
     || (operator[](1) == e.end() && operator[](2) == e.start())
     || (operator[](2) == e.end() && operator[](0) == e.start())
    )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline bool CML::operator==(const triFace& a, const triFace& b)
{
    return triFace::compare(a,b) != 0;
}


inline bool CML::operator!=(const triFace& a, const triFace& b)
{
    return triFace::compare(a,b) == 0;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Type CML::triFace::average
(
    const pointField& meshPoints,
    const Field<Type>& fld
) const
{
    // a triangle, do a direct calculation
    return
    (
        (1.0/3.0)
      *
        (
            fld[operator[](0)]
          + fld[operator[](1)]
          + fld[operator[](2)]
        )
    );
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
