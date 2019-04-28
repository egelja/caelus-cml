/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
Copyright (C) 2016 Applied CCM Pty Ltd
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
    v_((sin(0.5*theta)/mag(d))*d)
{}


inline CML::quaternion::quaternion
(
    const vector& d,
    const scalar cosTheta,
    const bool normalized
)
{
    scalar cosHalfTheta2 = 0.5*(cosTheta + 1);
    w_ = sqrt(cosHalfTheta2);

    if (normalized)
    {
        v_ = sqrt(1 - cosHalfTheta2)*d;
    }
    else
    {
        v_ = (sqrt(1 - cosHalfTheta2)/mag(d))*d;
    }
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

inline CML::quaternion CML::quaternion::unit(const vector& v)
{
    return quaternion(sqrt(1 - magSqr(v)), v);
}

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

inline CML::quaternion::quaternion
(
    const rotationSequence rs,
    const vector& angles
)
{
    switch(rs)
    {
        case ZYX:
            operator=(quaternion(vector(0, 0, 1), angles.x()));
            operator*=(quaternion(vector(0, 1, 0), angles.y()));
            operator*=(quaternion(vector(1, 0, 0), angles.z()));
            break;

        case ZYZ:
            operator=(quaternion(vector(0, 0, 1), angles.x()));
            operator*=(quaternion(vector(0, 1, 0), angles.y()));
            operator*=(quaternion(vector(0, 0, 1), angles.z()));
            break;

        case ZXY:
            operator=(quaternion(vector(0, 0, 1), angles.x()));
            operator*=(quaternion(vector(1, 0, 0), angles.y()));
            operator*=(quaternion(vector(0, 1, 0), angles.z()));
            break;

        case ZXZ:
            operator=(quaternion(vector(0, 0, 1), angles.x()));
            operator*=(quaternion(vector(1, 0, 0), angles.y()));
            operator*=(quaternion(vector(0, 0, 1), angles.z()));
            break;

        case YXZ:
            operator=(quaternion(vector(0, 1, 0), angles.x()));
            operator*=(quaternion(vector(1, 0, 0), angles.y()));
            operator*=(quaternion(vector(0, 0, 1), angles.z()));
            break;

        case YXY:
            operator=(quaternion(vector(0, 1, 0), angles.x()));
            operator*=(quaternion(vector(1, 0, 0), angles.y()));
            operator*=(quaternion(vector(0, 1, 0), angles.z()));
            break;

        case YZX:
            operator=(quaternion(vector(0, 1, 0), angles.x()));
            operator*=(quaternion(vector(0, 0, 1), angles.y()));
            operator*=(quaternion(vector(1, 0, 0), angles.z()));
            break;

        case YZY:
            operator=(quaternion(vector(0, 1, 0), angles.x()));
            operator*=(quaternion(vector(0, 0, 1), angles.y()));
            operator*=(quaternion(vector(0, 1, 0), angles.z()));
            break;

        case XYZ:
            operator=(quaternion(vector(1, 0, 0), angles.x()));
            operator*=(quaternion(vector(0, 1, 0), angles.y()));
            operator*=(quaternion(vector(0, 0, 1), angles.z()));
            break;

        case XYX:
            operator=(quaternion(vector(1, 0, 0), angles.x()));
            operator*=(quaternion(vector(0, 1, 0), angles.y()));
            operator*=(quaternion(vector(1, 0, 0), angles.z()));
            break;

        case XZY:
            operator=(quaternion(vector(1, 0, 0), angles.x()));
            operator*=(quaternion(vector(0, 0, 1), angles.y()));
            operator*=(quaternion(vector(0, 1, 0), angles.z()));
            break;

        case XZX:
            operator=(quaternion(vector(1, 0, 0), angles.x()));
            operator*=(quaternion(vector(0, 0, 1), angles.y()));
            operator*=(quaternion(vector(1, 0, 0), angles.z()));
            break;

        default:
            FatalErrorInFunction
                << "Unknown rotation sequence " << rs << abort(FatalError);
            break;
    }
}

inline CML::quaternion::quaternion
(
    const tensor& rotationTensor
)
{
    scalar trace =
        rotationTensor.xx()
      + rotationTensor.yy()
      + rotationTensor.zz();

    if (trace > 0)
    {
        scalar s = 0.5/CML::sqrt(trace + 1.0);

        w_ = 0.25/s;
        v_[0] = (rotationTensor.zy() - rotationTensor.yz())*s;
        v_[1] = (rotationTensor.xz() - rotationTensor.zx())*s;
        v_[2] = (rotationTensor.yx() - rotationTensor.xy())*s;
    }
    else
    {
        if
        (
            rotationTensor.xx() > rotationTensor.yy()
         && rotationTensor.xx() > rotationTensor.zz()
        )
        {
            scalar s = 2.0*CML::sqrt
            (
                1.0
              + rotationTensor.xx()
              - rotationTensor.yy()
              - rotationTensor.zz()
            );

            w_ = (rotationTensor.zy() - rotationTensor.yz())/s;
            v_[0] = 0.25*s;
            v_[1] = (rotationTensor.xy() + rotationTensor.yx())/s;
            v_[2] = (rotationTensor.xz() + rotationTensor.zx())/s;
        }
        else if
        (
            rotationTensor.yy() > rotationTensor.zz()
        )
        {
            scalar s = 2.0*CML::sqrt
            (
                1.0
              + rotationTensor.yy()
              - rotationTensor.xx()
              - rotationTensor.zz()
            );

            w_ = (rotationTensor.xz() - rotationTensor.xz())/s;
            v_[0] = (rotationTensor.xy() + rotationTensor.yx())/s;
            v_[1] = 0.25*s;
            v_[2] = (rotationTensor.yz() + rotationTensor.zy())/s;
        }
        else
        {
            scalar s = 2.0*CML::sqrt
            (
                1.0
              + rotationTensor.zz()
              - rotationTensor.xx()
              - rotationTensor.yy()
            );

            w_ = (rotationTensor.yx() - rotationTensor.xy())/s;
            v_[0] = (rotationTensor.xz() + rotationTensor.zx())/s;
            v_[1] = (rotationTensor.yz() + rotationTensor.zy())/s;
            v_[2] = 0.25*s;
        }
    }
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

inline CML::quaternion CML::quaternion::normalized() const
{
    return *this/mag(*this);
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


inline CML::tensor CML::quaternion::R() const
{
    const scalar w2 = sqr(w());
    const scalar x2 = sqr(v().x());
    const scalar y2 = sqr(v().y());
    const scalar z2 = sqr(v().z());

    const scalar txy = 2*v().x()*v().y();
    const scalar twz = 2*w()*v().z();
    const scalar txz = 2*v().x()*v().z();
    const scalar twy = 2*w()*v().y();
    const scalar tyz = 2*v().y()*v().z();
    const scalar twx = 2*w()*v().x();

    return tensor
    (
        w2 + x2 - y2 - z2,  txy - twz,          txz + twy,
        txy + twz,          w2 - x2 + y2 - z2,  tyz - twx,
        txz - twy,          tyz + twx,          w2 - x2 - y2 + z2
    );
}


inline CML::vector CML::quaternion::eulerAngles(const quaternion& q) const
{
    vector angles(vector::zero);

    const scalar& w = q.w();
    const vector& v = q.v();

    // Calculate angle about X
    angles[0] = CML::atan2
    (
        2*(w*v.x() + v.y()*v.z()),
        1 - 2*(sqr(v.x()) + sqr(v.y()))
    );
    // Calculate angle about Y
    angles[1] = CML::asin(2*(w*v.y() - v.z()*v.x()));

    // Calculate angle about z
    angles[2] = CML::atan2
    (
        2*(w*v.z() + v.x()*v.y()),
        1 - 2*(sqr(v.y()) + sqr(v.z()))
    );


    return angles;

}


inline CML::vector CML::quaternion::twoAxes
(
    const scalar t11,
    const scalar t12,
    const scalar c2,
    const scalar t31,
    const scalar t32
)
{
    return vector(atan2(t11, t12), acos(c2), atan2(t31, t32));
}


inline CML::vector CML::quaternion::threeAxes
(
    const scalar t11,
    const scalar t12,
    const scalar s2,
    const scalar t31,
    const scalar t32
)
{
    return vector(atan2(t11, t12), asin(s2), atan2(t31, t32));
}


inline CML::vector CML::quaternion::eulerAngles
(
    const rotationSequence rs
) const
{
    const scalar w2 = sqr(w());
    const scalar x2 = sqr(v().x());
    const scalar y2 = sqr(v().y());
    const scalar z2 = sqr(v().z());

    switch(rs)
    {
        case ZYX:
            return threeAxes
            (
                2*(v().x()*v().y() + w()*v().z()),
                w2 + x2 - y2 - z2,
                2*(w()*v().y() - v().x()*v().z()),
                2*(v().y()*v().z() + w()*v().x()),
                w2 - x2 - y2 + z2
            );
            break;

        case ZYZ:
            return twoAxes
            (
                2*(v().y()*v().z() - w()*v().x()),
                2*(v().x()*v().z() + w()*v().y()),
                w2 - x2 - y2 + z2,
                2*(v().y()*v().z() + w()*v().x()),
                2*(w()*v().y() - v().x()*v().z())
            );
            break;

        case ZXY:
            return threeAxes
            (
                2*(w()*v().z() - v().x()*v().y()),
                w2 - x2 + y2 - z2,
                2*(v().y()*v().z() + w()*v().x()),
                2*(w()*v().y() - v().x()*v().z()),
                w2 - x2 - y2 + z2
            );
            break;

        case ZXZ:
            return twoAxes
            (
                2*(v().x()*v().z() + w()*v().y()),
                2*(w()*v().x() - v().y()*v().z()),
                w2 - x2 - y2 + z2,
                2*(v().x()*v().z() - w()*v().y()),
                2*(v().y()*v().z() + w()*v().x())
            );
            break;

        case YXZ:
            return threeAxes
            (
                2*(v().x()*v().z() + w()*v().y()),
                w2 - x2 - y2 + z2,
                2*(w()*v().x() - v().y()*v().z()),
                2*(v().x()*v().y() + w()*v().z()),
                w2 - x2 + y2 - z2
            );
            break;

        case YXY:
            return twoAxes
            (
                2*(v().x()*v().y() - w()*v().z()),
                2*(v().y()*v().z() + w()*v().x()),
                w2 - x2 + y2 - z2,
                2*(v().x()*v().y() + w()*v().z()),
                2*(w()*v().x() - v().y()*v().z())
            );
            break;

        case YZX:
            return threeAxes
            (
                2*(w()*v().y() - v().x()*v().z()),
                w2 + x2 - y2 - z2,
                2*(v().x()*v().y() + w()*v().z()),
                2*(w()*v().x() - v().y()*v().z()),
                w2 - x2 + y2 - z2
            );
            break;

        case YZY:
            return twoAxes
            (
                2*(v().y()*v().z() + w()*v().x()),
                2*(w()*v().z() - v().x()*v().y()),
                w2 - x2 + y2 - z2,
                2*(v().y()*v().z() - w()*v().x()),
                2*(v().x()*v().y() + w()*v().z())
            );
            break;

        case XYZ:
            return threeAxes
            (
                2*(w()*v().x() - v().y()*v().z()),
                w2 - x2 - y2 + z2,
                2*(v().x()*v().z() + w()*v().y()),
                2*(w()*v().z() - v().x()*v().y()),
                w2 + x2 - y2 - z2
            );
            break;

        case XYX:
            return twoAxes
            (
                2*(v().x()*v().y() + w()*v().z()),
                2*(w()*v().y() - v().x()*v().z()),
                w2 + x2 - y2 - z2,
                2*(v().x()*v().y() - w()*v().z()),
                2*(v().x()*v().z() + w()*v().y())
            );
            break;

        case XZY:
            return threeAxes
            (
                2*(v().y()*v().z() + w()*v().x()),
                w2 - x2 + y2 - z2,
                2*(w()*v().z() - v().x()*v().y()),
                2*(v().x()*v().z() + w()*v().y()),
                w2 + x2 - y2 - z2
            );
            break;

        case XZX:
            return twoAxes
            (
                2*(v().x()*v().z() - w()*v().y()),
                2*(v().x()*v().y() + w()*v().z()),
                w2 + x2 - y2 - z2,
                2*(v().x()*v().z() + w()*v().y()),
                2*(w()*v().z() - v().x()*v().y())
            );
            break;
        default:
            FatalErrorInFunction
                << "Unknown rotation sequence " << rs << abort(FatalError);
            return vector::zero;
            break;
    }
}


inline CML::vector CML::quaternion::rotVector(const quaternion& q) const
{
    if (sqrt(scalar(1) - sqr(q.w())) > SMALL)
    {
        return q.v()/sqrt(scalar(1) - sqr(q.w()));
    }
    else
    {
        // Return arbitrary unit vector
        return vector(1,0,0);
    }
}


inline CML::scalar CML::quaternion::rotAngle(const quaternion& q) const
{
    return scalar(2)*acos(q.w());
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
