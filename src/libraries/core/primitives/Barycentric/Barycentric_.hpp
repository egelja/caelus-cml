/*---------------------------------------------------------------------------*\
Copyright (C) 2016-2018 OpenFOAM Foundation
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
    CML::Barycentric

Description
    Templated 3D Barycentric derived from VectorSpace. Has 4 components, one of
    which is redundant.


\*---------------------------------------------------------------------------*/

#ifndef Barycentric_HPP
#define Barycentric_HPP

#include "VectorSpace.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class Barycentric Declaration
\*---------------------------------------------------------------------------*/

template<class Cmpt>
class Barycentric
:
    public VectorSpace<Barycentric<Cmpt>, Cmpt, 4>
{
public:

    //- Equivalent type of labels used for valid component indexing
    typedef Barycentric<label> labelType;


    // Member constants

        //- Rank of Barycentric is 1
        static const direction rank = 1;


    //- Component labeling enumeration
    enum components { A, B, C, D };


    // Constructors

        //- Construct null
        inline Barycentric();

        //- Construct initialized to zero
        inline Barycentric(const CML::zero);

        //- Construct given four components
        inline Barycentric
        (
            const Cmpt& va,
            const Cmpt& vb,
            const Cmpt& vc,
            const Cmpt& vd
        );


    // Member Functions

        // Access

            inline const Cmpt& a() const;
            inline const Cmpt& b() const;
            inline const Cmpt& c() const;
            inline const Cmpt& d() const;

            inline Cmpt& a();
            inline Cmpt& b();
            inline Cmpt& c();
            inline Cmpt& d();
};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Cmpt>
inline CML::Barycentric<Cmpt>::Barycentric()
{}


template<class Cmpt>
inline CML::Barycentric<Cmpt>::Barycentric(const CML::zero)
:
    Barycentric::vsType(Zero)
{}


template<class Cmpt>
inline CML::Barycentric<Cmpt>::Barycentric
(
    const Cmpt& va,
    const Cmpt& vb,
    const Cmpt& vc,
    const Cmpt& vd
)
{
    this->v_[A] = va;
    this->v_[B] = vb;
    this->v_[C] = vc;
    this->v_[D] = vd;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Cmpt>
inline const Cmpt& CML::Barycentric<Cmpt>::a() const
{
    return this->v_[A];
}


template<class Cmpt>
inline const Cmpt& CML::Barycentric<Cmpt>::b() const
{
    return this->v_[B];
}


template<class Cmpt>
inline const Cmpt& CML::Barycentric<Cmpt>::c() const
{
    return this->v_[C];
}


template<class Cmpt>
inline const Cmpt& CML::Barycentric<Cmpt>::d() const
{
    return this->v_[D];
}


template<class Cmpt>
inline Cmpt& CML::Barycentric<Cmpt>::a()
{
    return this->v_[A];
}


template<class Cmpt>
inline Cmpt& CML::Barycentric<Cmpt>::b()
{
    return this->v_[B];
}


template<class Cmpt>
inline Cmpt& CML::Barycentric<Cmpt>::c()
{
    return this->v_[C];
}


template<class Cmpt>
inline Cmpt& CML::Barycentric<Cmpt>::d()
{
    return this->v_[D];
}


namespace CML
{

// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template<class Cmpt>
inline Cmpt operator&(const Barycentric<Cmpt>& b1, const Barycentric<Cmpt>& b2)
{
    return b1.a()*b2.a() + b1.b()*b2.b() + b1.c()*b2.c() + b1.d()*b2.d();
}


} // End namespace CML


#endif
