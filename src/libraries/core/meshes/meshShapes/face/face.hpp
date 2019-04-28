/*---------------------------------------------------------------------------*\
Copyright (C) 2014 Applied CCM
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
    CML::face

Description
    A face is a list of labels corresponding to mesh vertices.

SeeAlso
    CML::triFace

SourceFiles
    faceI.hpp
    face.cpp
    faceIntersection.cpp
    faceContactSphere.cpp
    faceAreaInContact.cpp
    faceTemplates.cpp

\*---------------------------------------------------------------------------*/

#ifndef face_H
#define face_H

#include "pointField.hpp"
#include "labelList.hpp"
#include "edgeList.hpp"
#include "vectorField.hpp"
#include "faceListFwd.hpp"
#include "intersection.hpp"
#include "pointHit.hpp"
#include "ListListOps.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

class face;
class triFace;

template<class T, unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
class DynamicList;

inline bool operator==(const face& a, const face& b);
inline bool operator!=(const face& a, const face& b);
inline Istream& operator>>(Istream&, face&);

/*---------------------------------------------------------------------------*\
                           Class face Declaration
\*---------------------------------------------------------------------------*/

class face
:
    public labelList
{
    // Private Member Functions

        //- Edge to the right of face vertex i
        inline label right(const label i) const;

        //- Edge to the left of face vertex i
        inline label left(const label i) const;

        //- Construct list of edge vectors for face
        tmp<vectorField> calcEdges
        (
            const pointField& points
        ) const;

        //- Cos between neighbouring edges
        scalar edgeCos
        (
            const vectorField& edges,
            const label index
        ) const;

        //- Find index of largest internal angle on face
        label mostConcaveAngle
        (
            const pointField& points,
            const vectorField& edges,
            scalar& edgeCos
        ) const;

        //- Enumeration listing the modes for split()
        enum splitMode
        {
            COUNTTRIANGLE,  // count if split into triangles
            COUNTQUAD,      // count if split into triangles&quads
            SPLITTRIANGLE,  // split into triangles
            SPLITQUAD       // split into triangles&quads
        };

        //- Split face into triangles or triangles&quads.
        //  Stores results quadFaces[quadI], triFaces[triI]
        //  Returns number of new faces created
        label split
        (
            const splitMode mode,
            const pointField& points,
            label& triI,
            label& quadI,
            faceList& triFaces,
            faceList& quadFaces
        ) const;


public:

    //- Return types for classify
    enum proxType
    {
        NONE,
        POINT,  // Close to point
        EDGE    // Close to edge
    };

    // Static data members

        static const char* const typeName;


    // Constructors

        //- Construct null
        inline face();

        //- Construct given size
        explicit inline face(label);

        //- Construct from list of labels
        explicit inline face(const labelUList&);

        //- Construct from list of labels
        explicit inline face(const labelList&);

        //- Construct by transferring the parameter contents
        explicit inline face(const Xfer<labelList>&);

        //- Copy construct from triFace
        face(const triFace&);

        //- Construct from Istream
        inline face(Istream&);


    // Member Functions

        //- Collapse face by removing duplicate point labels
        //  return the collapsed size
        label collapse();

        //- Flip the face in-place.
        //  The starting points of the original and flipped face are identical.
        void flip();

        //- Return the points corresponding to this face
        inline pointField points(const pointField&) const;

        //- Centre point of face
        point centre(const pointField&) const;

        //- Calculate average value at centroid of face
        template<class Type>
        Type average(const pointField&, const Field<Type>&) const;

        //- Return vector area
        vector area(const pointField&) const;

        //- Return scalar magnitude
        inline scalar mag(const pointField&) const;

        //- Return unit normal
        vector normal(const pointField&) const;

        //- Return face with reverse direction
        //  The starting points of the original and reverse face are identical.
        face reverseFace() const;

        //- Navigation through face vertices

            //- Which vertex on face (face index given a global index)
            //  returns -1 if not found
            label which(const label globalIndex) const;

            //- Next vertex on face
            inline label nextLabel(const label i) const;

            //- Previous vertex on face
            inline label prevLabel(const label i) const;


        //- Return the volume swept out by the face when its points move
        scalar sweptVol
        (
            const pointField& oldPoints,
            const pointField& newPoints
        ) const;

        //- Return the inertia tensor, with optional reference
        //  point and density specification
        tensor inertia
        (
            const pointField&,
            const point& refPt = vector::zero,
            scalar density = 1.0
        ) const;

        //- Return potential intersection with face with a ray starting
        //  at p, direction n (does not need to be normalized)
        //  Does face-centre decomposition and returns triangle intersection
        //  point closest to p. Face-centre is calculated from point average.
        //  For a hit, the distance is signed.  Positive number
        //  represents the point in front of triangle
        //  In case of miss the point is the nearest point on the face
        //  and the distance is the distance between the intersection point
        //  and the original point.
        //  The half-ray or full-ray intersection and the contact
        //  sphere adjustment of the projection vector is set by the
        //  intersection parameters
        pointHit ray
        (
            const point& p,
            const vector& n,
            const pointField&,
            const intersection::algorithm alg = intersection::FULL_RAY,
            const intersection::direction dir = intersection::VECTOR
        ) const;

        //- Fast intersection with a ray.
        //  Does face-centre decomposition and returns triangle intersection
        //  point closest to p. See triangle::intersection for details.
        pointHit intersection
        (
            const point& p,
            const vector& q,
            const point& ctr,
            const pointField&,
            const intersection::algorithm alg,
            const scalar tol = 0.0
        ) const;

        //- Return nearest point to face
        pointHit nearestPoint
        (
            const point& p,
            const pointField&
        ) const;

        //- Return nearest point to face and classify it:
        //  + near point (nearType=POINT, nearLabel=0, 1, 2)
        //  + near edge (nearType=EDGE, nearLabel=0, 1, 2)
        //    Note: edges are counted from starting vertex so
        //    e.g. edge n is from f[n] to f[0], where the face has n + 1
        //    points
        pointHit nearestPointClassify
        (
            const point& p,
            const pointField&,
            label& nearType,
            label& nearLabel
        ) const;

        //- Return contact sphere diameter
        scalar contactSphereDiameter
        (
            const point& p,
            const vector& n,
            const pointField&
        ) const;

        //- Return area in contact, given the displacement in vertices
        scalar areaInContact
        (
            const pointField&,
            const scalarField& v
        ) const;

        //- Return number of edges
        inline label nEdges() const;

        //- Return edges in face point ordering,
        //  i.e. edges()[0] is edge between [0] and [1]
        edgeList edges() const;

        //- Return n-th face edge
        inline edge faceEdge(const label n) const;

        //- Return the edge direction on the face
        //  Returns:
        //  -  0: edge not found on the face
        //  - +1: forward (counter-clockwise) on the face
        //  - -1: reverse (clockwise) on the face
        int edgeDirection(const edge&) const;

        // Face splitting utilities

            //- Number of triangles after splitting
            inline label nTriangles() const;

            //- Number of triangles after splitting
            label nTriangles(const pointField& points) const;

            //- Split into triangles using existing points.
            //  Result in triFaces[triI..triI+nTri].
            //  Splits intelligently to maximize triangle quality.
            //  Returns number of faces created.
            label triangles
            (
                const pointField& points,
                label& triI,
                faceList& triFaces
            ) const;

            //- Split into triangles using existing points.
            //  Append to DynamicList.
            //  Returns number of faces created.
            template<unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
            label triangles
            (
                const pointField& points,
                DynamicList<face, SizeInc, SizeMult, SizeDiv>& triFaces
            ) const;

            //- Number of triangles and quads after splitting
            //  Returns the sum of both
            label nTrianglesQuads
            (
                const pointField& points,
                label& nTris,
                label& nQuads
            ) const;

            //- Split into triangles and quads.
            //  Results in triFaces (starting at triI) and quadFaces
            //  (starting at quadI).
            //  Returns number of new faces created.
            label trianglesQuads
            (
                const pointField& points,
                label& triI,
                label& quadI,
                faceList& triFaces,
                faceList& quadFaces
            ) const;

        //- Compare faces
        //   0: different
        //  +1: identical
        //  -1: same face, but different orientation
        static int compare(const face&, const face&);

        //- Return true if the faces have the same vertices
        static bool sameVertices(const face&, const face&);


    // Friend Operators

        friend bool operator==(const face& a, const face& b);
        friend bool operator!=(const face& a, const face& b);


    // Istream Operator

        friend Istream& operator>>(Istream&, face&);
};


//- Hash specialization to offset faces in ListListOps::combineOffset
template<>
class offsetOp<face>
{

public:

    inline face operator()
    (
        const face& x,
        const label offset
    ) const
    {
        face result(x.size());

        forAll(x, xI)
        {
            result[xI] = x[xI] + offset;
        }
        return result;
    }
};


// Global functions

//- Find the longest edge on a face. Face point labels index into pts.
label longestEdge(const face& f, const pointField& pts);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

// Edge to the right of face vertex i
inline CML::label CML::face::right(const label i) const
{
    return i;
}


// Edge to the left of face vertex i
inline CML::label CML::face::left(const label i) const
{
    return rcIndex(i);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::face::face()
{}


inline CML::face::face(label s)
:
    labelList(s, -1)
{}


inline CML::face::face(const labelUList& lst)
:
    labelList(lst)
{}


inline CML::face::face(const labelList& lst)
:
    labelList(lst)
{}


inline CML::face::face(const Xfer<labelList>& lst)
:
    labelList(lst)
{}


inline CML::face::face(Istream& is)
{
    is >> *this;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::pointField CML::face::points(const pointField& meshPoints) const
{
    // There are as many points as there labels for them
    pointField p(size());

    // For each point in list, set it to the point in 'pnts' addressed
    // by 'labs'
    forAll(p, i)
    {
        p[i] = meshPoints[operator[](i)];
    }

    // Return list
    return p;
}


inline CML::scalar CML::face::mag(const pointField& points) const
{
    return ::CML::mag(area(points));
}


inline CML::label CML::face::nEdges() const
{
    // for a closed polygon a number of edges is the same as number of points
    return size();
}


inline CML::edge CML::face::faceEdge(const label n) const
{
    return edge(operator[](n), operator[](fcIndex(n)));
}


// Next vertex on face
inline CML::label CML::face::nextLabel(const label i) const
{
    return operator[](fcIndex(i));
}


// Previous vertex on face
inline CML::label CML::face::prevLabel(const label i) const
{
    return operator[](rcIndex(i));
}

// Number of triangles directly known from number of vertices
inline CML::label CML::face::nTriangles() const
{
    return size() - 2;
}

// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

inline bool CML::operator==(const face& a, const face& b)
{
    return face::compare(a,b) != 0;
}


inline bool CML::operator!=(const face& a, const face& b)
{
    return face::compare(a,b) == 0;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

inline CML::Istream& CML::operator>>(Istream& is, face& f)
{
    if (is.version() == IOstream::originalVersion)
    {
        // Read starting (
        is.readBegin("face");

        // Read the 'name' token for the face
        token t(is);

        // Read labels
        is >> static_cast<labelList&>(f);

        // Read end)
        is.readEnd("face");
    }
    else
    {
        is >> static_cast<labelList&>(f);
    }

    // Check state of Ostream
    is.check("Istream& operator>>(Istream&, face&)");

    return is;
}


#include "DynamicList.hpp"

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<unsigned SizeInc, unsigned SizeMult, unsigned SizeDiv>
CML::label CML::face::triangles
(
    const pointField& points,
    DynamicList<face, SizeInc, SizeMult, SizeDiv>& triFaces
) const
{
    label triI = triFaces.size();
    label quadI = 0;
    faceList quadFaces;

    // adjust the addressable size (and allocate space if needed)
    triFaces.setSize(triI + nTriangles());

    return split(SPLITTRIANGLE, points, triI, quadI, triFaces, quadFaces);
}


template<class Type>
Type CML::face::average
(
    const pointField& meshPoints,
    const Field<Type>& fld
) const
{
    // Calculate the average by breaking the face into triangles and
    // area-weighted averaging their averages

    // If the face is a triangle, do a direct calculation
    if (size() == 3)
    {
        return
            (1.0/3.0)
           *(
               fld[operator[](0)]
             + fld[operator[](1)]
             + fld[operator[](2)]
            );
    }

    label nPoints = size();

    point centrePoint = point::zero;
    Type cf = pTraits<Type>::zero;

    for (register label pI=0; pI<nPoints; pI++)
    {
        centrePoint += meshPoints[operator[](pI)];
        cf += fld[operator[](pI)];
    }

    centrePoint /= nPoints;
    cf /= nPoints;

    scalar sumA = 0;
    Type sumAf = pTraits<Type>::zero;

    for (register label pI=0; pI<nPoints; pI++)
    {
        // Calculate 3*triangle centre field value
        Type ttcf  =
        (
            fld[operator[](pI)]
          + fld[operator[]((pI + 1) % nPoints)]
          + cf
        );

        // Calculate 2*triangle area
        scalar ta = CML::mag
        (
            (meshPoints[operator[](pI)] - centrePoint)
          ^ (meshPoints[operator[]((pI + 1) % nPoints)] - centrePoint)
        );

        sumA += ta;
        sumAf += ta*ttcf;
    }

    if (sumA > VSMALL)
    {
        return sumAf/(3*sumA);
    }
    else
    {
        return cf;
    }
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
