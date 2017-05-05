/*---------------------------------------------------------------------------*\
Copyright (C) 2011-2012 OpenFOAM Foundation
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
    CML::Table

Description
    Templated table container data entry. Items are stored in a list of
    Tuple2's. First column is always stored as scalar entries. Data is read
    in Tuple2 form, e.g. for an entry \<entryName\> that is (scalar, vector):

    \verbatim
        <entryName>   table
        (
            (0.0 (1 2 3))
            (1.0 (4 5 6))
        );
    \endverbatim

SourceFiles
    Table.cpp

\*---------------------------------------------------------------------------*/

#ifndef Table_H
#define Table_H

#include "DataEntry.hpp"
#include "Tuple2.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
class Table;

template<class Type>
Ostream& operator<<
(
    Ostream&,
    const Table<Type>&
);

/*---------------------------------------------------------------------------*\
                           Class Table Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class Table
:
    public DataEntry<Type>,
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


    // Member Functions

        // Manipulation

            //- Convert time
            virtual void convertTimeBase(const Time& t)
            {
                TableBase<Type>::convertTimeBase(t);
            }


        // Evaluation

            //- Return Table value
            virtual Type value(const scalar x) const
            {
                return TableBase<Type>::value(x);
            }

            //- Integrate between two (scalar) values
            virtual Type integrate(const scalar x1, const scalar x2) const
            {
                return TableBase<Type>::integrate(x1, x2);
            }

             //- Return dimensioned constant value
            virtual dimensioned<Type> dimValue(const scalar x) const
            {
                return TableBase<Type>::dimValue(x);
            }

            //- Integrate between two values and return dimensioned type
            virtual dimensioned<Type> dimIntegrate
            (
                const scalar x1,
                const scalar x2
            )
            {
                return TableBase<Type>::dimIntegrate(x1, x2);
            }


    // I/O

        //- Ostream Operator
        friend Ostream& operator<< <Type>
        (
            Ostream& os,
            const Table<Type>& tbl
        );

        //- Write in dictionary format
        virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::Table<Type>::Table(const word& entryName, const dictionary& dict)
:
    DataEntry<Type>(entryName),
    TableBase<Type>(entryName, dict)
{
    Istream& is(dict.lookup(entryName));
    word entryType(is);

    token firstToken(is);
    is.putBack(firstToken);
    if (firstToken == token::BEGIN_SQR)
    {
        is >> this->dimensions_;
    }
    is  >> this->table_;

    TableBase<Type>::check();
}


template<class Type>
CML::Table<Type>::Table(const Table<Type>& tbl)
:
    DataEntry<Type>(tbl),
    TableBase<Type>(tbl)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::Table<Type>::~Table()
{}


// * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * * * //

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const Table<Type>& tbl
)
{
    os  << static_cast<const DataEntry<Type>&>(tbl)
        << static_cast<const TableBase<Type>&>(tbl);

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const Table<Type>&)"
    );

    return os;
}


template<class Type>
void CML::Table<Type>::writeData(Ostream& os) const
{
    DataEntry<Type>::writeData(os);
    TableBase<Type>::writeData(os);
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
