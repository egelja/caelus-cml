/*---------------------------------------------------------------------------*\
Copyright (C) 2004-6 H. Jasak All rights reserved
Copyright (C) 2018 Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software; you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus. If not, see <http://www.gnu.org/licenses/>.

Class
    BlockCoeff specialisation for terminal class.

Description
    Specialisation of a block coefficient for a scalar: always a scalar.

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved.

\*---------------------------------------------------------------------------*/

#ifndef scalarBlockCoeff_HPP
#define scalarBlockCoeff_HPP

#include "BlockCoeff.hpp"


namespace CML
{

// Forward declaration of friend functions and operators
Ostream& operator<<
(
    Ostream&,
    const BlockCoeff<scalar>&
);


/*---------------------------------------------------------------------------*\
                          Class BlockCoeff Declaration
\*---------------------------------------------------------------------------*/

template<>
class BlockCoeff<scalar>
:
    public blockCoeffBase
{
public:

    //- Component type
    typedef scalar Type;

    typedef Type xType;

    //- Coefficient type
    typedef scalar scalarType;


    //- Multiplication trait
    class multiply
    {
    public:

        multiply() {}

        Type operator()(const scalarType& c, const Type& x) const
        {
            return c*x;
        }
        Type operator()(const BlockCoeff<Type>& c, const Type& x) const
        {
            return c.asScalar()*x;
        }

        // Inverse functions
        scalarType inverse(const scalarType& c) const
        {
            return 1.0/c;
        }

        // Triple product of coefficients
        scalarType tripleProduct
        (
            const scalarType& a,
            const scalarType& b,
            const scalarType& c
        ) const
        {
            return a*c/b;
        }
    };


private:

    //- Scalar coefficient
    scalar scalarCoeff_;


public:

    //- Construct null
    explicit BlockCoeff();

    //- Construct as copy
    BlockCoeff(const BlockCoeff<scalar>&);

    //- Construct from Istream
    BlockCoeff(Istream&);

    //- Clone
    BlockCoeff<scalar> clone() const;


    ~BlockCoeff();


    // Member functions

    //- Return active type
    blockCoeffBase::activeLevel activeType() const;

    //- Return as scalar
    const scalar& asScalar() const
    {
        return scalarCoeff_;
    }

    scalar& asScalar()
    {
        return scalarCoeff_;
    }

    //- Return as linear
    const scalar& asLinear() const
    {
        return scalarCoeff_;
    }

    scalar& asLinear()
    {
        return scalarCoeff_;
    }

    //- Return as square
    const scalar& asSquare() const
    {
        return scalarCoeff_;
    }

    scalar& asSquare()
    {
        return scalarCoeff_;
    }


    //- Return component
    scalar component(const direction) const;


    // Member operators
    void operator=(const BlockCoeff<scalar>&);


    // IOstream operators
    friend Ostream& operator<<
    (
        Ostream&,
        const BlockCoeff<scalar>&
    );
};


} // End namespace CML
#endif
