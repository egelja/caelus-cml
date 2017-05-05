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

inline CML::RBD::rigidBody::rigidBody
(
    const word& name,
    const scalar& m,
    const vector& c,
    const symmTensor& Ic
)
:
    rigidBodyInertia(m, c, Ic),
    name_(name)
{}


inline CML::RBD::rigidBody::rigidBody
(
    const word& name,
    const rigidBodyInertia& rbi
)
:
    rigidBodyInertia(rbi),
    name_(name)
{}


inline CML::RBD::rigidBody::rigidBody
(
    const word& name,
    const dictionary& dict
)
:
    rigidBodyInertia(dict),
    name_(name)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline const CML::word& CML::RBD::rigidBody::name() const
{
    return name_;
}


// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

inline CML::Ostream& CML::RBD::operator<<(Ostream& os, const rigidBody& rb)
{
    rb.write(os);
    return os;
}


// ************************************************************************* //
