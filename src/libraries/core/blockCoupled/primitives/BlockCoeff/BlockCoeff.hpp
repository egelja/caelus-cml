/*---------------------------------------------------------------------------*\
Copyright (C) 2004-6 H. Jasak All rights reserved
Copyright (C) 2015   V. Vukcevic
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
    blockCoeff

Description
    Block coefficient combines a scalar, linear and square coefficient
    for different levels of coupling.

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved
    Vuko Vukcevic

\*---------------------------------------------------------------------------*/

#ifndef BlockCoeff_HPP
#define BlockCoeff_HPP

#include "blockCoeffBase.hpp"
#include "expandTensor.hpp"
#include "Field.hpp"
#include "demandDrivenData.hpp"


namespace CML
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //

template<class Type>
class BlockCoeff;

template<class Type>
Ostream& operator<<(Ostream&, const BlockCoeff<Type>&);

/*---------------------------------------------------------------------------*\
                          Class BlockCoeff Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class BlockCoeff
:
    public blockCoeffBase
{
public:

    //- Component type
    typedef Type xType;
    typedef Field<xType> xTypeField;

    //- Coefficient type
    typedef typename pTraits<Type>::cmptType scalarType;
    typedef Type linearType;
    typedef typename outerProduct<Type, Type>::type squareType;

    //- Field type
    typedef Field<scalarType> scalarTypeField;
    typedef Field<linearType> linearTypeField;
    typedef Field<squareType> squareTypeField;


    //- Multiplication trait
    class multiply
    {
    public:

        multiply() {}

        // Coefficient times type multiplication
        Type operator()(const scalarType& c, const Type& x) const
        {
            return c*x;
        }

        Type operator()(const linearType& c, const Type& x) const
        {
            return cmptMultiply(c, x);
        }

        Type operator()(const squareType& c, const Type& x) const
        {
            return (c & x);
        }

        Type operator()(const BlockCoeff<Type>& c, const Type& x) const
        {
            if (c.scalarCoeffPtr_)
            {
                return operator()(*c.scalarCoeffPtr_, x);
            }
            else if (c.linearCoeffPtr_)
            {
                return operator()(*c.linearCoeffPtr_, x);
            }
            else if (c.squareCoeffPtr_)
            {
                return operator()(*c.squareCoeffPtr_, x);
            }
            else
            {
                return pTraits<Type>::zero;
            }
        }

        // Coefficient times coefficient multiplication. Needed for BlockILUCp
        // preconditioner. VV, 12/Jul/2015.
        scalarType activeTypeMultiply
        (
            const scalarType& a,
            const scalarType& b
        ) const
        {
            return a*b;
        }

        linearType activeTypeMultiply
        (
            const linearType& a,
            const linearType& b
        ) const
        {
            return cmptMultiply(a, b);
        }

        squareType activeTypeMultiply
        (
            const squareType& a,
            const squareType& b
        ) const
        {
            return (a & b);
        }

        // Transpose functions
        scalarType transpose(const scalarType& c) const
        {
            return c;
        }

        linearType transpose(const linearType& c) const
        {
            return c;
        }

        squareType transpose(const squareType& c) const
        {
            return c.T();
        }

        // Inverse functions
        scalarType inverse(const scalarType& c) const
        {
            return 1.0/c;
        }

        linearType inverse(const linearType& c) const
        {
            return cmptDivide(pTraits<linearType>::one, c);
        }

        squareType inverse(const squareType& c) const
        {
            return inv(c);
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

        linearType tripleProduct
        (
            const scalarType& a,
            const linearType& b,
            const scalarType& c
        ) const
        {
            return a*c*inverse(b);
        }

        linearType tripleProduct
        (
            const linearType& a,
            const linearType& b,
            const linearType& c
        ) const
        {
            return cmptDivide(cmptMultiply(a, c), b);
        }

        squareType tripleProduct
        (
            const scalarType& a,
            const squareType& b,
            const scalarType& c
        ) const
        {
            return a*c*inv(b);
        }

        squareType tripleProduct
        (
            const linearType& a,
            const squareType& b,
            const linearType& c
        ) const
        {
            squareType result;
            linearType sac = cmptMultiply(a, c);

            expandLinear(result, sac);
            return result & inv(b);
        }

        squareType tripleProduct
        (
            const squareType& a,
            const squareType& b,
            const squareType& c
        ) const
        {
            return (a & inv(b)) & c;
        }
    };


private:

    //- Scalar coefficient
    mutable scalarType* scalarCoeffPtr_;

    //- Linear coefficient
    mutable linearType* linearCoeffPtr_;

    //- Square coefficient
    mutable squareType* squareCoeffPtr_;


    //- Promote to scalar
    scalarType& toScalar();

    //- Promote to linear
    linearType& toLinear();

    //- Promote to square
    squareType& toSquare();


public:

    //- Construct null
    explicit BlockCoeff();

    //- Construct as copy
    BlockCoeff(const BlockCoeff<Type>&);

    //- Construct from Istream
    BlockCoeff(Istream&);

    //- Clone
    BlockCoeff<Type> clone() const;

    ~BlockCoeff();

    //- Clear data
    void clear();


    // Member functions

    //- Return active type
    blockCoeffBase::activeLevel activeType() const;

    //- Check pointers: only one type should be active (debug only)
    void checkActive() const;

    // Return as typed.  Fails when asked for the incorrect type

    //- Return as scalar
    const scalarType& asScalar() const;
    scalarType& asScalar();

    //- Return as linear
    const linearType& asLinear() const;
    linearType& asLinear();

    //- Return as square
    const squareType& asSquare() const;
    squareType& asSquare();

    //- Return component
    scalarType component(const direction) const;

    // Member operators
    void operator=(const BlockCoeff<Type>&);


    // IOstream operators
    friend Ostream& operator<< <Type>
    (
        Ostream&,
        const BlockCoeff<Type>&
    );
};


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
typename BlockCoeff<Type>::scalarType&
BlockCoeff<Type>::toScalar()
{
    if (not scalarCoeffPtr_)
    {
        // Debug check: demotion
        if (linearCoeffPtr_ || squareCoeffPtr_)
        {
            FatalErrorInFunction
                << "Detected demotion to scalar.  Probably an error"
                << abort(FatalError);
        }

        scalarCoeffPtr_ = new scalarType(pTraits<scalarType>::zero);
    }

    return *scalarCoeffPtr_;
}


template<class Type>
typename BlockCoeff<Type>::linearType&
BlockCoeff<Type>::toLinear()
{
    if (not linearCoeffPtr_)
    {
        // Debug check: demotion
        if (squareCoeffPtr_)
        {
            FatalErrorInFunction
                << "Detected demotion to linear.  Probably an error"
                << abort(FatalError);
        }

        linearCoeffPtr_ = new linearType(pTraits<linearType>::zero);

        // If scalar is active, promote to linear
        if (scalarCoeffPtr_)
        {
            *linearCoeffPtr_ = (*scalarCoeffPtr_)*pTraits<linearType>::one;
            deleteDemandDrivenData(scalarCoeffPtr_);
        }
    }

    return *linearCoeffPtr_;
}


template<class Type>
typename BlockCoeff<Type>::squareType&
BlockCoeff<Type>::toSquare()
{
    if (not squareCoeffPtr_)
    {
        squareCoeffPtr_ = new squareType(pTraits<squareType>::zero);

        // If scalar is active, promote to square
        if (scalarCoeffPtr_)
        {
            expandScalar(*squareCoeffPtr_, *scalarCoeffPtr_);
            deleteDemandDrivenData(scalarCoeffPtr_);
        }

        // If linear is active, promote to square
        if (linearCoeffPtr_)
        {
            expandLinear(*squareCoeffPtr_, *linearCoeffPtr_);
            deleteDemandDrivenData(linearCoeffPtr_);
        }
    }

    return *squareCoeffPtr_;
}

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
BlockCoeff<Type>::BlockCoeff()
:
    scalarCoeffPtr_(nullptr),
    linearCoeffPtr_(nullptr),
    squareCoeffPtr_(nullptr)
{}


template<class Type>
BlockCoeff<Type>::BlockCoeff(const BlockCoeff<Type>& f)
:
    scalarCoeffPtr_(nullptr),
    linearCoeffPtr_(nullptr),
    squareCoeffPtr_(nullptr)
{
    if (f.scalarCoeffPtr_)
    {
        scalarCoeffPtr_ = new scalarType(*(f.scalarCoeffPtr_));
    }
    else if (f.linearCoeffPtr_)
    {
        linearCoeffPtr_ = new linearType(*(f.linearCoeffPtr_));
    }
    else if (f.squareCoeffPtr_)
    {
        squareCoeffPtr_ = new squareType(*(f.squareCoeffPtr_));
    }
}


template<class Type>
BlockCoeff<Type>::BlockCoeff(Istream& is)
:
    scalarCoeffPtr_(nullptr),
    linearCoeffPtr_(nullptr),
    squareCoeffPtr_(nullptr)
{
    // Read keyword and pick up allocated field
    word key(is);

    if
    (
        key
     == blockCoeffBase::activeLevelNames_[blockCoeffBase::UNALLOCATED]
    )
    {
    }
    else if
    (
        key
     == blockCoeffBase::activeLevelNames_[blockCoeffBase::SCALAR]
    )
    {
        scalarCoeffPtr_ = new scalarType(readScalar(is));
    }
    else if
    (
        key
     == blockCoeffBase::activeLevelNames_[blockCoeffBase::LINEAR]
    )
    {
        linearCoeffPtr_ = new linearType(is);
    }
    else if
    (
        key
     == blockCoeffBase::activeLevelNames_[blockCoeffBase::SQUARE]
    )
    {
        squareCoeffPtr_ = new squareType(is);
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "invalid keyword while reading: " << key
            << exit(FatalIOError);
    }
}


template<class Type>
BlockCoeff<Type> CML::BlockCoeff<Type>::clone() const
{
    return BlockCoeff<Type>(*this);
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
BlockCoeff<Type>::~BlockCoeff()
{
    this->clear();
}


template<class Type>
inline void BlockCoeff<Type>::clear()
{
    deleteDemandDrivenData(scalarCoeffPtr_);
    deleteDemandDrivenData(linearCoeffPtr_);
    deleteDemandDrivenData(squareCoeffPtr_);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
blockCoeffBase::activeLevel
BlockCoeff<Type>::activeType() const
{
    if (scalarCoeffPtr_)
    {
        return blockCoeffBase::SCALAR;
    }
    else if (linearCoeffPtr_)
    {
        return blockCoeffBase::LINEAR;
    }
    else if (squareCoeffPtr_)
    {
        return blockCoeffBase::SQUARE;
    }
    else
    {
        return blockCoeffBase::UNALLOCATED;
    }
}


template<class Type>
void BlockCoeff<Type>::checkActive() const
{
    label nActive = 0;

    if (scalarCoeffPtr_) {nActive++;}
    if (linearCoeffPtr_) {nActive++;}
    if (squareCoeffPtr_) {nActive++;}

    if (nActive > 1)
    {
        FatalErrorInFunction
            << "Activation/deactivation error.  nActive = " << nActive
            << abort(FatalError);
    }
}


template<class Type>
const typename BlockCoeff<Type>::scalarType&
BlockCoeff<Type>::asScalar() const
{
    if (not scalarCoeffPtr_)
    {
        FatalErrorInFunction
            << "Requested scalar but active type is: "
            << blockCoeffBase::activeLevelNames_[this->activeType()]
            << ".  This is not allowed."
            << abort(FatalError);
    }

    return *scalarCoeffPtr_;
}


template<class Type>
const typename BlockCoeff<Type>::linearType&
BlockCoeff<Type>::asLinear() const
{
    if (not linearCoeffPtr_)
    {
        FatalErrorInFunction
            << "Requested linear but active type is: "
            << blockCoeffBase::activeLevelNames_[this->activeType()]
            << ".  This is not allowed."
            << abort(FatalError);
    }

    return *linearCoeffPtr_;
}


template<class Type>
const typename BlockCoeff<Type>::squareType&
BlockCoeff<Type>::asSquare() const
{
    if (not squareCoeffPtr_)
    {
        FatalErrorInFunction
            << "Requested square but active type is: "
            << blockCoeffBase::activeLevelNames_[this->activeType()]
            << ".  This is not allowed."
            << abort(FatalError);
    }

    return *squareCoeffPtr_;
}


template<class Type>
typename BlockCoeff<Type>::scalarType&
BlockCoeff<Type>::asScalar()
{
    if (linearCoeffPtr_ || squareCoeffPtr_)
    {
        FatalErrorInFunction
            << "Requested scalar but active type is: "
            << blockCoeffBase::activeLevelNames_[this->activeType()]
            << ".  This is not allowed."
            << abort(FatalError);
    }

    if (not scalarCoeffPtr_)
    {
        return this->toScalar();
    }

    return *scalarCoeffPtr_;
}


template<class Type>
typename BlockCoeff<Type>::linearType&
BlockCoeff<Type>::asLinear()
{
    if (squareCoeffPtr_)
    {
        FatalErrorInFunction
            << "Requested linear but active type is: "
            << blockCoeffBase::activeLevelNames_[this->activeType()]
            << ".  This is not allowed."
            << abort(FatalError);
    }

    if (not linearCoeffPtr_)
    {
        return this->toLinear();
    }

    return *linearCoeffPtr_;
}


template<class Type>
typename BlockCoeff<Type>::squareType&
BlockCoeff<Type>::asSquare()
{
    if (not squareCoeffPtr_)
    {
        return this->toSquare();
    }

    return *squareCoeffPtr_;
}


template<class Type>
typename BlockCoeff<Type>::scalarType
BlockCoeff<Type>::component(const direction dir) const
{
    if (scalarCoeffPtr_)
    {
        return *scalarCoeffPtr_;
    }
    else if (linearCoeffPtr_)
    {
        return linearCoeffPtr_->component(dir);
    }
    else if (squareCoeffPtr_)
    {
        return contractLinear
        (
            *squareCoeffPtr_
        ).component(dir);
    }
    else
    {
        FatalErrorInFunction
            << " not allocated."
            << abort(FatalError);
    }

    // Dummy return to keep compiler happy
    return *scalarCoeffPtr_;
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void BlockCoeff<Type>::operator=(const BlockCoeff<Type>& f)
{
    if (this == &f)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    if (f.scalarCoeffPtr_)
    {
        this->toScalar() = *(f.scalarCoeffPtr_);
    }
    else if (f.linearCoeffPtr_)
    {
        this->toLinear() = *(f.linearCoeffPtr_);
    }
    else if (f.squareCoeffPtr_)
    {
        this->toSquare() = *(f.squareCoeffPtr_);
    }
    else
    {
        // Not allocated - do nothing
    }
}

// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Type>
Ostream& operator<<(Ostream& os, const BlockCoeff<Type>& f)
{
    // Write active type
    os << blockCoeffBase::activeLevelNames_[f.activeType()] << nl;

    if (f.scalarCoeffPtr_)
    {
        os << *(f.scalarCoeffPtr_);
    }
    else if (f.linearCoeffPtr_)
    {
        os << *(f.linearCoeffPtr_);
    }
    else if (f.squareCoeffPtr_)
    {
        os << *(f.squareCoeffPtr_);
    }

    return os;
}


} // End namespace CML
#endif
