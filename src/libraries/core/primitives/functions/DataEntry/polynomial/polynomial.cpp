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

#include "polynomial.hpp"
#include "Time.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(polynomial, 0);
    addToRunTimeSelectionTable(scalarDataEntry, polynomial, dictionary);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::polynomial::polynomial(const word& entryName, const dictionary& dict)
:
    scalarDataEntry(entryName),
    coeffs_(),
    canIntegrate_(true),
    dimensions_(dimless)
{
    Istream& is(dict.lookup(entryName));
    word entryType(is);

    token firstToken(is);
    is.putBack(firstToken);
    if (firstToken == token::BEGIN_SQR)
    {
        is  >> this->dimensions_;
    }

    is  >> coeffs_;

    if (!coeffs_.size())
    {
        FatalErrorInFunction
            << "polynomial coefficients for entry " << this->name_
            << " are invalid (empty)" << nl << exit(FatalError);
    }

    forAll(coeffs_, i)
    {
        if (mag(coeffs_[i].second() + 1) < ROOTVSMALL)
        {
            canIntegrate_ = false;
            break;
        }
    }

    if (debug)
    {
        if (!canIntegrate_)
        {
            WarningInFunction
                << "Polynomial " << this->name_ << " cannot be integrated"
                << endl;
        }
    }
}


CML::polynomial::polynomial
(
    const word& entryName,
    const List<Tuple2<scalar, scalar> >& coeffs
)
:
    scalarDataEntry(entryName),
    coeffs_(coeffs),
    canIntegrate_(true),
    dimensions_(dimless)
{
    if (!coeffs_.size())
    {
        FatalErrorInFunction
            << "polynomial coefficients for entry " << this->name_
            << " are invalid (empty)" << nl << exit(FatalError);
    }

    forAll(coeffs_, i)
    {
        if (mag(coeffs_[i].second() + 1) < ROOTVSMALL)
        {
            canIntegrate_ = false;
            break;
        }
    }

    if (debug)
    {
        if (!canIntegrate_)
        {
            WarningInFunction
                << "Polynomial " << this->name_ << " cannot be integrated"
                << endl;
        }
    }
}


CML::polynomial::polynomial(const polynomial& poly)
:
    scalarDataEntry(poly),
    coeffs_(poly.coeffs_),
    canIntegrate_(poly.canIntegrate_),
    dimensions_(poly.dimensions_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::polynomial::~polynomial()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

void CML::polynomial::convertTimeBase(const Time& t)
{
    forAll(coeffs_, i)
    {
        scalar value = coeffs_[i].first();
        coeffs_[i].first() = t.userTimeToTime(value);
    }
}


CML::scalar CML::polynomial::value(const scalar x) const
{
    scalar y = 0.0;
    forAll(coeffs_, i)
    {
        y += coeffs_[i].first()*pow(x, coeffs_[i].second());
    }

    return y;
}


CML::scalar CML::polynomial::integrate(const scalar x1, const scalar x2) const
{
    scalar intx = 0.0;

    if (canIntegrate_)
    {
        forAll(coeffs_, i)
        {
            intx +=
                coeffs_[i].first()/(coeffs_[i].second() + 1)
               *(
                    pow(x2, coeffs_[i].second() + 1)
                  - pow(x1, coeffs_[i].second() + 1)
                );
        }
    }

    return intx;
}


CML::dimensioned<CML::scalar> CML::polynomial::dimValue
(
    const scalar x
) const
{
    return dimensioned<scalar>("dimensionedValue", dimensions_, value(x));
}


CML::dimensioned<CML::scalar> CML::polynomial::dimIntegrate
(
    const scalar x1,
    const scalar x2
) const
{
    return dimensioned<scalar>
    (
        "dimensionedValue",
        dimensions_,
        integrate(x1, x2)
    );
}


// ************************************************************************* //
