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

#include "fft.hpp"
#include "turbGen.hpp"
#include "Kmesh.hpp"
#include "primitiveFields.hpp"
#include "Ek.hpp"
#include "mathematicalConstants.hpp"


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::turbGen::turbGen(const Kmesh& k, const scalar EA, const scalar K0)
:
    K(k),
    Ea(EA),
    k0(K0),
    RanGen(label(0))
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::vectorField CML::turbGen::U()
{
    vectorField s(K.size());
    scalarField rndPhases(K.size());

    forAll(K, i)
    {
        s[i] = RanGen.sample01<vector>();
        rndPhases[i] = RanGen.scalar01();
    }

    s = K ^ s;
    s = s/(mag(s) + 1.0e-20);

    s = Ek(Ea, k0, mag(K))*s;

    complexVectorField up
    (
        fft::reverseTransform
        (
            ComplexField(cos(constant::mathematical::twoPi*rndPhases)*s,
            sin(constant::mathematical::twoPi*rndPhases)*s),
            K.nn()
        )
    );

    return ReImSum(up);
}


// ************************************************************************* //
