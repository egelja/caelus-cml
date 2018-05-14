/*---------------------------------------------------------------------------*\
Copyright (C) 2016 OpenFOAM Foundation
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
    CML::DataEntryTypes::Square

Description
    Templated square-wave function with support for an offset level.

        \f[
            a square(f (t - t_0)) s + l
        \f]

    where

    \f$ square(t) \f$ is the square-wave function in range \f$ [-1, 1] \f$
    with a mark/space ratio of \f$ r \f$

    \vartable
        symbol  | Description       | Data type         | Default
        a       | Amplitude         | DataEntry<scalar> |
        f       | Frequency [1/s]   | DataEntry<scalar> |
        s       | Type scale factor | DataEntry<Type>   |
        l       | Type offset level | DataEntry<Type>   |
        t_0     | Start time [s]    | scalar            | 0
        r       | mark/space ratio  | scalar            | 1
        t       | Time [s]          | scalar
    \endvartable

    Example for a scalar:
    \verbatim
        <entryName> square;
        <entryName>Coeffs
        {
            frequency 10;
            amplitude 0.1;
            scale     2e-6;
            level     2e-6;
        }
    \endverbatim

    Example for a vector:
    \verbatim
        <entryName> square;
        <entryName>Coeffs
        {
            frequency 10;
            amplitude 1;
            scale     (1 0.1 0);
            level     (10 1 0);
        }
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef Square_HPP
#define Square_HPP

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace DataEntryTypes
{

/*---------------------------------------------------------------------------*\
                           Class Square Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Square
:
    public DataEntry<Type>
{
    // Private data

        //- Start-time for the square function
        scalar t0_;

        //- Mark/space ratio of the square function
        scalar markSpace_;

        //- Scalar amplitude of the square function
        autoPtr<DataEntry<scalar> > amplitude_;

        //- Frequency of the square function
        autoPtr<DataEntry<scalar> > frequency_;

        //- Scaling factor of the square function
        autoPtr<DataEntry<Type> > scale_;

        //- Level to which the square function is added
        autoPtr<DataEntry<Type> > level_;


    // Private Member Functions

        //- Read the coefficients from the given dictionary
        void read(const dictionary& coeffs);

        //- Disallow default bitwise assignment
        void operator=(const Square<Type>&);


public:

    // Runtime type information
    TypeName("square");


    // Constructors

        //- Construct from entry name and dictionary
        Square
        (
            const word& entryName,
            const dictionary& dict
        );

        //- Copy constructor
        Square(const Square<Type>& se);

        //- Construct and return a clone
        virtual tmp<DataEntry<Type> > clone() const
        {
            return tmp<DataEntry<Type> >(new Square<Type>(*this));
        }


    //- Destructor
    virtual ~Square();


    // Member Functions

        //- Return value for time t
        Type value(const scalar t) const;

        //- Write in dictionary format
        virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace DataEntryTypes
} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
void CML::DataEntryTypes::Square<Type>::read(const dictionary& coeffs)
{
    t0_ = coeffs.lookupOrDefault<scalar>("t0", 0);
    markSpace_ = coeffs.lookupOrDefault<scalar>("markSpace", 1);
    amplitude_ = DataEntry<scalar>::New("amplitude", coeffs);
    frequency_ = DataEntry<scalar>::New("frequency", coeffs);
    scale_ = DataEntry<Type>::New("scale", coeffs);
    level_ = DataEntry<Type>::New("level", coeffs);
}


template<class Type>
CML::DataEntryTypes::Square<Type>::Square
(
    const word& entryName,
    const dictionary& dict
)
:
    DataEntry<Type>(entryName)
{
    read(dict);
}


template<class Type>
CML::DataEntryTypes::Square<Type>::Square(const Square<Type>& se)
:
    DataEntry<Type>(se),
    t0_(se.t0_),
    markSpace_(se.markSpace_),
    amplitude_(se.amplitude_, false),
    frequency_(se.frequency_, false),
    scale_(se.scale_, false),
    level_(se.level_, false)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::Square<Type>::~Square()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Type CML::DataEntryTypes::Square<Type>::value(const scalar t) const
{
    // Number of waves including fractions
    scalar waves = frequency_->value(t)*(t - t0_);

    // Number of complete waves
    scalar nWaves;

    // Fraction of last incomplete wave
    scalar waveFrac = std::modf(waves, &nWaves);

    // Mark fraction of a wave
    scalar markFrac = markSpace_/(1.0 + markSpace_);

    return
        amplitude_->value(t)
       *(waveFrac < markFrac ? 1 : -1)
       *scale_->value(t)
      + level_->value(t);
}


template<class Type>
void CML::DataEntryTypes::Square<Type>::writeData(Ostream& os) const
{
    DataEntry<Type>::writeData(os);
    os  << token::END_STATEMENT << nl;
    os  << indent << word(this->name() + "Coeffs") << nl;
    os  << indent << token::BEGIN_BLOCK << incrIndent << nl;
    os.writeKeyword("t0") << t0_ << token::END_STATEMENT << nl;
    os.writeKeyword("markSpace") << markSpace_ << token::END_STATEMENT << nl;
    amplitude_->writeData(os);
    frequency_->writeData(os);
    scale_->writeData(os);
    level_->writeData(os);
    os  << decrIndent << indent << token::END_BLOCK << endl;
}


#endif

// ************************************************************************* //
