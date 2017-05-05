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

inline CML::RBD::subBody::subBody
(
    const autoPtr<rigidBody>& bodyPtr,
    const word& masterName,
    const label masterID,
    const spatialTransform& masterXT
)
:
    body_(bodyPtr),
    masterName_(masterName),
    masterID_(masterID),
    masterXT_(masterXT)
{}


inline CML::autoPtr<CML::RBD::subBody> CML::RBD::subBody::clone() const
{
    return autoPtr<subBody>(new subBody(*this));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::RBD::rigidBody& CML::RBD::subBody::body() const
{
    return body_();
}


inline const CML::word& CML::RBD::subBody::name() const
{
    return body_->name();
}


inline const CML::word& CML::RBD::subBody::masterName() const
{
    return masterName_;
}


inline CML::label CML::RBD::subBody::masterID() const
{
    return masterID_;
}


inline const CML::spatialTransform& CML::RBD::subBody::masterXT() const
{
    return masterXT_;
}


// ************************************************************************* //
