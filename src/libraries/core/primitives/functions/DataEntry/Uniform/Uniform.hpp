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
    CML::DataEntryTypes::Uniform

Description
    Templated function that returns a constant value.

    Provides backward-compatibility for cases where a field is spatially
    "uniform" and may be treated as a constant value.

    Usage - for entry \<entryName\> returning the value <value>:
    \verbatim
        <entryName>    uniform  <value>
    \endverbatim

SourceFiles
    Uniform.C

\*---------------------------------------------------------------------------*/

#ifndef Uniform_HPP
#define Uniform_HPP

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace DataEntryTypes
{

/*---------------------------------------------------------------------------*\
                           Class Uniform Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Uniform
:
    public Constant<Type>
{
    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const Uniform<Type>&);


public:

    // Runtime type information
    TypeName("uniform");


    // Constructors

        //- Construct from entry name and dictionary
        Uniform(const word& entryName, const dictionary& dict);

        //- Construct and return a clone
        virtual tmp<DataEntry<Type> > clone() const
        {
            return tmp<DataEntry<Type> >(new Uniform<Type>(*this));
        }
};


} // End namespace DataEntryTypes
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::Uniform<Type>::Uniform
(
    const word& entryName,
    const dictionary& dict
)
:
    Constant<Type>(entryName, dict)
{}


#endif

// ************************************************************************* //
