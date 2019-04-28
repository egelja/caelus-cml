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

#include "dimensionSet.hpp"
#include "dimensionedScalar.hpp"
#include "OStringStream.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //


namespace CML
{
    defineTypeNameAndDebug(dimensionSet, 1);
    const scalar dimensionSet::smallExponent = SMALL;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::dimensionSet::dimensionSet
(
    const scalar mass,
    const scalar length,
    const scalar time,
    const scalar temperature,
    const scalar moles,
    const scalar current,
    const scalar luminousIntensity
)
{
    exponents_[MASS] = mass;
    exponents_[LENGTH] = length;
    exponents_[TIME] = time;
    exponents_[TEMPERATURE] = temperature;
    exponents_[MOLES] = moles;
    exponents_[CURRENT] = current;
    exponents_[LUMINOUS_INTENSITY] = luminousIntensity;
}


CML::dimensionSet::dimensionSet
(
    const scalar mass,
    const scalar length,
    const scalar time,
    const scalar temperature,
    const scalar moles
)
{
    exponents_[MASS] = mass;
    exponents_[LENGTH] = length;
    exponents_[TIME] = time;
    exponents_[TEMPERATURE] = temperature;
    exponents_[MOLES] = moles;
    exponents_[CURRENT] = 0;
    exponents_[LUMINOUS_INTENSITY] = 0;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::dimensionSet::dimensionless() const
{
    for (int Dimension=0; Dimension<nDimensions; ++Dimension)
    {
        // ie, mag(exponents_[Dimension]) > smallExponent
        if
        (
            exponents_[Dimension] > smallExponent
         || exponents_[Dimension] < -smallExponent
        )
        {
            return false;
        }
    }

    return true;
}


void CML::dimensionSet::reset(const dimensionSet& ds)
{
    for (int Dimension=0; Dimension<nDimensions; ++Dimension)
    {
        exponents_[Dimension] = ds.exponents_[Dimension];
    }
}


CML::string CML::dimensionSet::asText() const
{
    OStringStream buf;

    bool Dimensionless = true;

    for (int Dimension=0; Dimension < dimensionSet::nDimensions-1; ++Dimension)
    {
        const scalar& expt = exponents_[Dimension];

        if (expt < smallExponent && expt > -smallExponent)
        {
            continue;
        }

        if (Dimensionless)
        {
            Dimensionless = false;
        }
        else
        {
            buf << ' ';
        }

        // note: currently only handle SI
        switch (Dimension)
        {
            case MASS:
                buf << "kg";
                break;

            case LENGTH:
                buf << "m";
                break;

            case TIME:
                buf << "s";
                break;

            case TEMPERATURE:
                buf << "K";
                break;

            case MOLES:
                buf << "mol";
                break;

            case CURRENT:
                buf << "A";
                break;

            case LUMINOUS_INTENSITY:
                buf << "Cd";
                break;

            default:
                buf << "??";  // this shouldn't be - flag as being weird
                break;
        }

        if (expt != 1)
        {
            buf << '^' << expt;
        }
    }

    if (Dimensionless)
    {
        return "none";
    }
    else
    {
        return buf.str();
    }
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

CML::scalar CML::dimensionSet::operator[](const dimensionType type) const
{
    return exponents_[type];
}


CML::scalar& CML::dimensionSet::operator[](const dimensionType type)
{
    return exponents_[type];
}


bool CML::dimensionSet::operator==(const dimensionSet& ds) const
{
    for (int Dimension=0; Dimension < nDimensions; ++Dimension)
    {
        if
        (
            mag(exponents_[Dimension] - ds.exponents_[Dimension])
          > smallExponent
        )
        {
            return false;
        }
    }

    return true;
}


bool CML::dimensionSet::operator!=(const dimensionSet& ds) const
{
    return !(operator==(ds));
}


bool CML::dimensionSet::operator=(const dimensionSet& ds) const
{
    if (dimensionSet::debug && *this != ds)
    {
        FatalErrorInFunction
            << "Different dimensions for =" << endl
            << "     dimensions : " << *this << " = " << ds << endl
            << abort(FatalError);
    }

    return true;
}


bool CML::dimensionSet::operator+=(const dimensionSet& ds) const
{
    if (dimensionSet::debug && *this != ds)
    {
        FatalErrorInFunction
            << "Different dimensions for +=" << endl
            << "     dimensions : " << *this << " = " << ds << endl
            << abort(FatalError);
    }

    return true;
}


bool CML::dimensionSet::operator-=(const dimensionSet& ds) const
{
    if (dimensionSet::debug && *this != ds)
    {
        FatalErrorInFunction
            << "Different dimensions for -=" << endl
            << "     dimensions : " << *this << " = " << ds << endl
            << abort(FatalError);
    }

    return true;
}


bool CML::dimensionSet::operator*=(const dimensionSet& ds)
{
    reset((*this)*ds);

    return true;
}


bool CML::dimensionSet::operator/=(const dimensionSet& ds)
{
    reset((*this)/ds);

    return true;
}


// * * * * * * * * * * * * * * * Friend functions * * * * * * * * * * * * * * //

CML::dimensionSet CML::max(const dimensionSet& ds1, const dimensionSet& ds2)
{
    if (dimensionSet::debug && ds1 != ds2)
    {
        FatalErrorInFunction
            << "Arguments of max have different dimensions" << endl
            << "     dimensions : " << ds1 << " and " << ds2 << endl
            << abort(FatalError);
    }

    return ds1;
}


CML::dimensionSet CML::min(const dimensionSet& ds1, const dimensionSet& ds2)
{
    if (dimensionSet::debug && ds1 != ds2)
    {
        FatalErrorInFunction
            << "Arguments of min have different dimensions" << endl
            << "     dimensions : " << ds1 << " and " << ds2 << endl
            << abort(FatalError);
    }

    return ds1;
}


CML::dimensionSet CML::cmptMultiply
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    return ds1*ds2;
}


CML::dimensionSet CML::cmptDivide
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    return ds1/ds2;
}


CML::dimensionSet CML::pow(const dimensionSet& ds, const scalar p)
{
    dimensionSet dimPow
    (
        ds[dimensionSet::MASS]*p,
        ds[dimensionSet::LENGTH]*p,
        ds[dimensionSet::TIME]*p,
        ds[dimensionSet::TEMPERATURE]*p,
        ds[dimensionSet::MOLES]*p,
        ds[dimensionSet::CURRENT]*p,
        ds[dimensionSet::LUMINOUS_INTENSITY]*p
    );

    return dimPow;
}


CML::dimensionSet CML::pow
(
    const dimensionSet& ds,
    const dimensionedScalar& dS
)
{
    if (dimensionSet::debug && !dS.dimensions().dimensionless())
    {
        FatalErrorInFunction
            << "Exponent of pow is not dimensionless"
            << abort(FatalError);
    }

    dimensionSet dimPow
    (
        ds[dimensionSet::MASS]*dS.value(),
        ds[dimensionSet::LENGTH]*dS.value(),
        ds[dimensionSet::TIME]*dS.value(),
        ds[dimensionSet::TEMPERATURE]*dS.value(),
        ds[dimensionSet::MOLES]*dS.value(),
        ds[dimensionSet::CURRENT]*dS.value(),
        ds[dimensionSet::LUMINOUS_INTENSITY]*dS.value()
    );

    return dimPow;
}


CML::dimensionSet CML::pow
(
    const dimensionedScalar& dS,
    const dimensionSet& ds
)
{
    if
    (
        dimensionSet::debug
     && !dS.dimensions().dimensionless()
     && !ds.dimensionless()
    )
    {
        FatalErrorInFunction
            << "Argument or exponent of pow not dimensionless" << endl
            << abort(FatalError);
    }

    return ds;
}


CML::dimensionSet CML::sqr(const dimensionSet& ds)
{
    return pow(ds, 2);
}


CML::dimensionSet CML::pow3(const dimensionSet& ds)
{
    return pow(ds, 3);
}


CML::dimensionSet CML::pow4(const dimensionSet& ds)
{
    return pow(ds, 4);
}


CML::dimensionSet CML::pow5(const dimensionSet& ds)
{
    return pow(ds, 5);
}


CML::dimensionSet CML::pow6(const dimensionSet& ds)
{
    return pow(ds, 6);
}


CML::dimensionSet CML::pow025(const dimensionSet& ds)
{
    return sqrt(sqrt(ds));
}


CML::dimensionSet CML::sqrt(const dimensionSet& ds)
{
    return pow(ds, 0.5);
}


CML::dimensionSet CML::cbrt(const dimensionSet& ds)
{
    return pow(ds, 1.0/3.0);
}


CML::dimensionSet CML::magSqr(const dimensionSet& ds)
{
    return pow(ds, 2);
}


CML::dimensionSet CML::mag(const dimensionSet& ds)
{
    return ds;
}


CML::dimensionSet CML::sign(const dimensionSet&)
{
    return dimless;
}


CML::dimensionSet CML::pos(const dimensionSet&)
{
    return dimless;
}


CML::dimensionSet CML::neg(const dimensionSet&)
{
    return dimless;
}


CML::dimensionSet CML::inv(const dimensionSet& ds)
{
    return dimless/ds;
}


CML::dimensionSet CML::trans(const dimensionSet& ds)
{
    if (dimensionSet::debug && !ds.dimensionless())
    {
        FatalErrorInFunction
            << "Argument of trancendental function not dimensionless"
            << abort(FatalError);
    }

    return ds;
}


CML::dimensionSet CML::transform(const dimensionSet& ds)
{
    return ds;
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

CML::dimensionSet CML::operator-(const dimensionSet& ds)
{
    return ds;
}


CML::dimensionSet CML::operator+
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    dimensionSet dimSum(ds1);

    if (dimensionSet::debug && ds1 != ds2)
    {
        FatalErrorInFunction
            << "LHS and RHS of + have different dimensions" << endl
            << "     dimensions : " << ds1 << " + " << ds2 << endl
            << abort(FatalError);
    }

    return dimSum;
}


CML::dimensionSet CML::operator-
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    dimensionSet dimDifference(ds1);

    if (dimensionSet::debug && ds1 != ds2)
    {
        FatalErrorInFunction
            << "LHS and RHS of - have different dimensions" << endl
            << "     dimensions : " << ds1 << " - " << ds2 << endl
            << abort(FatalError);
    }

    return dimDifference;
}


CML::dimensionSet CML::operator*
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    dimensionSet dimProduct(ds1);

    for (int Dimension=0; Dimension<dimensionSet::nDimensions; Dimension++)
    {
        dimProduct.exponents_[Dimension] += ds2.exponents_[Dimension];
    }

    return dimProduct;
}


CML::dimensionSet CML::operator/
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    dimensionSet dimQuotient(ds1);

    for (int Dimension=0; Dimension<dimensionSet::nDimensions; Dimension++)
    {
        dimQuotient.exponents_[Dimension] -= ds2.exponents_[Dimension];
    }

    return dimQuotient;
}


CML::dimensionSet CML::operator&
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    return ds1*ds2;
}


CML::dimensionSet CML::operator^
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    return ds1*ds2;
}


CML::dimensionSet CML::operator&&
(
    const dimensionSet& ds1,
    const dimensionSet& ds2
)
{
    return ds1*ds2;
}


// ************************************************************************* //
