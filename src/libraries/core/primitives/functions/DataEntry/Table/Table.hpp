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
    CML::DataEntryTypes::Table

Description
    Templated table container function.

    Items are stored in a list of Tuple2's. First column is always stored as
    scalar entries. Data is read in Tuple2 form.

    Usage:
    \verbatim
        <entryName>   table
        (
            (0.0 (1 2 3))
            (1.0 (4 5 6))
        );
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef Table_HPP
#define Table_HPP

#include "DataEntry.hpp"
#include "Tuple2.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace DataEntryTypes
{

/*---------------------------------------------------------------------------*\
                           Class Table Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Table
:
    public TableBase<Type>
{
    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const Table<Type>&);


public:

    //- Runtime type information
    TypeName("table");


    // Constructors

        //- Construct from entry name and Istream
        Table(const word& entryName, const dictionary& dict);

        //- Copy constructor
        Table(const Table<Type>& tbl);

        //- Construct and return a clone
        virtual tmp<DataEntry<Type> > clone() const
        {
            return tmp<DataEntry<Type> >(new Table<Type>(*this));
        }


    //- Destructor
    virtual ~Table();
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace DataEntryTypes
} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::Table<Type>::Table
(
    const word& entryName,
    const dictionary& dict
)
:
    TableBase<Type>(entryName, dict)
{
    Istream& is(dict.lookup(entryName));
    word entryType(is);
    is  >> this->table_;
    TableBase<Type>::check();
}


template<class Type>
CML::DataEntryTypes::Table<Type>::Table(const Table<Type>& tbl)
:
    TableBase<Type>(tbl)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::Table<Type>::~Table()
{}


#endif

// ************************************************************************* //
