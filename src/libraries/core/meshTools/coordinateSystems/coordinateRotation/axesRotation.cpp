/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2015 OpenFOAM Foundation
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

#include "axesRotation.hpp"
#include "dictionary.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(axesRotation, 0);
    addToRunTimeSelectionTable(coordinateRotation, axesRotation, dictionary);
    addToRunTimeSelectionTable
    (
        coordinateRotation,
        axesRotation,
        objectRegistry
    );
}

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

void CML::axesRotation::calcTransform
(
    const vector& axis1,
    const vector& axis2,
    const axisOrder& order
)
{
    vector a = axis1/mag(axis1);
    vector b = axis2;

    b = b - (b & a)*a;

    if (mag(b) < SMALL)
    {
        FatalErrorInFunction
            << "axis1, axis2 appear co-linear: "
            << axis1 << ", " << axis2 << endl
            << abort(FatalError);
    }

    b = b/mag(b);
    vector c = a^b;

    tensor Rtr;
    switch (order)
    {
        case e1e2:
        {
            Rtr = tensor(a, b, c);
            break;
        }
        case e2e3:
        {
            Rtr = tensor(c, a, b);
            break;
        }
        case e3e1:
        {
            Rtr = tensor(b, c, a);
            break;
        }
        default:
        {
            FatalErrorInFunction
                << "Unhandled axes specifictation" << endl
                << abort(FatalError);

            Rtr = tensor::zero;
            break;
        }
    }

    // the global->local transformation
    Rtr_ = Rtr;
    // the local->global transformation
    R_ = Rtr.T();
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::axesRotation::axesRotation()
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{}


CML::axesRotation::axesRotation
(
    const vector& axis,
    const vector& dir
)
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{
    calcTransform(axis, dir, e3e1);
}


CML::axesRotation::axesRotation
(
    const dictionary& dict
)
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{
    operator=(dict);
}


CML::axesRotation::axesRotation
(
    const dictionary& dict,
    const objectRegistry& obr
)
:
    R_(sphericalTensor::I),
    Rtr_(R_)
{
    operator=(dict);
}


CML::axesRotation::axesRotation(const tensor& R)
:
    R_(R),
    Rtr_(R_.T())
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const CML::tensorField& CML::axesRotation::Tr() const
{
    NotImplemented;
    return NullSingletonRef<const tensorField>();
}


CML::tmp<CML::vectorField> CML::axesRotation::transform
(
    const vectorField& st
) const
{
    return (R_ & st);
}


CML::vector CML::axesRotation::transform(const vector& st) const
{
    return (R_ & st);
}


CML::tmp<CML::vectorField> CML::axesRotation::invTransform
(
    const vectorField& st
) const
{
    return (Rtr_ & st);
}


CML::vector CML::axesRotation::invTransform(const vector& st) const
{
    return (Rtr_ & st);
}


CML::tmp<CML::tensorField> CML::axesRotation::transformTensor
(
    const tensorField& st
) const
{
    NotImplemented;
    return tmp<tensorField>(nullptr);
}


CML::tensor CML::axesRotation::transformTensor
(
    const tensor& st
) const
{
    return (R_ & st & Rtr_);
}


CML::tmp<CML::tensorField> CML::axesRotation::transformTensor
(
    const tensorField& st,
    const labelList& cellMap
) const
{
    NotImplemented;
    return tmp<tensorField>(nullptr);
}


CML::tmp<CML::symmTensorField> CML::axesRotation::transformVector
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


CML::symmTensor CML::axesRotation::transformVector
(
    const vector& st
) const
{
    return transformPrincipal(R_, st);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

void CML::axesRotation::operator=(const dictionary& dict)
{
    if (debug)
    {
        Pout<< "axesRotation::operator=(const dictionary&) : "
            << "assign from " << dict << endl;
    }

    vector axis1, axis2;
    axisOrder order(e3e1);

    if (dict.readIfPresent("e1", axis1) && dict.readIfPresent("e2", axis2))
    {
        order = e1e2;
    }
    else if (dict.readIfPresent("e2", axis1)&& dict.readIfPresent("e3", axis2))
    {
        order = e2e3;
    }
    else if (dict.readIfPresent("e3", axis1)&& dict.readIfPresent("e1", axis2))
    {
        order = e3e1;
    }
    else if (dict.found("axis") || dict.found("direction"))
    {
        // let it bomb if only one of axis/direction is defined
        order = e3e1;
        dict.lookup("axis") >> axis1;
        dict.lookup("direction") >> axis2;
    }
    else
    {
        FatalErrorInFunction
            << "not entry of the type (e1, e2) or (e2, e3) or (e3, e1) "
            << "found "
            << exit(FatalError);
    }

    calcTransform(axis1, axis2, order);
}


void CML::axesRotation::write(Ostream& os) const
{
     os.writeKeyword("e1") << e1() << token::END_STATEMENT << nl;
     os.writeKeyword("e2") << e2() << token::END_STATEMENT << nl;
     os.writeKeyword("e3") << e3() << token::END_STATEMENT << nl;
}


// ************************************************************************* //
