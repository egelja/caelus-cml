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
    pointFieldPMG

Description
    This is a container with additional size to prevent re-allocation
    every time it is resized

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles


\*---------------------------------------------------------------------------*/

#ifndef pointFieldPMG_HPP
#define pointFieldPMG_HPP

#include "regIOobject.hpp"
#include "pointField.hpp"
#include "Istream.hpp"
#include "Ostream.hpp"
#include "SubList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class pointFieldPMG
:   public regIOobject,
    public pointField
{
    // Private data
        //- number of used elements
        label nElmts_;

    // Disallow bitwise assignment
        void operator=(const pointFieldPMG&);

        pointFieldPMG(const pointFieldPMG&);

    // Disallow transfer from pointField
        void transfer(pointField&);

public:

    // Declare type name
        TypeName("vectorField");

    // Constructors
        //- construct from IOobject
        inline pointFieldPMG(const IOobject&);

        //- construct from IOobject and size
        inline pointFieldPMG(const IOobject&, const label);

        //- construct from IOobject and pointField
        inline pointFieldPMG(const IOobject&, const pointField&);

    // Destructor
        inline ~pointFieldPMG();

    // Member functions
        //- return the number of used elements
        inline label size() const;

        //- set the number of used elements
        inline void setSize(const label nElmts);

        //- reserve the memory for the specified number of elements
        //- the request is ignored if the specified value is smaller than
        //- the current number of elements
        //- does not change the size
        inline void reserve(const label capacity);

        //- set the size to zero
        inline void clear();

        //- add a point at the end of the list
        inline void append(const point&);

        //- return an element with bound checking
        inline point& newElmt(const label);

        //- read/write the list onto disk
        inline bool writeData(Ostream&) const;

    // Member operators
        inline void operator=(const pointField&);

        friend inline Ostream& operator<<(Ostream&, const pointFieldPMG&);

        friend inline Istream& operator>>(Istream&, pointFieldPMG&);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Constructors

inline pointFieldPMG::pointFieldPMG(const IOobject& io)
:
    regIOobject(io),
    pointField(readStream(typeName)),
    nElmts_(pointField::size())
{
    close();
}

inline pointFieldPMG::pointFieldPMG(const IOobject& io, const label s)
:
    regIOobject(io),
    pointField(s),
    nElmts_(s)
{}

inline pointFieldPMG::pointFieldPMG(const IOobject& io, const pointField& pts)
:
    regIOobject(io),
    pointField(pts),
    nElmts_(pts.size())
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Destructor

inline pointFieldPMG::~pointFieldPMG()
{}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
            
inline label pointFieldPMG::size() const
{
    return nElmts_;
}

inline void pointFieldPMG::setSize(const label nElmts)
{
    if( nElmts >= pointField::size() )
    {
        Info << "Resizing points!" << endl;
        pointField::setSize(label(1.5*nElmts)+1);
    }

    nElmts_ = nElmts;
}

inline void pointFieldPMG::reserve(const label capacity)
{
    if( capacity > size() )
        this->setSize(capacity);
}

inline void pointFieldPMG::clear()
{
    nElmts_ = 0;
}

inline void pointFieldPMG::append(const point& p)
{
    const label i = nElmts_;
    setSize(i+1);
    this->operator[](i) = p;
}

inline point& pointFieldPMG::newElmt(const label pI)
{
    setSize(pI+1);
    return this->operator[](pI);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline bool pointFieldPMG::writeData(Ostream& os) const
{
    return (os << *this).good();
}

inline void pointFieldPMG::operator=(const pointField& pts)
{
    setSize(pts.size());
    forAll(pts, pI)
        this->operator[](pI) = pts[pI];
}
    
inline Ostream& operator<<(Ostream& os, const pointFieldPMG& pts)
{
    SubList<point> p(pts, pts.nElmts_, 0);
    
    os << p;
    return os;
}

inline Istream& operator>>(Istream& is, pointFieldPMG& pts)
{
    pointField& points = static_cast<pointField&>(pts);
    is >> points;
    pts.nElmts_  = points.size();
    
    return is;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
