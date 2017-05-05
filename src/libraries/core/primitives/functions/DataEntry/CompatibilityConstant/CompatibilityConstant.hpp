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
    CML::CompatibilityConstant

Description
    Templated basic entry that holds a constant value for backwards
    compatibility (when DataEntry type is not present)

    Usage - for entry \<entryName\> having the value <value>:
    \verbatim
        <entryName>    <value>
    \endverbatim

SourceFiles
    CompatibilityConstant.cpp

\*---------------------------------------------------------------------------*/

#ifndef CompatibilityConstant_H
#define CompatibilityConstant_H

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
class CompatibilityConstant;

template<class Type>
Ostream& operator<<(Ostream&, const CompatibilityConstant<Type>&);

/*---------------------------------------------------------------------------*\
                   Class CompatibilityConstant Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class CompatibilityConstant
:
    public DataEntry<Type>
{
    // Private data

        //- Constant value
        Type value_;


    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const CompatibilityConstant<Type>&);


public:

    // Runtime type information
    TypeName("CompatibilityConstant");


    // Constructors

        //- Construct from entry name and Istream
        CompatibilityConstant(const word& entryName, const dictionary& dict);

        //- Copy constructor
        CompatibilityConstant(const CompatibilityConstant<Type>& cnst);

        //- Construct and return a clone
        virtual tmp<DataEntry<Type> > clone() const
        {
            return tmp<DataEntry<Type> >
            (
                new CompatibilityConstant<Type>(*this)
            );
        }


    //- Destructor
    virtual ~CompatibilityConstant();


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
            const CompatibilityConstant<Type>& cnst
        );

        //- Write in dictionary format
        virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::CompatibilityConstant<Type>::CompatibilityConstant
(
    const word& entryName, const dictionary& dict
)
:
    DataEntry<Type>(entryName),
    value_(pTraits<Type>::zero)
{
    dict.lookup(entryName)  >> value_;
}


template<class Type>
CML::CompatibilityConstant<Type>::CompatibilityConstant
(
    const CompatibilityConstant<Type>& cnst
)
:
    DataEntry<Type>(cnst),
    value_(cnst.value_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::CompatibilityConstant<Type>::~CompatibilityConstant()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Type CML::CompatibilityConstant<Type>::value(const scalar x) const
{
    return value_;
}


template<class Type>
Type CML::CompatibilityConstant<Type>::integrate
(
    const scalar x1,
    const scalar x2
) const
{
    return (x2 - x1)*value_;
}


// * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * * * //

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const CompatibilityConstant<Type>& cnst
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
        "Ostream& operator<<(Ostream&, const CompatibilityConstant<Type>&)"
    );

    return os;
}


template<class Type>
void CML::CompatibilityConstant<Type>::writeData(Ostream& os) const
{
    os.writeKeyword(this->name_) << value_ << token::END_STATEMENT << nl;
}





// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
