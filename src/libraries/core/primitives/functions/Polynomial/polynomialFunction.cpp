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

#include "polynomialFunction.hpp"
#include "addToRunTimeSelectionTable.hpp"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace CML
{
    defineTypeNameAndDebug(polynomialFunction, 0);
}


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //


CML::polynomialFunction CML::polynomialFunction::cloneIntegral
(
    const polynomialFunction& poly,
    const scalar intConstant
)
{
    polynomialFunction newPoly(poly.size()+1);

    newPoly[0] = intConstant;
    forAll(poly, i)
    {
        newPoly[i+1] = poly[i]/(i + 1);
    }

    return newPoly;
}


CML::polynomialFunction CML::polynomialFunction::cloneIntegralMinus1
(
    const polynomialFunction& poly,
    const scalar intConstant
)
{
    polynomialFunction newPoly(poly.size()+1);

    if (poly[0] > VSMALL)
    {
        newPoly.logActive_ = true;
        newPoly.logCoeff_  = poly[0];
    }

    newPoly[0] = intConstant;
    for (label i=1; i < poly.size(); ++i)
    {
        newPoly[i] = poly[i]/i;
    }

    return newPoly;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

CML::polynomialFunction::polynomialFunction(const label order)
:
    scalarList(order, 0.0),
    logActive_(false),
    logCoeff_(0.0)
{
    if (this->empty())
    {
        FatalErrorInFunction
            << "polynomialFunction coefficients are invalid (empty)"
            << nl << exit(FatalError);
    }
}


CML::polynomialFunction::polynomialFunction(const polynomialFunction& poly)
:
    scalarList(poly),
    logActive_(poly.logActive_),
    logCoeff_(poly.logCoeff_)
{}


CML::polynomialFunction::polynomialFunction(const UList<scalar>& coeffs)
:
    scalarList(coeffs),
    logActive_(false),
    logCoeff_(0.0)
{
    if (this->empty())
    {
        FatalErrorInFunction
            << "polynomialFunction coefficients are invalid (empty)"
            << nl << exit(FatalError);
    }
}


CML::polynomialFunction::polynomialFunction(Istream& is)
:
    scalarList(is),
    logActive_(false),
    logCoeff_(0.0)
{
    if (this->empty())
    {
        FatalErrorInFunction
            << "polynomialFunction coefficients are invalid (empty)"
            << nl << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

CML::polynomialFunction::~polynomialFunction()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool CML::polynomialFunction::logActive() const
{
    return logActive_;
}


CML::scalar CML::polynomialFunction::logCoeff() const
{
    return logCoeff_;
}


CML::scalar CML::polynomialFunction::value(const scalar x) const
{
    const scalarList& coeffs = *this;
    scalar val = coeffs[0];

    // avoid costly pow() in calculation
    scalar powX = x;
    for (label i=1; i<coeffs.size(); ++i)
    {
        val += coeffs[i]*powX;
        powX *= x;
    }

    if (logActive_)
    {
        val += this->logCoeff_*log(x);
    }

    return val;
}


CML::scalar CML::polynomialFunction::integrate
(
    const scalar x1,
    const scalar x2
) const
{
    const scalarList& coeffs = *this;

    if (logActive_)
    {
        FatalErrorInFunction
            << "Cannot integrate polynomial with logarithmic coefficients"
            << nl << abort(FatalError);
    }

    // avoid costly pow() in calculation
    scalar powX1 = x1;
    scalar powX2 = x2;

    scalar val = coeffs[0]*(powX2 - powX1);
    for (label i=1; i<coeffs.size(); ++i)
    {
        val += coeffs[i]/(i + 1)*(powX2 - powX1);
        powX1 *= x1;
        powX2 *= x2;
    }

    return val;
}


CML::polynomialFunction
CML::polynomialFunction::integral(const scalar intConstant) const
{
    return cloneIntegral(*this, intConstant);
}


CML::polynomialFunction
CML::polynomialFunction::integralMinus1(const scalar intConstant) const
{
    return cloneIntegralMinus1(*this, intConstant);
}


// * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * * //

CML::polynomialFunction&
CML::polynomialFunction::operator+=(const polynomialFunction& poly)
{
    scalarList& coeffs = *this;

    if (coeffs.size() > poly.size())
    {
        forAll(poly, i)
        {
            coeffs[i] += poly[i];
        }
    }
    else
    {
        coeffs.setSize(poly.size(), 0.0);

        forAll(coeffs, i)
        {
            coeffs[i] += poly[i];
        }
    }

    return *this;
}


CML::polynomialFunction&
CML::polynomialFunction::operator-=(const polynomialFunction& poly)
{
    scalarList& coeffs = *this;

    if (coeffs.size() > poly.size())
    {
        forAll(poly, i)
        {
            coeffs[i] -= poly[i];
        }
    }
    else
    {
        coeffs.setSize(poly.size(), 0.0);

        forAll(coeffs, i)
        {
            coeffs[i] -= poly[i];
        }
    }

    return *this;
}


CML::polynomialFunction&
CML::polynomialFunction::operator*=(const scalar s)
{
    scalarList& coeffs = *this;
    forAll(coeffs, i)
    {
        coeffs[i] *= s;
    }

    return *this;
}


CML::polynomialFunction&
CML::polynomialFunction::operator/=(const scalar s)
{
    scalarList& coeffs = *this;
    forAll(coeffs, i)
    {
        coeffs[i] /= s;
    }

    return *this;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

CML::Ostream& CML::operator<<(Ostream& os, const polynomialFunction& poly)
{
    // output like VectorSpace
    os  << token::BEGIN_LIST;

    if (!poly.empty())
    {
        for (int i=0; i<poly.size()-1; i++)
        {
            os  << poly[i] << token::SPACE;
        }
        os  << poly.last();
    }
    os  << token::END_LIST;


    // Check state of Ostream
    os.check("operator<<(Ostream&, const polynomialFunction&)");

    return os;
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

CML::polynomialFunction
CML::operator+
(
    const polynomialFunction& p1,
    const polynomialFunction& p2
)
{
    polynomialFunction poly(p1);
    return poly += p2;
}


CML::polynomialFunction
CML::operator-
(
    const polynomialFunction& p1,
    const polynomialFunction& p2
)
{
    polynomialFunction poly(p1);
    return poly -= p2;
}


CML::polynomialFunction
CML::operator*
(
    const scalar s,
    const polynomialFunction& p
)
{
    polynomialFunction poly(p);
    return poly *= s;
}


CML::polynomialFunction
CML::operator/
(
    const scalar s,
    const polynomialFunction& p
)
{
    polynomialFunction poly(p);
    return poly /= s;
}


CML::polynomialFunction
CML::operator*
(
    const polynomialFunction& p,
    const scalar s
)
{
    polynomialFunction poly(p);
    return poly *= s;
}


CML::polynomialFunction
CML::operator/
(
    const polynomialFunction& p,
    const scalar s
)
{
    polynomialFunction poly(p);
    return poly /= s;
}


// ************************************************************************* //
