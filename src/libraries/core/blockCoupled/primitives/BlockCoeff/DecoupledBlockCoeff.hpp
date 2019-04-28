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
    BlockCoeff

Description
    Template for the terminal decoupled class.  It is designed to avoid
    endless expansion of tensor order by excluding block coupling at the
    terminal type level.

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Enhancements from FOAM-Extend-4.0 have been back ported.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved
    Vuko Vukcevic

\*---------------------------------------------------------------------------*/

#ifndef DecoupledBlockCoeff_HPP
#define DecoupledBlockCoeff_HPP

#include "blockCoeffBase.hpp"
#include "Field.hpp"
#include "demandDrivenData.hpp"


namespace CML
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //

template<class Type>
class DecoupledBlockCoeff;

template<class Type>
Ostream& operator<<(Ostream&, const DecoupledBlockCoeff<Type>&);

/*---------------------------------------------------------------------------*\
                      Class DecoupledBlockCoeff Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class DecoupledBlockCoeff
:
    public blockCoeffBase
{
public:

    //- Component type
    typedef Type xType;
    typedef Field<xType> xTypeField;

    //- Coefficient type
    typedef scalar scalarType;
    typedef Type linearType;

    //- Field type
    typedef Field<scalarType> scalarTypeField;
    typedef Field<linearType> linearTypeField;


    //- Multiplication trait
    class multiply
    {
    public:

        multiply() {}

        Type operator()(const scalarType& c, const Type& x) const
        {
            return c*x;
        }

        Type operator()(const linearType& c, const Type& x) const
        {
            return cmptMultiply(c, x);
        }

        Type operator()(const DecoupledBlockCoeff<Type>& c, const Type& x) const
        {
            if (c.scalarCoeffPtr_)
            {
                return operator()(*c.scalarCoeffPtr_, x);
            }
            else if (c.linearCoeffPtr_)
            {
                return operator()(*c.linearCoeffPtr_, x);
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

        // Inverse functions
        scalarType inverse(const scalarType& c) const
        {
            return 1.0/c;
        }

        linearType inverse(const linearType& c) const
        {
            return cmptDivide(pTraits<linearType>::one, c);
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
            const linearType& a,
            const linearType& b,
            const linearType& c
        ) const
        {
            return cmptDivide(cmptMultiply(a, c), b);
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
    };


private:

    //- Scalar coefficient
    mutable scalarType* scalarCoeffPtr_;

    //- Linear coefficient
    mutable linearType* linearCoeffPtr_;


    //- Promote to scalar
    scalarType& toScalar();

    //- Promote to linear
    linearType& toLinear();


public:

    //- Construct null
    explicit DecoupledBlockCoeff();

    //- Construct as copy
    DecoupledBlockCoeff(const DecoupledBlockCoeff<Type>&);

    //- Construct from Istream
    DecoupledBlockCoeff(Istream&);

    //- Clone
    DecoupledBlockCoeff<Type> clone() const;


    ~DecoupledBlockCoeff();

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


    //- Return component
    scalarType component(const direction) const;


    // Member operators
    void operator=(const DecoupledBlockCoeff<Type>&);


    // IOstream operators
    friend Ostream& operator<< <Type>
    (
        Ostream&,
        const DecoupledBlockCoeff<Type>&
    );
};


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
typename DecoupledBlockCoeff<Type>::scalarType&
DecoupledBlockCoeff<Type>::toScalar()
{
    if (not scalarCoeffPtr_)
    {
        // Debug check: demotion
        if (linearCoeffPtr_)
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
typename DecoupledBlockCoeff<Type>::linearType&
DecoupledBlockCoeff<Type>::toLinear()
{
    if (not linearCoeffPtr_)
    {
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

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
DecoupledBlockCoeff<Type>::DecoupledBlockCoeff()
:
    scalarCoeffPtr_(nullptr),
    linearCoeffPtr_(nullptr)
{}


template<class Type>
DecoupledBlockCoeff<Type>::DecoupledBlockCoeff
(
    const DecoupledBlockCoeff<Type>& f
)
:
    scalarCoeffPtr_(nullptr),
    linearCoeffPtr_(nullptr)
{
    if (f.scalarCoeffPtr_)
    {
        scalarCoeffPtr_ = new scalarType(*(f.scalarCoeffPtr_));
    }
    else if (f.linearCoeffPtr_)
    {
        linearCoeffPtr_ = new linearType(*(f.linearCoeffPtr_));
    }
}


template<class Type>
DecoupledBlockCoeff<Type>::DecoupledBlockCoeff(Istream& is)
:
    scalarCoeffPtr_(nullptr),
    linearCoeffPtr_(nullptr)
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
    else
    {
        FatalIOErrorInFunction(is)
            << "invalid keyword while reading: " << key
            << exit(FatalIOError);
    }
}


template<class Type>
DecoupledBlockCoeff<Type> DecoupledBlockCoeff<Type>::clone() const
{
    return DecoupledBlockCoeff<Type>(*this);
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
DecoupledBlockCoeff<Type>::~DecoupledBlockCoeff()
{
    this->clear();
}


template<class Type>
void DecoupledBlockCoeff<Type>::clear()
{
    deleteDemandDrivenData(scalarCoeffPtr_);
    deleteDemandDrivenData(linearCoeffPtr_);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
blockCoeffBase::activeLevel
DecoupledBlockCoeff<Type>::activeType() const
{
    if (scalarCoeffPtr_)
    {
        return blockCoeffBase::SCALAR;
    }
    else if (linearCoeffPtr_)
    {
        return blockCoeffBase::LINEAR;
    }
    else
    {
        return blockCoeffBase::UNALLOCATED;
    }
}


template<class Type>
void DecoupledBlockCoeff<Type>::checkActive() const
{
    label nActive = 0;

    if (scalarCoeffPtr_) {nActive++;}
    if (linearCoeffPtr_) {nActive++;}


    if (nActive > 1)
    {
        FatalErrorInFunction
            << "Activation/deactivation error.  nActive = " << nActive
            << abort(FatalError);
    }
}


template<class Type>
const typename DecoupledBlockCoeff<Type>::scalarType&
DecoupledBlockCoeff<Type>::asScalar() const
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
const typename DecoupledBlockCoeff<Type>::linearType&
DecoupledBlockCoeff<Type>::asLinear() const
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
typename DecoupledBlockCoeff<Type>::scalarType&
DecoupledBlockCoeff<Type>::asScalar()
{
    if (linearCoeffPtr_)
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
typename DecoupledBlockCoeff<Type>::linearType&
DecoupledBlockCoeff<Type>::asLinear()
{
    if (not linearCoeffPtr_)
    {
        return this->toLinear();
    }

    return *linearCoeffPtr_;
}


template<class Type>
typename DecoupledBlockCoeff<Type>::scalarType
DecoupledBlockCoeff<Type>::component(const direction dir) const
{
    if (scalarCoeffPtr_)
    {
        return *scalarCoeffPtr_;
    }
    else if (linearCoeffPtr_)
    {
        return linearCoeffPtr_->component(dir);
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
void DecoupledBlockCoeff<Type>::operator=
(
    const DecoupledBlockCoeff<Type>& f
)
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
    else
    {
        // Not allocated - do nothing
    }
}

// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Type>
Ostream& operator<<(Ostream& os, const DecoupledBlockCoeff<Type>& f)
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

    return os;
}


} // End namespace CML
#endif
