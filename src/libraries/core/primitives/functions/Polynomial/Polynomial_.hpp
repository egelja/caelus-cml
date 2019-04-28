/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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

Class
    CML::Polynomial

Description
    Polynomial templated on size (order):

    \verbatim
        poly = sum(coeffs[i]*x^i) + logCoeff*log(x)
    \endverbatim

    where <tt> 0 <= i <= N </tt>

    - integer powers, starting at zero
    - \c value(x) to evaluate the poly for a given value
    - \c derivative(x) returns derivative at value
    - \c integral(x1, x2) returns integral between two scalar values
    - \c integral() to return a new, integral coeff polynomial
      - increases the size (order)
    - \c integralMinus1() to return a new, integral coeff polynomial where
      the base poly starts at order -1


\*---------------------------------------------------------------------------*/

#ifndef Polynomial__H
#define Polynomial__H

#include "word.hpp"
#include "scalar.hpp"
#include "Ostream.hpp"
#include "VectorSpace.hpp"
#include "StaticAssert.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of classes
template<int PolySize>
class Polynomial;

// Forward declaration of friend functions
template<int PolySize>
Ostream& operator<<
(
    Ostream&,
    const Polynomial<PolySize>&
);


/*---------------------------------------------------------------------------*\
                        Class Polynomial Declaration
\*---------------------------------------------------------------------------*/

template<int PolySize>
class Polynomial
:
    public VectorSpace<Polynomial<PolySize>, scalar, PolySize>
{
    //- Size must be positive (non-zero)
    StaticAssert(PolySize > 0);

    // Private data

        //- Include the log term? - only activated using integralMinus1()
        bool logActive_;

        //- Log coefficient - only activated using integralMinus1()
        scalar logCoeff_;


public:

    typedef Polynomial<PolySize> polyType;

    typedef Polynomial<PolySize+1> intPolyType;


    // Constructors

        //- Construct null, with all coefficients = 0.0
        Polynomial();

        //- Copy constructor
        Polynomial(const Polynomial&);

        //- Construct from C-array of coefficients
        explicit Polynomial(const scalar coeffs[PolySize]);

        //- Construct from a list of coefficients
        explicit Polynomial(const UList<scalar>& coeffs);

        //- Construct from Istream
        Polynomial(Istream&);

        //- Construct from name and Istream
        Polynomial(const word& name, Istream&);


    // Member Functions

        // Access

            //- Return true if the log term is active
            bool logActive() const;

            //- Return the log coefficient
            scalar logCoeff() const;


        // Evaluation

            //- Return polynomial value
            scalar value(const scalar x) const;

            //- Return derivative of the polynomial at the given x
            scalar derivative(const scalar x) const;

            //- Return integral between two values
            scalar integral(const scalar x1, const scalar x2) const;

            //- Return integral coefficients.
            //  Argument becomes zero'th element (constant of integration)
            intPolyType integral(const scalar intConstant = 0.0) const;

            //- Return integral coefficients when lowest order is -1.
            //  Argument becomes zero'th element (constant of integration)
            polyType integralMinus1(const scalar intConstant = 0.0) const;


    //- Ostream Operator
    friend Ostream& operator<< <PolySize>
    (
        Ostream&,
        const Polynomial&
    );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<int PolySize>
CML::Polynomial<PolySize>::Polynomial()
:
    VectorSpace<Polynomial<PolySize>, scalar, PolySize>(),
    logActive_(false),
    logCoeff_(0.0)
{
    for (int i = 0; i < PolySize; ++i)
    {
        this->v_[i] = 0.0;
    }
}


template<int PolySize>
CML::Polynomial<PolySize>::Polynomial
(
    const Polynomial<PolySize>& poly
)
:
    VectorSpace<Polynomial<PolySize>, scalar, PolySize>(poly),
    logActive_(poly.logActive_),
    logCoeff_(poly.logCoeff_)
{}


template<int PolySize>
CML::Polynomial<PolySize>::Polynomial(const scalar coeffs[PolySize])
:
    VectorSpace<Polynomial<PolySize>, scalar, PolySize>(),
    logActive_(false),
    logCoeff_(0.0)
{
    for (int i=0; i<PolySize; i++)
    {
        this->v_[i] = coeffs[i];
    }
}


template<int PolySize>
CML::Polynomial<PolySize>::Polynomial(const UList<scalar>& coeffs)
:
    VectorSpace<Polynomial<PolySize>, scalar, PolySize>(),
    logActive_(false),
    logCoeff_(0.0)
{
    if (coeffs.size() != PolySize)
    {
        FatalErrorInFunction
            << "Size mismatch: Needed " << PolySize
            << " but given " << coeffs.size()
            << nl << exit(FatalError);
    }

    for (int i = 0; i < PolySize; ++i)
    {
        this->v_[i] = coeffs[i];
    }
}


template<int PolySize>
CML::Polynomial<PolySize>::Polynomial(Istream& is)
:
    VectorSpace<Polynomial<PolySize>, scalar, PolySize>(is),
    logActive_(false),
    logCoeff_(0.0)
{}


template<int PolySize>
CML::Polynomial<PolySize>::Polynomial(const word& name, Istream& is)
:
    VectorSpace<Polynomial<PolySize>, scalar, PolySize>(),
    logActive_(false),
    logCoeff_(0.0)
{
    word isName(is);

    if (isName != name)
    {
        FatalErrorInFunction
            << "Expected polynomial name " << name << " but read " << isName
            << nl << exit(FatalError);
    }

    VectorSpace<Polynomial<PolySize>, scalar, PolySize>::
        operator=(VectorSpace<Polynomial<PolySize>, scalar, PolySize>(is));

    if (this->size() == 0)
    {
        FatalErrorInFunction
            << "Polynomial coefficients for entry " << isName
            << " are invalid (empty)" << nl << exit(FatalError);
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<int PolySize>
bool CML::Polynomial<PolySize>::logActive() const
{
    return logActive_;
}


template<int PolySize>
CML::scalar CML::Polynomial<PolySize>::logCoeff() const
{
    return logCoeff_;
}


template<int PolySize>
CML::scalar CML::Polynomial<PolySize>::value(const scalar x) const
{
    scalar val = this->v_[0];

    scalar powX = 1;
    for (label i=1; i<PolySize; ++i)
    {
        powX *= x;
        val += this->v_[i]*powX;
    }

    if (logActive_)
    {
        val += logCoeff_*log(x);
    }

    return val;
}


template<int PolySize>
CML::scalar CML::Polynomial<PolySize>::derivative(const scalar x) const
{
    scalar deriv = 0;

    if (PolySize > 1)
    {
        deriv += this->v_[1];

        scalar powX = 1;
        for (label i=2; i<PolySize; ++i)
        {
            powX *= x;
            deriv += i*this->v_[i]*powX;
        }
    }

    if (logActive_)
    {
        deriv += logCoeff_/x;
    }

    return deriv;
}


template<int PolySize>
CML::scalar CML::Polynomial<PolySize>::integral
(
    const scalar x1,
    const scalar x2
) const
{
    scalar powX1 = x1;
    scalar powX2 = x2;

    scalar integ = this->v_[0]*(powX2 - powX1);
    for (label i=1; i<PolySize; ++i)
    {
        powX1 *= x1;
        powX2 *= x2;
        integ += this->v_[i]/(i + 1)*(powX2 - powX1);
    }

    if (logActive_)
    {
        integ += logCoeff_*((x2*log(x2) - x2) - (x1*log(x1) - x1));
    }

    return integ;
}


template<int PolySize>
typename CML::Polynomial<PolySize>::intPolyType
CML::Polynomial<PolySize>::integral(const scalar intConstant) const
{
    intPolyType newCoeffs;

    newCoeffs[0] = intConstant;
    forAll(*this, i)
    {
        newCoeffs[i+1] = this->v_[i]/(i + 1);
    }

    return newCoeffs;
}


template<int PolySize>
typename CML::Polynomial<PolySize>::polyType
CML::Polynomial<PolySize>::integralMinus1(const scalar intConstant) const
{
    polyType newCoeffs;

    if (this->v_[0] > VSMALL)
    {
        newCoeffs.logActive_ = true;
        newCoeffs.logCoeff_ = this->v_[0];
    }

    newCoeffs[0] = intConstant;
    for (label i=1; i<PolySize; ++i)
    {
        newCoeffs[i] = this->v_[i]/i;
    }

    return newCoeffs;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<int PolySize>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const Polynomial<PolySize>& poly
)
{
    os  << static_cast
            <VectorSpace<Polynomial<PolySize>, scalar, PolySize> >(poly);

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const Polynomial<PolySize>&)"
    );

    return os;
}


#endif

// ************************************************************************* //
