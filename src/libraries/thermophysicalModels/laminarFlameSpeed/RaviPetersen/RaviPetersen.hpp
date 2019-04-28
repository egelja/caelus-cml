/*---------------------------------------------------------------------------*\
Copyright (C) 2013-2018 OpenFOAM Foundation
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

Class
    CML::laminarFlameSpeedModels::RaviPetersen

Description
    Laminar flame speed obtained from Ravi and Petersen's correlation.

    The correlation for the laminar flame speed \f$Su\f$ is of the following
    form:
    \f[
        Su = \left( \sum \alpha_i \phi^i \right)
        \left( \frac{T}{T_{ref}} \right)^{\left( \sum \beta_j \phi^j \right)}
    \f]

    Where \f$\phi\f$ is the equivalence ratio, and \f$\alpha\f$ and \f$\beta\f$
    are polynomial coefficients given for a number of pressure and equivalence
    ratio points.

SourceFiles
    RaviPetersen.cpp

\*---------------------------------------------------------------------------*/

#ifndef RaviPetersen_HPP
#define RaviPetersen_HPP

#include "laminarFlameSpeed.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace laminarFlameSpeedModels
{

/*---------------------------------------------------------------------------*\
                        Class RaviPetersen Declaration
\*---------------------------------------------------------------------------*/

class RaviPetersen
:
    public laminarFlameSpeed
{

    dictionary coeffsDict_;

    //- Correlation pressure values
    List<scalar> pPoints_;

    //- Correlation equivalence ratios
    List<scalar> EqRPoints_;

    //- Correlation alpha coefficients
    List<List<List<scalar> > > alpha_;

    //- Correlation beta coefficients
    List<List<List<scalar> > > beta_;

    //- Reference temperature
    scalar TRef_;


    //- Check that input points are ordered
    void checkPointsMonotonicity
    (
        const word& name,
        const List<scalar>& x
    ) const;

    //- Check that the coefficient arrays are of the correct shape
    void checkCoefficientArrayShape
    (
        const word& name,
        const List<List<List<scalar> > >& x
    ) const;

    //- Find and interpolate a value in the data point arrays
    inline bool interval
    (
        const List<scalar>& xPoints,
        const scalar x,
        label& xIndex,
        scalar& xXi,
        scalar& xLim
    ) const
    {
        if (x < xPoints.first())
        {
            xIndex = 0;
            xXi = 0.0;
            xLim = xPoints.first();
            return false;
        }
        else if (x > xPoints.last())
        {
            xIndex = xPoints.size() - 2;
            xXi = 1.0;
            xLim = xPoints.last();
            return false;
        }

        for (xIndex = 0; x > xPoints[xIndex+1]; xIndex ++)
        {
            // increment xIndex until xPoints[xIndex] < x < xPoints[xIndex+1]
        }

        xXi = (x - xPoints[xIndex])/(xPoints[xIndex+1] - xPoints[xIndex]);
        xLim = x;

        return true;
    }

    //- Evaluate a polynomial
    inline scalar polynomial
    (
        const List<scalar>& coeffs,
        const scalar x
    ) const
    {
        scalar xPow = 1.0;
        scalar y = 0.0;
        forAll(coeffs, i)
        {
            y += coeffs[i]*xPow;
            xPow *= x;
        }
        return y;
    }

    //- Evaluate a polynomial differential
    inline scalar dPolynomial
    (
        const List<scalar>& coeffs,
        const scalar x
    ) const
    {
        scalar xPow = 1.0;
        scalar y = 0.0;
        for (label i = 1; i < coeffs.size(); i++)
        {
            y += i*coeffs[i]*xPow;
            xPow *= x;
        }
        return y;
    }

    //- Calculate normalised temperature to the power of the B polynomial
    inline scalar THatPowB
    (
        const label EqRIndex,
        const label pIndex,
        const scalar EqR,
        const scalar Tu
    ) const
    {
        return pow
        (
            Tu/TRef_,
            polynomial(beta_[EqRIndex][pIndex],EqR)
        );
    }

    //- Return the flame speed within the correlation range
    inline scalar correlationInRange
    (
        const label EqRIndex,
        const label pIndex,
        const scalar EqR,
        const scalar Tu
    ) const
    {
        // standard correlation
        return
            polynomial(alpha_[EqRIndex][pIndex],EqR)
           *THatPowB(EqRIndex, pIndex, EqR, Tu);
    }

    //- Extrapolate the flame speed correlation outside its range
    inline scalar correlationOutOfRange
    (
        const label EqRIndex,
        const label pIndex,
        const scalar EqR,
        const scalar EqRLim,
        const scalar Tu
    ) const
    {
        scalar A = polynomial(alpha_[EqRIndex][pIndex], EqRLim);
        scalar dA = dPolynomial(alpha_[EqRIndex][pIndex], EqRLim);
        scalar dB = dPolynomial(beta_[EqRIndex][pIndex], EqRLim);
        scalar TB = THatPowB(EqRIndex, pIndex, EqRLim, Tu);

        // linear extrapolation from the bounds of the correlation
        return max(TB*(A + (dA + A*log(Tu/TRef_)*dB)*(EqR - EqRLim)), 0.0);
    }

    //- Return the laminar flame speed [m/s]
    inline scalar speed
    (
        const scalar EqR,
        const scalar p,
        const scalar Tu
    ) const
    {
        scalar Su = 0, s;

        label EqRIndex, pIndex;
        scalar EqRXi, pXi;
        scalar EqRLim, pLim;
        bool EqRInRange;

        EqRInRange = interval(EqRPoints_, EqR, EqRIndex, EqRXi, EqRLim);

        interval(pPoints_, p, pIndex, pXi, pLim);

        for (label pI = 0; pI < 2; pI ++)
        {
            if (EqRInRange)
            {
                s = correlationInRange(EqRIndex, pIndex + pI, EqR, Tu);
            }
            else
            {
                s = correlationOutOfRange(EqRIndex, pIndex + pI, EqR, EqRLim, Tu);
            }

            Su += (1 - pXi)*s;
            pXi = 1 - pXi;
        }

        return Su;
    }


    //- Construct as copy (not implemented)
    RaviPetersen(const RaviPetersen&);
    void operator=(const RaviPetersen&);


public:

    //- Runtime type information
    TypeName("RaviPetersen");


    //- Construct from dictionary and psiuReactionThermo
    RaviPetersen
    (
        const dictionary&,
        const psiuReactionThermo&
    );


    //- Destructor
    virtual ~RaviPetersen()
    {}


    // Member functions

    //- Return the laminar flame speed [m/s]
    tmp<volScalarField> operator()() const;

};


} // End laminarFlameSpeedModels
} // End namespace CML


#endif
