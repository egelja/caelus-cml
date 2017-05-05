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
    faceListPMG

Description
    This is a container with additional size to prevent re-allocation
    every time it is resized

Author: Franjo Juretic (franjo.juretic@c-fields.com)

SourceFiles


\*---------------------------------------------------------------------------*/

#ifndef faceListPMG_HPP
#define faceListPMG_HPP

#include "regIOobject.hpp"
#include "faceList.hpp"
#include "Istream.hpp"
#include "Ostream.hpp"
#include "SubList.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

class faceListPMG
:   public regIOobject,
    public faceList
{
    // Private data
        //- number of used elements
        label nElmts_;

    // Disallow bitwise assignment
        void operator=(const faceListPMG&);

        faceListPMG(const faceListPMG&);

    // Disallow transfer from faceList
        void transfer(faceList&);

public:

        TypeName("faceList");

    // Constructors
        //- construct from IOobject
        inline faceListPMG(const IOobject&);

        //- construct from IOobject and size
        inline faceListPMG(const IOobject&, const label);

        //- construct from IOobject and faceList
        inline faceListPMG(const IOobject&, const faceList&);

    // Destructor
        inline ~faceListPMG();

    // Member functions
        //- return the number of used elements
        inline label size() const;

        //- set the number of used elements
        inline void setSize(const label nElmts);

        //- set the size to zero
        inline void clear();

        //- add a face at the end of the list
        inline void append(const face&);

        //- return an element with bound checking
        inline face& newElmt(const label);

        //- read/write the list onto disk
        inline bool writeData(Ostream&) const;

    // Member operators
        inline void operator=(const faceList&);

        friend inline Ostream& operator<<(Ostream&, const faceListPMG&);

        friend inline Istream& operator>>(Istream&, faceListPMG&);
};

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// Constructors

inline faceListPMG::faceListPMG(const IOobject& io)
:
    regIOobject(io),
    faceList(readStream(typeName)),
    nElmts_(faceList::size())
{
}

inline faceListPMG::faceListPMG(const IOobject& io, const label s)
:
    regIOobject(io),
    faceList(s),
    nElmts_(s)
{
}

inline faceListPMG::faceListPMG(const IOobject& io, const faceList& fcs)
:
    regIOobject(io),
    faceList(fcs),
    nElmts_(fcs.size())
{
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Destructor

inline faceListPMG::~faceListPMG()
{
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
            
inline label faceListPMG::size() const
{
    return nElmts_;
}

inline void faceListPMG::setSize(const label nElmts)
{
    if( nElmts >= faceList::size() )
    {
        if( faceList::size() != 0 )
        {
            Info << "Resizing faces!" << endl;
            faceList copy(label(1.5*nElmts));
            for(label i=0;i<nElmts_;++i)
                copy[i].transfer(this->operator[](i));
            
            faceList::transfer(copy);
        }
        else
        {
            faceList::setSize(label(1.5*nElmts));
        }
    }

    nElmts_ = nElmts;
}

inline void faceListPMG::clear()
{
    nElmts_ = 0;
}

inline void faceListPMG::append(const face& f)
{
    const label i = nElmts_;
    setSize(i+1);
    this->operator[](i) = f;
}

inline face& faceListPMG::newElmt(const label fI)
{
    setSize(fI+1);
    return this->operator[](fI);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

inline bool faceListPMG::writeData(Ostream& os) const
{
    return (os << *this).good();
}

inline void faceListPMG::operator=(const faceList& fcs)
{
    setSize(fcs.size());
    forAll(fcs, fI)
        this->operator[](fI) = fcs[fI];
}
    
inline Ostream& operator<<(Ostream& os, const faceListPMG& fcs)
{
    SubList<face> f(fcs, fcs.nElmts_, 0);
    
    os << f;
    return os;
}

inline Istream& operator>>(Istream& is, faceListPMG& fcs)
{
    faceList& faces = static_cast<faceList&>(fcs);
    is >> faces;
    fcs.nElmts_  = faces.size();
    
    return is;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
