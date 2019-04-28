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
    CML::DataEntryTypes::TableFile

Description
    Templated table container data entry where data is read from file.

    Usage:
    \verbatim
        <entryName> tableFile;
        <entryName>Coeffs
        {
            fileName            dataFile;    // name of data file
            outOfBounds         clamp;       // optional out-of-bounds handling
            interpolationScheme linear;      // optional interpolation method
        }
    \endverbatim

    Data is stored as a list of Tuple2's. First column is always stored as
    scalar entries.  Data is read in the form, e.g. for an entry \<entryName\>
    that is (scalar, vector):
    \verbatim
        (
            (0.0 (1 2 3))
            (1.0 (4 5 6))
        );
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef TableFile_HPP
#define TableFile_HPP

#include "DataEntry.hpp"
#include "Tuple2.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace DataEntryTypes
{

/*---------------------------------------------------------------------------*\
                           Class TableFile Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class TableFile
:
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


    // I/O

        //- Write in dictionary format
        virtual void writeData(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace DataEntryTypes
} // End namespace CML


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::TableFile<Type>::TableFile
(
    const word& entryName,
    const dictionary& dict
)
:
    TableBase<Type>(entryName, dict),
    fName_("none")
{
    dict.lookup("file") >> fName_;

    fileName expandedFile(fName_);
    IFstream is(expandedFile.expand());

    if (!is.good())
    {
        FatalIOErrorInFunction(is)
            << "Cannot open file." << exit(FatalIOError);
    }

    is  >> this->table_;

    TableBase<Type>::check();
}


template<class Type>
CML::DataEntryTypes::TableFile<Type>::TableFile(const TableFile<Type>& tbl)
:
    TableBase<Type>(tbl),
    fName_(tbl.fName_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::TableFile<Type>::~TableFile()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void CML::DataEntryTypes::TableFile<Type>::writeData(Ostream& os) const
{
    DataEntry<Type>::writeData(os);

    os  << token::END_STATEMENT << nl
        << indent << word(this->name() + "Coeffs") << nl
        << indent << token::BEGIN_BLOCK << nl << incrIndent;

    // Note: for TableBase write the dictionary entries it needs but not
    // the values themselves
    TableBase<Type>::writeEntries(os);

    os.writeKeyword("file")<< fName_ << token::END_STATEMENT << nl;
    os  << decrIndent << indent << token::END_BLOCK << endl;
}


#endif

// ************************************************************************* //
