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
    CML::DataEntryTypes::CSV

Description
    Templated CSV function.

    Reference column is always a scalar, e.g. time.

    Usage:
    \verbatim
        <entryName> csvFile;
        <entryName>Coeffs
        {
            nHeaderLine         4;          // number of header lines
            refColumn           0;          // reference column index
            componentColumns    (1 2 3);    // component column indices
            separator           ",";        // optional (defaults to ",")
            mergeSeparators     no;         // merge multiple separators
            fileName            "fileXYZ";  // name of csv data file
            outOfBounds         clamp;      // optional out-of-bounds handling
            interpolationScheme linear;     // optional interpolation scheme
        }
    \endverbatim


\*---------------------------------------------------------------------------*/

#ifndef CSV_HPP
#define CSV_HPP

#include "DataEntry.hpp"
#include "TableBase.hpp"
#include "Tuple2.hpp"
#include "labelList.hpp"
#include "ISstream.hpp"

#include "DynamicList.hpp"
#include "IFstream.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{
namespace DataEntryTypes
{

/*---------------------------------------------------------------------------*\
                           Class CSV Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class CSV
:
    public TableBase<Type>
{
    // Private data

        //- Number header lines
        label nHeaderLine_;

        //- Column of the time
        label refColumn_;

        //- Labels of the components
        labelList componentColumns_;

        //- Separator character
        char separator_;

        //- Merge separators flag, e.g. ',,,' becomes ','
        bool mergeSeparators_;

        //- File name for csv table
        fileName fName_;


    // Private Member Functions

        //- Read csv data table
        void read();

        //- Read the next value from the splitted string
        Type readValue(const List<string>&);

        //- Disallow default bitwise assignment
        void operator=(const CSV<Type>&);


public:

    //- Runtime type information
    TypeName("csvFile");


    // Constructors

        //- Construct from entry name and dictionary
        CSV
        (
            const word& entryName,
            const dictionary& dict
        );

        //- Copy constructor
        CSV(const CSV<Type>& tbl);

        //- Construct and return a clone
        virtual tmp<DataEntry<Type> > clone() const
        {
            return tmp<DataEntry<Type> >(new CSV<Type>(*this));
        }


    //- Destructor
    virtual ~CSV();


    // Member Functions

        //- Return const access to the file name
        virtual const fileName& fName() const;

        //- Write in dictionary format
        virtual void writeData(Ostream& os) const;
};


template<>
label CSV<label>::readValue(const List<string>& splitted);

template<>
CML::scalar CSV<scalar>::readValue(const List<string>& splitted);


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace DataEntryTypes
} // End namespace CML


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<>
CML::label CML::DataEntryTypes::CSV<CML::label>::readValue
(
    const List<string>& splitted
)
{
    if (componentColumns_[0] >= splitted.size())
    {
        FatalErrorInFunction
            << "No column " << componentColumns_[0] << " in "
            << splitted << endl
            << exit(FatalError);
    }

    return readLabel(IStringStream(splitted[componentColumns_[0]])());
}


template<>
CML::scalar CML::DataEntryTypes::CSV<CML::scalar>::readValue
(
    const List<string>& splitted
)
{
    if (componentColumns_[0] >= splitted.size())
    {
        FatalErrorInFunction
            << "No column " << componentColumns_[0] << " in "
            << splitted << endl
            << exit(FatalError);
    }

    return readScalar(IStringStream(splitted[componentColumns_[0]])());
}


template<class Type>
Type CML::DataEntryTypes::CSV<Type>::readValue(const List<string>& splitted)
{
    Type result;

    for (label i = 0; i < pTraits<Type>::nComponents; i++)
    {
        if (componentColumns_[i] >= splitted.size())
        {
            FatalErrorInFunction
            << "No column " << componentColumns_[i] << " in "
                << splitted << endl
                << exit(FatalError);
        }

        result[i] =
        readScalar(IStringStream(splitted[componentColumns_[i]])());
    }

    return result;
}


template<class Type>
void CML::DataEntryTypes::CSV<Type>::read()
{
    fileName expandedFile(fName_);
    IFstream is(expandedFile.expand());

    if (!is.good())
    {
        FatalIOErrorInFunction(is)
            << "Cannot open CSV file for reading."
            << exit(FatalIOError);
    }

    DynamicList<Tuple2<scalar, Type> > values;

    // skip header
    for (label i = 0; i < nHeaderLine_; i++)
    {
        string line;
        is.getLine(line);
    }

    label nEntries = max(componentColumns_);

    // read data
    while (is.good())
    {
        string line;
        is.getLine(line);


        label n = 0;
        std::size_t pos = 0;
        DynamicList<string> splitted;

        if (mergeSeparators_)
        {
            std::size_t nPos = 0;

            while ((pos != std::string::npos) && (n <= nEntries))
            {
                bool found = false;
                while (!found)
                {
                    nPos = line.find(separator_, pos);

                    if ((nPos != std::string::npos) && (nPos - pos == 0))
                    {
                        pos = nPos + 1;
                    }
                    else
                    {
                        found = true;
                    }
                }

                nPos = line.find(separator_, pos);

                if (nPos == std::string::npos)
                {
                    splitted.append(line.substr(pos));
                    pos = nPos;
                    n++;
                }
                else
                {
                    splitted.append(line.substr(pos, nPos - pos));
                    pos = nPos + 1;
                    n++;
                }
            }
        }
        else
        {
            while ((pos != std::string::npos) && (n <= nEntries))
            {
                std::size_t nPos = line.find(separator_, pos);

                if (nPos == std::string::npos)
                {
                    splitted.append(line.substr(pos));
                    pos = nPos;
                    n++;
                }
                else
                {
                    splitted.append(line.substr(pos, nPos - pos));
                    pos = nPos + 1;
                    n++;
                }
            }
        }


        if (splitted.size() <= 1)
        {
            break;
        }

        scalar x = readScalar(IStringStream(splitted[refColumn_])());
        Type value = readValue(splitted);

        values.append(Tuple2<scalar,Type>(x, value));
    }

    this->table_.transfer(values);
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::CSV<Type>::CSV
(
    const word& entryName,
    const dictionary& dict
)
:
    TableBase<Type>(entryName, dict),
    nHeaderLine_(readLabel(dict.lookup("nHeaderLine"))),
    refColumn_(readLabel(dict.lookup("refColumn"))),
    componentColumns_(dict.lookup("componentColumns")),
    separator_(dict.lookupOrDefault<string>("separator", string(","))[0]),
    mergeSeparators_(readBool(dict.lookup("mergeSeparators"))),
    fName_(dict.lookup("file"))
{
    if (componentColumns_.size() != pTraits<Type>::nComponents)
    {
        FatalErrorInFunction
            << componentColumns_ << " does not have the expected length of "
            << pTraits<Type>::nComponents << endl
            << exit(FatalError);
    }

    read();

    TableBase<Type>::check();
}


template<class Type>
CML::DataEntryTypes::CSV<Type>::CSV(const CSV<Type>& tbl)
:
    TableBase<Type>(tbl),
    nHeaderLine_(tbl.nHeaderLine_),
    refColumn_(tbl.refColumn_),
    componentColumns_(tbl.componentColumns_),
    separator_(tbl.separator_),
    mergeSeparators_(tbl.mergeSeparators_),
    fName_(tbl.fName_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::DataEntryTypes::CSV<Type>::~CSV()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
const CML::fileName& CML::DataEntryTypes::CSV<Type>::fName() const
{
    return fName_;
}


template<class Type>
void CML::DataEntryTypes::CSV<Type>::writeData(Ostream& os) const
{
    DataEntry<Type>::writeData(os);
    os  << token::END_STATEMENT << nl;
    os  << indent << word(this->name() + "Coeffs") << nl;
    os  << indent << token::BEGIN_BLOCK << incrIndent << nl;

    // Note: for TableBase write the dictionary entries it needs but not
    // the values themselves
    TableBase<Type>::writeEntries(os);

    os.writeKeyword("nHeaderLine") << nHeaderLine_ << token::END_STATEMENT
        << nl;
    os.writeKeyword("refColumn") << refColumn_ << token::END_STATEMENT << nl;

    // Force writing labelList in ascii
    os.writeKeyword("componentColumns");
    if (os.format() == IOstream::BINARY)
    {
        os.format(IOstream::ASCII);
        os  << componentColumns_;
        os.format(IOstream::BINARY);
    }
    else
    {
        os  << componentColumns_;
    }
    os  << token::END_STATEMENT << nl;

    os.writeKeyword("separator") << string(separator_)
        << token::END_STATEMENT << nl;
    os.writeKeyword("mergeSeparators") << mergeSeparators_
        << token::END_STATEMENT << nl;
    os.writeKeyword("file") << fName_ << token::END_STATEMENT << nl;
    os  << decrIndent << indent << token::END_BLOCK << endl;
}


#endif

// ************************************************************************* //
