/*---------------------------------------------------------------------------*\
Copyright (C) 2017-2018 OpenFOAM Foundation
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
    CML::linearEqn

Description
    Linear equation of the form a*x + b = 0

\*---------------------------------------------------------------------------*/

#ifndef linearEqn_HPP
#define linearEqn_HPP

#include "Roots.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class linearEqn Declaration
\*---------------------------------------------------------------------------*/

class linearEqn
:
    public VectorSpace<linearEqn, scalar, 2>
{
public:

    //- Coefficient indexing enumeration
    struct coefficient
    {
        enum {a, b};
    };


    // Constructors

        //- Construct null
        inline linearEqn();

        //- Construct initialized to zero
        inline linearEqn(const CML::zero);

        //- Construct from components
        inline linearEqn(const scalar a, const scalar b);


    // Member Functions

        // Access

            inline scalar a() const;
            inline scalar b() const;

            inline scalar& a();
            inline scalar& b();

        //- Evaluate at x
        inline scalar value(const scalar x) const;

        //- Evaluate the derivative at x
        inline scalar derivative(const scalar x) const;

        //- Estimate the error of evaluation at x
        inline scalar error(const scalar x) const;

        //- Get the roots
        inline Roots<1> roots() const;
};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::linearEqn::linearEqn()
{}


inline CML::linearEqn::linearEqn(const CML::zero)
:
    VectorSpace<linearEqn, scalar, 2>(CML::zero())
{}


inline CML::linearEqn::linearEqn(const scalar a, const scalar b)
{
    this->v_[coefficient::a] = a;
    this->v_[coefficient::b] = b;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::scalar CML::linearEqn::a() const
{
    return this->v_[coefficient::a];
}


inline CML::scalar CML::linearEqn::b() const
{
    return this->v_[coefficient::b];
}


inline CML::scalar& CML::linearEqn::a()
{
    return this->v_[coefficient::a];
}


inline CML::scalar& CML::linearEqn::b()
{
    return this->v_[coefficient::b];
}


inline CML::scalar CML::linearEqn::value(const scalar x) const
{
    return x*a() + b();
}


inline CML::scalar CML::linearEqn::derivative(const scalar x) const
{
    return a();
}


inline CML::scalar CML::linearEqn::error(const scalar x) const
{
    return SMALL*(mag(x*a()) + mag(b()));
}


inline CML::Roots<1> CML::linearEqn::roots() const
{
    /*

    This function solves a linear equation of the following form:

        a*x + b = 0
          x + b = 0

    */

    const scalar a = this->a();
    const scalar b = this->b();

    if (a == 0)
    {
        return Roots<1>(rootType::nan, 0);
    }

    if (mag(b/VGREAT) >= mag(a))
    {
        return Roots<1>
        (
            sign(a) == sign(b)
          ? rootType::negInf
          : rootType::posInf,
          0
        );
    }

    return Roots<1>(rootType::real, -b/a);
}


#endif
