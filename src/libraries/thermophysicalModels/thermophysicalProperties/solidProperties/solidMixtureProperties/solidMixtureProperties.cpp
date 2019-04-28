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

#include "solidMixtureProperties.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::solidMixtureProperties::solidMixtureProperties(const dictionary& dict)
:
    components_(),
    properties_()
{
    components_ = dict.toc();
    properties_.setSize(components_.size());

    forAll(components_, i)
    {
        if (dict.isDict(components_[i]))
        {
            properties_.set
            (
                i,
                solidProperties::New(dict.subDict(components_[i]))
            );
        }
        else
        {
            properties_.set
            (
                i,
                solidProperties::New(components_[i])
            );
        }
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

CML::scalar CML::solidMixtureProperties::rho(const scalarField& Y) const
{
    scalar rrho = 0;
    forAll(properties_,i)
    {
        rrho += Y[i]/properties_[i].rho();
    }

    return 1/rrho;
}


CML::scalar CML::solidMixtureProperties::Cp(const scalarField& Y) const
{
    scalar Cp = 0;

    forAll(properties_, i)
    {
        Cp += Y[i]*properties_[i].Cp();
    }

    return Cp;
}
