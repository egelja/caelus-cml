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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::RBD::joint::joint(const label nDoF)
:
    S_(nDoF),
    index_(0),
    qIndex_(0)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::label CML::RBD::joint::nDoF() const
{
    return S_.size();
}

inline bool CML::RBD::joint::unitQuaternion() const
{
    return false;
}

inline CML::label CML::RBD::joint::index() const
{
    return index_;
}

inline CML::label CML::RBD::joint::qIndex() const
{
    return qIndex_;
}

inline const CML::List<CML::spatialVector>& CML::RBD::joint::S() const
{
    return S_;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline CML::quaternion CML::RBD::joint::unitQuaternion
(
    const scalarField& q
) const
{
    if (!unitQuaternion())
    {
        FatalErrorInFunction
            << "Attempt to get the quaternion for a non-spherical joint"
            << abort(FatalError);
    }

    return quaternion::unit(q.block<vector>(qIndex_));
}


inline void CML::RBD::joint::unitQuaternion
(
    const quaternion& quat,
    scalarField& q
) const
{
    if (!unitQuaternion())
    {
        FatalErrorInFunction
            << "Attempt to set quaternion for a non-spherical joint"
            << abort(FatalError);
    }

    q[qIndex_] = quat.v().x();
    q[qIndex_+1] = quat.v().y();
    q[qIndex_+2] = quat.v().z();
}


CML::autoPtr<CML::RBD::joint> CML::RBD::joint::iNew::operator()
(
    Istream& is
) const
{
    dictionary dict(is);
    return New(dict);
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline CML::Ostream& CML::RBD::operator<<(Ostream& os, const joint& j)
{
    os  << indent << token::BEGIN_BLOCK << incrIndent << endl;
    j.write(os);
    os  << decrIndent << indent << token::END_BLOCK;

    return os;
}


// ************************************************************************* //
