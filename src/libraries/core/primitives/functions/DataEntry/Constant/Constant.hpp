/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2017 OpenFOAM Foundation
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
    CML::DataEntryTypes::Constant

Description
    Templated basic entry that returns a constant value.

    Usage - for entry \<entryName\> returning the value <value>:
    \verbatim
        <entryName>    constant  <value>
    \endverbatim

SourceFiles
    Constant.C

\*---------------------------------------------------------------------------*/

#ifndef Constant_HPP
#define Constant_HPP

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace DataEntryTypes
{

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

        //- Construct from entry name and value
        Constant(const word& entryName, const Type& val);

        //- Construct from entry name and dictionary
        Constant(const word& entryName, const dictionary& dict);

        //- Construct from entry name and Istream
        //  Reads the constant value without the DataEntry type
        //  for backward compatibility
        Constant(const word& entryName, Istream& is);

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

        //- Write in dictionary format
        virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace DataEntryTypes
} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::Constant<Type>::Constant
(
    const word& entryName,
    const Type& val
)
:
    DataEntry<Type>(entryName),
    value_(val)
{}


template<class Type>
CML::DataEntryTypes::Constant<Type>::Constant
(
    const word& entryName,
    const dictionary& dict
)
:
    DataEntry<Type>(entryName),
    value_(Zero)
{
    Istream& is(dict.lookup(entryName));
    word entryType(is);
    is  >> value_;
}


template<class Type>
CML::DataEntryTypes::Constant<Type>::Constant
(
    const word& entryName,
    Istream& is
)
:
    DataEntry<Type>(entryName),
    value_(pTraits<Type>(is))
{}


template<class Type>
CML::DataEntryTypes::Constant<Type>::Constant(const Constant<Type>& cnst)
:
    DataEntry<Type>(cnst),
    value_(cnst.value_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::Constant<Type>::~Constant()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Type CML::DataEntryTypes::Constant<Type>::value(const scalar x) const
{
    return value_;
}


template<class Type>
Type CML::DataEntryTypes::Constant<Type>::integrate
(
    const scalar x1,
    const scalar x2
) const
{
    return (x2 - x1)*value_;
}


template<class Type>
void CML::DataEntryTypes::Constant<Type>::writeData(Ostream& os) const
{
    DataEntry<Type>::writeData(os);

    os  << token::SPACE << value_ << token::END_STATEMENT << nl;
}


template<class Type>
CML::autoPtr<CML::DataEntry<Type> > CML::DataEntry<Type>::New
(
    const word& entryName,
    const dictionary& dict
)
{
    if (dict.isDict(entryName))
    {
        const dictionary& coeffsDict(dict.subDict(entryName));

        const word DataEntryType(coeffsDict.lookup("type"));

        typename dictionaryConstructorTable::iterator cstrIter =
            dictionaryConstructorTablePtr_->find(DataEntryType);

        if (cstrIter == dictionaryConstructorTablePtr_->end())
        {
            FatalErrorInFunction
                << "Unknown DataEntry type "
                << DataEntryType << " for DataEntry "
                << entryName << nl << nl
                << "Valid DataEntry types are:" << nl
                << dictionaryConstructorTablePtr_->sortedToc() << nl
                << exit(FatalError);
        }

        return cstrIter()(entryName, coeffsDict);
    }
    else
    {
        Istream& is(dict.lookup(entryName, false));

        token firstToken(is);
        word DataEntryType;

        if (!firstToken.isWord())
        {
            is.putBack(firstToken);
            return autoPtr<DataEntry<Type> >
            (
                new DataEntryTypes::Constant<Type>(entryName, is)
            );
        }
        else
        {
            DataEntryType = firstToken.wordToken();
        }

        typename dictionaryConstructorTable::iterator cstrIter =
            dictionaryConstructorTablePtr_->find(DataEntryType);

        if (cstrIter == dictionaryConstructorTablePtr_->end())
        {
            FatalErrorInFunction
                << "Unknown DataEntry type "
                << DataEntryType << " for DataEntry "
                << entryName << nl << nl
                << "Valid DataEntry types are:" << nl
                << dictionaryConstructorTablePtr_->sortedToc() << nl
                << exit(FatalError);
        }

        return cstrIter()
        (
            entryName,
            dict.found(entryName + "Coeffs")
          ? dict.subDict(entryName + "Coeffs")
          : dict
        );
    }
}


#endif

// ************************************************************************* //
