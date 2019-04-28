/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
Copyright (C) 2015 Applied CCM
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

#include "STARCDCoordinateRotation.hpp"

#include "mathematicalConstants.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(STARCDCoordinateRotation, 0);
    addToRunTimeSelectionTable
    (
        coordinateRotation,
        STARCDCoordinateRotation,
        dictionary
    );
    addToRunTimeSelectionTable
    (
        coordinateRotation,
        STARCDCoordinateRotation,
        objectRegistry
    );
}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

CML::vector CML::STARCDCoordinateRotation::transform(const vector& st) const
{
    return (R_ & st);
}


CML::vector CML::STARCDCoordinateRotation::invTransform
(
    const vector& st
) const
{
    return (Rtr_ & st);
}


CML::tmp<CML::vectorField> CML::STARCDCoordinateRotation::transform
(
    const vectorField& st
) const
{
    NotImplemented;
    return tmp<vectorField>(nullptr);
}


CML::tmp<CML::vectorField> CML::STARCDCoordinateRotation::invTransform
(
    const vectorField& st
) const
{
    NotImplemented;
    return tmp<vectorField>(nullptr);
}


const CML::tensorField& CML::STARCDCoordinateRotation::Tr() const
{
    NotImplemented;
     return NullSingletonRef<const tensorField>();
}


CML::tmp<CML::tensorField> CML::STARCDCoordinateRotation::transformTensor
(
    const tensorField& st
) const
{
     NotImplemented;
    return tmp<tensorField>(nullptr);
}


CML::tensor CML::STARCDCoordinateRotation::transformTensor
(
    const tensor& st
) const
{
    return (R_ & st & Rtr_);
}


CML::tmp<CML::tensorField> CML::STARCDCoordinateRotation::transformTensor
(
    const tensorField& st,
    const labelList& cellMap
) const
{
    NotImplemented;
    return tmp<tensorField>(nullptr);
}


CML::tmp<CML::symmTensorField> CML::STARCDCoordinateRotation::
transformVector
(
    const vectorField& st
) const
{
    tmp<symmTensorField> tfld(new symmTensorField(st.size()));
    symmTensorField& fld = tfld();

    forAll(fld, i)
    {
        fld[i] = transformPrincipal(R_, st[i]);
    }
    return tfld;
}


CML::symmTensor CML::STARCDCoordinateRotation::transformVector
(
    const vector& st
) const
{
    return transformPrincipal(R_, st);
}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::STARCDCoordinateRotation::calcTransform
(
    const scalar rotZ,
    const scalar rotX,
    const scalar rotY,
    const bool inDegrees
)
{
    scalar x = rotX;
    scalar y = rotY;
    scalar z = rotZ;

    if (inDegrees)
    {
        x *= constant::mathematical::pi/180.0;
        y *= constant::mathematical::pi/180.0;
        z *= constant::mathematical::pi/180.0;
    }

    R_ =
    (
        tensor
        (
            cos(y)*cos(z) - sin(x)*sin(y)*sin(z),
            -cos(x)*sin(z),
            sin(x)*cos(y)*sin(z) + sin(y)*cos(z),

            cos(y)*sin(z) + sin(x)*sin(y)*cos(z),
            cos(x)*cos(z),
            sin(y)*sin(z) - sin(x)*cos(y)*cos(z),

            -cos(x)*sin(y),
            sin(x),
            cos(x)*cos(y)
        )
    );

    Rtr_ = R_.T();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::STARCDCoordinateRotation::STARCDCoordinateRotation()
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{}


CML::STARCDCoordinateRotation::STARCDCoordinateRotation
(
    const vector& rotZrotXrotY,
    const bool inDegrees
)
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{
    calcTransform
    (
        rotZrotXrotY.component(vector::X),
        rotZrotXrotY.component(vector::Y),
        rotZrotXrotY.component(vector::Z),
        inDegrees
    );
}


CML::STARCDCoordinateRotation::STARCDCoordinateRotation
(
    const scalar rotZ,
    const scalar rotX,
    const scalar rotY,
    const bool inDegrees
)
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{
    calcTransform(rotZ, rotX, rotY, inDegrees);
}


CML::STARCDCoordinateRotation::STARCDCoordinateRotation
(
    const dictionary& dict
)
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{
    vector rotation(dict.lookup("rotation"));

    calcTransform
    (
        rotation.component(vector::X),
        rotation.component(vector::Y),
        rotation.component(vector::Z),
        dict.lookupOrDefault("degrees", true)
    );
}


CML::STARCDCoordinateRotation::STARCDCoordinateRotation
(
    const dictionary& dict,
    const objectRegistry&
)
{
    vector rotation(dict.lookup("rotation"));

    calcTransform
    (
        rotation.component(vector::X),
        rotation.component(vector::Y),
        rotation.component(vector::Z),
        dict.lookupOrDefault("degrees", true)
    );
}


void CML::STARCDCoordinateRotation::write(Ostream& os) const
{
     os.writeKeyword("e1") << e1() << token::END_STATEMENT << nl;
     os.writeKeyword("e2") << e2() << token::END_STATEMENT << nl;
     os.writeKeyword("e3") << e3() << token::END_STATEMENT << nl;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
