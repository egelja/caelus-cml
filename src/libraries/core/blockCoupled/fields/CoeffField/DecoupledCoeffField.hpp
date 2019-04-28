/*---------------------------------------------------------------------------*\
Copyright (C) 2004-6 H. Jasak
Copyright (C) Applied CCM Pty Ltd
-------------------------------------------------------------------------------
License
    This file is part of Caelus.

    Caelus is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    Caelus is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Caelus.  If not, see <http://www.gnu.org/licenses/>.

Class
    DecoupledCoeffField<T>

Description
    Generic coefficient field type.  Used in BlockLduMatrix.  HJ, 2/Apr/2005

    This file was obtained from Hrvoje Jasak at the NUMAP-FOAM summer school
    in Zagreb, Croatia, 2010.

    Please report any omissions of authorship and/or copyright to
    info@appliedccm.com.au. Corrections will be made upon provision of proof.

Author
    Hrvoje Jasak, Wikki Ltd.  All rights reserved

\*---------------------------------------------------------------------------*/

#ifndef DecoupledCoeffField_HPP
#define DecoupledCoeffField_HPP

#include "VectorSpace.hpp"
#include "primitiveFields.hpp"
#include "blockCoeffs.hpp"
#include "demandDrivenData.hpp"
#include "expandTensorField.hpp"


namespace CML
{

// * * * * * * Forward declaration of template friend fuctions * * * * * * * //

template<class Type>
class DecoupledCoeffField;

template<class Type>
Ostream& operator<<(Ostream&, const DecoupledCoeffField<Type>&);

template<class Type>
Ostream& operator<<(Ostream&, const tmp<DecoupledCoeffField<Type> >&);


/*---------------------------------------------------------------------------*\
                          Class DecoupledCoeffField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class DecoupledCoeffField
:
    public refCount
{
public:

    //- Component type
    typedef typename BlockCoeff<Type>::scalarType scalarType;
    typedef typename BlockCoeff<Type>::linearType linearType;

    //- Field type
    typedef typename BlockCoeff<Type>::scalarTypeField scalarTypeField;
    typedef typename BlockCoeff<Type>::linearTypeField linearTypeField;


private:

    //- Scalar coefficient
    mutable scalarTypeField* scalarCoeffPtr_;

    //- Linear coefficient
    mutable linearTypeField* linearCoeffPtr_;

    //- Number of elements
    label size_;

    //- Check size for arithmetic operations: resizing is not allowed!
    template<class Type2>
    inline void checkSize(const UList<Type2>&) const;

    //- Promote to scalar
    scalarTypeField& toScalar();

    //- Promote to linear
    linearTypeField& toLinear();


public:

    static const char* const typeName;

    //- Construct given size
    explicit DecoupledCoeffField(const label);

    //- Construct as copy
    DecoupledCoeffField(const DecoupledCoeffField<Type>&);

    //- Construct from Istream
    explicit DecoupledCoeffField(Istream&);

    //- Clone
    tmp<DecoupledCoeffField<Type> > clone() const;

    ~DecoupledCoeffField();

    //- Clear data
    void clear();

    // Member functions
    //- Return size
    inline label size() const;

    //- Return active type
    blockCoeffBase::activeLevel activeType() const;

    //- Check pointers: only one type should be active (debug only)
    void checkActive() const;

    //- Negate this field
    void negate();

    //- Return the field transpose
    tmp<DecoupledCoeffField<Type> > transpose() const;

    // Return as typed.  Fails when asked for the incorrect type
    //- Return as scalar field
    const scalarTypeField& asScalar() const;

    //- Return as linear field
    const linearTypeField& asLinear() const;

    // Return as typed.  Fails when asked for demotion
    //- Return as scalar field
    scalarTypeField& asScalar();

    //- Return as linear field
    linearTypeField& asLinear();

    //- Return component
    tmp<scalarTypeField> component(const direction) const;

    //- Return coefficient as block
    BlockCoeff<Type> getCoeff(const label index) const;

    //- Set coefficient from a block
    void setCoeff(const label index, const BlockCoeff<Type>& coeff);

    // Subset operations
    //- Get subset with offset and size and store in given field
    void getSubset
    (
        DecoupledCoeffField<Type>& f,
        const label start,
        const label size
    ) const;

    //- Get subset with addressing and store in given field
    void getSubset
    (
        DecoupledCoeffField<Type>& f,
        const labelList& addr
    ) const;

    //- Set subset with offset and size from given field
    void setSubset
    (
        const DecoupledCoeffField<Type>& f,
        const label start,
        const label size
    );

    //- Get subset with addressing and store in target field
    void setSubset
    (
         const DecoupledCoeffField<Type>& f,
         const labelList& addr
    );

    //- Zero out subset with offset and size
    void zeroOutSubset
    (
        const label start,
        const label size
    );

    //- Zero out subset with addressing
    void zeroOutSubset
    (
        const labelList& addr
    );

    //- Add subset with addressing to field
    void addSubset
    (
        const DecoupledCoeffField<Type>& f,
        const labelList& addr
    );

    // Member operators
    void operator=(const DecoupledCoeffField<Type>&);
    void operator=(const tmp<DecoupledCoeffField<Type> >&);

    void operator=(const scalarTypeField&);
    void operator=(const tmp<scalarTypeField>&);

    void operator=(const linearTypeField&);
    void operator=(const tmp<linearTypeField>&);

    void operator+=(const DecoupledCoeffField<Type>&);
    void operator+=(const tmp<DecoupledCoeffField<Type> >&);

    void operator+=(const scalarTypeField&);
    void operator+=(const tmp<scalarTypeField>&);

    void operator+=(const linearTypeField&);
    void operator+=(const tmp<linearTypeField>&);

    void operator-=(const DecoupledCoeffField<Type>&);
    void operator-=(const tmp<DecoupledCoeffField<Type> >&);

    void operator-=(const scalarTypeField&);
    void operator-=(const tmp<scalarTypeField>&);

    void operator-=(const linearTypeField&);
    void operator-=(const tmp<linearTypeField>&);

    void operator*=(const UList<scalar>&);
    void operator*=(const tmp<Field<scalar> >&);
    void operator*=(const scalar&);

    void operator/=(const UList<scalar>&);
    void operator/=(const tmp<Field<scalar> >&);
    void operator/=(const scalar&);

    // IOstream operators
    friend Ostream& operator<< <Type>
    (
        Ostream&,
        const DecoupledCoeffField<Type>&
    );

    friend Ostream& operator<< <Type>
    (
        Ostream&,
        const tmp<DecoupledCoeffField<Type> >&
    );
};


} // End namespace CML


#include "DecoupledCoeffFieldFunctions.hpp"


namespace CML
{

// * * * * * * * * * * * * * * * Static Members  * * * * * * * * * * * * * * //

template<class Type>
const char* const
DecoupledCoeffField<Type>::typeName("DecoupledCoeffField");

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Type>
template<class Type2>
inline void DecoupledCoeffField<Type>::checkSize
(
    const UList<Type2>& f
) const
{
    if (f.size() != this->size())
    {
        FatalErrorInFunction
            << "Incorrect field size: " << f.size()
            << " local size: " << size()
            << abort(FatalError);
    }
}


template<class Type>
typename CML::DecoupledCoeffField<Type>::scalarTypeField&
DecoupledCoeffField<Type>::toScalar()
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

        scalarCoeffPtr_ =
            new scalarTypeField(size(), pTraits<scalarType>::zero);
    }

    return *scalarCoeffPtr_;
}


template<class Type>
typename CML::DecoupledCoeffField<Type>::linearTypeField&
DecoupledCoeffField<Type>::toLinear()
{
    if (not linearCoeffPtr_)
    {
        linearCoeffPtr_ =
            new linearTypeField(size(), pTraits<linearType>::zero);

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
DecoupledCoeffField<Type>::DecoupledCoeffField(const label size)
:
    scalarCoeffPtr_(nullptr),
    linearCoeffPtr_(nullptr),
    size_(size)
{}


template<class Type>
DecoupledCoeffField<Type>::DecoupledCoeffField
(
    const DecoupledCoeffField<Type>& f
)
:
    refCount(),
    scalarCoeffPtr_(nullptr),
    linearCoeffPtr_(nullptr),
    size_(f.size())
{
    if (f.scalarCoeffPtr_)
    {
        scalarCoeffPtr_ = new scalarTypeField(*(f.scalarCoeffPtr_));
    }
    else if (f.linearCoeffPtr_)
    {
        linearCoeffPtr_ = new linearTypeField(*(f.linearCoeffPtr_));
    }
}


template<class Type>
DecoupledCoeffField<Type>::DecoupledCoeffField(Istream& is)
:
    scalarCoeffPtr_(nullptr),
    linearCoeffPtr_(nullptr),
    size_(0)
{
    // Read keyword and pick up allocated field
    word key(is);

    if
    (
        key
     == blockCoeffBase::activeLevelNames_[blockCoeffBase::UNALLOCATED]
    )
    {
        size_ = readLabel(is);
    }
    else if
    (
        key
     == blockCoeffBase::activeLevelNames_[blockCoeffBase::SCALAR]
    )
    {
        scalarCoeffPtr_ = new scalarTypeField(is);
        size_ = scalarCoeffPtr_->size();
    }
    else if
    (
        key
     == blockCoeffBase::activeLevelNames_[blockCoeffBase::LINEAR]
    )
    {
        linearCoeffPtr_ = new linearTypeField(is);
        size_ = linearCoeffPtr_->size();
    }
    else
    {
        FatalIOErrorInFunction(is)
            << "invalid keyword while reading: " << key
            << exit(FatalIOError);
    }
}


template<class Type>
tmp<DecoupledCoeffField<Type> >
DecoupledCoeffField<Type>::clone() const
{
    return tmp<DecoupledCoeffField<Type> >
    (
        new DecoupledCoeffField<Type>(*this)
    );
}

// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
DecoupledCoeffField<Type>::~DecoupledCoeffField()
{
    this->clear();
}


template<class Type>
void DecoupledCoeffField<Type>::clear()
{
    deleteDemandDrivenData(scalarCoeffPtr_);
    deleteDemandDrivenData(linearCoeffPtr_);
}

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
inline label DecoupledCoeffField<Type>::size() const
{
    return size_;
}


template<class Type>
void DecoupledCoeffField<Type>::negate()
{
    if (scalarCoeffPtr_)
    {
        scalarCoeffPtr_->negate();
    }
    else if (linearCoeffPtr_)
    {
        linearCoeffPtr_->negate();
    }
}


template<class Type>
tmp<DecoupledCoeffField<Type> >
DecoupledCoeffField<Type>::transpose() const
{
    tmp<DecoupledCoeffField<Type> > tt
    (
        new DecoupledCoeffField<Type>(this->size())
    );
    DecoupledCoeffField<Type>& t = tt();

    if (scalarCoeffPtr_)
    {
        t.toScalar() = *scalarCoeffPtr_;
    }
    else if (linearCoeffPtr_)
    {
        t.toLinear() = *linearCoeffPtr_;
    }
    else
    {
        // Not allocated - do nothing
    }

    return tt;
}


template<class Type>
blockCoeffBase::activeLevel
DecoupledCoeffField<Type>::activeType() const
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
void DecoupledCoeffField<Type>::checkActive() const
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
const typename CML::DecoupledCoeffField<Type>::scalarTypeField&
DecoupledCoeffField<Type>::asScalar() const
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
const typename CML::DecoupledCoeffField<Type>::linearTypeField&
DecoupledCoeffField<Type>::asLinear() const
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
typename CML::DecoupledCoeffField<Type>::scalarTypeField&
DecoupledCoeffField<Type>::asScalar()
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
typename CML::DecoupledCoeffField<Type>::linearTypeField&
DecoupledCoeffField<Type>::asLinear()
{
    if (not linearCoeffPtr_)
    {
        return this->toLinear();
    }

    return *linearCoeffPtr_;
}


template<class Type>
CML::tmp<typename CML::DecoupledCoeffField<Type>::scalarTypeField>
DecoupledCoeffField<Type>::component(const direction dir) const
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
            << "Field not allocated."
            << abort(FatalError);
    }

    // Dummy return to keep compiler happy
    return *scalarCoeffPtr_;
}


template<class Type>
BlockCoeff<Type>
DecoupledCoeffField<Type>::getCoeff(const label index) const
{
    BlockCoeff<Type> result;

    if (scalarCoeffPtr_)
    {
        result.asScalar() = (*scalarCoeffPtr_)[index];
    }
    else if (linearCoeffPtr_)
    {
        result.asLinear() = (*linearCoeffPtr_)[index];
    }

    return result;
}


template<class Type>
void DecoupledCoeffField<Type>::setCoeff
(
    const label index,
    const BlockCoeff<Type>& coeff
)
{
    BlockCoeff<Type> result;

    if (coeff.activeType() == blockCoeffBase::SCALAR)
    {
        (*scalarCoeffPtr_)[index] = result.asScalar();
    }
    else if (coeff.activeType() == blockCoeffBase::LINEAR)
    {
        (*linearCoeffPtr_)[index] = result.asLinear();
    }
}


template<class Type>
void DecoupledCoeffField<Type>::getSubset
(
    DecoupledCoeffField<Type>& f,
    const label start,
    const label size
) const
{
    // Check sizes
    if (f.size() != size)
    {
        FatalErrorInFunction
            << "Incompatible sizes: " << f.size() << " and " << size
            << abort(FatalError);
    }

    if (scalarCoeffPtr_)
    {
        scalarTypeField& ff = f.asScalar();

        const scalarTypeField& localF = (*scalarCoeffPtr_);

        forAll (ff, ffI)
        {
            ff[ffI] = localF[start + ffI];
        }
    }
    else if (linearCoeffPtr_)
    {
        linearTypeField& ff = f.asLinear();

        const linearTypeField& localF = (*linearCoeffPtr_);

        forAll (ff, ffI)
        {
            ff[ffI] = localF[start + ffI];
        }
    }
}


template<class Type>
void DecoupledCoeffField<Type>::getSubset
(
    DecoupledCoeffField<Type>& f,
    const labelList& addr
) const
{
    // Check sizes
    if (f.size() != addr.size())
    {
        FatalErrorInFunction
            << "Incompatible sizes: " << f.size() << " and " << addr.size()
            << abort(FatalError);
    }

    if (scalarCoeffPtr_)
    {
        scalarTypeField& ff = f.asScalar();

        const scalarTypeField& localF = (*scalarCoeffPtr_);

        forAll (ff, ffI)
        {
            ff[ffI] = localF[addr[ffI]];
        }
    }
    else if (linearCoeffPtr_)
    {
        linearTypeField& ff = f.asLinear();

        const linearTypeField& localF = (*linearCoeffPtr_);

        forAll (ff, ffI)
        {
            ff[ffI] = localF[addr[ffI]];
        }
    }
}


template<class Type>
void DecoupledCoeffField<Type>::setSubset
(
    const DecoupledCoeffField<Type>& f,
    const label start,
    const label size
)
{
    // Check sizes
    if (f.size() != size)
    {
        FatalErrorInFunction
            << "Incompatible sizes: " << f.size() << " and " << size
            << abort(FatalError);
    }

    if (f.activeType() == blockCoeffBase::SCALAR)
    {
        const scalarTypeField& ff = f.asScalar();

        scalarTypeField& localF = this->asScalar();

        forAll (ff, ffI)
        {
            localF[start + ffI] = ff[ffI];
        }
    }
    else if (f.activeType() == blockCoeffBase::LINEAR)
    {
        const linearTypeField& ff = f.asLinear();

        linearTypeField& localF = this->asLinear();

        forAll (ff, ffI)
        {
            localF[start + ffI] = ff[ffI];
        }
    }
}


template<class Type>
void DecoupledCoeffField<Type>::setSubset
(
    const DecoupledCoeffField<Type>& f,
    const labelList& addr
)
{
    // Check sizes
    if (f.size() != addr.size())
    {
        FatalErrorInFunction
            << "Incompatible sizes: " << f.size() << " and " << addr.size()
            << abort(FatalError);
    }

    if (f.activeType() == blockCoeffBase::SCALAR)
    {
        const scalarTypeField& ff = f.asScalar();

        scalarTypeField& localF = this->asScalar();

        forAll (ff, ffI)
        {
            localF[addr[ffI]] = ff[ffI];
        }
    }
    else if (f.activeType() == blockCoeffBase::LINEAR)
    {
        const linearTypeField& ff = f.asLinear();

        linearTypeField& localF = this->asLinear();

        forAll (ff, ffI)
        {
            localF[addr[ffI]] = ff[ffI];
        }
    }
}


template<class Type>
void DecoupledCoeffField<Type>::zeroOutSubset
(
    const label start,
    const label size
)
{
    if (scalarCoeffPtr_)
    {
        scalarTypeField& localF = this->asScalar();

        for (label ffI = 0; ffI < size; ffI++)
        {
            localF[start + ffI] = pTraits<scalarType>::zero;
        }
    }
    else if (linearCoeffPtr_)
    {
        linearTypeField& localF = this->asLinear();

        for (label ffI = 0; ffI < size; ffI++)
        {
            localF[start + ffI] = pTraits<linearType>::zero;
        }
    }
}


template<class Type>
void DecoupledCoeffField<Type>::zeroOutSubset
(
    const labelList& addr
)
{
    if (scalarCoeffPtr_)
    {
        scalarTypeField& localF = this->asScalar();

        forAll (addr, ffI)
        {
            localF[addr[ffI]] = pTraits<scalarType>::zero;
        }
    }
    else if (linearCoeffPtr_)
    {
        linearTypeField& localF = this->asLinear();

        forAll (addr, ffI)
        {
            localF[addr[ffI]] = pTraits<linearType>::zero;
        }
    }
}


template<class Type>
void DecoupledCoeffField<Type>::addSubset
(
    const DecoupledCoeffField<Type>& f,
    const labelList& addr
)
{
    // Check sizes
    if (f.size() != addr.size())
    {
        FatalErrorInFunction
            << "Incompatible sizes: " << f.size() << " and " << addr.size()
            << abort(FatalError);
    }

    if
    (
        f.activeType() == blockCoeffBase::LINEAR
     || this->activeType() == blockCoeffBase::LINEAR
    )
    {
        const linearTypeField& ff = f.asLinear();

        linearTypeField& localF = this->asLinear();

        forAll (ff, ffI)
        {
            localF[addr[ffI]] += ff[ffI];
        }
    }
    else if
    (
        f.activeType() == blockCoeffBase::SCALAR
     && this->activeType() == blockCoeffBase::SCALAR
    )
    {
        const scalarTypeField& ff = f.asScalar();

        scalarTypeField& localF = this->asScalar();

        forAll (ff, ffI)
        {
            localF[addr[ffI]] += ff[ffI];
        }
    }
    else
    {
        FatalErrorInFunction
            << "Incompatible combination of types"
            << abort(FatalError);
    }
}

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template<class Type>
void DecoupledCoeffField<Type>::operator=
(
    const DecoupledCoeffField<Type>& f
)
{
    if (this == &f)
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    // Check field sizes
    if (f.size() != this->size())
    {
        FatalErrorInFunction
            << "Incorrect field size: " << f.size()
            << " local size: " << size()
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


template<class Type>
void DecoupledCoeffField<Type>::operator=
(
    const tmp<DecoupledCoeffField>& tf
)
{
    if (this == &(tf()))
    {
        FatalErrorInFunction
            << "attempted assignment to self"
            << abort(FatalError);
    }

    operator=(tf());
    tf.clear();
}


} // End namespace CML


#define COMPUTED_BASE_ASSIGNMENT(op)                                          \
                                                                              \
template<class Type>                                                          \
void CML::DecoupledCoeffField<Type>::operator op                             \
(                                                                             \
    const DecoupledCoeffField<Type>& f                                        \
)                                                                             \
{                                                                             \
    if (f.size() != this->size())                                             \
    {                                                                         \
        FatalErrorInFunction                                                  \
            << "Incorrect field size: " << f.size()                           \
            << " local size: " << size()                                      \
            << abort(FatalError);                                             \
    }                                                                         \
                                                                              \
                                                                              \
    if (f.scalarCoeffPtr_)                                                    \
    {                                                                         \
        this->toScalar() op *(f.scalarCoeffPtr_);                             \
    }                                                                         \
    else if (f.linearCoeffPtr_)                                               \
    {                                                                         \
        this->toLinear() op *(f.linearCoeffPtr_);                             \
    }                                                                         \
    else                                                                      \
    {                                                                         \
    }                                                                         \
}                                                                             \
                                                                              \
                                                                              \
template<class Type>                                                          \
void CML::DecoupledCoeffField<Type>::operator op                             \
(                                                                             \
    const tmp<DecoupledCoeffField<Type> >& tf                                 \
)                                                                             \
{                                                                             \
    operator op(tf());                                                        \
    tf.clear();                                                               \
}


#define COMPUTED_ARG_ASSIGNMENT(op)                                           \
                                                                              \
template<class Type>                                                          \
void CML::DecoupledCoeffField<Type>::operator op(const scalarTypeField& f)   \
{                                                                             \
    checkSize(f);                                                             \
                                                                              \
    const blockCoeffBase::activeLevel al = this->activeType();                \
                                                                              \
    if (al == blockCoeffBase::UNALLOCATED || al == blockCoeffBase::SCALAR)    \
    {                                                                         \
        this->toScalar() op f;                                                \
    }                                                                         \
    else if (al == blockCoeffBase::LINEAR)                                    \
    {                                                                         \
        this->toLinear() op f*pTraits<linearType>::one;                       \
    }                                                                         \
    else                                                                      \
    {                                                                         \
    }                                                                         \
}                                                                             \
                                                                              \
                                                                              \
template<class Type>                                                          \
void CML::DecoupledCoeffField<Type>::operator op                             \
(                                                                             \
    const tmp<scalarTypeField>& tf                                            \
)                                                                             \
{                                                                             \
    operator op(tf());                                                        \
    tf.clear();                                                               \
}                                                                             \
                                                                              \
                                                                              \
template<class Type>                                                          \
void CML::DecoupledCoeffField<Type>::operator op(const linearTypeField& f)   \
{                                                                             \
    checkSize(f);                                                             \
                                                                              \
    const blockCoeffBase::activeLevel al = this->activeType();                \
                                                                              \
    if                                                                        \
    (                                                                         \
        al == blockCoeffBase::UNALLOCATED                                     \
     || al == blockCoeffBase::SCALAR                                          \
     || al == blockCoeffBase::LINEAR                                          \
    )                                                                         \
    {                                                                         \
        this->toLinear() op f;                                                \
    }                                                                         \
    else                                                                      \
    {                                                                         \
    }                                                                         \
}                                                                             \
                                                                              \
                                                                              \
template<class Type>                                                          \
void CML::DecoupledCoeffField<Type>::operator op                             \
(                                                                             \
    const tmp<linearTypeField>& tf                                            \
)                                                                             \
{                                                                             \
    operator op(tf());                                                        \
    tf.clear();                                                               \
}                                                                             \


#define COMPUTED_BASE_OPERATOR(TYPE, op)                                      \
                                                                              \
template<class Type>                                                          \
void CML::DecoupledCoeffField<Type>::operator op(const TYPE& t)              \
{                                                                             \
    if (scalarCoeffPtr_)                                                      \
    {                                                                         \
        *(scalarCoeffPtr_) op t;                                              \
    }                                                                         \
    else if (linearCoeffPtr_)                                                 \
    {                                                                         \
        *(linearCoeffPtr_) op t;                                              \
    }                                                                         \
    else                                                                      \
    {                                                                         \
    }                                                                         \
}                                                                             \
                                                                              \
                                                                              \
template<class Type>                                                          \
void CML::DecoupledCoeffField<Type>::operator op(const UList<TYPE>& tf)      \
{                                                                             \
    checkSize(tf);                                                            \
                                                                              \
    if (scalarCoeffPtr_)                                                      \
    {                                                                         \
        *(scalarCoeffPtr_) op tf;                                             \
    }                                                                         \
    else if (linearCoeffPtr_)                                                 \
    {                                                                         \
        *(linearCoeffPtr_) op tf;                                             \
    }                                                                         \
    else                                                                      \
    {                                                                         \
    }                                                                         \
}                                                                             \
                                                                              \
template<class Type>                                                          \
void CML::DecoupledCoeffField<Type>::operator op                             \
(                                                                             \
    const tmp<Field<TYPE> >& tf                                               \
)                                                                             \
{                                                                             \
    operator op(tf());                                                        \
    tf.clear();                                                               \
}


#define COMPUTED_ASSIGNMENT(op)                                               \
COMPUTED_BASE_ASSIGNMENT(op)                                                  \
COMPUTED_ARG_ASSIGNMENT(op)

// Remaining operator=
COMPUTED_ARG_ASSIGNMENT(=)

COMPUTED_ASSIGNMENT(+=)
COMPUTED_ASSIGNMENT(-=)

COMPUTED_BASE_OPERATOR(scalar, *=)
COMPUTED_BASE_OPERATOR(scalar, /=)

#undef COMPUTED_BASE_OPERATOR
#undef COMPUTED_BASE_ASSIGNMENT
#undef COMPUTED_ARG_ASSIGNMENT
#undef COMPUTED_ASSIGNMENT

// * * * * * * * * * * * * * * * Ostream Operator  * * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const DecoupledCoeffField<Type>& f
)
{
    // Write active type
    os << blockCoeffBase::activeLevelNames_[f.activeType()] << nl;

    if (f.activeType() == blockCoeffBase::SCALAR)
    {
        os << f.asScalar();
    }
    else if (f.activeType() == blockCoeffBase::LINEAR)
    {
        os << f.asLinear();
    }
    else
    {
        // Not allocated: write size
        os << f.size();
    }

    return os;
}


template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const tmp<DecoupledCoeffField<Type> >& tf
)
{
    os << tf();
    tf.clear();
    return os;
}


#   include "DecoupledCoeffFieldFunctions.hxx"
#endif
