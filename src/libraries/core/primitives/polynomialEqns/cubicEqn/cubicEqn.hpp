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
    CML::cubicEqn

Description
    Cubic equation of the form a*x^3 + b*x^2 + c*x + d = 0

SourceFiles
    cubicEqn.cpp

\*---------------------------------------------------------------------------*/

#ifndef cubicEqn_HPP
#define cubicEqn_HPP

#include "Roots.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

/*---------------------------------------------------------------------------*\
                         Class cubicEqn Declaration
\*---------------------------------------------------------------------------*/

class cubicEqn
:
    public VectorSpace<cubicEqn, scalar, 4>
{
public:

    //- Coefficient indexing enumeration
    struct coefficient
    {
        enum {a, b, c, d};
    };


    // Constructors

        //- Construct null
        inline cubicEqn();

        //- Construct initialized to zero
        inline cubicEqn(const CML::zero);

        //- Construct from components
        inline cubicEqn
        (
            const scalar a,
            const scalar b,
            const scalar c,
            const scalar d
        );


    // Member Functions

        // Access

            inline scalar a() const;
            inline scalar b() const;
            inline scalar c() const;
            inline scalar d() const;

            inline scalar& a();
            inline scalar& b();
            inline scalar& c();
            inline scalar& d();

        //- Evaluate at x
        inline scalar value(const scalar x) const;

        //- Evaluate the derivative at x
        inline scalar derivative(const scalar x) const;

        //- Estimate the error of evaluation at x
        inline scalar error(const scalar x) const;

        //- Get the roots
        Roots<3> roots() const;
};


} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline CML::cubicEqn::cubicEqn()
{}


inline CML::cubicEqn::cubicEqn(const CML::zero)
:
    VectorSpace<cubicEqn, scalar, 4>(CML::zero())
{}


inline CML::cubicEqn::cubicEqn
(
    const scalar a,
    const scalar b,
    const scalar c,
    const scalar d
)
{
    this->v_[coefficient::a] = a;
    this->v_[coefficient::b] = b;
    this->v_[coefficient::c] = c;
    this->v_[coefficient::d] = d;
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline CML::scalar CML::cubicEqn::a() const
{
    return this->v_[coefficient::a];
}


inline CML::scalar CML::cubicEqn::b() const
{
    return this->v_[coefficient::b];
}


inline CML::scalar CML::cubicEqn::c() const
{
    return this->v_[coefficient::c];
}


inline CML::scalar CML::cubicEqn::d() const
{
    return this->v_[coefficient::d];
}


inline CML::scalar& CML::cubicEqn::a()
{
    return this->v_[coefficient::a];
}


inline CML::scalar& CML::cubicEqn::b()
{
    return this->v_[coefficient::b];
}


inline CML::scalar& CML::cubicEqn::c()
{
    return this->v_[coefficient::c];
}


inline CML::scalar& CML::cubicEqn::d()
{
    return this->v_[coefficient::d];
}


inline CML::scalar CML::cubicEqn::value(const scalar x) const
{
    return x*(x*(x*a() + b()) + c()) + d();
}


inline CML::scalar CML::cubicEqn::derivative(const scalar x) const
{
    return x*(x*3*a() + 2*b()) + c();
}


inline CML::scalar CML::cubicEqn::error(const scalar x) const
{
    return
        SMALL*magSqr(x)*(mag(x*a()) + mag(b()))
      + SMALL*mag(x)*(mag(x*(x*a() + b())) + mag(c()))
      + SMALL*(mag(x*(x*(x*a() + b()) + c())) + mag(d()));
}


#endif
