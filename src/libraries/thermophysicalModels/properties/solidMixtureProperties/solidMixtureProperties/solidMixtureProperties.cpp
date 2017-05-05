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

#include "solidMixtureProperties.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidMixtureProperties::solidMixtureProperties
(
    const dictionary& thermophysicalProperties
)
:
    components_(thermophysicalProperties.lookup("solidComponents")),
    properties_(components_.size())
{
    // can use sub-dictionary "solidProperties" to avoid
    // collisions with identically named gas-phase entries
    const dictionary* subDictPtr = thermophysicalProperties.subDictPtr
    (
        "solidProperties"
    );

    const dictionary& props =
    (
        subDictPtr ? *subDictPtr : thermophysicalProperties
    );

    forAll(components_, i)
    {
        properties_.set(i, solidProperties::New(props.subDict(components_[i])));
    }
}


CML::solidMixtureProperties::solidMixtureProperties
(
    const solidMixtureProperties& s
)
:
    components_(s.components_),
    properties_(s.properties_.size())
{
    forAll(properties_, i)
    {
        properties_.set(i, s.properties_(i)->clone());
    }
}


// * * * * * * * * * * * * * * * * Selectors * * * * * * * * * * * * * * * * //

CML::autoPtr<CML::solidMixtureProperties> CML::solidMixtureProperties::New
(
    const dictionary& thermophysicalProperties
)
{
    return autoPtr<solidMixtureProperties>
    (
        new solidMixtureProperties(thermophysicalProperties)
    );
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

CML::scalarField CML::solidMixtureProperties::X(const scalarField& Y) const
{
    scalarField X(Y.size());
    scalar rhoInv = 0.0;
    forAll(X, i)
    {
        rhoInv += Y[i]/properties_[i].rho();
        X[i] = Y[i]/properties_[i].rho();
    }

    tmp<scalarField> tfld(X/rhoInv);
    return tfld();
}


CML::scalar CML::solidMixtureProperties::rho(const scalarField& X) const
{
    scalar val = 0.0;
    forAll(properties_, i)
    {
        val += properties_[i].rho()*X[i];
    }
    return val;
}


CML::scalar CML::solidMixtureProperties::Cp(const scalarField& Y) const
{
    scalar val = 0.0;
    forAll(properties_, i)
    {
        val += properties_[i].Cp()*Y[i];
    }
    return val;
}


// ************************************************************************* //
