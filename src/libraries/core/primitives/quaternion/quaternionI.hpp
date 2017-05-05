/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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

inline CML::quaternion::quaternion()
{}

inline CML::quaternion::quaternion(const scalar w, const vector& v)
:
    w_(w),
    v_(v)
{}

inline CML::quaternion::quaternion(const vector& d, const scalar theta)
:
    w_(cos(0.5*theta)),
    v_((sin(0.5*theta)/magSqr(d))*d)
{
    normalize();
}

inline CML::quaternion::quaternion(const scalar w)
:
    w_(w),
    v_(vector::zero)
{}

inline CML::quaternion::quaternion(const vector& v)
:
    w_(0),
    v_(v)
{}

inline CML::quaternion::quaternion
(
    const scalar angleX,
    const scalar angleY,
    const scalar angleZ
)
{
    operator=(quaternion(vector(1, 0, 0), angleX));
    operator*=(quaternion(vector(0, 1, 0), angleY));
    operator*=(quaternion(vector(0, 0, 1), angleZ));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::scalar CML::quaternion::w() const
{
    return w_;
}


inline const CML::vector& CML::quaternion::v() const
{
    return v_;
}


inline CML::scalar& CML::quaternion::w()
{
    return w_;
}


inline CML::vector& CML::quaternion::v()
{
    return v_;
}


inline void CML::quaternion::normalize()
{
    operator/=(mag(*this));
}


inline CML::quaternion CML::quaternion::mulq0v(const vector& u) const
{
    return quaternion(-(v() & u), w()*u + (v() ^ u));
}


inline CML::vector CML::quaternion::transform(const vector& u) const
{
    return (mulq0v(u)*conjugate(*this)).v();
}


inline CML::vector CML::quaternion::invTransform(const vector& u) const
{
    return (conjugate(*this).mulq0v(u)*(*this)).v();
}


inline CML::quaternion CML::quaternion::transform(const quaternion& q) const
{
    return CML::normalize((*this)*q);
}


inline CML::quaternion CML::quaternion::invTransform
(
    const quaternion& q
) const
{
    return CML::normalize(conjugate(*this)*q);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline void CML::quaternion::operator=(const quaternion& q)
{
    w_ = q.w_;
    v_ = q.v_;
}

inline void CML::quaternion::operator+=(const quaternion& q)
{
    w_ += q.w_;
    v_ += q.v_;
}

inline void CML::quaternion::operator-=(const quaternion& q)
{
    w_ -= q.w_;
    v_ -= q.v_;
}

inline void CML::quaternion::operator*=(const quaternion& q)
{
    scalar w0 = w();
    w() = w()*q.w() - (v() & q.v());
    v() = w0*q.v() + q.w()*v() + (v() ^ q.v());
}

inline void CML::quaternion::operator/=(const quaternion& q)
{
    return operator*=(inv(q));
}


inline void CML::quaternion::operator=(const scalar s)
{
    w_ = s;
}


inline void CML::quaternion::operator=(const vector& v)
{
    v_ = v;
}


inline void CML::quaternion::operator*=(const scalar s)
{
    w_ *= s;
    v_ *= s;
}

inline void CML::quaternion::operator/=(const scalar s)
{
    w_ /= s;
    v_ /= s;
}


// * * * * * * * * * * * * * * * Global Functions  * * * * * * * * * * * * * //

inline CML::scalar CML::magSqr(const quaternion& q)
{
    return magSqr(q.w()) + magSqr(q.v());
}


inline CML::scalar CML::mag(const quaternion& q)
{
    return sqrt(magSqr(q));
}


inline CML::quaternion CML::conjugate(const quaternion& q)
{
    return quaternion(q.w(), -q.v());
}


inline CML::quaternion CML::inv(const quaternion& q)
{
    scalar magSqrq = magSqr(q);
    return quaternion(q.w()/magSqrq, -q.v()/magSqrq);
}


inline CML::quaternion CML::normalize(const quaternion& q)
{
    return q/mag(q);
}


inline CML::tensor CML::quaternion::R() const
{
    scalar w2 = sqr(w());
    scalar x2 = sqr(v().x());
    scalar y2 = sqr(v().y());
    scalar z2 = sqr(v().z());

    scalar txy = 2*v().x()*v().y();
    scalar twz = 2*w()*v().z();
    scalar txz = 2*v().x()*v().z();
    scalar twy = 2*w()*v().y();
    scalar tyz = 2*v().y()*v().z();
    scalar twx = 2*w()*v().x();

    return tensor
    (
        w2 + x2 - y2 - z2,  txy - twz,          txz + twy,
        txy + twz,          w2 - x2 + y2 - z2,  tyz - twx,
        txz - twy,          tyz + twx,          w2 - x2 - y2 + z2
    );
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

inline bool CML::operator==(const quaternion& q1, const quaternion& q2)
{
    return (equal(q1.w(), q2.w()) && equal(q1.v(), q2.v()));
}


inline bool CML::operator!=(const quaternion& q1, const quaternion& q2)
{
    return !operator==(q1, q2);
}


inline CML::quaternion CML::operator+
(
    const quaternion& q1,
    const quaternion& q2
)
{
    return quaternion(q1.w() + q2.w(), q1.v() + q2.v());
}


inline CML::quaternion CML::operator-(const quaternion& q)
{
    return quaternion(-q.w(), -q.v());
}


inline CML::quaternion CML::operator-
(
    const quaternion& q1,
    const quaternion& q2
)
{
    return quaternion(q1.w() - q2.w(), q1.v() - q2.v());
}


inline CML::scalar CML::operator&(const quaternion& q1, const quaternion& q2)
{
    return q1.w()*q2.w() + (q1.v() & q2.v());
}


inline CML::quaternion CML::operator*
(
    const quaternion& q1,
    const quaternion& q2
)
{
    return quaternion
    (
        q1.w()*q2.w() - (q1.v() & q2.v()),
        q1.w()*q2.v() + q2.w()*q1.v() + (q1.v() ^ q2.v())
    );
}


inline CML::quaternion CML::operator/
(
    const quaternion& q1,
    const quaternion& q2
)
{
    return q1*inv(q2);
}


inline CML::quaternion CML::operator*(const scalar s, const quaternion& q)
{
    return quaternion(s*q.w(), s*q.v());
}


inline CML::quaternion CML::operator*(const quaternion& q, const scalar s)
{
    return quaternion(s*q.w(), s*q.v());
}


inline CML::quaternion CML::operator/(const quaternion& q, const scalar s)
{
    return quaternion(q.w()/s, q.v()/s);
}


// ************************************************************************* //
