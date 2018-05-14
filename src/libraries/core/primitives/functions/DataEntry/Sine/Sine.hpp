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
    CML::DataEntryTypes::Sine

Description
    Templated sine function with support for an offset level.

        \f[
            a sin(2 \pi f (t - t_0)) s + l
        \f]

    where

    \vartable
        symbol  | Description       | Data type
        a       | Amplitude         | DataEntry<scalar>
        f       | Frequency [1/s]   | DataEntry<scalar>
        s       | Type scale factor | DataEntry<Type>
        l       | Type offset level | DataEntry<Type>
        t_0     | Start time [s]    | scalar
        t       | Time [s]          | scalar
    \endvartable

    Example for a scalar:
    \verbatim
        <entryName> sine;
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
        <entryName> sine;
        <entryName>Coeffs
        {
            frequency 10;
            amplitude 1;
            scale     (1 0.1 0);
            level     (10 1 0);
        }
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef Sine_HPP
#define Sine_HPP

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace DataEntryTypes
{

/*---------------------------------------------------------------------------*\
                           Class Sine Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Sine
:
    public DataEntry<Type>
{
    // Private data

        //- Start-time for the sin function
        scalar t0_;

        //- Scalar amplitude of the sin function
        autoPtr<DataEntry<scalar> > amplitude_;

        //- Frequency of the sin function
        autoPtr<DataEntry<scalar> > frequency_;

        //- Scaling factor of the sin function
        autoPtr<DataEntry<Type> > scale_;

        //- Level to which the sin function is added
        autoPtr<DataEntry<Type> > level_;


    // Private Member Functions

        //- Read the coefficients from the given dictionary
        void read(const dictionary& coeffs);

        //- Disallow default bitwise assignment
        void operator=(const Sine<Type>&);


public:

    // Runtime type information
    TypeName("sine");


    // Constructors

        //- Construct from entry name and dictionary
        Sine
        (
            const word& entryName,
            const dictionary& dict
        );

        //- Copy constructor
        Sine(const Sine<Type>& se);

        //- Construct and return a clone
        virtual tmp<DataEntry<Type> > clone() const
        {
            return tmp<DataEntry<Type> >(new Sine<Type>(*this));
        }


    //- Destructor
    virtual ~Sine();


    // Member Functions

        //- Return value for time t
        Type value(const scalar t) const;

        //- Write in dictionary format
        virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace DataEntryTypes
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "mathematicalConstants.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
void CML::DataEntryTypes::Sine<Type>::read(const dictionary& coeffs)
{
    t0_ = coeffs.lookupOrDefault<scalar>("t0", 0);
    amplitude_ = DataEntry<scalar>::New("amplitude", coeffs);
    frequency_ = DataEntry<scalar>::New("frequency", coeffs);
    scale_ = DataEntry<Type>::New("scale", coeffs);
    level_ = DataEntry<Type>::New("level", coeffs);
}


template<class Type>
CML::DataEntryTypes::Sine<Type>::Sine
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
CML::DataEntryTypes::Sine<Type>::Sine(const Sine<Type>& se)
:
    DataEntry<Type>(se),
    t0_(se.t0_),
    amplitude_(se.amplitude_, false),
    frequency_(se.frequency_, false),
    scale_(se.scale_, false),
    level_(se.level_, false)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::Sine<Type>::~Sine()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
inline Type CML::DataEntryTypes::Sine<Type>::value(const scalar t) const
{
    return
        amplitude_->value(t)
       *sin(constant::mathematical::twoPi*frequency_->value(t)*(t - t0_))
       *scale_->value(t)
      + level_->value(t);
}


template<class Type>
void CML::DataEntryTypes::Sine<Type>::writeData(Ostream& os) const
{
    DataEntry<Type>::writeData(os);
    os  << token::END_STATEMENT << nl;
    os  << indent << word(this->name() + "Coeffs") << nl;
    os  << indent << token::BEGIN_BLOCK << incrIndent << nl;
    os.writeKeyword("t0") << t0_ << token::END_STATEMENT << nl;
    amplitude_->writeData(os);
    frequency_->writeData(os);
    scale_->writeData(os);
    level_->writeData(os);
    os  << decrIndent << indent << token::END_BLOCK << endl;
}


#endif

// ************************************************************************* //
