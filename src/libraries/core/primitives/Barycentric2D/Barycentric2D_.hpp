/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    CML::Barycentric2D

Description
    Templated 2D Barycentric derived from VectorSpace. Has 3 components, one of
    which is redundant.


\*---------------------------------------------------------------------------*/

#ifndef Barycentric2D_HPP
#define Barycentric2D_HPP

#include "VectorSpace.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class Barycentric2D Declaration
\*---------------------------------------------------------------------------*/

template<class Cmpt>
class Barycentric2D
:
    public VectorSpace<Barycentric2D<Cmpt>, Cmpt, 3>
{
public:

    //- Equivalent type of labels used for valid component indexing
    typedef Barycentric2D<label> labelType;


    // Member constants

        //- Rank of Barycentric2D is 1
        static const direction rank = 1;


    //- Component labeling enumeration
    enum components { A, B, C };


    // Constructors

        //- Construct null
        inline Barycentric2D();

        //- Construct initialized to zero
        inline Barycentric2D(const CML::zero);

        //- Construct given four components
        inline Barycentric2D
        (
            const Cmpt& va,
            const Cmpt& vb,
            const Cmpt& vc
        );


    // Member Functions

        // Access

            inline const Cmpt& a() const;
            inline const Cmpt& b() const;
            inline const Cmpt& c() const;

            inline Cmpt& a();
            inline Cmpt& b();
            inline Cmpt& c();
};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Cmpt>
inline CML::Barycentric2D<Cmpt>::Barycentric2D()
{}


template<class Cmpt>
inline CML::Barycentric2D<Cmpt>::Barycentric2D(const CML::zero)
:
    Barycentric2D::vsType(Zero)
{}


template<class Cmpt>
inline CML::Barycentric2D<Cmpt>::Barycentric2D
(
    const Cmpt& va,
    const Cmpt& vb,
    const Cmpt& vc
)
{
    this->v_[A] = va;
    this->v_[B] = vb;
    this->v_[C] = vc;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Cmpt>
inline const Cmpt& CML::Barycentric2D<Cmpt>::a() const
{
    return this->v_[A];
}


template<class Cmpt>
inline const Cmpt& CML::Barycentric2D<Cmpt>::b() const
{
    return this->v_[B];
}


template<class Cmpt>
inline const Cmpt& CML::Barycentric2D<Cmpt>::c() const
{
    return this->v_[C];
}


template<class Cmpt>
inline Cmpt& CML::Barycentric2D<Cmpt>::a()
{
    return this->v_[A];
}


template<class Cmpt>
inline Cmpt& CML::Barycentric2D<Cmpt>::b()
{
    return this->v_[B];
}


template<class Cmpt>
inline Cmpt& CML::Barycentric2D<Cmpt>::c()
{
    return this->v_[C];
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template<class Cmpt>
inline Cmpt operator&
(
    const Barycentric2D<Cmpt>& b1,
    const Barycentric2D<Cmpt>& b2
)
{
    return b1.a()*b2.a() + b1.b()*b2.b() + b1.c()*b2.c();
}


} // End namespace CML


#endif
