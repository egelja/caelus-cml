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

\*---------------------------------------------------------------------------*/

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Cmpt>
inline CML::Vector<Cmpt>::Vector()
{}


template<class Cmpt>
inline CML::Vector<Cmpt>::Vector(const CML::zero)
:
    Vector::vsType(Zero)
{}


template<class Cmpt>
template<class Cmpt2>
inline CML::Vector<Cmpt>::Vector
(
    const VectorSpace<Vector<Cmpt2>, Cmpt2, 3>& vs
)
:
    Vector::vsType(vs)
{}


template<class Cmpt>
inline CML::Vector<Cmpt>::Vector
(
    const Cmpt& vx,
    const Cmpt& vy,
    const Cmpt& vz
)
{
    this->v_[X] = vx;
    this->v_[Y] = vy;
    this->v_[Z] = vz;
}


template<class Cmpt>
inline CML::Vector<Cmpt>::Vector(Istream& is)
:
    Vector::vsType(is)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Cmpt>
inline const Cmpt& CML::Vector<Cmpt>::x() const
{
    return this->v_[X];
}

template<class Cmpt>
inline const Cmpt& CML::Vector<Cmpt>::y() const
{
    return this->v_[Y];
}

template<class Cmpt>
inline const Cmpt& CML::Vector<Cmpt>::z() const
{
    return this->v_[Z];
}


template<class Cmpt>
inline Cmpt& CML::Vector<Cmpt>::x()
{
    return this->v_[X];
}

template<class Cmpt>
inline Cmpt& CML::Vector<Cmpt>::y()
{
    return this->v_[Y];
}

template<class Cmpt>
inline Cmpt& CML::Vector<Cmpt>::z()
{
    return this->v_[Z];
}


template<class Cmpt>
inline const Cmpt& CML::Vector<Cmpt>::operator()
(
    const direction i
) const
{
    return this->operator[](i);
}


template<class Cmpt>
inline Cmpt& CML::Vector<Cmpt>::operator()
(
    const direction i
)
{
    return this->operator[](i);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Cmpt>
inline const CML::Vector<Cmpt>& CML::Vector<Cmpt>::centre
(
    const CML::List<Vector<Cmpt> >&
)const
{
    return *this;
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Dummy innerProduct for scalar to allow the construction of vtables for
//  virtual member functions involving the inner-products of fields
//  for which a "NotImplemented" specialization for scalar is provided.
template<class Cmpt>
class innerProduct<Vector<Cmpt>, scalar>
{
public:

    typedef scalar type;
};


template<class Cmpt>
inline typename innerProduct<Vector<Cmpt>, Vector<Cmpt> >::type
operator&(const Vector<Cmpt>& v1, const Vector<Cmpt>& v2)
{
    return Cmpt(v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z());
}


template<class Cmpt>
inline Vector<Cmpt> operator^(const Vector<Cmpt>& v1, const Vector<Cmpt>& v2)
{
    return Vector<Cmpt>
    (
        (v1.y()*v2.z() - v1.z()*v2.y()),
        (v1.z()*v2.x() - v1.x()*v2.z()),
        (v1.x()*v2.y() - v1.y()*v2.x())
    );
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

template<class Cmpt>
inline Vector<Cmpt> perpendicular(const Vector<Cmpt>& v)
{
    Vector<Cmpt> u(Zero);
    u[findMin(cmptMag(v))] = 1;
    return u ^ v;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
