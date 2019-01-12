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
    CML::dimensioned

Description
    Generic dimensioned Type class

SourceFiles
    dimensionedType.cpp

\*---------------------------------------------------------------------------*/

#ifndef dimensionedType_H
#define dimensionedType_H

#include "word.hpp"
#include "direction.hpp"
#include "dimensionSet.hpp"
#include "VectorSpace.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// Forward declaration of friend functions and operators

template<class Type> class dimensioned;

template<class Type>
Istream& operator>>(Istream&, dimensioned<Type>&);

template<class Type>
Ostream& operator<<(Ostream&, const dimensioned<Type>&);

class dictionary;

/*---------------------------------------------------------------------------*\
                           Class dimensioned Declaration
\*---------------------------------------------------------------------------*/

template <class Type>
class dimensioned
{
    // Private data

        //- Variable name
        word name_;

        //- The dimension set
        dimensionSet dimensions_;

        //- The data value
        Type value_;


    // Private member functions

        //- Initialize from Istream
        //  Helper-function for constructors
        void initialize(Istream& is);


public:

    //- Component type
    typedef typename pTraits<Type>::cmptType cmptType;


    // Constructors

        //- Construct given a name, a value and its dimensionSet.
        dimensioned(const word&, const dimensionSet&, const Type);

        //- Construct from a dimensioned<Type> changing the name.
        dimensioned(const word&, const dimensioned<Type>&);

        //- Construct given a value (creates dimensionless value).
        dimensioned(const Type& t)
        :
            name_(::CML::name(t)),
            dimensions_(dimless),
            value_(t)
        {}

        //- Construct from Istream.
        dimensioned(Istream&);

        //- Construct from an Istream with a given name
        dimensioned(const word&, Istream&);

        //- Construct from an Istream with a given name and dimensions
        dimensioned(const word&, const dimensionSet&, Istream&);

        //- Construct from dictionary lookup with a given name and dimensions
        dimensioned(const word&, const dimensionSet&, const dictionary&);

        //- Null constructor
        dimensioned();


    // Static member functions

        //- Construct from dictionary, with default value.
        static dimensioned<Type> lookupOrDefault
        (
            const word&,
            const dictionary&,
            const Type& defaultValue = pTraits<Type>::zero,
            const dimensionSet& dims = dimless
        );

        //- Construct from dictionary, with default value.
        //  If the value is not found, it is added into the dictionary.
        static dimensioned<Type> lookupOrAddToDict
        (
            const word&,
            dictionary&,
            const Type& defaultValue = pTraits<Type>::zero,
            const dimensionSet& dims = dimless
        );


    // Member functions

        //- Return const reference to name.
        const word& name() const;

        //- Return non-const reference to name.
        word& name();

        //- Return const reference to dimensions.
        const dimensionSet& dimensions() const;

        //- Return non-const reference to dimensions.
        dimensionSet& dimensions();

        //- Return const reference to value.
        const Type& value() const;

        //- Return non-const reference to value.
        Type& value();

        //- Return a component as a dimensioned<cmptType>
        dimensioned<cmptType> component(const direction) const;

        //- Return a component with a dimensioned<cmptType>
        void replace(const direction, const dimensioned<cmptType>&);

        //- Return transpose.
        dimensioned<Type> T() const;

        //- Update the value of dimensioned<Type>
        void read(const dictionary&);

        //- Update the value of dimensioned<Type> if found in the dictionary.
        bool readIfPresent(const dictionary&);


    // Member operators

        //- Return a component as a dimensioned<cmptType>
        dimensioned<cmptType> operator[](const direction) const;

        void operator+=(const dimensioned<Type>&);
        void operator-=(const dimensioned<Type>&);
        void operator*=(const scalar);
        void operator/=(const scalar);


    // IOstream operators

        friend Istream& operator>> <Type>
        (Istream&, dimensioned<Type>&);

        friend Ostream& operator<< <Type>
        (Ostream&, const dimensioned<Type>&);
};


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template<class Type, CML::direction r>
dimensioned<typename powProduct<Type, r>::type>
pow
(
    const dimensioned<Type>&,
    typename powProduct<Type, r>::type
  = pTraits<typename powProduct<Type, r>::type>::zero
);

template<class Type>
dimensioned<typename outerProduct<Type, Type>::type>
sqr(const dimensioned<Type>&);

template<class Type>
dimensioned<scalar> magSqr(const dimensioned<Type>&);

template<class Type>
dimensioned<scalar> mag(const dimensioned<Type>&);

template<class Type>
dimensioned<Type> cmptMultiply
(
    const dimensioned<Type>&,
    const dimensioned<Type>&
);

template<class Type>
dimensioned<Type> cmptDivide
(
    const dimensioned<Type>&,
    const dimensioned<Type>&
);

template<class Type>
dimensioned<Type> max(const dimensioned<Type>&, const dimensioned<Type>&);

template<class Type>
dimensioned<Type> min(const dimensioned<Type>&, const dimensioned<Type>&);

template<class Type>
bool operator>(const dimensioned<Type>&, const dimensioned<Type>&);

template<class Type>
bool operator<(const dimensioned<Type>&, const dimensioned<Type>&);

template<class Type>
dimensioned<Type> operator+(const dimensioned<Type>&, const dimensioned<Type>&);

template<class Type>
dimensioned<Type> operator-(const dimensioned<Type>&);

template<class Type>
dimensioned<Type> operator-(const dimensioned<Type>&, const dimensioned<Type>&);

template<class Type>
dimensioned<Type> operator*
(
    const dimensioned<scalar>&,
    const dimensioned<Type>&
);

template<class Type>
dimensioned<Type> operator/
(
    const dimensioned<Type>&,
    const dimensioned<scalar>&
);


// Products
// ~~~~~~~~

#define PRODUCT_OPERATOR(product, op, opFunc)                                 \
                                                                              \
template<class Type1, class Type2>                                            \
dimensioned<typename product<Type1, Type2>::type>                             \
operator op(const dimensioned<Type1>&, const dimensioned<Type2>&);            \
                                                                              \
template<class Type, class Form, class Cmpt, CML::direction nCmpt>            \
dimensioned<typename product<Type, Form>::type>                               \
operator op                                                                   \
(                                                                             \
    const dimensioned<Type>&,                                                 \
    const VectorSpace<Form,Cmpt,nCmpt>&                                       \
);                                                                            \
                                                                              \
template<class Type, class Form, class Cmpt, CML::direction nCmpt>            \
dimensioned<typename product<Form, Type>::type>                               \
operator op                                                                   \
(                                                                             \
    const VectorSpace<Form,Cmpt,nCmpt>&,                                      \
    const dimensioned<Type>&                                                  \
);

PRODUCT_OPERATOR(outerProduct, *, outer)
PRODUCT_OPERATOR(crossProduct, ^, cross)
PRODUCT_OPERATOR(innerProduct, &, dot)
PRODUCT_OPERATOR(scalarProduct, &&, dotdot)

#undef PRODUCT_OPERATOR


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "pTraits.hpp"
#include "dictionary.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void dimensioned<Type>::initialize(Istream& is)
{
    token nextToken(is);
    is.putBack(nextToken);

    // Check if the original format is used in which the name is provided
    // and reset the name to that read
    if (nextToken.isWord())
    {
        is >> name_;
        is >> nextToken;
        is.putBack(nextToken);
    }

    // If the dimensions are provided compare with the argument
    if (nextToken == token::BEGIN_SQR)
    {
        dimensionSet dims(is);

        if (dims != dimensions_)
        {
            FatalIOErrorInFunction(is)
              << "The dimensions " << dims
              << " provided do not match the required dimensions "
              << dimensions_
              << abort(FatalIOError);
        }
    }

    is >> value_;
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template <class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    const dimensionSet& dimSet,
    const Type t
)
:
    name_(name),
    dimensions_(dimSet),
    value_(t)
{}


template <class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    const dimensioned<Type>& dt
)
:
    name_(name),
    dimensions_(dt.dimensions_),
    value_(dt.value_)
{}


template <class Type>
dimensioned<Type>::dimensioned
(
    Istream& is
)
:
    name_(is),
    dimensions_(is),
    value_(pTraits<Type>(is))
{}


template <class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    Istream& is
)
:
    name_(name),
    dimensions_(is),
    value_(pTraits<Type>(is))
{}


template <class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    const dimensionSet& dimSet,
    Istream& is
)
:
    name_(name),
    dimensions_(dimSet),
    value_(pTraits<Type>::zero)
{
    initialize(is);
}


template<class Type>
dimensioned<Type>::dimensioned
(
    const word& name,
    const dimensionSet& dimSet,
    const dictionary& dict
)
:
    name_(name),
    dimensions_(dimSet),
    value_(pTraits<Type>::zero)
{
    initialize(dict.lookup(name));
}


template <class Type>
dimensioned<Type>::dimensioned
()
:
    name_("undefined"),
    dimensions_(dimless),
    value_(pTraits<Type>::zero)
{}


// * * * * * * * * * * * * * Static Member Functions * * * * * * * * * * * * //

template <class Type>
dimensioned<Type> dimensioned<Type>::lookupOrDefault
(
    const word& name,
    const dictionary& dict,
    const Type& defaultValue,
    const dimensionSet& dims
)
{
    if (dict.found(name))
    {
        return dimensioned<Type>(name, dims, dict.lookup(name));
    }
    else
    {
        return dimensioned<Type>(name, dims, defaultValue);
    }
}


template <class Type>
dimensioned<Type> dimensioned<Type>::lookupOrAddToDict
(
    const word& name,
    dictionary& dict,
    const Type& defaultValue,
    const dimensionSet& dims
)
{
    Type value = dict.lookupOrAddDefault<Type>(name, defaultValue);
    return dimensioned<Type>(name, dims, value);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template <class Type>
const word& dimensioned<Type>::name() const
{
    return name_;
}

template <class Type>
word& dimensioned<Type>::name()
{
    return name_;
}


template <class Type>
const dimensionSet& dimensioned<Type>::dimensions() const
{
    return dimensions_;
}

template <class Type>
dimensionSet& dimensioned<Type>::dimensions()
{
    return dimensions_;
}


template <class Type>
const Type& dimensioned<Type>::value() const
{
    return value_;
}

template <class Type>
Type& dimensioned<Type>::value()
{
    return value_;
}


template <class Type>
dimensioned<typename dimensioned<Type>::cmptType> dimensioned<Type>::component
(
    const direction d
) const
{
    return dimensioned<cmptType>
    (
        name_ + ".component(" + CML::name(d) + ')',
        dimensions_,
        value_.component(d)
    );
}


template <class Type>
void dimensioned<Type>::replace
(
    const direction d,
    const dimensioned<typename dimensioned<Type>::cmptType>& dc
)
{
    dimensions_ = dc.dimensions();
    value_.replace(d, dc.value());
}


template <class Type>
void dimensioned<Type>::read(const dictionary& dict)
{
    dict.lookup(name_) >> value_;
}


template <class Type>
bool dimensioned<Type>::readIfPresent(const dictionary& dict)
{
    return dict.readIfPresent(name_, value_);
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

template <class Type>
dimensioned<typename dimensioned<Type>::cmptType> dimensioned<Type>::operator[]
(
    const direction d
) const
{
    return component(d);
}


template <class Type>
void dimensioned<Type>::operator+=
(
    const dimensioned<Type>& dt
)
{
    dimensions_ += dt.dimensions_;
    value_ += dt.value_;
}


template <class Type>
void dimensioned<Type>::operator-=
(
    const dimensioned<Type>& dt
)
{
    dimensions_ -= dt.dimensions_;
    value_ -= dt.value_;
}


template <class Type>
void dimensioned<Type>::operator*=
(
    const scalar s
)
{
    value_ *= s;
}


template <class Type>
void dimensioned<Type>::operator/=
(
    const scalar s
)
{
    value_ /= s;
}


// * * * * * * * * * * * * * * * Friend Functions  * * * * * * * * * * * * * //

template<class Type, CML::direction r>
dimensioned<typename powProduct<Type, r>::type>
pow(const dimensioned<Type>& dt, typename powProduct<Type, r>::type)
{
    return dimensioned<typename powProduct<Type, r>::type>
    (
        "pow(" + dt.name() + ',' + name(r) + ')',
        pow(dt.dimensions(), r),
        pow(dt.value(), 2)
    );
}


template<class Type>
dimensioned<typename outerProduct<Type, Type>::type>
sqr(const dimensioned<Type>& dt)
{
    return dimensioned<typename outerProduct<Type, Type>::type>
    (
        "sqr(" + dt.name() + ')',
        sqr(dt.dimensions()),
        sqr(dt.value())
    );
}

template<class Type>
dimensioned<scalar> magSqr(const dimensioned<Type>& dt)
{
    return dimensioned<scalar>
    (
        "magSqr(" + dt.name() + ')',
        magSqr(dt.dimensions()),
        magSqr(dt.value())
    );
}

template<class Type>
dimensioned<scalar> mag(const dimensioned<Type>& dt)
{
    return dimensioned<scalar>
    (
        "mag(" + dt.name() + ')',
        dt.dimensions(),
        mag(dt.value())
    );
}


template<class Type>
dimensioned<Type> cmptMultiply
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    return dimensioned<Type>
    (
        "cmptMultiply(" + dt1.name() + ',' + dt2.name() + ')',
        cmptMultiply(dt1.dimensions(), dt2.dimensions()),
        cmptMultiply(dt1.value(), dt2.value())
    );
}

template<class Type>
dimensioned<Type> cmptDivide
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    return dimensioned<Type>
    (
        "cmptDivide(" + dt1.name() + ',' + dt2.name() + ')',
        cmptDivide(dt1.dimensions(), dt2.dimensions()),
        cmptDivide(dt1.value(), dt2.value())
    );
}


template<class Type>
dimensioned<Type> max
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    if (dt1.dimensions() != dt2.dimensions())
    {
        FatalErrorInFunction
            << "dimensions of arguments are not equal"
            << abort(FatalError);
    }

    return dimensioned<Type>
    (
        "max(" + dt1.name() + ',' + dt2.name() + ')',
        dt1.dimensions(),
        max(dt1.value(), dt2.value())
    );
}


template <class Type>
dimensioned<Type> min
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    if (dt1.dimensions() != dt2.dimensions())
    {
        FatalErrorInFunction
            << "dimensions of arguments are not equal"
            << abort(FatalError);
    }

    return dimensioned<Type>
    (
        "min(" + dt1.name() + ',' + dt2.name() + ')',
        dt1.dimensions(),
        min(dt1.value(), dt2.value())
    );
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template <class Type>
Istream& operator>>(Istream& is, dimensioned<Type>& dt)
{
    token nextToken(is);
    is.putBack(nextToken);

    // Check if the original format is used in which the name is provided
    // and reset the name to that read
    if (nextToken.isWord())
    {
        is >> dt.name_;
        is >> nextToken;
        is.putBack(nextToken);
    }

    // If the dimensions are provided reset the dimensions to those read
    if (nextToken == token::BEGIN_SQR)
    {
        is >> dt.dimensions_;
    }

    // Read the value
    is >> dt.value_;

    // Check state of Istream
    is.check("Istream& operator>>(Istream&, dimensioned<Type>&)");

    return is;
}


template <class Type>
Ostream& operator<<(Ostream& os, const dimensioned<Type>& dt)
{
    // Do a stream write op for a dimensionSet
    os  << dt.name() << token::SPACE
        << dt.dimensions() << token::SPACE
        << dt.value();

    // Check state of Ostream
    os.check("Ostream& operator<<(Ostream&, const dimensioned<Type>&)");

    return os;
}


// * * * * * * * * * * * * * * * Global Operators  * * * * * * * * * * * * * //

template <class Type>
bool operator>
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    return dt1.value() > dt2.value();
}


template <class Type>
bool operator<
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    return dt1.value() < dt2.value();
}


template <class Type>
dimensioned<Type> operator+
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    return dimensioned<Type>
    (
        '(' + dt1.name() + '+' + dt2.name() + ')',
        dt1.dimensions() + dt2.dimensions(),
        dt1.value() + dt2.value()
    );
}


template <class Type>
dimensioned<Type> operator-(const dimensioned<Type>& dt)
{
    return dimensioned<Type>
    (
        '-' + dt.name(),
        dt.dimensions(),
        -dt.value()
    );
}


template <class Type>
dimensioned<Type> operator-
(
    const dimensioned<Type>& dt1,
    const dimensioned<Type>& dt2
)
{
    return dimensioned<Type>
    (
        '(' + dt1.name() + '-' + dt2.name() + ')',
        dt1.dimensions() - dt2.dimensions(),
        dt1.value() - dt2.value()
    );
}


template <class Type>
dimensioned<Type> operator*
(
    const dimensioned<scalar>& ds,
    const dimensioned<Type>& dt
)
{
    return dimensioned<Type>
    (
        '(' + ds.name() + '*' + dt.name() + ')',
        ds.dimensions() * dt.dimensions(),
        ds.value() * dt.value()
    );
}


template <class Type>
dimensioned<Type> operator/
(
    const dimensioned<Type>& dt,
    const dimensioned<scalar>& ds
)
{
    return dimensioned<Type>
    (
        '(' + dt.name() + '|' + ds.name() + ')',
        dt.dimensions()/ds.dimensions(),
        dt.value()/ds.value()
    );
}


// Products
// ~~~~~~~~

#define PRODUCT_OPERATOR(product, op, opFunc)                                 \
                                                                              \
template<class Type1, class Type2>                                            \
dimensioned<typename product<Type1, Type2>::type>                             \
operator op(const dimensioned<Type1>& dt1, const dimensioned<Type2>& dt2)     \
{                                                                             \
    return dimensioned<typename product<Type1, Type2>::type>                  \
    (                                                                         \
        '(' + dt1.name() + #op + dt2.name() + ')',                            \
        dt1.dimensions() op dt2.dimensions(),                                 \
        dt1.value() op dt2.value()                                            \
    );                                                                        \
}                                                                             \
                                                                              \
template<class Type, class Form, class Cmpt, CML::direction nCmpt>            \
dimensioned<typename product<Type, Form>::type>                               \
operator op                                                                   \
(                                                                             \
    const dimensioned<Type>& dt1,                                             \
    const VectorSpace<Form,Cmpt,nCmpt>& t2                                    \
)                                                                             \
{                                                                             \
    return dimensioned<typename product<Type, Form>::type>                    \
    (                                                                         \
        '(' + dt1.name() + #op + name(t2) + ')',                              \
        dt1.dimensions(),                                                     \
        dt1.value() op static_cast<const Form&>(t2)                           \
    );                                                                        \
}                                                                             \
                                                                              \
template<class Type, class Form, class Cmpt, CML::direction nCmpt>            \
dimensioned<typename product<Form, Type>::type>                               \
operator op                                                                   \
(                                                                             \
    const VectorSpace<Form,Cmpt,nCmpt>& t1,                                   \
    const dimensioned<Type>& dt2                                              \
)                                                                             \
{                                                                             \
    return dimensioned<typename product<Form, Type>::type>                    \
    (                                                                         \
        '(' + name(t1) + #op + dt2.name() + ')',                              \
        dt2.dimensions(),                                                     \
        static_cast<const Form&>(t1) op dt2.value()                           \
    );                                                                        \
}


PRODUCT_OPERATOR(outerProduct, *, outer)
PRODUCT_OPERATOR(crossProduct, ^, cross)
PRODUCT_OPERATOR(innerProduct, &, dot)
PRODUCT_OPERATOR(scalarProduct, &&, dotdot)

#undef PRODUCT_OPERATOR


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
