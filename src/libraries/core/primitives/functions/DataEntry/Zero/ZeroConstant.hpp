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
    CML::DataEntryTypes::ZeroConstant

Description
    Templated function that returns the corresponding 0 (zero).

    Usage:
    \verbatim
        <entryName> zero;
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef ZeroConstant_HPP
#define ZeroConstant_HPP

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace DataEntryTypes
{

/*---------------------------------------------------------------------------*\
                           Class ZeroConstant Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class ZeroConstant
:
    public DataEntry<Type>
{
    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const ZeroConstant<Type>&);


public:

    // Runtime type information
    TypeName("zero");


    // Constructors

        //- Construct from entry name and dictionary
        ZeroConstant(const word& entryName, const dictionary& dict);

        //- Construct and return a clone
        virtual tmp<DataEntry<Type> > clone() const
        {
            return tmp<DataEntry<Type> >(new ZeroConstant<Type>(*this));
        }


    //- Destructor
    virtual ~ZeroConstant();


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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::ZeroConstant<Type>::ZeroConstant
(
    const word& entryName,
    const dictionary& dict
)
:
    DataEntry<Type>(entryName)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::ZeroConstant<Type>::~ZeroConstant()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
Type CML::DataEntryTypes::ZeroConstant<Type>::value(const scalar x) const
{
    return pTraits<Type>::zero;
}


template<class Type>
Type CML::DataEntryTypes::ZeroConstant<Type>::integrate
(
    const scalar x1,
    const scalar x2
) const
{
    return pTraits<Type>::zero;
}


template<class Type>
void CML::DataEntryTypes::ZeroConstant<Type>::writeData(Ostream& os) const
{
    DataEntry<Type>::writeData(os);

    os  << token::END_STATEMENT << nl;
}


#endif

// ************************************************************************* //
