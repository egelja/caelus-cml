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

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline CML::symmTensor CML::RBD::cuboid::I
(
    const scalar m,
    const vector& L
) const
{
    const scalar mBy12 = m/12.0;
    const scalar mSqrLx = mBy12*sqr(L.x());
    const scalar mSqrLy = mBy12*sqr(L.y());
    const scalar mSqrLz = mBy12*sqr(L.z());

    return symmTensor
    (
        mSqrLy + mSqrLz, 0,               0,
                         mSqrLx + mSqrLz, 0,
                                          mSqrLx + mSqrLy
    );
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::RBD::cuboid::cuboid
(
    const word& name,
    const scalar m,
    const vector& c,
    const vector& L
)
:
    rigidBody(name, m, c, I(m, L)),
    L_(L)
{}


inline CML::RBD::cuboid::cuboid
(
    const word& name,
    const dictionary& dict
)
:
    rigidBody(name, rigidBodyInertia()),
    L_(dict.lookup("L"))
{
    const scalar m(readScalar(dict.lookup("mass")));
    const vector c(dict.lookup("centreOfMass"));
    rigidBodyInertia::operator=(rigidBodyInertia(m, c, I(m, L_)));
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

inline const CML::vector& CML::RBD::cuboid::L() const
{
    return L_;
}


// ************************************************************************* //
