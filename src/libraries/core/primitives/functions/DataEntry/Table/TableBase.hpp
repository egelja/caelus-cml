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
    CML::TableBase

Description
    Base class for table with bounds handling, interpolation and integration

SourceFiles
    TableBase.cpp

\*---------------------------------------------------------------------------*/

#ifndef TableBase_H
#define TableBase_H

#include "DataEntry.hpp"
#include "Tuple2.hpp"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace CML
{

template<class Type>
class TableBase;

template<class Type>
Ostream& operator<<
(
    Ostream&,
    const TableBase<Type>&
);

/*---------------------------------------------------------------------------*\
                        Class TableBase Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class TableBase
{
public:

    // Public data types

        //- Enumeration for handling out-of-bound values
        enum boundsHandling
        {
            ERROR,          /*!< Exit with a FatalError */
            WARN,           /*!< Issue warning and clamp value (default) */
            CLAMP,          /*!< Clamp value to the start/end value */
            REPEAT          /*!< Treat as a repeating list */
        };


protected:

    // Protected data

        //- Table name
        word name_;

        //- Enumeration for handling out-of-bound values
        boundsHandling boundsHandling_;

        //- Table data
        List<Tuple2<scalar, Type> > table_;


    // Protected Member Functions

        //- Disallow default bitwise assignment
        void operator=(const TableBase<Type>&);


public:

    // Constructors

        //- Construct from dictionary - note table is not populated
        TableBase(const word& name, const dictionary& dict);

        //- Copy constructor
        TableBase(const TableBase<Type>& tbl);


    //- Destructor
    virtual ~TableBase();


    // Member Functions

        //- Return the out-of-bounds handling as a word
        word boundsHandlingToWord(const boundsHandling& bound) const;

        //- Return the out-of-bounds handling as an enumeration
        boundsHandling wordToBoundsHandling(const word& bound) const;

        //- Set the out-of-bounds handling from enum, return previous setting
        boundsHandling outOfBounds(const boundsHandling& bound);

        //- Check the table for size and consistency
        void check() const;

        //- Check minimum table bounds
        bool checkMinBounds(const scalar x, scalar& xDash) const;

        //- Check maximum table bounds
        bool checkMaxBounds(const scalar x, scalar& xDash) const;

        //- Convert time
        virtual void convertTimeBase(const Time& t);

        //- Return Table value
        virtual Type value(const scalar x) const;

        //- Integrate between two (scalar) values
        virtual Type integrate(const scalar x1, const scalar x2) const;


    // I/O

        //- Ostream Operator
        friend Ostream& operator<< <Type>
        (
            Ostream& os,
            const TableBase<Type>& tbl
        );

        //- Write all table data in dictionary format
        virtual void writeData(Ostream& os) const;

        //- Write keywords only in dictionary format. Used for non-inline
        //  table types
        virtual void writeEntries(Ostream& os) const;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace CML

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#include "Time.hpp"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
CML::TableBase<Type>::TableBase(const word& name, const dictionary& dict)
:
    name_(name),
    boundsHandling_
    (
        wordToBoundsHandling
        (
            dict.lookupOrDefault<word>("outOfBounds", "clamp")
        )
    ),
    table_()
{}


template<class Type>
CML::TableBase<Type>::TableBase(const TableBase<Type>& tbl)
:
    name_(tbl.name_),
    boundsHandling_(tbl.boundsHandling_),
    table_(tbl.table_)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class Type>
CML::TableBase<Type>::~TableBase()
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
CML::word CML::TableBase<Type>::boundsHandlingToWord
(
     const boundsHandling& bound
) const
{
    word enumName("warn");

    switch (bound)
    {
        case ERROR:
        {
            enumName = "error";
            break;
        }
        case WARN:
        {
            enumName = "warn";
            break;
        }
        case CLAMP:
        {
            enumName = "clamp";
            break;
        }
        case REPEAT:
        {
            enumName = "repeat";
            break;
        }
    }

    return enumName;
}


template<class Type>
typename CML::TableBase<Type>::boundsHandling
CML::TableBase<Type>::wordToBoundsHandling
(
    const word& bound
) const
{
    if (bound == "error")
    {
        return ERROR;
    }
    else if (bound == "warn")
    {
        return WARN;
    }
    else if (bound == "clamp")
    {
        return CLAMP;
    }
    else if (bound == "repeat")
    {
        return REPEAT;
    }
    else
    {
        WarningIn("CML::TableBase<Type>::wordToBoundsHandling(const word&)")
            << "bad outOfBounds specifier " << bound << " using 'warn'"
            << endl;

        return WARN;
    }
}


template<class Type>
typename CML::TableBase<Type>::boundsHandling
CML::TableBase<Type>::outOfBounds
(
    const boundsHandling& bound
)
{
    boundsHandling prev = boundsHandling_;
    boundsHandling_ = bound;
    return prev;
}


template<class Type>
void CML::TableBase<Type>::check() const
{
    if (!table_.size())
    {
        FatalErrorIn("CML::TableBase<Type>::check() const")
            << "Table for entry " << this->name_ << " is invalid (empty)"
            << nl << exit(FatalError);
    }

    label n = table_.size();
    scalar prevValue = table_[0].first();

    for (label i = 1; i < n; ++i)
    {
        const scalar currValue = table_[i].first();

        // avoid duplicate values (divide-by-zero error)
        if (currValue <= prevValue)
        {
            FatalErrorIn("CML::TableBase<Type>::check() const")
                << "out-of-order value: " << currValue << " at index " << i
                << exit(FatalError);
        }
        prevValue = currValue;
    }
}


template<class Type>
bool CML::TableBase<Type>::checkMinBounds
(
    const scalar x,
    scalar& xDash
) const
{
    if (x < table_[0].first())
    {
        switch (boundsHandling_)
        {
            case ERROR:
            {
                FatalErrorIn
                (
                    "bool CML::TableBase<Type>::checkMinBounds"
                    "("
                        "const scalar, "
                        "scalar&"
                    ") const"
                )   << "value (" << x << ") underflow"
                    << exit(FatalError);
                break;
            }
            case WARN:
            {
                WarningIn
                (
                    "bool CML::TableBase<Type>::checkMinBounds"
                    "("
                        "const scalar, "
                        "scalar&"
                    ") const"
                )   << "value (" << x << ") underflow" << nl
                    << endl;

                // fall-through to 'CLAMP'
            }
            case CLAMP:
            {
                xDash = table_[0].first();
                return true;
                break;
            }
            case REPEAT:
            {
                // adjust x to >= minX
                scalar span = table_.last().first() - table_[0].first();
                xDash = fmod(x - table_[0].first(), span) + table_[0].first();
                break;
            }
        }
    }
    else
    {
        xDash = x;
    }

    return false;
}


template<class Type>
bool CML::TableBase<Type>::checkMaxBounds
(
    const scalar x,
    scalar& xDash
) const
{
    if (x > table_.last().first())
    {
        switch (boundsHandling_)
        {
            case ERROR:
            {
                FatalErrorIn
                (
                    "bool CML::TableBase<Type>::checkMaxBounds"
                    "("
                        "const scalar, "
                        "scalar&"
                    ") const"
                )   << "value (" << x << ") overflow"
                    << exit(FatalError);
                break;
            }
            case WARN:
            {
                WarningIn
                (
                    "bool CML::TableBase<Type>::checkMaxBounds"
                    "("
                        "const scalar, "
                        "scalar&"
                    ") const"
                )   << "value (" << x << ") overflow" << nl
                    << endl;

                // fall-through to 'CLAMP'
            }
            case CLAMP:
            {
                xDash = table_.last().first();
                return true;
                break;
            }
            case REPEAT:
            {
                // adjust x to >= minX
                scalar span = table_.last().first() - table_[0].first();
                xDash = fmod(x - table_[0].first(), span) + table_[0].first();
                break;
            }
        }
    }
    else
    {
        xDash = x;
    }

    return false;
}


template<class Type>
void CML::TableBase<Type>::convertTimeBase(const Time& t)
{
    forAll(table_, i)
    {
        scalar value = table_[i].first();
        table_[i].first() = t.userTimeToTime(value);
    }
}


template<class Type>
Type CML::TableBase<Type>::value(const scalar x) const
{
    scalar xDash = x;

    if (checkMinBounds(x, xDash))
    {
        return table_[0].second();
    }

    if (checkMaxBounds(xDash, xDash))
    {
        return table_.last().second();
    }

    // Find i such that x(i) < xDash < x(i+1)
    label i = 0;
    while ((i+1 < table_.size()) && (table_[i+1].first() < xDash))
    {
        i++;
    }

    if (i+1 == table_.size())
    {
        // Reached end of table. This can happen for tables of size 1.
        return table_[i].second();
    }

    // Linear interpolation to find value
    return Type
    (
        (xDash - table_[i].first())/(table_[i+1].first() - table_[i].first())
      * (table_[i+1].second() - table_[i].second())
      + table_[i].second()
    );
}


template<class Type>
Type CML::TableBase<Type>::integrate(const scalar x1, const scalar x2) const
{
    // Initialise return value
    Type sum = pTraits<Type>::zero;

    // Return zero if out of bounds
    if ((x1 > table_.last().first()) || (x2 < table_[0].first()))
    {
        return sum;
    }

    // Find next index greater than x1
    label id1 = 0;
    while ((table_[id1].first() < x1) && (id1 < table_.size()))
    {
        id1++;
    }

    // Find next index less than x2
    label id2 = table_.size() - 1;
    while ((table_[id2].first() > x2) && (id2 >= 1))
    {
        id2--;
    }

    if ((id1 - id2) == 1)
    {
        // x1 and x2 lie within 1 interval
        sum = 0.5*(value(x1) + value(x2))*(x2 - x1);
    }
    else
    {
        // x1 and x2 cross multiple intervals

        // Integrate table body
        for (label i=id1; i<id2; i++)
        {
            sum +=
                (table_[i].second() + table_[i+1].second())
              * (table_[i+1].first() - table_[i].first());
        }
        sum *= 0.5;

        // Add table ends (partial segments)
        if (id1 > 0)
        {
            sum += 0.5
              * (value(x1) + table_[id1].second())
              * (table_[id1].first() - x1);
        }
        if (id2 < table_.size() - 1)
        {
            sum += 0.5
              * (table_[id2].second() + value(x2))
              * (x2 - table_[id2].first());
        }
    }

    return sum;
}


// * * * * * * * * * * * * * *  IOStream operators * * * * * * * * * * * * * //

#include "DataEntry.hpp"

// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

template<class Type>
CML::Ostream& CML::operator<<
(
    Ostream& os,
    const TableBase<Type>& tbl
)
{
    if (os.format() == IOstream::ASCII)
    {
         os << token::SPACE << tbl.table_;
    }
    else
    {
        os.write
        (
            reinterpret_cast<const char*>(&tbl.table_),
            sizeof(tbl.table_)
        );
    }

    // Check state of Ostream
    os.check
    (
        "Ostream& operator<<(Ostream&, const TableBase<Type>&, const bool)"
    );

    return os;
}


template<class Type>
void CML::TableBase<Type>::writeData(Ostream& os) const
{
    os  << nl << indent << table_ << token::END_STATEMENT << nl;
    writeEntries(os);
}


template<class Type>
void CML::TableBase<Type>::writeEntries(Ostream& os) const
{
    if (boundsHandling_ != CLAMP)
    {
        os.writeKeyword("outOfBounds") << boundsHandlingToWord(boundsHandling_)
            << token::END_STATEMENT << nl;
    }
}




// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
