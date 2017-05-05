/*---------------------------------------------------------------------------*\
Copyright (C) 2011 OpenFOAM Foundation
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
    CML::Constant

Description
    Templated basic entry that holds a constant value.

    Usage - for entry \<entryName\> having the value <value>:
    \verbatim
        <entryName>    constant  <value>
    \endverbatim

SourceFiles
    Constant.cpp

\*---------------------------------------------------------------------------*/

#ifndef Constant_H
#define Constant_H

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
class Constant;

template<class Type>
Ostream& operator<<(Ostream&, const Constant<Type>&);

/*---------------------------------------------------------------------------*\
                           Class Constant Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Constant
:
    public DataEntry<Type>
{
    // Private data

        //- Constant value
        Type value_;


    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const Constant<Type>&);


public:

    // Runtime type information
    TypeName("constant");


    // Constructors

        //- Construct from entry name and Istream
        Constant(const word& entryName, const dictionary& dict);

        //- Copy constructor
        Constant(const Constant<Type>& cnst);

        //- Construct and return a clone
        virtual tmp<DataEntry<Type> > clone() const
        {
            return tmp<DataEntry<Type> >(new Constant<Type>(*this));
        }


    //- Destructor
    virtual ~Constant();


    // Member Functions

        //- Return constant value
        Type value(const scalar) const;

        //- Integrate between two values
        Type integrate(const scalar x1, const scalar x2) const;


    // I/O

        //- Ostream Operator
        friend Ostream& operator<< <Type>
        (
            Ostream& os,
            const Constant<Type>& cnst
        );

        //- Write in dictionary format
        virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::Constant<Type>::Constant(const word& entryName, const dictionary& dict)
:
    DataEntry<Type>(entryName),
    value_(pTraits<Type>::zero)
{
    Istream& is(dict.lookup(entryName));
    word entryType(is);

    is  >> value_;
}


template<class Type>
CML::Constant<Type>::Constant(const Constant<Type>& cnst)
:
    DataEntry<Type>(cnst),
    value_(cnst.value_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::Constant<Type>::~Constant()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Type CML::Constant<Type>::value(const scalar x) const
{
    return value_;
}


template<class Type>
Type CML::Constant<Type>::integrate(const scalar x1, const scalar x2) const
{
    return (x2 - x1)*value_;
}


// * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const Constant<Type>& cnst
)
{
    if (os.format() == IOstream::ASCII)
    {
        os  << static_cast<const DataEntry<Type>& >(cnst)
            << token::SPACE << cnst.value_;
    }
    else
    {
        os  << static_cast<const DataEntry<Type>& >(cnst);
        os.write
        (
            reinterpret_cast<const char*>(&cnst.value_),
            sizeof(cnst.value_)
        );
    }

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const Constant<Type>&)"
    );

    return os;
}


template<class Type>
void CML::Constant<Type>::writeData(Ostream& os) const
{
    DataEntry<Type>::writeData(os);

    os  << token::SPACE << value_ << token::END_STATEMENT << nl;
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
