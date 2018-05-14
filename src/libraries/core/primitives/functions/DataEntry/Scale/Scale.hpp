/*---------------------------------------------------------------------------*\
Copyright (C) 2017 OpenFOAM Foundation
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
    CML::DataEntryTypes::Scale

Description
    DataEntry which scales a given 'value' function by a scalar 'scale'
    function.

    This is particularly useful to ramp a time-varying value by one of the
    monotonic ramp functions.

    Usage for a vector:
    \verbatim
        <entryName>
        {
            type      scale;

            scale
            {
                type        linearRamp;

                start       0;
                duration    10;
            }

            value
            {
                type        sine;

                frequency   10;
                amplitude   1;
                scale       (1 0.1 0);
                level       (10 1 0);
            }
        }
    \endverbatim

    Where:
    \table
        Property | Description                                  | Required
        value    | Function of type DataEntry<Type>             | yes
        scale    | Scaling function of type DataEntry<scalar>   | yes
    \endtable


\*---------------------------------------------------------------------------*/

#ifndef Scale_HPP
#define Scale_HPP

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace DataEntryTypes
{

/*---------------------------------------------------------------------------*\
                           Class Scale Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Scale
:
    public DataEntry<Type>
{
    // Private data

        //- Scalar scaling function
        autoPtr<DataEntry<scalar> > scale_;

        //- Value function
        autoPtr<DataEntry<Type> > value_;


    // Private Member Functions

        //- Read the coefficients from the given dictionary
        void read(const dictionary& coeffs);

        //- Disallow default bitwise assignment
        void operator=(const Scale<Type>&);


public:

    // Runtime type information
    TypeName("scale");


    // Constructors

        //- Construct from entry name and dictionary
        Scale
        (
            const word& entryName,
            const dictionary& dict
        );

        //- Copy constructor
        Scale(const Scale<Type>& se);

        //- Construct and return a clone
        virtual tmp<DataEntry<Type> > clone() const
        {
            return tmp<DataEntry<Type> >(new Scale<Type>(*this));
        }


    //- Destructor
    virtual ~Scale();


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
void CML::DataEntryTypes::Scale<Type>::read(const dictionary& coeffs)
{
    scale_ = DataEntry<scalar>::New("scale", coeffs);
    value_ = DataEntry<Type>::New("value", coeffs);
}


template<class Type>
CML::DataEntryTypes::Scale<Type>::Scale
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
CML::DataEntryTypes::Scale<Type>::Scale(const Scale<Type>& se)
:
    DataEntry<Type>(se),
    scale_(se.scale_, false),
    value_(se.value_, false)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::Scale<Type>::~Scale()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Type CML::DataEntryTypes::Scale<Type>::value(const scalar t) const
{
    return scale_->value(t)*value_->value(t);
}


template<class Type>
void CML::DataEntryTypes::Scale<Type>::writeData(Ostream& os) const
{
    DataEntry<Type>::writeData(os);
    os  << token::END_STATEMENT << nl;
    os  << indent << word(this->name() + "Coeffs") << nl;
    os  << indent << token::BEGIN_BLOCK << incrIndent << nl;
    scale_->writeData(os);
    value_->writeData(os);
    os  << decrIndent << indent << token::END_BLOCK << endl;
}


#endif

// ************************************************************************* //
