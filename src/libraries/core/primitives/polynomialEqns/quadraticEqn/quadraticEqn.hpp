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
    CML::quadraticEqn

Description
    Quadratic equation of the form a*x^2 + b*x + c = 0

SourceFiles
    quadraticEqn.cpp

\*---------------------------------------------------------------------------*/

#ifndef quadraticEqn_HPP
#define quadraticEqn_HPP

#include "Roots.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class quadraticEqn Declaration
\*---------------------------------------------------------------------------*/

class quadraticEqn
:
    public VectorSpace<quadraticEqn, scalar, 3>
{
public:

    //- Coefficient indexing enumeration
    struct coefficient
    {
        enum {a, b, c};
    };


    // Constructors

        //- Construct null
        inline quadraticEqn();

        //- Construct initialized to zero
        inline quadraticEqn(const CML::zero);

        //- Construct from components
        inline quadraticEqn(const scalar a, const scalar b, const scalar c);


    // Member Functions

        // Access

            inline scalar a() const;
            inline scalar b() const;
            inline scalar c() const;

            inline scalar& a();
            inline scalar& b();
            inline scalar& c();

        //- Evaluate at x
        inline scalar value(const scalar x) const;

        //- Evaluate the derivative at x
        inline scalar derivative(const scalar x) const;

        //- Estimate the error of evaluation at x
        inline scalar error(const scalar x) const;

        //- Get the roots
        Roots<2> roots() const;
};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::quadraticEqn::quadraticEqn()
{}


inline CML::quadraticEqn::quadraticEqn(const CML::zero)
:
    VectorSpace<quadraticEqn, scalar, 3>(CML::zero())
{}


inline CML::quadraticEqn::quadraticEqn
(
    const scalar a,
    const scalar b,
    const scalar c
)
{
    this->v_[coefficient::a] = a;
    this->v_[coefficient::b] = b;
    this->v_[coefficient::c] = c;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::scalar CML::quadraticEqn::a() const
{
    return this->v_[coefficient::a];
}


inline CML::scalar CML::quadraticEqn::b() const
{
    return this->v_[coefficient::b];
}


inline CML::scalar CML::quadraticEqn::c() const
{
    return this->v_[coefficient::c];
}


inline CML::scalar& CML::quadraticEqn::a()
{
    return this->v_[coefficient::a];
}


inline CML::scalar& CML::quadraticEqn::b()
{
    return this->v_[coefficient::b];
}


inline CML::scalar& CML::quadraticEqn::c()
{
    return this->v_[coefficient::c];
}


inline CML::scalar CML::quadraticEqn::value(const scalar x) const
{
    return x*(x*a() + b()) + c();
}


inline CML::scalar CML::quadraticEqn::derivative(const scalar x) const
{
    return x*2*a() + b();
}


inline CML::scalar CML::quadraticEqn::error(const scalar x) const
{
    return
        SMALL*mag(x)*(mag(x*a()) + mag(b()))
      + SMALL*(mag(x*(x*a() + b())) + mag(c()));
}


#endif
