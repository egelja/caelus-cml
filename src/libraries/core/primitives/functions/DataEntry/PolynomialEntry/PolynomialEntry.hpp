/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2016 OpenFOAM Foundation
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
    CML::DataEntryTypes::PolynomialEntry

Description
    PolynomialEntry container data entry for scalars. Items are stored in a
    list of Tuple2's. Data is input in the form,
    e.g. for an entry \<entryName\> that describes y = x^2 + 2x^3

    \verbatim
        <entryName>  polynomial
        (
            (1    2)
            (2    3)
        );
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef PolynomialEntry_HPP
#define PolynomialEntry_HPP

#include "DataEntry.hpp"
#include "Tuple2.hpp"
#include "DataEntryFwd.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace DataEntryTypes
{

/*---------------------------------------------------------------------------*\
                        Class Polynomial Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Polynomial
:
    public DataEntry<Type>
{
    // Private data

        //- Polynomial coefficients - list of prefactor, exponent
        List<Tuple2<Type, Type> > coeffs_;

        //- Flag to indicate whether poly can be integrated
        bool canIntegrate_;


    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const Polynomial<Type>&);


public:

    //- Runtime type information
    TypeName("polynomial");


    // Constructors

        Polynomial(const word& entryName, const dictionary& dict);

        //- Construct from components
        Polynomial
        (
            const word& entryName,
            const List<Tuple2<Type, Type> >&
        );

        //- Copy constructor
        Polynomial(const Polynomial& poly);

        //- Construct and return a clone
        virtual tmp<DataEntry<Type> > clone() const
        {
            return tmp<DataEntry<Type> >(new Polynomial(*this));
        }


    //- Destructor
    virtual ~Polynomial();


    // Member Functions

        // Manipulation

            //- Convert time
            virtual void convertTimeBase(const Time& t);


        // Evaluation

            //- Return Polynomial value
            Type value(const scalar x) const;

            //- Integrate between two (scalar) values
            Type integrate(const scalar x1, const scalar x2) const;


        //- Write in dictionary format
        virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace DataEntryTypes
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::Polynomial<Type>::Polynomial
(
    const word& entryName,
    const dictionary& dict
)
:
    DataEntry<Type>(entryName),
    coeffs_(),
    canIntegrate_(true)
{
    Istream& is(dict.lookup(entryName));
    word entryType(is);

    is  >> coeffs_;

    if (!coeffs_.size())
    {
        FatalErrorInFunction
            << "Polynomial coefficients for entry " << this->name_
            << " are invalid (empty)" << nl << exit(FatalError);
    }

    forAll(coeffs_, i)
    {
        if (mag(coeffs_[i].second() + pTraits<Type>::one) < ROOTVSMALL)
        {
            canIntegrate_ = false;
            break;
        }
    }

    if (debug)
    {
        if (!canIntegrate_)
        {
            WarningInFunction
                << "Polynomial " << this->name_ << " cannot be integrated"
                << endl;
        }
    }
}


template<class Type>
CML::DataEntryTypes::Polynomial<Type>::Polynomial
(
    const word& entryName,
    const List<Tuple2<Type, Type> >& coeffs
)
:
    DataEntry<Type>(entryName),
    coeffs_(coeffs),
    canIntegrate_(true)
{
    if (!coeffs_.size())
    {
        FatalErrorInFunction
            << "Polynomial coefficients for entry " << this->name_
            << " are invalid (empty)" << nl << exit(FatalError);
    }

    forAll(coeffs_, i)
    {
        if (mag(coeffs_[i].second() + 1) < ROOTVSMALL)
        {
            canIntegrate_ = false;
            break;
        }
    }

    if (debug)
    {
        if (!canIntegrate_)
        {
            WarningInFunction
                << "Polynomial " << this->name_ << " cannot be integrated"
                << endl;
        }
    }
}


template<class Type>
CML::DataEntryTypes::Polynomial<Type>::Polynomial(const Polynomial& poly)
:
    DataEntry<Type>(poly),
    coeffs_(poly.coeffs_),
    canIntegrate_(poly.canIntegrate_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::Polynomial<Type>::~Polynomial()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::DataEntryTypes::Polynomial<Type>::convertTimeBase(const Time& t)
{
    forAll(coeffs_, i)
    {
        Type value = coeffs_[i].first();
        for (direction cmpt = 0; cmpt < pTraits<Type>::nComponents; cmpt++)
        {
            setComponent(coeffs_[i].first(), cmpt) =
                t.userTimeToTime(component(value, cmpt));
        }
    }
}


template<class Type>
Type CML::DataEntryTypes::Polynomial<Type>::value(const scalar x) const
{
    Type y(Zero);
    forAll(coeffs_, i)
    {
        y += cmptMultiply
        (
            coeffs_[i].first(),
            cmptPow(pTraits<Type>::one*x, coeffs_[i].second())
        );
    }

    return y;
}


template<class Type>
Type CML::DataEntryTypes::Polynomial<Type>::integrate
(
    const scalar x1,
    const scalar x2
) const
{
    Type intx(Zero);

    if (canIntegrate_)
    {
        forAll(coeffs_, i)
        {
            intx += cmptMultiply
            (
                cmptDivide
                (
                    coeffs_[i].first(),
                    coeffs_[i].second() + pTraits<Type>::one
                ),
                cmptPow
                (
                    pTraits<Type>::one*x2,
                    coeffs_[i].second() + pTraits<Type>::one
                )
              - cmptPow
                (
                    pTraits<Type>::one*x1,
                    coeffs_[i].second() + pTraits<Type>::one
                )
            );
        }
    }

    return intx;
}


template<class Type>
void CML::DataEntryTypes::Polynomial<Type>::writeData(Ostream& os) const
{
    DataEntry<Type>::writeData(os);

    os  << nl << indent << coeffs_ << token::END_STATEMENT << nl;
}


#endif

// ************************************************************************* //
