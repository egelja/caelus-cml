/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
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

\*---------------------------------------------------------------------------*/

#include "transform.hpp"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

CML::tensor CML::spatialTransform::Erx() const
{
    return E_ & *r_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::spatialTransform::spatialTransform()
:
    E_(tensor::I),
    r_(Zero)
{}


inline CML::spatialTransform::spatialTransform
(
    const tensor& E,
    const vector& r
)
:
    E_(E),
    r_(r)
{}


inline CML::spatialTransform::spatialTransform(Istream& is)
:
    E_(is),
    r_(is)
{}


inline CML::spatialTransform::transpose::transpose(const spatialTransform& X)
:
    X_(X)
{}


inline CML::spatialTransform::dual::dual(const spatialTransform& X)
:
    X_(X)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::tensor& CML::spatialTransform::E() const
{
    return E_;
}

inline CML::tensor& CML::spatialTransform::E()
{
    return E_;
}

inline const CML::vector& CML::spatialTransform::r() const
{
    return r_;
}

inline CML::vector& CML::spatialTransform::r()
{
    return r_;
}


inline CML::spatialTransform::transpose CML::spatialTransform::T() const
{
    return transpose(*this);
}


inline CML::spatialTransform CML::spatialTransform::inv() const
{
    return spatialTransform(E_.T(), -(E_ & r_));
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline CML::spatialTransform::dual CML::spatialTransform::operator*() const
{
    return dual(*this);
}


inline CML::spatialTransform::operator spatialTensor() const
{
    return spatialTensor
    (
        E_,             Zero,
       -Erx(),          E_
    );
}


inline void CML::spatialTransform::operator&=(const spatialTransform& X)
{
    E_ &= X.E_;
    r_ = X.r_ + (r_ & X.E_);
}


inline CML::spatialTransform CML::spatialTransform::operator&
(
    const spatialTransform& X
) const
{
    return spatialTransform(E_ & X.E_, X.r_ + (r_ & X.E_));
}


inline CML::spatialVector CML::spatialTransform::operator&
(
    const spatialVector& v
) const
{
    return spatialVector
    (
        E_ & v.w(),
        E_ & (v.l() - (r_ ^ v.w()))
    );
}


inline CML::spatialVector CML::spatialTransform::operator&&
(
    const spatialVector& v
) const
{
    return spatialVector
    (
        E_ & v.w(),
        E_ & (v.l() - r_)
    );
}


inline CML::vector CML::spatialTransform::transformPoint
(
    const vector& p
) const
{
    return E_ & (p - r_);
}


inline CML::spatialTransform::transpose::operator spatialTensor() const
{
    return spatialTensor
    (
        X_.E().T(),    -X_.Erx().T(),
        Zero,           X_.E().T()
    );
}


inline CML::spatialVector CML::spatialTransform::transpose::operator&
(
    const spatialVector& f
) const
{
    vector ETfl(X_.E().T() & f.l());

    return spatialVector
    (
        (X_.E().T() & f.w()) + (X_.r() ^ ETfl),
        ETfl
    );
}


inline CML::spatialTransform::dual::operator spatialTensor() const
{
    return spatialTensor
    (
        X_.E(),        -X_.Erx(),
        Zero,           X_.E()
    );
}


inline CML::spatialVector CML::spatialTransform::dual::operator&
(
    const spatialVector& f
) const
{
    return spatialVector
    (
        X_.E() & (f.w() - (X_.r() ^ f.l())),
        X_.E() & f.l()
    );
}


// * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * * //

inline CML::Istream& CML::operator>>(CML::Istream& is, spatialTransform& X)
{
    is >> X.E() >> X.r();
    return is;
}


inline CML::Ostream& CML::operator<<
(
    CML::Ostream& os,
    const spatialTransform& X
)
{
    os << X.E() << token::SPACE << X.r();
    return os;
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

//- Rotational spatial transformation tensor about the x-axis by omega radians
inline spatialTransform Xrx(const scalar& omega)
{
    return spatialTransform(Rx(omega), Zero);
}

//- Rotational spatial transformation tensor about the x-axis by omega radians
inline spatialTransform Xry(const scalar& omega)
{
    return spatialTransform(Ry(omega), Zero);
}

//- Rotational spatial transformation tensor about the z-axis by omega radians
inline spatialTransform Xrz(const scalar& omega)
{
    return spatialTransform(Rz(omega), Zero);
}

//- Rotational spatial transformation tensor about axis a by omega radians
inline spatialTransform Xr(const vector& a, const scalar omega)
{
    return spatialTransform(Ra(a, omega), Zero);
}

//- Translational spatial transformation tensor for translation r
inline spatialTransform Xt(const vector& r)
{
    return spatialTransform(tensor::I, r);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// ************************************************************************* //
