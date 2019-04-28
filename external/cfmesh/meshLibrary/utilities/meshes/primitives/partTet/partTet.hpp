/*---------------------------------------------------------------------------*\
Copyright (C) Creative Fields, Ltd.
-------------------------------------------------------------------------------
License
    This file is part of cfMesh.

    cfMesh is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation; either version 3 of the License, or (at your
    option) any later version.

    cfMesh is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with cfMesh.  If not, see <http://www.gnu.org/licenses/>.

Class
    partTet

Description
    A partTet is a primitive needed during mesh smoothing

Author: Franjo Juretic (franjo.juretic@c-fields.com)


\*---------------------------------------------------------------------------*/

#ifndef partTet_HPP
#define partTet_HPP

#include "pointField.hpp"
#include "edge.hpp"
#include "FixedList.hpp"
#include "triangle.hpp"
#include "tetrahedron.hpp"
#include "IOstreams.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

class Istream;
class Ostream;

/*---------------------------------------------------------------------------*\
                           class partTet Declaration
\*---------------------------------------------------------------------------*/

class partTet
{
protected:

    // Protected data
        label data_[4];

public:

    // Constructors
        //- Null construct
        inline partTet();

        //- Construct from point labels
        inline partTet
        (
            const label a,
            const label b,
            const label c,
            const label d
        );

    // Destructor
        ~partTet();


    // Member Functions

        // Access

            //- Return vertices
            inline label a() const;

            inline label b() const;

            inline label c() const;

            inline label d() const;

            inline label size() const;

        // Searches
            //- find position of the node in the partTet
            inline label whichPosition(const label) const;

        // Properties

            //- Return face normal
            template<class PointField>
            inline vector Sa(const PointField&) const;

            template<class PointField>
            inline vector Sb(const PointField&) const;

            template<class PointField>
            inline vector Sc(const PointField&) const;

            template<class PointField>
            inline vector Sd(const PointField&) const;


            //- Return volume
            template<class PointField>
            inline scalar mag(const PointField&) const;

            //- Return circum-centre
            template<class PointField>
            inline point crcmCentre(const PointField&) const;

            template<class PointField>
            inline scalar crcmRadius(const PointField&) const;

            //- Return centroid of the tetrahedron
            template<class PointField>
            inline point centroid(const PointField&) const;

            //- Return edges
            inline FixedList<edge, 6> edges() const;

            //- Return edge
            inline edge getEdge(const label) const;

    // Member operators

            inline label operator[](const label) const;

            inline void operator=(const partTet&);

            inline bool operator==(const partTet&) const;
            inline bool operator!=(const partTet&) const;

    // IOstream operators

            friend Ostream& operator<<(Ostream&, const partTet&);
};


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline partTet::partTet()
{
}

inline partTet::partTet
(
    const label a,
    const label b,
    const label c,
    const label d
)
{
    data_[0] = a;
    data_[1] = b;
    data_[2] = c;
    data_[3] = d;
}

inline partTet::~partTet()
{
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline label partTet::a() const
{
    return data_[0];
}

inline label partTet::b() const
{
    return data_[1];
}

inline label partTet::c() const
{
    return data_[2];
}

inline label partTet::d() const
{
    return data_[3];
}

inline label partTet::size() const
{
    return 4;
}

inline label partTet::whichPosition(const label pointI) const
{
    for(label i=0;i<4;++i)
        if( data_[i] == pointI )
            return i;

    return -1;
}

template<class PointField>
inline vector partTet::Sa(const PointField& points) const
{
    triangle<point, point> tria
    (
        points[data_[1]],
        points[data_[2]],
        points[data_[3]]
    );

    return tria.normal();
    //return triangle<point, point>(b_, c_, d_).normal();
}

template<class PointField>
inline vector partTet::Sb(const PointField& points) const
{
    triangle<point, point> tria
    (
        points[data_[0]],
        points[data_[3]],
        points[data_[2]]
    );

    return tria.normal();
    //return triangle<point, point>(a_, d_, c_).normal();
}

template<class PointField>
inline vector partTet::Sc(const PointField& points) const
{
    triangle<point, point> tria
    (
        points[data_[0]],
        points[data_[1]],
        points[data_[3]]
    );

    return tria.normal();
    //return triangle<point, point>(a_, b_, d_).normal();
}

template<class PointField>
inline vector partTet::Sd(const PointField& points) const
{
    triangle<point, point> tria
    (
        points[data_[0]],
        points[data_[2]],
        points[data_[1]]
    );

    return tria.normal();
    //return triangle<point, point>(a_, c_, b_).normal();
}

template<class PointField>
inline scalar partTet::mag(const PointField& points) const
{
    tetrahedron<point, point> tet
    (
        points[data_[0]],
        points[data_[1]],
        points[data_[2]],
        points[data_[3]]
    );

    return tet.mag();
    //return (1.0/6.0)*(((b_ - a_) ^ (c_ - a_)) & (d_ - a_));
}

template<class PointField>
inline point partTet::crcmCentre(const PointField& points) const
{
    tetrahedron<point, point> tet
    (
        points[data_[0]],
        points[data_[1]],
        points[data_[2]],
        points[data_[3]]
    );

    return tet.circumCentre();
}

template<class PointField>
inline scalar partTet::crcmRadius(const PointField& points) const
{
    tetrahedron<point, point> tet
    (
        points[data_[0]],
        points[data_[1]],
        points[data_[2]],
        points[data_[3]]
    );

    return tet.circumRadius();
}

template<class PointField>
inline point partTet::centroid(const PointField& points) const
{
    point p = points[data_[0]];
    for(label i=1;i<4;++i)
        p += points[data_[i]];

    p /= 4;
    return p;
}

inline FixedList<edge, 6> partTet::edges() const
{
    FixedList<edge, 6> edges;
    edges[0] = edge(data_[0], data_[1]);
    edges[1] = edge(data_[0], data_[2]);
    edges[2] = edge(data_[0], data_[3]);
    edges[3] = edge(data_[3], data_[1]);
    edges[4] = edge(data_[1], data_[2]);
    edges[5] = edge(data_[3], data_[2]);

    return edges;
}

inline edge partTet::getEdge(const label eI) const
{
    switch( eI )
    {
        case 0:
        {
            return edge(data_[0], data_[1]);
        } break;
        case 1:
        {
            return edge(data_[0], data_[2]);
        } break;
        case 2:
        {
            return edge(data_[0], data_[3]);
        } break;
        case 3:
        {
            return edge(data_[3], data_[1]);
        } break;
        case 4:
        {
            return edge(data_[1], data_[2]);
        } break;
        case 5:
        {
            return edge(data_[3], data_[2]);
        } break;
    }

    FatalErrorInFunction
        << "Invalid edge index given " << eI << abort(FatalError);

    return edge();
}

inline label partTet::operator[](const label i) const
{
    return data_[i];
}

inline void partTet::operator=(const partTet& tet)
{
    for(label i=0;i<4;++i)
        data_[i] = tet.data_[i];
}

inline bool partTet::operator==(const partTet& tet) const
{
    if( this->a() != tet.a() )
        return false;
    if( this->b() != tet.b() )
        return false;
    if( this->c() != tet.c() )
        return false;
    if( this->d() != tet.d() )
        return false;

    return true;
}

inline bool partTet::operator!=(const partTet& tet) const
{
    return !this->operator==(tet);
}

// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline Ostream& operator<<(Ostream& os, const partTet& t)
{
    os  << nl
        << token::BEGIN_LIST
        << t.a() << token::SPACE << t.b()
        << token::SPACE << t.c() << token::SPACE << t.d()
        << token::END_LIST;

    return os;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
