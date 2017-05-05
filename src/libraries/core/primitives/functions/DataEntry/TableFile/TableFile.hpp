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
    CML::TableFile

Description
    Templated table container data entry where data is read from file.

    \verbatim
        <entryName>   tableFile;
        tableFileCoeffs
        {
            fileName        dataFile;   // name of data file
            outOfBounds     clamp;      // optional out-of-bounds handling
        }
    \endverbatim

    Items are stored in a list of Tuple2's. First column is always stored as
    scalar entries.  Data is read in the form, e.g. for an entry \<entryName\>
    that is (scalar, vector):
    \verbatim
        (
            0.0 (1 2 3)
            1.0 (4 5 6)
        );
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef TableFile_H
#define TableFile_H

#include "DataEntry.hpp"
#include "Tuple2.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
class TableFile;

template<class Type>
Ostream& operator<<
(
    Ostream&,
    const TableFile<Type>&
);

/*---------------------------------------------------------------------------*\
                           Class TableFile Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class TableFile
:
    public DataEntry<Type>,
    public TableBase<Type>
{
    // Private data

        //- File name for csv table (optional)
        fileName fName_;


    // Private Member Functions

        //- Disallow default bitwise assignment
        void operator=(const TableFile<Type>&);


public:

    //- Runtime type information
    TypeName("tableFile");


    // Constructors

        //- Construct from entry name and Istream
        TableFile(const word& entryName, const dictionary& dict);

        //- Copy constructor
        TableFile(const TableFile<Type>& tbl);

        //- Construct and return a clone
        virtual tmp<DataEntry<Type> > clone() const
        {
            return tmp<DataEntry<Type> >(new TableFile<Type>(*this));
        }


    //- Destructor
    virtual ~TableFile();


    // Member Functions

        // Manipulation

            //- Convert time
            virtual void convertTimeBase(const Time& t)
            {
                TableBase<Type>::convertTimeBase(t);
            }


        // Evaluation

            //- Return TableFile value
            virtual Type value(const scalar x) const
            {
                return TableBase<Type>::value(x);
            }

            //- Integrate between two (scalar) values
            virtual Type integrate(const scalar x1, const scalar x2) const
            {
                return TableBase<Type>::integrate(x1, x2);
            }


    // I/O

        //- Ostream Operator
        friend Ostream& operator<< <Type>
        (
            Ostream& os,
            const TableFile<Type>& tbl
        );

        //- Write in dictionary format
        virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::TableFile<Type>::TableFile(const word& entryName, const dictionary& dict)
:
    DataEntry<Type>(entryName),
    TableBase<Type>(entryName, dict.subDict(type() + "Coeffs")),
    fName_("none")
{
    const dictionary coeffs(dict.subDict(type() + "Coeffs"));
    coeffs.lookup("fileName") >> fName_;

    fileName expandedFile(fName_);
    IFstream is(expandedFile.expand());

    is  >> this->table_;

    TableBase<Type>::check();
}


template<class Type>
CML::TableFile<Type>::TableFile(const TableFile<Type>& tbl)
:
    DataEntry<Type>(tbl),
    TableBase<Type>(tbl),
    fName_(tbl.fName_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::TableFile<Type>::~TableFile()
{}


// * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * * * //

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const TableFile<Type>& tbl
)
{
    os  << static_cast<const DataEntry<Type>&>(tbl)
        << static_cast<const TableBase<Type>&>(tbl);

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const TableFile<Type>&)"
    );

    return os;
}


template<class Type>
void CML::TableFile<Type>::writeData(Ostream& os) const
{
    DataEntry<Type>::writeData(os);

    os  << token::END_STATEMENT << nl
        << indent << word(type() + "Coeffs") << nl
        << indent << token::BEGIN_BLOCK << nl << incrIndent;

    // Note: for TableBase write the dictionary entries it needs but not
    // the values themselves
    TableBase<Type>::writeEntries(os);

    os.writeKeyword("fileName")<< fName_ << token::END_STATEMENT << nl;
    os  << decrIndent << indent << token::END_BLOCK << endl;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
